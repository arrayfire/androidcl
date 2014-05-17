#include "processor.h"
#include
#include
#include

bool throwJavaException(JNIEnv *env,std::string method_name,std::string exception_msg, int errorCode=0)
{
    char buf[8];
    sprintf(buf,"%d",errorCode);
    std::string code(buf);

    std::string msg = "@" + method_name + ": " + exception_msg + " ";
    if(errorCode!=0) msg += code;

    jclass generalExp = env-&gt;FindClass("java/lang/Exception");
    if (generalExp != 0) {
        env-&gt;ThrowNew(generalExp, msg.c_str());
        return true;
    }
    return false;
}

void blur_helper(uint32_t* out, int osize, uint8_t* in, int isize, int w, int h)
{
    uint32_t* tmp = new uint32_t[osize];
    convertNV21toRGBA8888(tmp,in,w,h);
    gblur(out,tmp,w,h);
    delete[] tmp;
}

JNIEXPORT void JNICALL Java_com_example_CameraPreview_gaussianBlur(
        JNIEnv *env,
        jclass clazz,
        jobject outBmp,
        jByteArray inData,
        jint width,
        jint height)
{
    int outsz = width*height;
    int insz = outsz + outsz/2;
    AndroidBitmapInfo bmpInfo;
    if (AndroidBitmap_getInfo(env, outBmp, &amp;bmpInfo) &lt; 0) {
        throwJavaException(env,"gaussianBlur","Error retrieving bitmap meta data");
        return;
    }
    if (bmpInfo.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        throwJavaException(env,"gaussianBlur","Expecting RGBA_8888 format");
        return;
    }
    uint32_t* bmpContent;
    if (AndroidBitmap_lockPixels(env, outBmp,(void**)&amp;bmpContent) &lt; 0) { throwJavaException(env,"gaussianBlur","Unable to lock bitmap pixels"); return; } jbyte* inPtr = (jbyte*)env-&gt;GetPrimitiveArrayCritical(inData, 0);
    if (inPtr == NULL) {
        throwJavaException(env,"gaussianBlur","NV21 byte stream getPointer returned NULL");
        return;
    }

    blur_helper(bmpContent,outsz,inPtr,insz,width,height);

    // This is absolutely neccessary before calling any other JNI function
    env-&gt;ReleasePrimitiveArrayCritical(inData,inPtr,0);
    AndroidBitmap_unlockPixels(env, outBmp);
}

