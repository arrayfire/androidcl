package com.example;

import java.io.IOException;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ImageFormat;
import android.graphics.Paint;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.hardware.Camera;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback, Camera.PreviewCallback {
    static {
        System.loadLibrary("JNIProcessor");
    }

	private static final String TAG = "CameraPreview";

    private Camera 	mCamera;
    private byte[] 	mVideoSource;
    private Bitmap 	mBackBuffer;
    private int[]	mImgDims;
    private Paint 	mPaint;
    private Rect	mSrcRect;
    private Rect	mTrgtRect;
    private int     mChoice;

    native private void runfilter(Bitmap out, byte[] in, int width, int height, int choice);

    public CameraPreview(Context context) {
        super(context);
        getHolder().addCallback(this);
        setWillNotDraw(false);
        mImgDims 	= new int[3];
        mImgDims[2] = 4;
        mPaint 		= new Paint();
        mPaint.setTextSize(64);
        mPaint.setColor(0xFFFF0000);
    }

    public void surfaceCreated(SurfaceHolder pHolder) {
        try {
            mCamera = Camera.open(0);
            Camera.Parameters params = mCamera.getParameters();
            params.setPreviewFormat(ImageFormat.NV21);
            params.setPreviewSize(Constants.MAX_DISP_IMG_WIDTH,
                    Constants.MAX_DISP_IMG_HEIGHT);
            params.setPreviewFpsRange(Constants.MIN_FPS,Constants.MAX_FPS);
            mCamera.setParameters(params);
            mCamera.setDisplayOrientation(0);
            mCamera.setPreviewDisplay(null);
            mCamera.setPreviewCallbackWithBuffer(this);
        } catch (IOException eIOException) {
            Log.i(TAG, "Error setting camera preview: " + eIOException.getMessage());
            throw new IllegalStateException();
        }
        catch (Exception e) {
            Log.i(TAG, e.getMessage());
        }
    }

    public void surfaceChanged(SurfaceHolder pHolder, int pFormat, int pW, int pH) {

    	if (pHolder.getSurface() == null) {
    		Log.i(TAG,"No proper holder");
            return;
        }
        try {
            mCamera.stopPreview();
        } catch (Exception e) {
        	Log.i(TAG,"tried to stop a non-existent preview");
        	return;
        }
        PixelFormat pxlFrmt = new PixelFormat();
        PixelFormat.getPixelFormatInfo(mCamera.getParameters().getPreviewFormat(), pxlFrmt);
        int srcSize 		= Constants.MAX_DISP_IMG_WIDTH * Constants.MAX_DISP_IMG_HEIGHT * pxlFrmt.bitsPerPixel/8;
        mVideoSource        = new byte[srcSize];
        mBackBuffer         = Bitmap.createBitmap(Constants.MAX_DISP_IMG_WIDTH,
        		Constants.MAX_DISP_IMG_HEIGHT,Bitmap.Config.ARGB_8888);
    	Camera.Parameters camParams = mCamera.getParameters();
    	camParams.setPreviewSize(Constants.MAX_DISP_IMG_WIDTH,
                Constants.MAX_DISP_IMG_HEIGHT);
        camParams.setPreviewFormat(ImageFormat.NV21);
        camParams.setPreviewFpsRange(Constants.MIN_FPS,Constants.MAX_FPS);
        mCamera.setParameters(camParams);

        mImgDims[0] = Constants.MAX_DISP_IMG_WIDTH;
        mImgDims[1] = Constants.MAX_DISP_IMG_HEIGHT;

        mSrcRect	= new Rect(0,0,mImgDims[0],mImgDims[1]);
        mTrgtRect	= pHolder.getSurfaceFrame();

        mCamera.addCallbackBuffer(mVideoSource);

        try {
            mCamera.setPreviewDisplay(pHolder);
            mCamera.startPreview();
        } catch (Exception e){
            Log.d(TAG, "@SurfaceChanged:Error starting camera preview: " + e.getMessage());
        }
    }

    public void setProcessedPreview(int choice) {
    	mChoice = choice;
    }

    public void onPreviewFrame(byte[] data, Camera camera) {
    	try {
            runfilter(mBackBuffer,data,mImgDims[0],mImgDims[1],mChoice);
    	} catch(Exception e) {
    		Log.i(TAG, e.getMessage());
    	}
        invalidate();
    }

    @Override
    protected void onDraw(Canvas pCanvas) {
        if( mCamera != null ) {
            if( mBackBuffer!=null ) {
            	pCanvas.drawBitmap(mBackBuffer, mSrcRect, mTrgtRect, null);
            	pCanvas.drawText("nv21 to rgba", 64, 64, mPaint);
            }
			mCamera.addCallbackBuffer(mVideoSource);
        }
    }

	private void releaseCamera(){
        if (mCamera != null){
            mCamera.release();
            mCamera = null;
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
    	if (mCamera != null) {
            mCamera.stopPreview();
            mCamera.setPreviewCallback(null);
            releaseCamera();
            mVideoSource = null;
            mBackBuffer = null;
        }
    }

}
