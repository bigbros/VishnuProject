package com.madtape.vishnuproject;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.annotation.SuppressLint;
import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

@SuppressLint("ClickableViewAccessibility")
public class GLES2View
extends GLSurfaceView
implements GLSurfaceView.Renderer
{

	private String mDataPath = null;
	private long mNativeResource = 0;
	private boolean mSurfaceReady = false;
	private boolean mIsContinue = false;
	
	public GLES2View(Context context, String dataPath) {
		super(context);
		mDataPath = dataPath;
		long resource = getContextNdk();
		if(resource == 0) {
			mNativeResource = setEnvironmentNdk(4 * 1024 * 1024, dataPath);
		} else {
			mIsContinue = true;
			mNativeResource = resource;
		}
		this.setEGLContextClientVersion(2);
		setRenderer(this);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		int width = this.getWidth();
		int height = this.getHeight();
		if(!mIsContinue) {
			onSurfaceCreatedNdk(mNativeResource, width, height);
		} else {
			onSurfaceNdk(mNativeResource);
		}
		mSurfaceReady = true;
	}
	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if(mSurfaceReady) onSurfaceChangedNdk(mNativeResource, width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		onDrawFrameNdk(mNativeResource);
	}
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int idx = event.getActionIndex();
		float x = event.getX(idx);
		float y = event.getY(idx);
		float force = event.getPressure(idx);
		int id = event.getPointerId(idx);
		switch(event.getAction()) {
		case MotionEvent.ACTION_DOWN: onTouchEventNdk(mNativeResource, 0, id, x, y, force);	break;
		case MotionEvent.ACTION_MOVE: onTouchEventNdk(mNativeResource, 1, id, x, y, force); break;
		case MotionEvent.ACTION_UP:	  onTouchEventNdk(mNativeResource, 2, id, x, y, force);	break;
			default: break;
		}
		return true;
	}

	@Override
	public void onPause() {
		onPauseNdk(mNativeResource);
	}
	
	@Override
	public void onResume() {
		onResumeNdk(mNativeResource);
	}
	/*
	@Override
	public void onDetachedFromWindow() {
//		onDetachNdk(mNativeResource);
		mSurfaceReady = false;
	}
	*/
	public void Restart() {
		// 現在の環境を破棄
//		long ptr = mNativeResource;
//		mNativeResource = 0;
//		disposeNdk(ptr);
		
		// ゲーム側環境を作り直す
//		mNativeResource = setEnvironmentNdk(4 * 1024 * 1024, mDataPath);
//		int width = this.getWidth();
//		int height = this.getHeight();
//		onSurfaceCreatedNdk(mNativeResource, width, height);
	}
	private native long getContextNdk();
	private native void disposeNdk(long ptr);
	private native long setEnvironmentNdk(int heapSize, String dataPath); 
	private native void onSurfaceCreatedNdk(long ptr, int width, int height);
	private native void onSurfaceChangedNdk(long ptr, int width, int height);
	private native void onDrawFrameNdk(long ptr);
	private native void onTouchEventNdk(long ptr, int type, int id, float x, float y, float force);
	private native void onPauseNdk(long ptr);
	private native void onResumeNdk(long ptr);
	private native void onSurfaceNdk(long ptr);
	private native void onDetachNdk(long ptr);

	static {
		System.loadLibrary("VishnuProject");		
	}
}
