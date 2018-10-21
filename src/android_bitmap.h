#pragma once

#include "fake_jni.h"

#include <utility>

// Currently the only supported format is RGBA8888
const int ANDROID_BITMAP_FORMAT_RGBA_8888 = 1;

// Do not modify the contents of this struct, it must remain binary compatible
struct AndroidBitmapInfo {
  unsigned int width;
  unsigned int height;
  unsigned int stride;
  int format;
  unsigned int flags;

  // Helper constructor since most values are constant
  AndroidBitmapInfo(int width, int height)
      : width(width), height(height), stride(width * 4),
        format(ANDROID_BITMAP_FORMAT_RGBA_8888), flags(0) {}
};

typedef std::pair<AndroidBitmapInfo*, void*> AndroidBitmap;
int AndroidBitmap_getInfo(JNIEnv**, AndroidBitmap* p, AndroidBitmapInfo* info);
int AndroidBitmap_lockPixels(JNIEnv**, AndroidBitmap* p, void** addrPtr);
int AndroidBitmap_unlockPixels(JNIEnv**, jobject);
