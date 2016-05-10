// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include "glesbox.hpp"

#include <math.h>

#include "libyuv.h"

#include "platform.hpp"
#include "simpleimage.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

struct GlesBox::core {
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
  SimpleImage frameimage_;

  core() : width_(0), height_(0), display_(nullptr), surface_(nullptr),
    context_(nullptr), config_(nullptr), framebuffer_old_(0),
    framebuffer_(0), texture_render_(0), depth_render_(0), frame_data_(nullptr) {
  }
  ~core() {
    if (frame_data_ != nullptr)
      delete[] frame_data_;
    frame_data_ = nullptr;
  }
};

GlesBox::GlesBox() : core_(new core()){
}

GlesBox::~GlesBox() {
  if (hasEgl())
    bindEGLContext(core_->width_, core_->height_, 0);
  releaseFrameBuffer();

  if (core_->surface_ != nullptr)
    eglDestroySurface(core_->display_, core_->surface_);
  core_->surface_ = nullptr;
  if (core_->context_ != nullptr)
    eglDestroyContext(core_->display_, core_->context_);
  core_->context_ = nullptr;
  core_->display_ = nullptr;
  if (hasEgl())
    unbindEGLContext();
  core_->width_ = 0;
  core_->height_ = 0;
}

bool GlesBox::draw_begin(const GBConfig& conf) {
  uint64_t native_windows_id = 0;
  uint32_t width(0), height(0);
  bool need_egl = false, need_fbo = false;
  switch(conf.type) {
  case GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT:
    need_egl = true;
    native_windows_id = conf.screen_native_id;
  case GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT:
    width = conf.screen_width;
    height = conf.screen_height;
    break;
  case GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT:
  case GB_DRAW_OFFLINE_WITHOUT_OPENGLES_CONTEXT:
    need_egl = true;
    native_windows_id = conf.screen_native_id;
  case GB_DRAW_BOTH_WITH_OPENGLES_CONTEXT:
  case GB_DRAW_OFFLINE_WITH_OPENGLES_CONTEXT:
    need_fbo = true;
    width = conf.offline_width;
    height = conf.offline_height;
    break;
  default:
    LOGE("GlesBox: not support this draw type.");
    return false;
  }

  if (need_egl)
    bindEGLContext(width, height, native_windows_id);
  glGetIntegerv(GL_VIEWPORT, core_->viewport_old_);
  if (need_fbo)
    bindFrameBuffer(width, height);
  else if (width > 0 && height > 0)
    glViewport(conf.screen_x, conf.screen_y, width, height);
  if (need_fbo || need_egl)
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  return true;
}

bool GlesBox::draw_end(GBConfig& conf) {
  float angle = 0.0f;
  bool need_egl = false, need_fbo = false, need_online = false;
  switch(conf.type) {
  case GB_DRAW_ONLINE_WITHOUT_OPENGLES_CONTEXT:
    need_egl = true;
  case GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT:
    angle = conf.screen_angle;
    break;
  case GB_DRAW_BOTH_WITH_OPENGLES_CONTEXT:
    need_online = true;
  case GB_DRAW_OFFLINE_WITH_OPENGLES_CONTEXT:
    need_fbo = true;
    angle = conf.offline_angle;
    break;
  case GB_DRAW_BOTH_WITHOUT_OPENGLES_CONTEXT:
    need_online = true;
  case GB_DRAW_OFFLINE_WITHOUT_OPENGLES_CONTEXT:
    need_fbo = true;
    need_egl = true;
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
    
    if (conf.screen_width > 0 && conf.screen_height > 0)
      glViewport(conf.screen_x, conf.screen_y, conf.screen_width, conf.screen_height);
    if (need_online)
      swap(conf.screen_angle + angle + 180.0f);

    switch(conf.offline_type) {
    case GB_IMAGE_YUV420:
      libyuv::ARGBToI420((uint8*)image, core_->width_*4,
        conf.offline_data, conf.offline_widthstep,
        conf.offline_data + conf.offline_widthstep * conf.offline_height,
        conf.offline_widthstep >> 1,
        conf.offline_data + conf.offline_widthstep * conf.offline_height * 5 / 4,
        conf.offline_widthstep >> 1,
        conf.offline_width, conf.offline_height);
      break;
    case GB_IMAGE_RGB24:
      libyuv::ARGBToRGB24((uint8*)image, core_->width_*4,
        conf.offline_data, conf.offline_widthstep, conf.offline_width, conf.offline_height);
      break;
    case GB_IMAGE_RGBA32:
      memcpy(conf.offline_data, image, core_->width_*core_->height_*4);
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
  glViewport(core_->viewport_old_[0], core_->viewport_old_[1],
    core_->viewport_old_[2], core_->viewport_old_[3]);

  return true;
}

//-------------------------------------------------------------------------------------------------
bool GlesBox::bindEGLContext(uint32_t width, uint32_t height, uint64_t native_windows_id) {
  if (core_->display_ == nullptr) {
    // Get Display
    core_->display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (core_->display_ == EGL_NO_DISPLAY) {
      LOGE("Cann't get EGL display on native windows.");
      return false;
    }

    // Initialize EGL
    EGLint majorVersion(0), minorVersion(0);
    if (!eglInitialize(core_->display_, &majorVersion, &minorVersion)) {
      LOGE("Cann't eglInitialize EGL display.");
      return false;
    }
    eglBindAPI(EGL_OPENGL_ES_API);

    // Get configs
    EGLint numConfigs(0);
    if (!eglGetConfigs(core_->display_, NULL, 0, &numConfigs)) {
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
    if (!eglChooseConfig(core_->display_, attribList, &core_->config_, 1, &numConfigs)) {
      LOGE("eglChooseConfig fail.");
      return false;
    }

    createSurface(width, height, native_windows_id);
    // Create a GL context
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};
    core_->context_ = 
      eglCreateContext(core_->display_, core_->config_, EGL_NO_CONTEXT, contextAttribs);
    if (core_->context_ == EGL_NO_CONTEXT) {
      LOGE("eglCreateContext fail.");
      return false;
    }
  }

  //renew a surface because of size changed
  if (core_->width_ != width || core_->height_ != height) {
    createSurface(width, height, native_windows_id);
  }

  // Make the context current
  if ( !eglMakeCurrent(core_->display_, core_->surface_, core_->surface_, core_->context_) ) {
    LOGE("eglMakeCurrent fail.");
    return false;
  }

  return true;
}

void GlesBox::unbindEGLContext() {
  if (core_->display_ != nullptr)
    eglSwapBuffers(core_->display_, core_->surface_);
  eglMakeCurrent(core_->display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
}

bool GlesBox::createSurface(uint32_t width, uint32_t height, uint64_t native_windows_id) {
  if (core_->surface_ != nullptr) {
    eglDestroySurface(core_->surface_, core_->surface_);
    core_->surface_ = nullptr;
  }

  // Create a surface
  core_->width_ = width;
  core_->height_ = height;
  EGLint PBufAttribs[] = { 
    EGL_WIDTH, (EGLint)core_->width_,
    EGL_HEIGHT, (EGLint)core_->height_,
    EGL_LARGEST_PBUFFER, EGL_TRUE,
    EGL_NONE
  };
  if (native_windows_id != 0) {//attach on native windows
    EGLNativeWindowType handle = reinterpret_cast<EGLNativeWindowType>(native_windows_id);
    core_->surface_ = eglCreateWindowSurface(core_->display_, core_->config_, handle, NULL);
  }
  else // off-render
    core_->surface_ = eglCreatePbufferSurface(core_->display_, core_->config_, PBufAttribs);

  if (core_->surface_ == EGL_NO_SURFACE) {
    LOGE("eglCreateWindowSurface fail: %d.", eglGetError());
    return false;
  }

  return true;
}

bool GlesBox::createFrameBuffer(uint32_t width, uint32_t height) {
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&core_->framebuffer_old_);
  glEnable(GL_TEXTURE_2D);
  //glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &core_->texture_render_);
  glBindTexture(GL_TEXTURE_2D, core_->texture_render_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenRenderbuffers(1, &core_->depth_render_);
  glBindRenderbuffer(GL_RENDERBUFFER, core_->depth_render_);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glGenFramebuffers(1, &core_->framebuffer_);
  glBindFramebuffer(GL_FRAMEBUFFER, core_->framebuffer_);
  glFramebufferTexture2D(
    GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, core_->texture_render_, 0);
  glFramebufferRenderbuffer(
    GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, core_->depth_render_);
  
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
  glBindFramebuffer(GL_FRAMEBUFFER, core_->framebuffer_old_);
  glDisable(GL_TEXTURE_2D);
  
  core_->frame_data_ = new uint8_t[width*height*4]; //RGBA 4 chanel

  return true;
}

void GlesBox::releaseFrameBuffer() {
  if (core_->framebuffer_ != 0)
    glDeleteFramebuffers(1, &core_->framebuffer_);
  core_->framebuffer_ = 0;
  if (core_->texture_render_ != 0)
    glDeleteTextures(1, &core_->texture_render_);
  core_->texture_render_ = 0;
  if (core_->depth_render_ != 0)
    glDeleteRenderbuffers(1, &core_->depth_render_);
  core_->depth_render_ = 0;
  if (core_->frame_data_ != nullptr)
    delete[] core_->frame_data_;
  core_->frame_data_ = nullptr;
  core_->framebuffer_old_ = 0;
}

bool GlesBox::bindFrameBuffer(uint32_t width, uint32_t height) {
  //renew framebuffer
  if (core_->framebuffer_ == 0 || core_->width_ != width || core_->height_ != height) {
    releaseFrameBuffer();
    createFrameBuffer(width, height);
    core_->width_ = width;
    core_->height_ = height;
  }
  glEnable(GL_TEXTURE_2D);
  //glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, core_->texture_render_);
  glBindFramebuffer(GL_FRAMEBUFFER, core_->framebuffer_);
  glViewport(0, 0, core_->width_, core_->height_);
  
  return true;
}

void GlesBox::unbindFrameBuffer() {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, core_->framebuffer_old_);
  glViewport(core_->viewport_old_[0], core_->viewport_old_[1],
    core_->viewport_old_[2], core_->viewport_old_[3]);
  glDisable(GL_TEXTURE_2D);
}

const uint8_t* GlesBox::readFromGPU() {
  //TODO(binfei): use PBO to accelerate
  glReadPixels(0, 0, core_->width_, core_->height_, GL_RGBA, GL_UNSIGNED_BYTE, core_->frame_data_);

  return core_->frame_data_;
}

bool GlesBox::swap(const float angle) {
  //1. render framebuffer to screen
  core_->frameimage_.setTextureid(core_->texture_render_);
  GBConfig conf;
  conf.type = GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT;
  conf.screen_angle = angle;
  core_->frameimage_.draw(conf);

  return true;
}

const bool GlesBox::hasEgl() const {
  return core_->display_ != nullptr;
}

//-------------------------------------------------------------------------------------------------
} //! namespace libgb