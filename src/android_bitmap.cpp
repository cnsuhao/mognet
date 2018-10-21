#include "android_bitmap.h"

#include <cstring>

int AndroidBitmap_getInfo(JNIEnv**, AndroidBitmap* p, AndroidBitmapInfo* info) {
  std::memcpy(info, p->first, sizeof(AndroidBitmapInfo));
  return 0;
}

int AndroidBitmap_lockPixels(JNIEnv**, AndroidBitmap* p, void** addrPtr) {
  *addrPtr = p->second; // FIXME nullptr check
  return 0;
}

int AndroidBitmap_unlockPixels(JNIEnv**, jobject) { return 0; }
