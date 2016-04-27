// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include "yuvimage.hpp"

#include <algorithm>
#include <math.h>
#include <vector>

#include "platform.hpp"
#include "shaderprogram.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

#define YUV_CHANEL 3
static const char * image_show_vertex_shader_source =
  "uniform mat4 model_view_project;\n"
  "attribute vec4 a_position;\n"
  "attribute vec2 a_texturecoord;\n"
  "varying vec2 v_texturecoord;\n"
  "void main() {\n"
  "v_texturecoord = a_texturecoord;\n"
  "gl_Position = model_view_project * a_position;\n"
  "}";

static const char * image_show_fragment_shader_source =
  "precision mediump float;\n"
  "varying highp vec2 v_texturecoord;\n"
  "uniform sampler2D s_texture_y;\n"
  "uniform sampler2D s_texture_u;\n"
  "uniform sampler2D s_texture_v;\n"
  "void main() {\n"
  "highp float y = texture2D(s_texture_y, v_texturecoord).r;\n"
  "highp float u = texture2D(s_texture_u, v_texturecoord).r - 0.5;\n"
  "highp float v = texture2D(s_texture_v, v_texturecoord).r - 0.5;\n"
  "highp float b = y + 1.402 * v;\n"
  "highp float g = y - 0.344 * u - 0.714 * v;\n"
  "highp float r = y + 1.772 * u;\n"
  "gl_FragColor = vec4(r, g, b, 1.0);\n"
  "}";

struct YuvImage::core {
  std::vector<GLuint> textureids_;
  GLuint vert_vboid_;
  GLuint texc_vboid_;
  ShaderProgram shaderProgram_;

  bool update_text;
  bool update_vert;
  bool update_texc;
  uint32_t texture_width_;
  uint32_t texture_height_;
  uint32_t triangle_size_;
  uint8_t texture_chanel_;
  std::vector<uint8_t> texture_data;
  std::vector<float> vert_data;
  std::vector<float> texc_data;

  core() : vert_vboid_(0), texc_vboid_(0), texture_width_(0),
    texture_height_(0), texture_chanel_(0), triangle_size_(0),
    update_text(false), update_vert(false), update_texc(false) {
    textureids_.resize(YUV_CHANEL);
    memset(textureids_.data(), 0x0, YUV_CHANEL*sizeof(GLuint));
  }
  ~core() {
    glDeleteTextures(YUV_CHANEL, textureids_.data());
    if (vert_vboid_ != 0)
      glDeleteBuffers(1, &vert_vboid_);
    vert_vboid_ = 0;
    if (texc_vboid_ != 0)
      glDeleteBuffers(1, &texc_vboid_);
    texc_vboid_ = 0;
  }
};

YuvImage::YuvImage() : core_(new core()){
}

YuvImage::~YuvImage() {
}

void YuvImage::reset() {
  core_.reset(new core());
}

void YuvImage::setTextData(
  uint32_t width, uint32_t height, uint8_t chanel, const uint8_t *data) {
  auto size = width * height * sizeof(uint8_t) * 3 / 2;
  core_->update_text = true;
  if (core_->texture_data.empty())
    core_->texture_data.resize(size);
  if (core_->texture_width_ != width || core_->texture_height_ != height ||
    core_->texture_chanel_ != chanel) {
    glDeleteTextures(YUV_CHANEL, core_->textureids_.data());
    memset(core_->textureids_.data(), 0x0, YUV_CHANEL*sizeof(GLuint));
  }
  memcpy(core_->texture_data.data(), data, size);
  core_->texture_width_ = width;
  core_->texture_height_ = height;
  core_->texture_chanel_ = chanel;
}

void YuvImage::setVertData(const std::vector<float> &data) {
  core_->update_vert = true;
  core_->vert_data.resize(data.size());
  core_->vert_data = data;
}

void YuvImage::setTexcData(const std::vector<float> &data) {
  core_->update_texc = true;
  core_->texc_data.resize(data.size());
  core_->texc_data = data;
}

void YuvImage::setTriangleSize(uint32_t size) {
  core_->triangle_size_ = size;
}

bool YuvImage::load(const std::string &file) {
  //TODO(binfei):
  return true;
}

bool YuvImage::draw(const GBConfig& conf) {
  GLint viewport_old_[4];
  glGetIntegerv(GL_VIEWPORT, viewport_old_);
  //TODO(just for online render)
  if (conf.screen_width > 0 && conf.screen_height > 0)
    glViewport(conf.screen_x, conf.screen_y, conf.screen_width, conf.screen_height);
  glEnable(GL_TEXTURE_2D);
  //load shader
  if (core_->shaderProgram_.getProgram() == 0) {
    core_->shaderProgram_.addShader(GL_VERTEX_SHADER, image_show_vertex_shader_source);
    core_->shaderProgram_.addShader(GL_FRAGMENT_SHADER, image_show_fragment_shader_source);
    core_->shaderProgram_.link();
  }

  //only for yuv420
  const uint32_t widths[] = 
    {core_->texture_width_, core_->texture_width_ / 2, core_->texture_width_ / 2};
  const uint32_t heights[] = 
    {core_->texture_height_, core_->texture_height_ / 2, core_->texture_height_ / 2};
  auto cursor = 0; auto yuv_ptr = core_->texture_data.data();
  std::for_each(core_->textureids_.begin(), core_->textureids_.end(), [&](GLuint &id) {
    if (id == 0) {
      glGenTextures(1, &id);
      glBindTexture(GL_TEXTURE_2D, id);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // texture wrap
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, widths[cursor], heights[cursor],
        0, GL_LUMINANCE, GL_UNSIGNED_BYTE, yuv_ptr);
      if (id == YUV_CHANEL - 1)
        //std::swap(texture_data, std::vector<uint8_t>());
        std::move(core_->texture_data); //release the static image
    } else if (core_->update_text) {
      glBindTexture(GL_TEXTURE_2D, id);
      glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, widths[cursor], heights[cursor],
        GL_LUMINANCE, GL_UNSIGNED_BYTE, yuv_ptr);
    }
    yuv_ptr += widths[cursor] * heights[cursor] * sizeof(uint8_t);
    ++cursor;
  });
  core_->update_text = false;

  //load verture, texcoord to GPU
  if (core_->texc_vboid_ == 0) {
    glGenBuffers(1, &core_->texc_vboid_);
    if (core_->update_texc) {
      glBindBuffer(GL_ARRAY_BUFFER, core_->texc_vboid_);
      glBufferData(GL_ARRAY_BUFFER, core_->texc_data.size()*sizeof(core_->texc_data[0]),
        core_->texc_data.data(), GL_DYNAMIC_DRAW);
    } else {
      const GLfloat texcoord_array[] = {
        0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0
      };
      glBindBuffer(GL_ARRAY_BUFFER, core_->texc_vboid_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_array), texcoord_array, GL_STATIC_DRAW);
    }
    
  } else if (core_->update_texc) {
    glBindBuffer(GL_ARRAY_BUFFER, core_->texc_vboid_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, core_->texc_data.size()*sizeof(core_->texc_data[0]),
      core_->texc_data.data());
  }
  core_->update_texc = false;

  if (core_->vert_vboid_ == 0) {
    glGenBuffers(1, &core_->vert_vboid_);
    if (core_->update_vert) {
      glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
      glBufferData(GL_ARRAY_BUFFER, core_->vert_data.size()*sizeof(core_->vert_data[0]),
        core_->vert_data.data(), GL_DYNAMIC_DRAW);
    } else {
      const GLfloat vertex_array[] = {
        -1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, -1.0, 0.0, -1.0, -1.0, 0.0
      };
      glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_STATIC_DRAW);
    }
  } else if (core_->update_vert) {
    glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, core_->vert_data.size()*sizeof(core_->vert_data[0]),
      core_->vert_data.data());
  }
  core_->update_vert = false;

  //model_view_project, rotate on aixs because 2d render
  auto angle = conf.offline_angle > 0.0 ? conf.offline_angle : conf.screen_angle;
  auto radian = angle* PI/ 180.0f;
  float projection[] = { 
    static_cast<float>(cos(radian)), static_cast<float>(-sin(radian)), 0.0f, 0.0f,
    static_cast<float>(sin(radian)), static_cast<float>(cos(radian)), 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };

  core_->shaderProgram_.bind();
  core_->shaderProgram_.setUniformMatrixValue4fv("model_view_project", projection);
  const char* unif_name[] = {"s_texture_y", "s_texture_u", "s_texture_v"};
  cursor = 0;
  std::for_each(core_->textureids_.begin(), core_->textureids_.end(), [&](GLuint id) {
    glActiveTexture(GL_TEXTURE0 + cursor);
    glBindTexture(GL_TEXTURE_2D, id);
    core_->shaderProgram_.setUniformValue1i(unif_name[cursor], cursor);
    ++cursor;
  });
  glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
  GLuint a_position = core_->shaderProgram_.GetAttribLocation("a_position");
  glEnableVertexAttribArray(a_position);
  glVertexAttribPointer(a_position, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, core_->texc_vboid_);
  GLuint a_texturecoord = core_->shaderProgram_.GetAttribLocation("a_texturecoord");
  glEnableVertexAttribArray(a_texturecoord);
  glVertexAttribPointer(a_texturecoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
  if (core_->triangle_size_ > 0)
    glDrawArrays(GL_TRIANGLES, 0, core_->triangle_size_);
  else
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(a_position);
  glDisableVertexAttribArray(a_texturecoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  core_->shaderProgram_.unbind();

  glDisable(GL_TEXTURE_2D);
  glViewport(viewport_old_[0], viewport_old_[1], viewport_old_[2], viewport_old_[3]);

  return true;
}

//-------------------------------------------------------------------------------------------------
} //! namespace libgb