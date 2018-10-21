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

#include "fake_jni.h"

#include <string>

FakeJNI FakeJNI::instance;

FakeJNI::FakeJNI() {
  env.GetStringUTFChars = [](JNIEnv**, jstring string,
                             jboolean*) -> const char* {
    return (const char*) string;
  };

  env.ReleaseStringUTFChars = [](JNIEnv**, jstring, const char*) {
    // Since the string is opaque to the program, we just pass a string as the
    // string object, therefore, we do not need to free it
  };

  env.NewByteArray = [](JNIEnv**, jsize size) -> void* {
    return (void*) new std::string(size, 0);
  };

  env.SetByteArrayRegion = [](JNIEnv**, void* ret, jsize, jsize l,
                              const jbyte* data) {
    std::string* s = (std::string*) ret;
    s->assign(&data[0], &data[0] + l);
  };
}
