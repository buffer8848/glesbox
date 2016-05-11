package com.buffer.mirror2;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.Window;
import android.view.WindowManager;
import android.widget.RelativeLayout;

import com.buffer.mirror2.R;

public class MirrorActivity extends Activity {

  static {
    System.loadLibrary("mirror");
  }

  @Override
  public boolean onCreateOptionsMenu(Menu menu) {
    // Inflate the menu; this adds items to the action bar if it is present.
    getMenuInflater().inflate(R.menu.beauty, menu);
    return true;
  }

  @Override
  public boolean onOptionsItemSelected(MenuItem item) {
    // Handle action bar item clicks here. The action bar will
    // automatically handle clicks on the Home/Up button, so long
    // as you specify a parent activity in AndroidManifest.xml.
    int id = item.getItemId();
    if (id == R.id.action_settings) {
      return true;
    }
    return super.onOptionsItemSelected(item);
  }

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    
  	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
  	    WindowManager.LayoutParams.FLAG_FULLSCREEN); // full screen
  	requestWindowFeature(Window.FEATURE_NO_TITLE); // no title
  	
  	setContentView(R.layout.activity_mirror);
  	RelativeLayout layout = (RelativeLayout)findViewById(R.id.container);
  	layout.addView(new MirrorSurfaceView(this, null));
  }

  @Override
  protected void onDestroy() {
    super.onDestroy();
  }

  @Override
  protected void onPause() {
    super.onPause();
  }

  @Override
  protected void onStop() {
    super.onStop();
  }
  
  @Override
  protected void onResume() {
    super.onResume();
  }
}
