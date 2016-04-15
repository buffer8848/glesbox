// Copyright (c) 2016 Intel Corporation.
// Author:  binfeix.li@intel.com (2016.v2.0)

#ifndef LIBGB_PLATFORM_H_
#define LIBGB_PLATFORM_H_

#include <memory>

//-------------------------------------------------------------------------------------------------
namespace libgb {

#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
#include "EGL/egl.h"
#pragma comment(lib, "libGLESv2.lib")
#pragma comment(lib, "libEGL.lib")
#pragma comment(lib, "libyuv.lib")
#elif defined(_IOS)
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#include "EGL/egl.h"
#elif defined(_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
#include <android/log.h>
#define LOG_TAG "glesbox"
#else
#include <sys/time.h>
#include <unistd.h> 
#include <fcntl.h>
#endif

//-------------------------------------------------------------------------------------------------

#ifdef _ANDROID
#define LOGD(...) \
  do{ \
  __android_log_print(3, __VA_ARGS__); \
  }while(0);

#define LOGE(...) \
  do{ \
  __android_log_print(6, __VA_ARGS__); \
  }while(0);
#else
#define LOGD(...) \
  do{ \
  printf(__VA_ARGS__); \
  }while(0);

#define LOGE(...) \
  do{ \
  printf(__VA_ARGS__); \
  }while(0);
#endif

#ifndef PI
#define PI (3.1415926f)
#endif

//-------------------------------------------------------------------------------------------------
} //! namespace libgb
#endif //!LIBGB_PLATFORM_H_
