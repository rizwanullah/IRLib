#include <jni.h>
#include <string>
#include "RemoteFinder.h"
#include "Encoder.h"
#include "Decoder.h"

extern "C"
JNIEXPORT jint JNICALL Java_com_curiologix_pelirlib_IRManager_findRemoteIdForAcModel(JNIEnv *env, jobject, jstring acModelStr) {
    jboolean isCopy;
    RemoteFinder *remoteFinder = new RemoteFinder();
    int32_t remoteId = remoteFinder->findRemoteId(std::string(env->GetStringUTFChars(acModelStr, &isCopy)));
    return remoteId;
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_curiologix_pelirlib_IRManager_encodeRemoteIrData(JNIEnv *env, jobject, jstring remoteJsonStr) {
    jboolean isCopy;
    Encoder *encoder = new Encoder();
    std::string encodedIrJsonStr = encoder->encode(std::string(env->GetStringUTFChars(remoteJsonStr, &isCopy)));
    return env->NewStringUTF(encodedIrJsonStr.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL Java_com_curiologix_pelirlib_IRManager_decodeRemoteIrData(JNIEnv *env, jobject, jstring encodedIrJsonStr) {
    jboolean isCopy;
    Decoder *decoder = new Decoder();
    std::string remoteIrJsonStr = decoder->decode(std::string(env->GetStringUTFChars(encodedIrJsonStr, &isCopy)));
    return env->NewStringUTF(remoteIrJsonStr.c_str());
}
