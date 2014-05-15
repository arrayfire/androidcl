#ifndef __JNI_H__
#define __JNI_H__
#ifdef __cplusplus

#include
#include &lt;android/bitmap.h&gt; // This header will enable us to read bitmap objects
#include &lt;android/log.h&gt; // This header will enable us to print messages from jni functions

#define app_name "JNIProcessor"
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, app_name, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, app_name, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, app_name, __VA_ARGS__))
extern "C" {
#endif

#define DECLARE(returnType,fullClassName,func,...) \
JNIEXPORT returnType JNICALL Java_##fullClassName##_##func(JNIEnv *env, jclass clazz,__VA_ARGS__);

DECLARE(void,com_example_CameraPreview,gaussianBlur,jobject outBmp, jbyteArray inData, jint width, jint height)

#ifdef __cplusplus
}
#endif

#endif //__JNI_H__
