// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include <stdio.h>
#if defined(WIN32) || defined(_WIN32)
#include <windows.h>
#elif defined(_ANDROID)
#include <android/log.h>
#define LOG_TAG "glesbox"
#else
#include <sys/time.h>
#include <unistd.h> 
#include <fcntl.h>
#endif

//-------------------------------------------------------------------------------------------------
namespace libgb {

//-------------------------------------------------------------------------------------------------
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
private: \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

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