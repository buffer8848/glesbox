// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#ifndef LIBGB_GLESBOX_H_
#define LIBGB_GLESBOX_H_

#include "common.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

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

  const bool hasEgl() const;

private:
  struct core;
  std::unique_ptr<core> core_;
};

//-------------------------------------------------------------------------------------------------
} //! namespace libgb

#endif //!LIBGB_GLESBOX_H_