// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include "simpleimage.hpp"

#include <math.h>

#include "platform.hpp"
#include "shaderprogram.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

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
  "uniform sampler2D u_texture;\n"
  "varying vec2 v_texturecoord;\n"
  "void main() {\n"
  "gl_FragColor = texture2D(u_texture,v_texturecoord);\n"
  "}";

struct SimpleImage::core {
  bool guest_textid_;
  GLuint textureid_;
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

  core() : guest_textid_(false), textureid_(0), vert_vboid_(0), texc_vboid_(0),
    texture_width_(0), texture_height_(0), texture_chanel_(0), triangle_size_(0),
    update_text(false), update_vert(false), update_texc(false) {
  }
  ~core() {
    if(!guest_textid_ && textureid_ != 0)
      glDeleteTextures(1, &textureid_);
    textureid_ = 0;
    if (vert_vboid_ != 0)
      glDeleteBuffers(1, &vert_vboid_);
    vert_vboid_ = 0;
    if (texc_vboid_ != 0)
      glDeleteBuffers(1, &texc_vboid_);
    texc_vboid_ = 0;
  }
};

SimpleImage::SimpleImage() : core_(new core()){
}

SimpleImage::~SimpleImage() {
}

void SimpleImage::reset() {
  core_.reset(new core());
}

void SimpleImage::setTextureid(uint32_t id) {
    core_->guest_textid_ = true;
    core_->textureid_ = (GLuint)id;
  }

void SimpleImage::setTextData(
  uint32_t width, uint32_t height, uint8_t chanel, const uint8_t *data) {
  if (core_->guest_textid_) //if textureid from outside, do nothing
    return;

  auto size = width * height * chanel * sizeof(uint8_t);
  core_->update_text = true;
  if (core_->texture_data.empty())
    core_->texture_data.resize(size);
  if (core_->texture_width_ != width || core_->texture_height_ != height ||
    core_->texture_chanel_ != chanel) {
    glDeleteTextures(1, &core_->textureid_);
    core_->textureid_ = 0;
  }
  memcpy(core_->texture_data.data(), data, size);
  core_->texture_width_ = width;
  core_->texture_height_ = height;
  core_->texture_chanel_ = chanel;
}

void SimpleImage::setVertData(const std::vector<float> &data) {
  core_->update_vert = true;
  core_->vert_data.resize(data.size());
  core_->vert_data = data;
}

void SimpleImage::setTexcData(const std::vector<float> &data) {
  core_->update_texc = true;
  core_->texc_data.resize(data.size());
  core_->texc_data = data;
}

void SimpleImage::setTriangleSize(uint32_t size) {
  core_->triangle_size_ = size;
}

bool SimpleImage::load(const std::string &file) {
  //TODO(binfei):
  return true;
}

bool SimpleImage::draw(const GBConfig& conf) {
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
  //load texture to GPU
  auto text_draw_type = GL_RGBA;
  switch (core_->texture_chanel_) {
    case 3:
      text_draw_type = GL_RGB;
    default:
      break;
  }
  if (core_->textureid_ == 0) {
    glGenTextures(1, &core_->textureid_);
    glBindTexture(GL_TEXTURE_2D, core_->textureid_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // texture wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, text_draw_type, core_->texture_width_, core_->texture_height_,
      0, text_draw_type, GL_UNSIGNED_BYTE, core_->texture_data.data());
    //std::swap(texture_data, std::vector<uint8_t>());
    std::move(core_->texture_data); //release the static image
  } else if (core_->update_text) {
    glBindTexture(GL_TEXTURE_2D, core_->textureid_);
    glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, core_->texture_width_, core_->texture_height_,
      text_draw_type, GL_UNSIGNED_BYTE, core_->texture_data.data());
  }
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
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, core_->textureid_);
  core_->shaderProgram_.setUniformValue1i("u_texture", 0);
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