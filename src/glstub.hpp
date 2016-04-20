// Copyright (c) 2016 xxx
// Author:  LI,Binfei (binfeix.li@intel.com or 179770346@qq.com)
#ifndef LIBGB_GLSTUB_H_
#define LIBGB_GLSTUB_H_

#pragma warning(push)
#pragma warning(disable:4229 4005 4273)

namespace libgb {

/*
** Copyright (c) 2007-2013 The Khronos Group Inc.
**
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
**
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/* Call this function before calling any OpenGL ES 3.0 functions. It will
 * return GL_TRUE if the OpenGL ES 3.0 was successfully initialized, GL_FALSE
 * otherwise. */
bool glstubInit();
class Pre {
public:
  Pre() {
    glstubInit();
  }
};

/*-------------------------------------------------------------------------
 * Data type definitions
 *-----------------------------------------------------------------------*/
typedef void            GLvoid;
typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef signed char     GLbyte;
typedef short           GLshort;
typedef int             GLint;
typedef int             GLsizei;
typedef unsigned char   GLubyte;
typedef unsigned short  GLushort;
typedef unsigned int    GLuint;
typedef float           GLfloat;
typedef float           GLclampf;
typedef int             GLfixed;
typedef int             GLclampx;

/* GL types for handling large vertex buffer objects */
typedef int             GLintptr;
typedef int             GLsizeiptr;

typedef unsigned short   GLhalf;
//#if __ANDROID_API__ <= 19
//typedef khronos_int64_t  GLint64;
//typedef khronos_uint64_t GLuint64;
//typedef struct __GLsync *GLsync;
//#endif

#if (defined(_WIN32) || defined(__VC32__)) && !defined(_WIN32_WCE) /* Win32 */
#define GL_APIENTRY __stdcall
#else
#define GL_APIENTRY
#endif
#ifndef GL_APIENTRYP
#   define GL_APIENTRYP GL_APIENTRY*
#endif

extern void         (GL_APIENTRYP glActiveTexture) (GLenum texture);
extern void         (GL_APIENTRYP glAttachShader) (GLuint program, GLuint shader);
extern void         (GL_APIENTRYP glBindAttribLocation) (GLuint program, GLuint index, const char* name);
extern void         (GL_APIENTRYP glBindBuffer) (GLenum target, GLuint buffer);
extern void         (GL_APIENTRYP glBindFramebuffer) (GLenum target, GLuint framebuffer);
extern void         (GL_APIENTRYP glBindRenderbuffer) (GLenum target, GLuint renderbuffer);
extern void         (GL_APIENTRYP glBindTexture) (GLenum target, GLuint texture);
extern void         (GL_APIENTRYP glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void         (GL_APIENTRYP glBlendEquation)( GLenum mode );
extern void         (GL_APIENTRYP glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
extern void         (GL_APIENTRYP glBlendFunc) (GLenum sfactor, GLenum dfactor);
extern void         (GL_APIENTRYP glBlendFuncSeparate) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
extern void         (GL_APIENTRYP glBufferData) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
extern void         (GL_APIENTRYP glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
extern GLenum       (GL_APIENTRYP glCheckFramebufferStatus) (GLenum target);
extern void         (GL_APIENTRYP glClear) (GLbitfield mask);
extern void         (GL_APIENTRYP glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void         (GL_APIENTRYP glClearDepthf) (GLclampf depth);
extern void         (GL_APIENTRYP glClearStencil) (GLint s);
extern void         (GL_APIENTRYP glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void         (GL_APIENTRYP glCompileShader) (GLuint shader);
extern void         (GL_APIENTRYP glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
extern void         (GL_APIENTRYP glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
extern void         (GL_APIENTRYP glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void         (GL_APIENTRYP glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern GLuint       (GL_APIENTRYP glCreateProgram) (void);
extern GLuint       (GL_APIENTRYP glCreateShader) (GLenum type);
extern void         (GL_APIENTRYP glCullFace) (GLenum mode);
extern void         (GL_APIENTRYP glDeleteBuffers) (GLsizei n, const GLuint* buffers);
extern void         (GL_APIENTRYP glDeleteFramebuffers) (GLsizei n, const GLuint* framebuffers);
extern void         (GL_APIENTRYP glDeleteTextures) (GLsizei n, const GLuint* textures);
extern void         (GL_APIENTRYP glDeleteProgram) (GLuint program);
extern void         (GL_APIENTRYP glDeleteRenderbuffers) (GLsizei n, const GLuint* renderbuffers);
extern void         (GL_APIENTRYP glDeleteShader) (GLuint shader);
extern void         (GL_APIENTRYP glDetachShader) (GLuint program, GLuint shader);
extern void         (GL_APIENTRYP glDepthFunc) (GLenum func);
extern void         (GL_APIENTRYP glDepthMask) (GLboolean flag);
extern void         (GL_APIENTRYP glDepthRangef) (GLclampf zNear, GLclampf zFar);
extern void         (GL_APIENTRYP glDisable) (GLenum cap);
extern void         (GL_APIENTRYP glDisableVertexAttribArray) (GLuint index);
extern void         (GL_APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count);
extern void         (GL_APIENTRYP glDrawElements) (GLenum mode, GLsizei count, GLenum type, const void* indices);
extern void         (GL_APIENTRYP glEnable) (GLenum cap);
extern void         (GL_APIENTRYP glEnableVertexAttribArray) (GLuint index);
extern void         (GL_APIENTRYP glFinish) (void);
extern void         (GL_APIENTRYP glFlush) (void);
extern void         (GL_APIENTRYP glFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
extern void         (GL_APIENTRYP glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
extern void         (GL_APIENTRYP glFrontFace) (GLenum mode);
extern void         (GL_APIENTRYP glGenBuffers) (GLsizei n, GLuint* buffers);
extern void         (GL_APIENTRYP glGenerateMipmap) (GLenum target);
extern void         (GL_APIENTRYP glGenFramebuffers) (GLsizei n, GLuint* framebuffers);
extern void         (GL_APIENTRYP glGenRenderbuffers) (GLsizei n, GLuint* renderbuffers);
extern void         (GL_APIENTRYP glGenTextures) (GLsizei n, GLuint* textures);
extern void         (GL_APIENTRYP glGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
extern void         (GL_APIENTRYP glGetActiveUniform) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
extern void         (GL_APIENTRYP glGetAttachedShaders) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
extern int          (GL_APIENTRYP glGetAttribLocation) (GLuint program, const char* name);
extern void         (GL_APIENTRYP glGetBooleanv) (GLenum pname, GLboolean* params);
extern void         (GL_APIENTRYP glGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params);
extern GLenum       (GL_APIENTRYP glGetError) (void);
extern void         (GL_APIENTRYP glGetFloatv) (GLenum pname, GLfloat* params);
extern void         (GL_APIENTRYP glGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetIntegerv) (GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetProgramiv) (GLuint program, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetProgramInfoLog) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
extern void         (GL_APIENTRYP glGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetShaderiv) (GLuint shader, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetShaderInfoLog) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
extern void         (GL_APIENTRYP glGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
extern void         (GL_APIENTRYP glGetShaderSource) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
extern const GLubyte*( GL_APIENTRYP glGetString) (GLenum name);
extern void         (GL_APIENTRYP glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat* params);
extern void         (GL_APIENTRYP glGetTexParameteriv) (GLenum target, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetUniformfv) (GLuint program, GLint location, GLfloat* params);
extern void         (GL_APIENTRYP glGetUniformiv) (GLuint program, GLint location, GLint* params);
extern int          (GL_APIENTRYP glGetUniformLocation) (GLuint program, const char* name);
extern void         (GL_APIENTRYP glGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat* params);
extern void         (GL_APIENTRYP glGetVertexAttribiv) (GLuint index, GLenum pname, GLint* params);
extern void         (GL_APIENTRYP glGetVertexAttribPointerv) (GLuint index, GLenum pname, void** pointer);
extern void         (GL_APIENTRYP glHint) (GLenum target, GLenum mode);
extern GLboolean    (GL_APIENTRYP glIsBuffer) (GLuint buffer);
extern GLboolean    (GL_APIENTRYP glIsEnabled) (GLenum cap);
extern GLboolean    (GL_APIENTRYP glIsFramebuffer) (GLuint framebuffer);
extern GLboolean    (GL_APIENTRYP glIsProgram) (GLuint program);
extern GLboolean    (GL_APIENTRYP glIsRenderbuffer) (GLuint renderbuffer);
extern GLboolean    (GL_APIENTRYP glIsShader) (GLuint shader);
extern GLboolean    (GL_APIENTRYP glIsTexture) (GLuint texture);
extern void         (GL_APIENTRYP glLineWidth) (GLfloat width);
extern void         (GL_APIENTRYP glLinkProgram) (GLuint program);
extern void         (GL_APIENTRYP glPixelStorei) (GLenum pname, GLint param);
extern void         (GL_APIENTRYP glPolygonOffset) (GLfloat factor, GLfloat units);
extern void         (GL_APIENTRYP glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
extern void         (GL_APIENTRYP glReleaseShaderCompiler) (void);
extern void         (GL_APIENTRYP glRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
extern void         (GL_APIENTRYP glSampleCoverage) (GLclampf value, GLboolean invert);
extern void         (GL_APIENTRYP glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
extern void         (GL_APIENTRYP glShaderBinary) (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
extern void         (GL_APIENTRYP glShaderSource) (GLuint shader, GLsizei count, const char** string, const GLint* length);
extern void         (GL_APIENTRYP glStencilFunc) (GLenum func, GLint ref, GLuint mask);
extern void         (GL_APIENTRYP glStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
extern void         (GL_APIENTRYP glStencilMask) (GLuint mask);
extern void         (GL_APIENTRYP glStencilMaskSeparate) (GLenum face, GLuint mask);
extern void         (GL_APIENTRYP glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
extern void         (GL_APIENTRYP glStencilOpSeparate) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
extern void         (GL_APIENTRYP glTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
extern void         (GL_APIENTRYP glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
extern void         (GL_APIENTRYP glTexParameterfv) (GLenum target, GLenum pname, const GLfloat* params);
extern void         (GL_APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param);
extern void         (GL_APIENTRYP glTexParameteriv) (GLenum target, GLenum pname, const GLint* params);
extern void         (GL_APIENTRYP glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
extern void         (GL_APIENTRYP glUniform1f) (GLint location, GLfloat x);
extern void         (GL_APIENTRYP glUniform1fv) (GLint location, GLsizei count, const GLfloat* v);
extern void         (GL_APIENTRYP glUniform1i) (GLint location, GLint x);
extern void         (GL_APIENTRYP glUniform1iv) (GLint location, GLsizei count, const GLint* v);
extern void         (GL_APIENTRYP glUniform2f) (GLint location, GLfloat x, GLfloat y);
extern void         (GL_APIENTRYP glUniform2fv) (GLint location, GLsizei count, const GLfloat* v);
extern void         (GL_APIENTRYP glUniform2i) (GLint location, GLint x, GLint y);
extern void         (GL_APIENTRYP glUniform2iv) (GLint location, GLsizei count, const GLint* v);
extern void         (GL_APIENTRYP glUniform3f) (GLint location, GLfloat x, GLfloat y, GLfloat z);
extern void         (GL_APIENTRYP glUniform3fv) (GLint location, GLsizei count, const GLfloat* v);
extern void         (GL_APIENTRYP glUniform3i) (GLint location, GLint x, GLint y, GLint z);
extern void         (GL_APIENTRYP glUniform3iv) (GLint location, GLsizei count, const GLint* v);
extern void         (GL_APIENTRYP glUniform4f) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void         (GL_APIENTRYP glUniform4fv) (GLint location, GLsizei count, const GLfloat* v);
extern void         (GL_APIENTRYP glUniform4i) (GLint location, GLint x, GLint y, GLint z, GLint w);
extern void         (GL_APIENTRYP glUniform4iv) (GLint location, GLsizei count, const GLint* v);
extern void         (GL_APIENTRYP glUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void         (GL_APIENTRYP glUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void         (GL_APIENTRYP glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
extern void         (GL_APIENTRYP glUseProgram) (GLuint program);
extern void         (GL_APIENTRYP glValidateProgram) (GLuint program);
extern void         (GL_APIENTRYP glVertexAttrib1f) (GLuint indx, GLfloat x);
extern void         (GL_APIENTRYP glVertexAttrib1fv) (GLuint indx, const GLfloat* values);
extern void         (GL_APIENTRYP glVertexAttrib2f) (GLuint indx, GLfloat x, GLfloat y);
extern void         (GL_APIENTRYP glVertexAttrib2fv) (GLuint indx, const GLfloat* values);
extern void         (GL_APIENTRYP glVertexAttrib3f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
extern void         (GL_APIENTRYP glVertexAttrib3fv) (GLuint indx, const GLfloat* values);
extern void         (GL_APIENTRYP glVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void         (GL_APIENTRYP glVertexAttrib4fv) (GLuint indx, const GLfloat* values);
extern void         (GL_APIENTRYP glVertexAttribPointer) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
extern void         (GL_APIENTRYP glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);

#pragma warning(pop)

} //!namespace libgb

#endif //!LIBGB_GLSTUB_H_
