// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#ifndef LIBGB_YUV_IMAGE_H_
#define LIBGB_YUV_IMAGE_H_

#include <memory>

#include "simpleimage.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

class YuvImage {
DISALLOW_COPY_AND_ASSIGN(YuvImage);
public:
  YuvImage();
  ~YuvImage();

  virtual bool load(const std::string &file);
  virtual bool draw(const GBConfig& conf);

  void reset();
  void setTextData(uint32_t width, uint32_t height, uint8_t chanel, const uint8_t *data);
  void setVertData(const std::vector<float> &data);
  void setTexcData(const std::vector<float> &data);
  void setTrigData(const std::vector<uint32_t> &data);
  void setTriangleSize(uint32_t size);

public:
  struct core;
  std::unique_ptr<core> core_;
};

//-------------------------------------------------------------------------------------------------
} //! namespace libgb

#endif //!LIBGB_SIMPLE_IMAGE_H_