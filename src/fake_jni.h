#pragma once

// Define JNI types used by the project to allow documentation parity
// (i.e.: void* is more confusing than jobject)
typedef signed char jbyte;
typedef int jsize;
typedef jsize jint;
typedef unsigned char jboolean;
typedef void* jobject;
typedef jobject jstring;

// Since only the following functions are used by the JNI interface,
// we fill the unused space to properly align the functions required
struct JNIEnv {
  int a[0xa9];

  const char* (*GetStringUTFChars)(JNIEnv**, jstring, jboolean*);
  void (*ReleaseStringUTFChars)(JNIEnv**, jstring, const char*);

  int b[0x5];

  void* (*NewByteArray)(JNIEnv**, jsize);

  int c[0x1f];

  void (*SetByteArrayRegion)(JNIEnv**, void*, jsize, jsize, const jbyte*);
};

struct FakeJNI {
private:
  JNIEnv env;

public:
  static FakeJNI instance;

  FakeJNI();

  JNIEnv* getFakeEnvironment() { return &env; }
};
