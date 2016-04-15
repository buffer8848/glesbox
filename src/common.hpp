// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#ifndef LIBGB_COMMON_H_
#define LIBGB_COMMON_H_

#include <memory>
#include <stdio.h>
#include <stdint.h>

//-------------------------------------------------------------------------------------------------
namespace libgb {

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
private: \
  TypeName(const TypeName&); \
  void operator=(const TypeName&)

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

  GBConfig() : type(GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT), screen_native_id(0),
    screen_x(0), screen_y(0), screen_width(0), screen_height(0), screen_angle(0.0f),
    offline_width(0), offline_height(0), offline_widthstep(0), offline_channel(0),
    offline_data(nullptr), offline_angle(0.0f), offline_type(GB_IMAGE_UNKNOWN) {
  }
}GBConfig;

//-------------------------------------------------------------------------------------------------

} //! namespace libgb

#endif //!LIBGB_COMMON_H_