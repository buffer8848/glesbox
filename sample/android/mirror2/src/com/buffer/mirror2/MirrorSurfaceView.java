package com.buffer.mirror2;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class MirrorSurfaceView extends SurfaceView
  implements Runnable, SurfaceHolder.Callback {

  public MirrorSurfaceView(Context context, AttributeSet attrs) {
    super(context, attrs);
    getHolder().addCallback(this);
  }

  @Override
  public void run() {
    while(true){
      if(getHolder().getSurface().isValid()){
        //Canvas canvas = getHolder().lockCanvas();
        renderFrame(this.getHolder().getSurface());
        //getHolder().unlockCanvasAndPost(canvas);
      }
     }
  }

  @Override
  public void surfaceChanged(SurfaceHolder holder, int format, int width,
      int height) {
    // TODO Auto-generated method stub
    
  }

  @Override
  public void surfaceCreated(SurfaceHolder holder) {
    initRender(this.getWidth(), this.getHeight());
    new Thread(this).start();
  }

  @Override
  public void surfaceDestroyed(SurfaceHolder holder) {
    // TODO Auto-generated method stub
    
  }

  private native void initRender(int render_width, int render_height);

  private native void renderFrame(Surface surface);
}
