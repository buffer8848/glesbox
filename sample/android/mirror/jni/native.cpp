#include <vector>

#include "glesbox.hpp"
#include "yuvimage.hpp"
#include "libyuv.h"

#include "util.h"

#define TAG "com.buffer.morror"

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

class JniCallBack {
public:
	static JniCallBack& getInstance();

	void setRenderCallback(jobject obj, jmethodID method);
	void getRenderCallback(jobject& obj, jmethodID& method);

private:
	typedef struct _Callback {
		_Callback() {
			obj = NULL;
			method = NULL;
		};
		~_Callback() {
			if (obj != NULL) {
			    JNIEnv * env;
			    ATTACH_JVM(env);
			    env->DeleteGlobalRef(obj);
			    DETACH_JVM();
			}
		}
		jobject obj;
		jmethodID method;
	}Callback, *PCallback;

	Callback renderCallback;
};

JniCallBack& JniCallBack::getInstance() {
  static JniCallBack instance;
  return instance;
}

void JniCallBack::setRenderCallback(jobject obj, jmethodID method) {
	renderCallback.obj = obj;
	renderCallback.method = method;
}

void JniCallBack::getRenderCallback(jobject& obj, jmethodID& method) {
	obj = renderCallback.obj;
	method = renderCallback.method;
}

void requestRender() {
  JNIEnv* env;
  jobject obj;
  jmethodID method;

  ATTACH_JVM(env);
  JniCallBack::getInstance().getRenderCallback(obj, method);
  env->CallVoidMethod(obj, method);
  DETACH_JVM();
}

JavaVM* android_jvm;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
  android_jvm = vm;
  return JNI_VERSION_1_6;
}

//-------------------------------------------------------------------------------------------------

JNIEXPORT void JNICALL Java_com_buffer_mirror_CameraView_render
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

	requestRender();
}

JNIEXPORT void JNICALL Java_com_buffer_mirror_MirrorRender_initRender
  (JNIEnv* env, jobject thisObj, jint render_width, jint render_height) {
	// init render callback
	jclass renderClass = env->GetObjectClass(thisObj);
	jmethodID requestRenderMethod = env->GetMethodID(renderClass, "requestRender", "()V");
	jobject newObj = env->NewGlobalRef(thisObj);
	JniCallBack::getInstance().setRenderCallback(newObj, requestRenderMethod);

	// init render viewport size
	m_rWidth = render_width;
	m_rHeight = render_height;
	LOGD(TAG, "[render size] width: %d, height: %d", render_width, render_height);
}

JNIEXPORT void JNICALL Java_com_buffer_mirror_MirrorRender_renderFrame
  (JNIEnv* env, jobject thisObj) {
  libgb::GBConfig conf;
  conf.type = libgb::GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT;
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
