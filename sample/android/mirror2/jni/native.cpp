#include <android/native_window_jni.h>

#include <vector>

#include "glesbox.hpp"
#include "yuvimage.hpp"
#include "libyuv.h"

#include "util.h"

#define TAG "com.buffer.morror2"

// render viewport size
int m_rWidth = 1080;
int m_rHeight = 1920;
int camera_w = 0;
int camera_h = 0;
const int channel = 3;

libgb::GlesBox engine;
libgb::YuvImage render;
std::vector<uint8_t> texture_data;

//-------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

//-------------------------------------------------------------------------------------------------

JNIEXPORT void JNICALL Java_com_buffer_mirror2_CameraView_render
  (JNIEnv* env, jobject thisObj, jbyteArray imageData, jint width, jint height) {
	if (camera_w != width || camera_h != height) {
    texture_data.resize(width * height * channel);
    camera_w = width;
    camera_h = height;
  }

	jbyte* nv21 = env->GetByteArrayElements(imageData, JNI_FALSE);
  unsigned char* i420 = texture_data.data();

	libyuv::NV21ToI420Rotate((unsigned char*)nv21, width,
			(unsigned char*)nv21 + width*height, width,
			i420, height,
			i420 + height * width, height >> 1,
			i420 + height * width * 5 / 4, height >> 1,
			width, -height, libyuv::kRotate270);

	env->ReleaseByteArrayElements(imageData, nv21, JNI_FALSE);
}

JNIEXPORT void JNICALL Java_com_buffer_mirror2_MirrorSurfaceView_initRender
  (JNIEnv* env, jobject thisObj, jint render_width, jint render_height) {
	// init render viewport size
	m_rWidth = render_width;
	m_rHeight = render_height;
	LOGD(TAG, "[render size] width: %d, height: %d", render_width, render_height);
}

JNIEXPORT void JNICALL Java_com_buffer_mirror2_MirrorSurfaceView_renderFrame
  (JNIEnv* env, jobject thisObj, jobject surface) {
  if (camera_w == 0 || camera_h == 0) //not ready
    return;
    
  libgb::GBConfig conf;
  conf.type = libgb::GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT;
  conf.screen_native_id = (unsigned long)ANativeWindow_fromSurface(env, surface);
  conf.screen_x = 0;
  conf.screen_y = 0;
  conf.screen_width = m_rWidth;
  conf.screen_height = m_rHeight;
  conf.screen_angle = 0.0f;
  engine.draw_begin(conf);
  render.setTextData(camera_h, camera_w, 1, texture_data.data()); //rotated
  render.draw(conf);
  engine.draw_end(conf);
}

#ifdef __cplusplus
}
#endif
