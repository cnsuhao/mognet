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
