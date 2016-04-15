// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#ifndef LIBGB_SIMPLE_IMAGE_H_
#define LIBGB_SIMPLE_IMAGE_H_

#include <memory>
#include <vector>

#include "model.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {
class Model;
class SimpleImage : protected Model {
public:
  SimpleImage();
  ~SimpleImage();

  virtual bool load(const std::string &file);
  virtual bool draw(const GBConfig& conf);

  void setTextureid(uint32_t id);
  void setTextData(
    uint32_t width, uint32_t height, uint8_t chanel, const std::vector<uint8_t> &data);
  void setVertData(const std::vector<float> &data);

private:
  struct core;
  std::unique_ptr<core> core_;
};

//-------------------------------------------------------------------------------------------------
} //! namespace libgb

#endif //!LIBGB_SIMPLE_IMAGE_H_