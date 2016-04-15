// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include "glesbox.hpp"

#include <math.h>

#include "libyuv.h"

//-------------------------------------------------------------------------------------------------
namespace libgb {

GlesBox::GlesBox() : 
  width_(0), height_(0), display_(nullptr), surface_(nullptr),
  context_(nullptr), config_(nullptr), framebuffer_old_(0),
  framebuffer_(0), texture_render_(0), depth_render_(0), frame_data_(nullptr) {
}

GlesBox::~GlesBox() {
  if (hasEgl())
    bindEGLContext(width_, height_, 0);
  releaseFrameBuffer();

  if (surface_ != nullptr)
    eglDestroySurface(display_, surface_);
  surface_ = nullptr;
  if (context_ != nullptr)
    eglDestroyContext(display_, context_);
  context_ = nullptr;
  display_ = nullptr;
  if (hasEgl())
    unbindEGLContext();
  width_ = 0;
  height_ = 0;
}

bool GlesBox::draw_begin(const GBConfig& conf) {
  uint64_t native_windows_id = 0;
  uint32_t width(0), height(0);
  bool need_egl = false, need_fbo = false;
  switch(conf.type) {
  case GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT:
    break;
  case GB_DRAW_OFFLINE_WITH_OPENGLES_CONTEXT:
  case GB_DRAW_BOTH_WITH_OPENGLES_CONTEXT:
    need_fbo = true;
    width = conf.offline_width;
    height = conf.offline_height;
    break;
  case GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT:
    width = conf.screen_width;
    height = conf.screen_height;
    need_egl = true;
    native_windows_id = conf.screen_native_id;
    break;
  case GB_DRAW_OFFLINE_WITHOUT_OPENGLES_CONTEXT:
  case GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT:
    need_fbo = true;
    need_egl = true;
    width = conf.offline_width;
    height = conf.offline_height;
    native_windows_id = conf.screen_native_id;
    break;
  default:
    LOGE("GlesBox: not support this draw type.");
    return false;
  }

  if (need_egl)
    bindEGLContext(width, height, native_windows_id);
  if (need_fbo)
    bindFrameBuffer(width, height);

  return true;
}

bool GlesBox::draw_end(GBConfig& conf) {
  float angle = 0.0f;
  bool need_egl = false, need_fbo = false, need_swap = false;
  switch(conf.type) {
  case GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT:
    angle = conf.screen_angle;
    break;
  case GB_DRAW_OFFLINE_WITH_OPENGLES_CONTEXT:
  case GB_DRAW_BOTH_WITH_OPENGLES_CONTEXT:
    need_fbo = true;
    need_swap = true;
    angle = conf.offline_angle;
    break;
  case GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT:
    need_egl = true;
    angle = conf.screen_angle;
    break;
  case GB_DRAW_OFFLINE_WITHOUT_OPENGLES_CONTEXT:
  case GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT:
    need_fbo = true;
    need_egl = true;
    need_swap = true;
    angle = conf.offline_angle;
    break;
  default:
    LOGE("GlesBox: not support this draw type.");
    return false;
  }
  if (need_fbo) {
    //read from GPU and do image translation
    const uint8_t *image = readFromGPU();
    unbindFrameBuffer();
    if (need_swap)
      swap(conf.screen_angle + angle + 180.0f);

    switch(conf.offline_type) {
    case GB_IMAGE_YUV420:
      libyuv::ARGBToI420((uint8*)image, width_*4,
        conf.offline_data, conf.offline_widthstep,
        conf.offline_data + conf.offline_widthstep * conf.offline_height,
        conf.offline_widthstep >> 1,
        conf.offline_data + conf.offline_widthstep * conf.offline_height * 5 / 4,
        conf.offline_widthstep >> 1,
        conf.offline_width, conf.offline_height);
      break;
    case GB_IMAGE_RGB24:
      libyuv::ARGBToRGB24((uint8*)image, width_*4,
        conf.offline_data, conf.offline_widthstep, conf.offline_width, conf.offline_height);
      break;
    case GB_IMAGE_RGBA32:
      memcpy(conf.offline_data, image, width_*height_*4);
      break;
    case GB_IMAGE_YUY2:
      LOGE("glesbox: will support this image format later.");
      return false;
    case GB_IMAGE_UNKNOWN:
    default:
      LOGE("glesbox: not support this image format.");
      return false;
    }
  }

  if (need_egl)
    unbindEGLContext();

  return true;
}

//-------------------------------------------------------------------------------------------------
bool GlesBox::bindEGLContext(uint32_t width, uint32_t height, uint64_t native_windows_id) {
  if (display_ == nullptr) {
     // Get Display
    display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if ( display_ == EGL_NO_DISPLAY ) {
      LOGE("Cann't get EGL display on native windows.");
      return false;
    }

    // Initialize EGL
    EGLint majorVersion(0), minorVersion(0);
    if ( !eglInitialize(display_, &majorVersion, &minorVersion) ) {
      LOGE("Cann't eglInitialize EGL display.");
      return false;
    }
    eglBindAPI(EGL_OPENGL_ES_API);

    // Get configs
    EGLint numConfigs(0);
    if ( !eglGetConfigs(display_, NULL, 0, &numConfigs) ) {
      LOGE("eglGetConfigs fail.");
      return false;
    }

    // Choose config
    EGLint attribList[] = {
      EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_ALPHA_SIZE, 8,
      EGL_NONE
    };
    if ( !eglChooseConfig(display_, attribList, &config_, 1, &numConfigs) ) {
      LOGE("eglChooseConfig fail.");
      return false;
    }

    createSurface(width, height, native_windows_id);
    // Create a GL context
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    context_ = eglCreateContext(display_, config_, EGL_NO_CONTEXT, contextAttribs );
    if ( context_ == EGL_NO_CONTEXT ) {
      LOGE("eglCreateContext fail.");
      return false;
    }
  }

  //renew a surface because of size changed
  if (width_ != width || height_ != height) {
    createSurface(width, height, native_windows_id);
  }

  // Make the context current
  if ( !eglMakeCurrent(display_, surface_, surface_, context_) ) {
    LOGE("eglMakeCurrent fail.");
    return false;
  }

  return true;
}

void GlesBox::unbindEGLContext() {
  if (display_ != nullptr)
    eglSwapBuffers(display_, surface_);
  eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool GlesBox::createSurface(uint32_t width, uint32_t height, uint64_t native_windows_id) {
  if (surface_ != nullptr) {
    eglDestroySurface(surface_, surface_);
    surface_ = nullptr;
  }

  // Create a surface
  width_ = width;
  height_ = height;
  EGLint PBufAttribs[] = { 
    EGL_WIDTH, (EGLint)width_,
    EGL_HEIGHT, (EGLint)height_,
    EGL_LARGEST_PBUFFER, EGL_TRUE,
    EGL_NONE
  };
  if (native_windows_id != 0) {//attach on native windows
    EGLNativeWindowType handle = reinterpret_cast<EGLNativeWindowType>(native_windows_id);
    surface_ = eglCreateWindowSurface(display_, config_, handle, NULL);
  }
  else // off-render
    surface_ = eglCreatePbufferSurface(display_, config_, PBufAttribs);

  if ( surface_ == EGL_NO_SURFACE ) {
    LOGE("eglCreateWindowSurface fail: %d.", eglGetError());
    return false;
  }

  return true;
}

bool GlesBox::createFrameBuffer(uint32_t width, uint32_t height) {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&framebuffer_old_);
  glGetIntegerv(GL_VIEWPORT, viewport_old_);
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texture_render_);
  glBindTexture(GL_TEXTURE_2D, texture_render_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &depth_render_);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_render_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &framebuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_render_, 0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_);
  
  // FBO status check
  GLenum status;
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch(status) {
    case GL_FRAMEBUFFER_COMPLETE:
      LOGD("glesbox: create framebuffer success.");
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
    default:
      LOGD("glesbox: create framebuffer fail: %d", status);
      break;
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  frame_data_ = new uint8_t[width_*height_*4]; //RGBA 4 chanel

  return true;
}

void GlesBox::releaseFrameBuffer() {
  if (framebuffer_ != 0)
    glDeleteFramebuffers(1, &framebuffer_);
  framebuffer_ = 0;
  if (texture_render_ != 0)
    glDeleteTextures(1, &texture_render_);
  texture_render_ = 0;
  if (depth_render_ != 0)
    glDeleteRenderbuffers(1, &depth_render_);
  depth_render_ = 0;
  if (frame_data_ != nullptr)
    delete[] frame_data_;
  frame_data_ = nullptr;
  framebuffer_old_ = 0;
}

bool GlesBox::bindFrameBuffer(uint32_t width, uint32_t height) {
  //renew framebuffer
  if (framebuffer_ == 0 || width_ != width || height_ != height) {
    releaseFrameBuffer();
    createFrameBuffer(width, height);
    width_ = width;
    height_ = height;
  }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_render_);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
  glViewport(0, 0, width_, height_);
  
  return true;
}

void GlesBox::unbindFrameBuffer() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_old_);
  glViewport(viewport_old_[0], viewport_old_[1], viewport_old_[2], viewport_old_[3]);
}

const uint8_t* GlesBox::readFromGPU() {
  //TODO(binfei): use PBO to accelerate
  glReadPixels(0, 0, width_, height_, GL_RGBA, GL_UNSIGNED_BYTE, frame_data_);

  return frame_data_;
}

bool GlesBox::swap(const float angle) {
  float radian = angle * PI/ 180.0f;
  float projection[] = { 
    static_cast<float>(cos(radian)), static_cast<float>(-sin(radian)), 0.0f, 0.0f,
    static_cast<float>(sin(radian)), static_cast<float>(cos(radian)), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  //1. render framebuffer to screen
  /*glEnable(GL_TEXTURE_2D);
  shaderProgram_.bind();
  shaderProgram_.setUniformMatrixValue4fv("model_view_project", projection);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_render_);
  shaderProgram_.setUniformValue1i("u_texture", 0);
  glBindBuffer(GL_ARRAY_BUFFER, face_vert_vbo_);
  GLuint a_position = shaderProgram_.GetAttribLocation("a_position");
  glEnableVertexAttribArray(a_position);
  glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, face_texc_vbo_);
  GLuint a_texturecoord = shaderProgram_.GetAttribLocation("a_texturecoord");
  glEnableVertexAttribArray(a_texturecoord);
  glVertexAttribPointer(a_texturecoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(a_position);
  glDisableVertexAttribArray(a_texturecoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  shaderProgram_.unbind();
  glDisable(GL_TEXTURE_2D);*/

  return true;
}

//-------------------------------------------------------------------------------------------------
} //! namespace libgb