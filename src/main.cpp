/*
 * Optical Character Recognition for Latin-based languages
 * Copyright (C) 2018  meme
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <chrono>
#include <cstring>
#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <linux/limits.h>
#include <memory>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "android_bitmap.h"
#include "fake_jni.h"
#include "link_util.h"
#include "log.h"
#include "symbols/m.h"

#include "proto/vision/result.pb.h"
#include "proto/vision/service.grpc.pb.h"

#include <grpc++/grpc++.h>

#include <hybris/common/dlfcn.h>
#include <hybris/common/hooks.h>

extern "C" {
#include "../../android_linker/src/common/jb/linker.h"
}

// #define ENABLE_NATIVE_OUTPUT

void __android_log_print(int, const char* tag, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  Log::log(LogLevel::LOG_INFO, tag, fmt, args);
  va_end(args);
}

void __android_log_write(int, const char* tag, const char* text) {
  Log::info(tag, text);
}

void* uprv_getICUData_(void) {
  Log::error("Launcher", "uprv_getICUData_ is not implemented");
  std::abort();
  return nullptr;
}

void* AAssetManager_(void) {
  Log::error("Launcher", "AAssetManager_ is not implemented");
  std::abort();
  return nullptr;
}

static jboolean (*nativeInitializeWithTypeAndResult)(void*, jobject, jstring,
                                                     jint);
static void (*nativeDeinitialize)(void*, jobject);
static jbyte* (*nativeRecognize)(void*, jobject, jobject);
static JNIEnv* env;

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using vision::Frame;
using vision::Ocr;
using vision::OcrResult;

class OcrServiceImpl final : public Ocr::Service {
  Status Recognize(ServerContext* context, const Frame* frame,
                   OcrResult* result) override {
    auto start = std::chrono::system_clock::now();

    int width = frame->width(), height = frame->height(),
        size = frame->image().size();

    // If an invalid image was passed (i.e.: needs to be the RGBA of the
    // dimensions)
    if (width * height * 4 != size) {
      return Status::CANCELLED; // FIXME
    }

    AndroidBitmapInfo i(width, height);
    std::pair<AndroidBitmapInfo*, void*> b =
        std::make_pair(&i, (void*) frame->image().data());

    jbyte* buf = nativeRecognize(&env, (void*) 0xdeadbeef, (void*) &b);

    vision::OcrResult r;
    bool success = r.ParseFromString(*(std::string*) buf);

    // If the data failed to parse
    if (!success) {
      return Status::CANCELLED;
    }

    result->CopyFrom(r);

    auto end = std::chrono::system_clock::now();

    Log::trace(
        "Server", "Processed request in %dms",
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count());

    return Status::OK;
  }
};

int main(void) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  hybris_set_hook_callback(hybris_hook_callback);

  if (!load_empty_library("libm.so") || !load_empty_library("libc.so"))
    return -1;

  if (!load_empty_library("liblog.so") ||
      !load_empty_library("libjnigraphics.so") ||
      !load_empty_library("libandroid.so") || !load_empty_library("libz.so"))
    return -1;

  // AssetManager
  hybris_hook("AAssetManager_fromJava", (void*) AAssetManager_);
  hybris_hook("AAssetManager_open", (void*) AAssetManager_);
  hybris_hook("AAsset_close", (void*) AAssetManager_);
  hybris_hook("AAsset_getLength", (void*) AAssetManager_);
  hybris_hook("AAsset_getRemainingLength", (void*) AAssetManager_);
  hybris_hook("AAsset_read", (void*) AAssetManager_);
  hybris_hook("AAsset_seek", (void*) AAssetManager_);

  // Android-specific logging
  hybris_hook("__android_log_print", (void*) __android_log_print);
  hybris_hook("__android_log_write", (void*) __android_log_write);

  // AndroidBitmap
  hybris_hook("AndroidBitmap_getInfo", (void*) AndroidBitmap_getInfo);
  hybris_hook("AndroidBitmap_lockPixels", (void*) AndroidBitmap_lockPixels);
  hybris_hook("AndroidBitmap_unlockPixels", (void*) AndroidBitmap_unlockPixels);

  // libicu
  hybris_hook("uprv_getICUData_collation", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_conversion", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_core", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_locale", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_tz", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_uts46", (void*) uprv_getICUData_);
  hybris_hook("uprv_getICUData_other", (void*) uprv_getICUData_);

  char _cwd[PATH_MAX];
  getcwd(_cwd, sizeof(_cwd));

  char libocr[PATH_MAX + 11];
  sprintf(libocr, "%s/libocr.so", _cwd);

  char models[PATH_MAX + 6];
  sprintf(models, "%s/data/models", _cwd);

  void* libmLib = load_os_library("libm.so.6", m_symbol_table);

  if (libmLib == NULL)
    return -1;

  void* handle = hybris_dlopen(libocrPath, RTLD_LAZY);

  if (handle == NULL)
    return -1;

  Log::trace("Launcher", "Loaded `libocr.so' at 0x%x",
             ((soinfo*) handle)->base);

  nativeInitializeWithTypeAndResult =
      (jboolean(*)(void*, jobject, jstring, jint)) hybris_dlsym(
          handle, "Java_com_google_android_libraries_vision_ocr_NativeOcr_"
                  "nativeInitializeWithTypeAndResult");

  nativeDeinitialize = (void (*)(void*, jobject)) hybris_dlsym(
      handle, "Java_com_google_android_libraries_vision_ocr_NativeOcr_"
              "nativeDeinitialize");

  nativeRecognize = (jbyte * (*) (void*, jobject, jobject)) hybris_dlsym(
      handle,
      "Java_com_google_android_libraries_vision_ocr_NativeOcr_nativeRecognize");

  env = FakeJNI::instance.getFakeEnvironment();
  bool s = nativeInitializeWithTypeAndResult(&env, (void*) 0xdeadbeef,
                                             (void*) models, 1);

  if (!s) {
    Log::error("Launcher", "NativeOcr failed to initialize");
    return -1;
  }

  std::string server_address("0.0.0.0:9999");
  OcrServiceImpl service;

  ServerBuilder builder;
  builder.SetMaxReceiveMessageSize(50000000);
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  Log::info("Server", "Server started");

  server->Wait();

  // Clean up the environment
  nativeDeinitialize(&env, (void*) 0xdeadbeef);
  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
