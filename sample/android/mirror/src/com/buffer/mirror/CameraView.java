package com.buffer.mirror;

import java.io.IOException;
import java.util.List;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;
import android.util.AttributeSet;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class CameraView extends SurfaceView implements SurfaceHolder.Callback {
	private final static String TAG = "CameraView";
	private static int DEF_DISPLAY_ROTATION = 90; // degree
	
	private SurfaceHolder mHolder;
	private Camera mCamera;
	
	private Size mPreviewSize;
	private List<Size> mSupportedPreviewSizes;
	
	private byte[] mPreviewData = new byte[640*480*3/2];
	
	public CameraView(Context context, AttributeSet set) {
		super(context, set);
		mCamera = null;
		mHolder = this.getHolder();
		mHolder.addCallback(this);
	}
	
	public void startCamera() {
		try {
			mCamera = Camera.open(1);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		if (mCamera != null) {
			mCamera.addCallbackBuffer(mPreviewData);
			mCamera.setPreviewCallbackWithBuffer(mPreviewCallback);
			this.requestLayout();
		}
	}
	
	public void stopCamera() {
		if (null != mCamera) {
			mCamera.stopPreview();
			mCamera.setPreviewCallbackWithBuffer(null);
			mCamera.release();
			mCamera = null;
		}
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		Log.d(TAG, "CameraView surface Created");
		mHolder = holder;
		startCamera();
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
		if (null == holder.getSurface()) {
			return;
		}
		mHolder = holder;
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		Log.d(TAG, "CameraView surface Destroyed");
		mHolder = null;
		stopCamera();
	}
	
	public void setCameraDisplayOrientation(Camera camera, int degree) {
		try {
			camera.setDisplayOrientation(degree);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	@Override
	protected void onLayout(boolean changed, int l, int t, int r, int b) {
		if (mCamera == null) {
			return;
		}
		int width = r - l;
		int height = b - t;
		mSupportedPreviewSizes = mCamera.getParameters().getSupportedPreviewSizes();
		if (mSupportedPreviewSizes != null) {
			mPreviewSize = getOptimalPreviewSize(mSupportedPreviewSizes, width, height);
			if (null == mPreviewSize || null == mCamera || null == mHolder) {
				return;
			}
			setCameraParam();
			try {
				mCamera.setPreviewDisplay(mHolder);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			setCameraDisplayOrientation(mCamera, DEF_DISPLAY_ROTATION);
			mCamera.startPreview();
		}
	}

	private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
		Size targetSize = null;
	
		double refRatio = (double) Math.min(w, h) / Math.max(w, h);
		double diff = 1;
		double tmpDiff = 0;
	
		for (Size size : sizes) {
			tmpDiff = Math.abs(refRatio - ((double) Math.min(size.width, size.height) / Math.max(size.width, size.height)));
			if (tmpDiff < diff) {
				diff = tmpDiff;
				targetSize = size;
			}
		}
	
		return targetSize;
	}
	
	private void setCameraParam() {
		Camera.Parameters param = mCamera.getParameters();
		param.setPreviewFormat(ImageFormat.NV21); 

        int range[] = new int[2];
        param.getPreviewFpsRange(range);
        
        // detect device type for s5
        if (android.os.Build.MODEL.equals("SM-G9006V")) {
        	param.setPreviewFpsRange(25000, range[1]);
		} else {
			param.setPreviewFpsRange(range[1], range[1]);
		}
        
        Log.d(TAG, "camera fps: " + range[0] + ", " + range[1]);

		mPreviewSize.width = 640;
		mPreviewSize.height = 480;
		param.setPreviewSize(mPreviewSize.width, mPreviewSize.height);

		try {
			mCamera.setParameters(param);
		} catch (Exception e) {
			Log.d(TAG, "set camera parameter error:" + e);
			param.setPreviewFpsRange(range[0], range[1]);
			try {
				mCamera.setParameters(param);
			} catch (Exception e2) {
				Log.e(TAG, "set camera parameter error:" + e2);
			}
		}
	}

	private PreviewCallback mPreviewCallback = new PreviewCallback() {
		
		@Override
		public void onPreviewFrame(byte[] data, Camera camera) {
			if (null != data)
			{
				// send yuv pixel image to opengl render
				render(data, mPreviewSize.width, mPreviewSize.height);
				mCamera.addCallbackBuffer(data);
			}
		}
	};
	
	private native void render(byte[] data, int width, int height);
}
