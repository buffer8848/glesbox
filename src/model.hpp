// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#ifndef LIBGB_MODEL_H_
#define LIBGB_MODEL_H_

#include <string>

#include "common.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

class Model {
public:
  virtual ~Model() = 0{}

  virtual bool load(const std::string &file) = 0;
  virtual bool draw(const GBConfig& conf) = 0;
};

//-------------------------------------------------------------------------------------------------
} //! namespace libgb

#endif //!LIBGB_MODEL_H_