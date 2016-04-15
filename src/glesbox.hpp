// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include <stdint.h>

#ifdef _WIN32
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "EGL/egl.h"
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libyuv.lib")
#elif _IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "EGL/egl.h"
#elif _ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#endif

#include "common.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

//-------------------------------------------------------------------------------------------------
typedef enum GBImageType {
  GB_IMAGE_UNKNOWN = 0,
  GB_IMAGE_YUV420,
  GB_IMAGE_RGB24,
  GB_IMAGE_RGBA32,
  GB_IMAGE_YUY2,
}GBImageType;

typedef enum GBDrawType {
  GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT,
  GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT,
  GB_DRAW_OFFLINE_WITHOUT_OPENGLES_CONTEXT,
  GB_DRAW_OFFLINE_WITH_OPENGLES_CONTEXT,
  GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT,
  GB_DRAW_BOTH_WITH_OPENGLES_CONTEXT
}GBDrawType;

typedef struct GBConfig {
  GBDrawType type;

  //screen render
  //native windows handle, got by:
  //  Windows OS: GetActiveWindow();
  //  Android OS: ANativeWindow_fromSurface();
  //  iOS-swift: unsafeAddressOf(UIView);
  //if screen has init OpenglES context itself, it should be 0;
  uint64_t screen_native_id;
  uint32_t screen_x;
  uint32_t screen_y;
  uint32_t screen_width;
  uint32_t screen_height;
  float screen_angle;

  //off-render
  uint32_t offline_width;
  uint32_t offline_height;
  uint32_t offline_widthstep; //bytes in each line
  uint32_t offline_channel;
  uint8_t* offline_data;
  float offline_angle;
  GBImageType offline_type;
}GBConfig;

//-------------------------------------------------------------------------------------------------
class GlesBox {
  DISALLOW_COPY_AND_ASSIGN(GlesBox);
public:
  GlesBox();
  ~GlesBox();

  bool draw_begin(const GBConfig& conf);
  bool draw_end(GBConfig& conf);

private:
  bool bindEGLContext(uint32_t width, uint32_t height, uint64_t native_windows_id);
  void unbindEGLContext();
  bool bindFrameBuffer(uint32_t width, uint32_t height);
  void unbindFrameBuffer();
  bool createSurface(uint32_t width, uint32_t height, uint64_t native_windows_id);
  bool createFrameBuffer(uint32_t width, uint32_t height);
  void releaseFrameBuffer();

  const uint8_t* readFromGPU();
  bool swap(const float angle);

  const bool hasEgl() const {
    return display_ != nullptr;
  }

private:
  uint32_t width_;
  uint32_t height_;
  EGLDisplay display_;
  EGLSurface surface_;
  EGLContext context_;
  EGLConfig config_;

  GLint viewport_old_[4];
  GLuint framebuffer_old_;
  GLuint framebuffer_;
  GLuint texture_render_;
  GLuint depth_render_;
  uint8_t *frame_data_;
};

//-------------------------------------------------------------------------------------------------
} //! namespace libgb