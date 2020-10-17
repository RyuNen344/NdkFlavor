#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_ryunen344_ndkflavor_FromNdk_getString(JNIEnv *env, jobject) {
    std::string str = "Flavor1 From Ndk";
    return env->NewStringUTF(str.c_str());
}
