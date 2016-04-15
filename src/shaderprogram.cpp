// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)
#include "shaderprogram.hpp"

#include "common.hpp"

namespace libgb {

ShaderProgram::ShaderProgram(void):program_(0) {
}

ShaderProgram::~ShaderProgram(void) {
  if(program_>0) {
    glDeleteProgram(program_);
    program_ = 0;
  }
}

void ShaderProgram::addShader(GLenum shaderType, const char* src){
  if (0 == program_) {
    program_=glCreateProgram();
  }
  shaderAttachFromString(program_, shaderType, src);
}

void ShaderProgram::link() {
  GLint result;
  /* link the program and make sure that there were no errors */
  glLinkProgram(program_);
  glGetProgramiv(program_, GL_LINK_STATUS, &result);
  if(result == GL_FALSE) {
    GLint length;
    char *log;
    /* get the program info log */
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
    log = new char[length];
    glGetProgramInfoLog(program_, length, &result, log);

    /* print an error message and the info log */
    LOGE("sceneInit(): Program linking failed: %s\n", log);
    delete[] log;
    log=0;

    /* delete the program */
    glDeleteProgram(program_);
    program_ = 0;
  }
  
}

void ShaderProgram::setUniformValue1i(const char* name, int value) {
  GLint loc = glGetUniformLocation(program_, name);
  glUniform1i(loc, value);
}

void ShaderProgram::setUniformMatrixValue4fv(const char* name, GLfloat* value) {
  GLint loc = glGetUniformLocation(program_, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, value);
}

GLuint ShaderProgram::GetAttribLocation(const char* name) {
  return glGetAttribLocation(program_, name);
}

void ShaderProgram::bind() {
  glUseProgram(program_);
}

void ShaderProgram::unbind() {
  glUseProgram(0);
}

GLuint ShaderProgram::shaderCompile(GLenum type, const char *source) {
  //char *source;
  GLuint shader;
  GLint length, result;
	if(nullptr == source) {
		return -1;	
	}

  /* create shader object, set the source, and compile */
  shader = glCreateShader(type);
  length = strlen(source);
  glShaderSource(shader, 1, (const char **)&source, &length);
  glCompileShader(shader);

  /* make sure the compilation was successful */
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE) {
    /* get the shader info log */
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char *log = new char[length];
    glGetShaderInfoLog(shader, length, &result, log);
        
    /* print an error message and the info log */
    LOGE("shaderCompile: Unable to compile %s\n", log);
    delete[] (log);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

void  ShaderProgram::shaderAttachFromString(GLuint program, GLenum type, const char *source) {
  /* compile the shader */
  GLuint shader = shaderCompile(type, source);
  if(0 != shader) {
    /* attach the shader to the program */
    glAttachShader(program, shader);

    /* delete the shader - it won't actually be
      * destroyed until the program that it's attached
      * to has been destroyed */
    glDeleteShader(shader);
  }
}

GLuint ShaderProgram::getProgram() {
  return program_;
}

}