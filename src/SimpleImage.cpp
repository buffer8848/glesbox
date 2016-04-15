// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)

#include "simpleimage.hpp"

#include "platform.hpp"
#include "shaderprogram.hpp"

//-------------------------------------------------------------------------------------------------
namespace libgb {

const char * image_show_vertex_shader_source =
  "uniform mat4 model_view_project;\n"
  "attribute vec4 a_position;\n"
  "attribute vec2 a_texturecoord;\n"
  "varying vec2 v_texturecoord;\n"
  "void main() {\n"
  "v_texturecoord = a_texturecoord;\n"
  "gl_Position = model_view_project * a_position;\n"
  "}";

const char * image_show_fragment_shader_source =
  "precision mediump float;\n"
  "uniform sampler2D u_texture;\n"
  "varying vec2 v_texturecoord;\n"
  "void main() {\n"
  "gl_FragColor = texture2D(u_texture,v_texturecoord);\n"
  "}";


struct SimpleImage::core {
  bool weak_textid_;
  GLuint textureid_;
  GLuint vert_vboid_;
  GLuint texc_vboid_;
  ShaderProgram shaderProgram_;

  bool update_text;
  bool update_vert;
  uint32_t texture_width_;
  uint32_t texture_height_;
  uint8_t texture_chanel_;
  std::vector<uint8_t> texture_data;
  std::vector<float> vert_data;

  core() : weak_textid_(false), textureid_(0), vert_vboid_(0), texc_vboid_(0),
    texture_width_(0), texture_height_(0), texture_chanel_(0),
    update_text(false), update_vert(false) {
  }
  ~core() {
    if(weak_textid_ && textureid_ != 0)
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

void SimpleImage::setTextureid(uint32_t id) {
    core_->weak_textid_ = true;
    core_->textureid_ = (GLuint)id;
  }

void SimpleImage::setTextData(
  uint32_t width, uint32_t height, uint8_t chanel, const std::vector<uint8_t> &data) {
  if (core_->weak_textid_) //if textureid from outside, do nothing
    return;

  core_->update_text = true;
  if (core_->texture_data.empty())
    core_->texture_data.reserve(data.size());
  if (core_->texture_width_ != width || core_->texture_height_ != height ||
    core_->texture_chanel_ != chanel) {
    glDeleteTextures(1, &core_->textureid_);
    core_->textureid_ = 0;
  }
  core_->texture_data = data;
  core_->texture_width_ = width;
  core_->texture_height_ = height;
  core_->texture_chanel_ = chanel;
}

void SimpleImage::setVertData(const std::vector<float> &data) {
  core_->update_vert = true;
  if (core_->vert_data.empty())
    core_->vert_data.reserve(data.size());
  core_->vert_data = data;
}

bool SimpleImage::load(const std::string &file) {
  //TODO(binfei):
  return true;
}

bool SimpleImage::draw(const GBConfig& conf) {
  glEnable(GL_TEXTURE_2D);
  //load shader
  if (core_->shaderProgram_.getProgram() == 0) {
    core_->shaderProgram_.addShader(GL_VERTEX_SHADER, image_show_vertex_shader_source);
    core_->shaderProgram_.addShader(GL_FRAGMENT_SHADER, image_show_fragment_shader_source);
    core_->shaderProgram_.link();
  }
  //load texture to GPU
  if (core_->textureid_ == 0) {
    glGenTextures(1, &core_->textureid_);
    glBindTexture(GL_TEXTURE_2D, core_->textureid_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // texture wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, core_->texture_width_, core_->texture_height_, 0,
      GL_RGB, GL_UNSIGNED_BYTE, core_->texture_data.data());
    //std::swap(texture_data, std::vector<uint8_t>());
    auto tmp = std::move(core_->texture_data); //release the static image
    core_->update_text = false;
  } else if (core_->update_text) {
    glBindTexture(GL_TEXTURE_2D, core_->textureid_);
    glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, core_->texture_width_, core_->texture_height_,
      GL_RGB, GL_UNSIGNED_BYTE, core_->texture_data.data());
    core_->update_text = false;
  }

  //load verture, texcoord to GPU
  if (core_->texc_vboid_ == 0) {
    glGenBuffers(1, &core_->texc_vboid_);
    const GLfloat texcoord_array[] = {
      0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0
    };
    glBindBuffer(GL_ARRAY_BUFFER, core_->texc_vboid_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texcoord_array), texcoord_array, GL_STATIC_DRAW);
  }

  if (core_->vert_vboid_ == 0) {
    glGenBuffers(1, &core_->vert_vboid_);
    const GLfloat vertex_array[] = {
      -1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, -1.0, 0.0, -1.0, -1.0, 0.0
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array), vertex_array, GL_DYNAMIC_DRAW);
    core_->update_vert = false;
  } else if (core_->update_vert) {
    glBindBuffer(GL_ARRAY_BUFFER, core_->vert_vboid_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 48, core_->vert_data.data());
    core_->update_vert = false;
  }

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
  glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
  glDisableVertexAttribArray(a_position);
  glDisableVertexAttribArray(a_texturecoord);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  core_->shaderProgram_.unbind();

  glDisable(GL_TEXTURE_2D);

  return true;
}

//-------------------------------------------------------------------------------------------------
} //! namespace libgb