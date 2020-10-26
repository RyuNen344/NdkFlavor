#include <jni.h>
#include <string>
#include "sha256.h"

#define SIGN "863F9F34C37F661AF3F6449A1534CDC5E45DC6AFC31BA4716898BC9A8F99FDF"

jobjectArray getSignatures(JNIEnv *env, jobject context, int buildVersion);
void hashByteArray(const void* data, size_t numBytes, char* resultData);
void formatSignature(char* data, char* resultData);

extern "C" JNIEXPORT jstring JNICALL
Java_com_ryunen344_ndkflavor_FromNdk_getString(JNIEnv *env, jobject) {
    std::string str = "Flavor1 From Ndk";
    return env->NewStringUTF(str.c_str());
}

extern "C" JNIEXPORT jint JNICALL
Java_com_ryunen344_ndkflavor_FromNdk_getSigFromNdk(JNIEnv *env, jobject, int buildVersion) {

    jclass jAppAppGlobalsClass = env->FindClass("android/app/AppGlobals");
    jmethodID jGetInitialApplication =
            env->GetStaticMethodID(
                    jAppAppGlobalsClass,
                    "getInitialApplication",
                    "()Landroid/app/Application;"
                    );
    jobject jApplicationObject = env->CallStaticObjectMethod(jAppAppGlobalsClass, jGetInitialApplication);

    jobjectArray cert_objectArray = getSignatures(env, jApplicationObject, buildVersion);
    int size = (int)env->GetArrayLength(cert_objectArray);

    for(int i = 0; i < size; i++) {
        jobject signature = env->GetObjectArrayElement(cert_objectArray, i);
        jclass signature_clazz = env->GetObjectClass(signature);
        jmethodID methodID_byteArray = env->GetMethodID(signature_clazz, "toByteArray", "()[B");
        jobject cert_obj = env->CallObjectMethod(signature, methodID_byteArray);
        jbyteArray cert_byteArray = static_cast<jbyteArray>(cert_obj);
        jsize certSize = env->GetArrayLength(cert_byteArray);
        jbyte* jbyteArray = new jbyte[certSize];
        env->GetByteArrayRegion(cert_byteArray, 0, certSize, jbyteArray);

        char certSHA256[128] = {0};
        hashByteArray(jbyteArray, certSize, certSHA256);
        if (strcmp(certSHA256, SIGN) == 0) {
            return certSize;
        }
    }

    return -size;
}

jobjectArray getSignatures(JNIEnv *env, jobject context, int buildVersion) {
    jclass context_clazz = env->GetObjectClass(context);

    jmethodID methodID_getPackageManager = env->GetMethodID(context_clazz, "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManager = env->CallObjectMethod(context, methodID_getPackageManager);
    jclass packageManager_clazz = env->GetObjectClass(packageManager);

    jmethodID methodID_getPackageInfo = env->GetMethodID(packageManager_clazz,"getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jmethodID methodID_getPackageName = env->GetMethodID(context_clazz,"getPackageName", "()Ljava/lang/String;");

    jobject application_package_obj = env->CallObjectMethod(context, methodID_getPackageName);
    jstring application_package = static_cast<jstring>(application_package_obj);
    const char* package_name = env->GetStringUTFChars(application_package, 0);

    jobjectArray signature_arr;
    if (buildVersion >= 28) {
        jobject packageInfo = env->CallObjectMethod(packageManager, methodID_getPackageInfo, application_package, 134217728);
        jclass packageinfo_clazz = env->GetObjectClass(packageInfo);
        jfieldID signingInfo_signatures = env->GetFieldID(packageinfo_clazz, "signingInfo", "Landroid/content/pm/SigningInfo;");
        jobject signingInfo = env->GetObjectField(packageInfo, signingInfo_signatures);
        jclass signingInfo_clazz = env->GetObjectClass(signingInfo);

        jmethodID methodID_hasMultipleSigners = env->GetMethodID(signingInfo_clazz, "hasMultipleSigners", "()Z");
        bool hasMultipleSigners = env->CallBooleanMethod(signingInfo, methodID_hasMultipleSigners);
        if (hasMultipleSigners) {
            jmethodID methodID_contentsSigners = env->GetMethodID(signingInfo_clazz, "getApkContentsSigners", "()[Landroid/content/pm/Signature;");
            signature_arr = (jobjectArray)env->CallObjectMethod(signingInfo, methodID_contentsSigners);
        } else {
            jmethodID methodID_certificateHistory = env->GetMethodID(signingInfo_clazz, "getSigningCertificateHistory", "()[Landroid/content/pm/Signature;");
            signature_arr = (jobjectArray)env->CallObjectMethod(signingInfo, methodID_certificateHistory);
        }
    } else {
        jobject packageInfo = env->CallObjectMethod(packageManager, methodID_getPackageInfo, application_package, 64);
        jclass packageinfo_clazz = env->GetObjectClass(packageInfo);
        jfieldID fieldID_signatures = env->GetFieldID(packageinfo_clazz, "signatures", "[Landroid/content/pm/Signature;");
        signature_arr = (jobjectArray)env->GetObjectField(packageInfo, fieldID_signatures);
    }

    return signature_arr;
}

void hashByteArray(const void* data, size_t numBytes, char* resultData){
    SHA256 sha256;
    std::string sha256String = sha256(data, numBytes);
    char* tabStr = new char [sha256String.length()+1];
    strcpy(tabStr, sha256String.c_str());
    formatSignature(tabStr, resultData);
}

void formatSignature(char* data, char* resultData) {
    int length = strlen(data);
    for(int i = 0; i < length; i++) {
        resultData[i] = static_cast<char>(toupper(data[i]));
    }
}
