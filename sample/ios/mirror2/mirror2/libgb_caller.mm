//
//  libgb_caller.m
//  maskshow
//
//  Created by buffer on 16/4/19.
//  Copyright © 2016年 xxx. All rights reserved.
//

#import "libgb_caller.h"

#include <vector>

#include "glesbox.hpp"
#include "SimpleImage.hpp"
#include "yuvimage.hpp"

using namespace std;
using namespace libgb;

//-------------------------------------------------------------------------------------------------
@implementation GBCaller

GlesBox gbox;
YuvImage image;
vector<uint8_t> texture_data;
const int8_t chanel = 3;

+(void) image_draw: (uint8_t*)data width: (uint32_t)width height: (uint32_t)height windows: (unsigned long)id{
  GBConfig conf;
  conf.type = GB_DRAW_ONLINE_WITH_OPENGLES_CONTEXT;
  conf.screen_angle = 90.0f;
  
  uint32_t size = width*height*chanel;
  texture_data.resize(size);
  memcpy(texture_data.data(), data, size);
  image.setTextData(width, height, chanel, texture_data.data());
  gbox.draw_begin(conf);
  image.draw(conf);
  gbox.draw_end(conf);
}

@end



