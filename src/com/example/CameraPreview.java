package com.example;

public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback, Camera.PreviewCallback {
    static {
        System.loadLibrary("JNIProcessor");
    }
    
    private void gaussianBlur(Bitmap out, byte[] in, int width, int height);

    private Bitmap mTexture;
    private int width, height;

    //TODO: constructors (setup configuration of mTexture like width and height)
    //
    //TODO: Code for surfaceCreated(), surfaceChanged(), surfaceDestroyed()
    //
    public void onPreviewFrame(byte[] data, Camera camera) {
        gaussianBlur(mTexture,data,width,height);
        invalidate();
    }

    //TODO: Code onDraw() method that can draw the processed bitmap on to canvas
}
