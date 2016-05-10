package com.buffer.mirror;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MirrorSurfaceView extends GLSurfaceView {

  private MirrorRender mRender = null;

  public MirrorSurfaceView(Context context, AttributeSet attrs) {
    super(context, attrs);
    setEGLContextClientVersion(2); // GLES 2.0
    mRender = new MirrorRender(context, this);
    setEGLConfigChooser(8, 8, 8, 8, 16, 0);
    setRenderer(mRender);
    setRenderMode(RENDERMODE_WHEN_DIRTY);
  }

}
