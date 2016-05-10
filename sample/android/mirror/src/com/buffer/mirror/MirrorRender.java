package com.buffer.mirror;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;

public class MirrorRender implements GLSurfaceView.Renderer {

  private GL10 contextGL = null;
  private Context context = null;
  private GLSurfaceView contextView = null;

  public MirrorRender(Context context, GLSurfaceView contextView) {
    this.context = context;
    this.contextView = contextView;
  }

  @Override
  public void onDrawFrame(GL10 gl) {
    GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT);
    renderFrame();
  }

  @Override
  public void onSurfaceChanged(GL10 gl, int width, int height) {
    GLES20.glViewport(0, 0, width, height);
  }

  @Override
  public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    contextGL = gl;
    initRender(this.contextView.getWidth(), this.contextView.getHeight());
  }

  /**
   * Invoked from native code
   */
  public void requestRender() {
    if (null != contextView) {
      contextView.requestRender();
    }
  }

  private native void initRender(int render_width, int render_height);

  private native void renderFrame();
}
