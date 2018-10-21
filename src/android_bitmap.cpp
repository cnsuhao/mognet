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
