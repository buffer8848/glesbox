#ifndef LIBGB_SHADER_PROGRAM_H_
#define LIBGB_SHADER_PROGRAM_H_

#include "platform.hpp"

namespace libgb {

enum ShaderType {
  ST_VERTEX,
  ST_FRAGMENT,
  ST_ALL
};

class ShaderProgram {
public:
  ShaderProgram(void);
  ~ShaderProgram(void);

public:
  void addShader(GLenum shaderType, const char* src);
  void setUniformValue1i(const char* name, int value);
  void setUniformMatrixValue4fv(const char* name, GLfloat* value);
  GLuint GetAttribLocation(const char* name);
  GLuint getProgram();
  void link();
  void bind();
  void unbind();

private:
  GLuint shaderCompile(GLenum type, const char *source);
  void shaderAttachFromString(GLuint program, GLenum type, const char *source);

private:
  GLuint program_;
};

} //!namespace libgb
#endif //!LIBGB_SHADER_PROGRAM_H_

