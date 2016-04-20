#include "glstub.hpp"

#include "EGL/egl.h"

namespace libgb {

static Pre pre;

bool glstubInit() {
    #define FIND_PROC(s) s = (decltype(s)(eglGetProcAddress(#s)));
    FIND_PROC(glActiveTexture                      )
    FIND_PROC(glAttachShader                       )
    FIND_PROC(glBindAttribLocation                 )
    FIND_PROC(glBindBuffer                         )
    FIND_PROC(glBindFramebuffer                    )
    FIND_PROC(glBindRenderbuffer                   )
    FIND_PROC(glBindTexture                        )
    FIND_PROC(glBlendColor                         )
    FIND_PROC(glBlendEquation                      )
    FIND_PROC(glBlendEquationSeparate              )
    FIND_PROC(glBlendFunc                          )
    FIND_PROC(glBlendFuncSeparate                  )
    FIND_PROC(glBufferData                         )
    FIND_PROC(glBufferSubData                      )
    FIND_PROC(glCheckFramebufferStatus             )
    FIND_PROC(glClear                              )
    FIND_PROC(glClearColor                         )
    FIND_PROC(glClearDepthf                        )
    FIND_PROC(glClearStencil                       )
    FIND_PROC(glColorMask                          )
    FIND_PROC(glCompileShader                      )
    FIND_PROC(glCompressedTexImage2D               )
    FIND_PROC(glCompressedTexSubImage2D            )
    FIND_PROC(glCopyTexImage2D                     )
    FIND_PROC(glCopyTexSubImage2D                  )
    FIND_PROC(glCreateProgram                      )
    FIND_PROC(glCreateShader                       )
    FIND_PROC(glCullFace                           )
    FIND_PROC(glDeleteBuffers                      )
    FIND_PROC(glDeleteFramebuffers                 )
    FIND_PROC(glDeleteTextures                     )
    FIND_PROC(glDeleteProgram                      )
    FIND_PROC(glDeleteRenderbuffers                )
    FIND_PROC(glDeleteShader                       )
    FIND_PROC(glDetachShader                       )
    FIND_PROC(glDepthFunc                          )
    FIND_PROC(glDepthMask                          )
    FIND_PROC(glDepthRangef                        )
    FIND_PROC(glDisable                            )
    FIND_PROC(glDisableVertexAttribArray           )
    FIND_PROC(glDrawArrays                         )
    FIND_PROC(glDrawElements                       )
    FIND_PROC(glEnable                             )
    FIND_PROC(glEnableVertexAttribArray            )
    FIND_PROC(glFinish                             )
    FIND_PROC(glFlush                              )
    FIND_PROC(glFramebufferRenderbuffer            )
    FIND_PROC(glFramebufferTexture2D               )
    FIND_PROC(glFrontFace                          )
    FIND_PROC(glGenBuffers                         )
    FIND_PROC(glGenerateMipmap                     )
    FIND_PROC(glGenFramebuffers                    )
    FIND_PROC(glGenRenderbuffers                   )
    FIND_PROC(glGenTextures                        )
    FIND_PROC(glGetActiveAttrib                    )
    FIND_PROC(glGetActiveUniform                   )
    FIND_PROC(glGetAttachedShaders                 )
    FIND_PROC(glGetAttribLocation                  )
    FIND_PROC(glGetBooleanv                        )
    FIND_PROC(glGetBufferParameteriv               )
    FIND_PROC(glGetError                           )
    FIND_PROC(glGetFloatv                          )
    FIND_PROC(glGetFramebufferAttachmentParameteriv)
    FIND_PROC(glGetIntegerv                        )
    FIND_PROC(glGetProgramiv                       )
    FIND_PROC(glGetProgramInfoLog                  )
    FIND_PROC(glGetRenderbufferParameteriv         )
    FIND_PROC(glGetShaderiv                        )
    FIND_PROC(glGetShaderInfoLog                   )
    FIND_PROC(glGetShaderPrecisionFormat           )
    FIND_PROC(glGetShaderSource                    )
    FIND_PROC(glGetString                          )
    FIND_PROC(glGetTexParameterfv                  )
    FIND_PROC(glGetTexParameteriv                  )
    FIND_PROC(glGetUniformfv                       )
    FIND_PROC(glGetUniformiv                       )
    FIND_PROC(glGetUniformLocation                 )
    FIND_PROC(glGetVertexAttribfv                  )
    FIND_PROC(glGetVertexAttribiv                  )
    FIND_PROC(glGetVertexAttribPointerv            )
    FIND_PROC(glHint                               )
    FIND_PROC(glIsBuffer                           )
    FIND_PROC(glIsEnabled                          )
    FIND_PROC(glIsFramebuffer                      )
    FIND_PROC(glIsProgram                          )
    FIND_PROC(glIsRenderbuffer                     )
    FIND_PROC(glIsShader                           )
    FIND_PROC(glIsTexture                          )
    FIND_PROC(glLineWidth                          )
    FIND_PROC(glLinkProgram                        )
    FIND_PROC(glPixelStorei                        )
    FIND_PROC(glPolygonOffset                      )
    FIND_PROC(glReadPixels                         )
    FIND_PROC(glReleaseShaderCompiler              )
    FIND_PROC(glRenderbufferStorage                )
    FIND_PROC(glSampleCoverage                     )
    FIND_PROC(glScissor                            )
    FIND_PROC(glShaderBinary                       )
    FIND_PROC(glShaderSource                       )
    FIND_PROC(glStencilFunc                        )
    FIND_PROC(glStencilFuncSeparate                )
    FIND_PROC(glStencilMask                        )
    FIND_PROC(glStencilMaskSeparate                )
    FIND_PROC(glStencilOp                          )
    FIND_PROC(glStencilOpSeparate                  )
    FIND_PROC(glTexImage2D                         )
    FIND_PROC(glTexParameterf                      )
    FIND_PROC(glTexParameterfv                     )
    FIND_PROC(glTexParameteri                      )
    FIND_PROC(glTexParameteriv                     )
    FIND_PROC(glTexSubImage2D                      )
    FIND_PROC(glUniform1f                          )
    FIND_PROC(glUniform1fv                         )
    FIND_PROC(glUniform1i                          )
    FIND_PROC(glUniform1iv                         )
    FIND_PROC(glUniform2f                          )
    FIND_PROC(glUniform2fv                         )
    FIND_PROC(glUniform2i                          )
    FIND_PROC(glUniform2iv                         )
    FIND_PROC(glUniform3f                          )
    FIND_PROC(glUniform3fv                         )
    FIND_PROC(glUniform3i                          )
    FIND_PROC(glUniform3iv                         )
    FIND_PROC(glUniform4f                          )
    FIND_PROC(glUniform4fv                         )
    FIND_PROC(glUniform4i                          )
    FIND_PROC(glUniform4iv                         )
    FIND_PROC(glUniformMatrix2fv                   )
    FIND_PROC(glUniformMatrix3fv                   )
    FIND_PROC(glUniformMatrix4fv                   )
    FIND_PROC(glUseProgram                         )
    FIND_PROC(glValidateProgram                    )
    FIND_PROC(glVertexAttrib1f                     )
    FIND_PROC(glVertexAttrib1fv                    )
    FIND_PROC(glVertexAttrib2f                     )
    FIND_PROC(glVertexAttrib2fv                    )
    FIND_PROC(glVertexAttrib3f                     )
    FIND_PROC(glVertexAttrib3fv                    )
    FIND_PROC(glVertexAttrib4f                     )
    FIND_PROC(glVertexAttrib4fv                    )
    FIND_PROC(glVertexAttribPointer                )
    FIND_PROC(glViewport                           )
    #undef FIND_PROC

    if (!glActiveTexture                      ||
        !glAttachShader                       ||
        !glBindAttribLocation                 ||
        !glBindBuffer                         ||
        !glBindFramebuffer                    ||
        !glBindRenderbuffer                   ||
        !glBindTexture                        ||
        !glBlendColor                         ||
        !glBlendEquation                      ||
        !glBlendEquationSeparate              ||
        !glBlendFunc                          ||
        !glBlendFuncSeparate                  ||
        !glBufferData                         ||
        !glBufferSubData                      ||
        !glCheckFramebufferStatus             ||
        !glClear                              ||
        !glClearColor                         ||
        !glClearDepthf                        ||
        !glClearStencil                       ||
        !glColorMask                          ||
        !glCompileShader                      ||
        !glCompressedTexImage2D               ||
        !glCompressedTexSubImage2D            ||
        !glCopyTexImage2D                     ||
        !glCopyTexSubImage2D                  ||
        !glCreateProgram                      ||
        !glCreateShader                       ||
        !glCullFace                           ||
        !glDeleteBuffers                      ||
        !glDeleteFramebuffers                 ||
        !glDeleteTextures                     ||
        !glDeleteProgram                      ||
        !glDeleteRenderbuffers                ||
        !glDeleteShader                       ||
        !glDetachShader                       ||
        !glDepthFunc                          ||
        !glDepthMask                          ||
        !glDepthRangef                        ||
        !glDisable                            ||
        !glDisableVertexAttribArray           ||
        !glDrawArrays                         ||
        !glDrawElements                       ||
        !glEnable                             ||
        !glEnableVertexAttribArray            ||
        !glFinish                             ||
        !glFlush                              ||
        !glFramebufferRenderbuffer            ||
        !glFramebufferTexture2D               ||
        !glFrontFace                          ||
        !glGenBuffers                         ||
        !glGenerateMipmap                     ||
        !glGenFramebuffers                    ||
        !glGenRenderbuffers                   ||
        !glGenTextures                        ||
        !glGetActiveAttrib                    ||
        !glGetActiveUniform                   ||
        !glGetAttachedShaders                 ||
        !glGetAttribLocation                  ||
        !glGetBooleanv                        ||
        !glGetBufferParameteriv               ||
        !glGetError                           ||
        !glGetFloatv                          ||
        !glGetFramebufferAttachmentParameteriv||
        !glGetIntegerv                        ||
        !glGetProgramiv                       ||
        !glGetProgramInfoLog                  ||
        !glGetRenderbufferParameteriv         ||
        !glGetShaderiv                        ||
        !glGetShaderInfoLog                   ||
        !glGetShaderPrecisionFormat           ||
        !glGetShaderSource                    ||
        !glGetString                          ||
        !glGetTexParameterfv                  ||
        !glGetTexParameteriv                  ||
        !glGetUniformfv                       ||
        !glGetUniformiv                       ||
        !glGetUniformLocation                 ||
        !glGetVertexAttribfv                  ||
        !glGetVertexAttribiv                  ||
        !glGetVertexAttribPointerv            ||
        !glHint                               ||
        !glIsBuffer                           ||
        !glIsEnabled                          ||
        !glIsFramebuffer                      ||
        !glIsProgram                          ||
        !glIsRenderbuffer                     ||
        !glIsShader                           ||
        !glIsTexture                          ||
        !glLineWidth                          ||
        !glLinkProgram                        ||
        !glPixelStorei                        ||
        !glPolygonOffset                      ||
        !glReadPixels                         ||
        !glReleaseShaderCompiler              ||
        !glRenderbufferStorage                ||
        !glSampleCoverage                     ||
        !glScissor                            ||
        !glShaderBinary                       ||
        !glShaderSource                       ||
        !glStencilFunc                        ||
        !glStencilFuncSeparate                ||
        !glStencilMask                        ||
        !glStencilMaskSeparate                ||
        !glStencilOp                          ||
        !glStencilOpSeparate                  ||
        !glTexImage2D                         ||
        !glTexParameterf                      ||
        !glTexParameterfv                     ||
        !glTexParameteri                      ||
        !glTexParameteriv                     ||
        !glTexSubImage2D                      ||
        !glUniform1f                          ||
        !glUniform1fv                         ||
        !glUniform1i                          ||
        !glUniform1iv                         ||
        !glUniform2f                          ||
        !glUniform2fv                         ||
        !glUniform2i                          ||
        !glUniform2iv                         ||
        !glUniform3f                          ||
        !glUniform3fv                         ||
        !glUniform3i                          ||
        !glUniform3iv                         ||
        !glUniform4f                          ||
        !glUniform4fv                         ||
        !glUniform4i                          ||
        !glUniform4iv                         ||
        !glUniformMatrix2fv                   ||
        !glUniformMatrix3fv                   ||
        !glUniformMatrix4fv                   ||
        !glUseProgram                         ||
        !glValidateProgram                    ||
        !glVertexAttrib1f                     ||
        !glVertexAttrib1fv                    ||
        !glVertexAttrib2f                     ||
        !glVertexAttrib2fv                    ||
        !glVertexAttrib3f                     ||
        !glVertexAttrib3fv                    ||
        !glVertexAttrib4f                     ||
        !glVertexAttrib4fv                    ||
        !glVertexAttribPointer                ||
        !glViewport                           
        ) {
      return false;
    }

    return true;
}

#pragma warning(push)
#pragma warning(disable:4229 4005 4273)

void         (GL_APIENTRYP glActiveTexture) (GLenum texture);
void         (GL_APIENTRYP glAttachShader) (GLuint program, GLuint shader);
void         (GL_APIENTRYP glBindAttribLocation) (GLuint program, GLuint index, const char* name);
void         (GL_APIENTRYP glBindBuffer) (GLenum target, GLuint buffer);
void         (GL_APIENTRYP glBindFramebuffer) (GLenum target, GLuint framebuffer);
void         (GL_APIENTRYP glBindRenderbuffer) (GLenum target, GLuint renderbuffer);
void         (GL_APIENTRYP glBindTexture) (GLenum target, GLuint texture);
void         (GL_APIENTRYP glBlendColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void         (GL_APIENTRYP glBlendEquation)( GLenum mode );
void         (GL_APIENTRYP glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
void         (GL_APIENTRYP glBlendFunc) (GLenum sfactor, GLenum dfactor);
void         (GL_APIENTRYP glBlendFuncSeparate) (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void         (GL_APIENTRYP glBufferData) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void         (GL_APIENTRYP glBufferSubData) (GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
GLenum       (GL_APIENTRYP glCheckFramebufferStatus) (GLenum target);
void         (GL_APIENTRYP glClear) (GLbitfield mask);
void         (GL_APIENTRYP glClearColor) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void         (GL_APIENTRYP glClearDepthf) (GLclampf depth);
void         (GL_APIENTRYP glClearStencil) (GLint s);
void         (GL_APIENTRYP glColorMask) (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void         (GL_APIENTRYP glCompileShader) (GLuint shader);
void         (GL_APIENTRYP glCompressedTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
void         (GL_APIENTRYP glCompressedTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void         (GL_APIENTRYP glCopyTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void         (GL_APIENTRYP glCopyTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLuint       (GL_APIENTRYP glCreateProgram) (void);
GLuint       (GL_APIENTRYP glCreateShader) (GLenum type);
void         (GL_APIENTRYP glCullFace) (GLenum mode);
void         (GL_APIENTRYP glDeleteBuffers) (GLsizei n, const GLuint* buffers);
void         (GL_APIENTRYP glDeleteFramebuffers) (GLsizei n, const GLuint* framebuffers);
void         (GL_APIENTRYP glDeleteTextures) (GLsizei n, const GLuint* textures);
void         (GL_APIENTRYP glDeleteProgram) (GLuint program);
void         (GL_APIENTRYP glDeleteRenderbuffers) (GLsizei n, const GLuint* renderbuffers);
void         (GL_APIENTRYP glDeleteShader) (GLuint shader);
void         (GL_APIENTRYP glDetachShader) (GLuint program, GLuint shader);
void         (GL_APIENTRYP glDepthFunc) (GLenum func);
void         (GL_APIENTRYP glDepthMask) (GLboolean flag);
void         (GL_APIENTRYP glDepthRangef) (GLclampf zNear, GLclampf zFar);
void         (GL_APIENTRYP glDisable) (GLenum cap);
void         (GL_APIENTRYP glDisableVertexAttribArray) (GLuint index);
void         (GL_APIENTRYP glDrawArrays) (GLenum mode, GLint first, GLsizei count);
void         (GL_APIENTRYP glDrawElements) (GLenum mode, GLsizei count, GLenum type, const void* indices);
void         (GL_APIENTRYP glEnable) (GLenum cap);
void         (GL_APIENTRYP glEnableVertexAttribArray) (GLuint index);
void         (GL_APIENTRYP glFinish) (void);
void         (GL_APIENTRYP glFlush) (void);
void         (GL_APIENTRYP glFramebufferRenderbuffer) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void         (GL_APIENTRYP glFramebufferTexture2D) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void         (GL_APIENTRYP glFrontFace) (GLenum mode);
void         (GL_APIENTRYP glGenBuffers) (GLsizei n, GLuint* buffers);
void         (GL_APIENTRYP glGenerateMipmap) (GLenum target);
void         (GL_APIENTRYP glGenFramebuffers) (GLsizei n, GLuint* framebuffers);
void         (GL_APIENTRYP glGenRenderbuffers) (GLsizei n, GLuint* renderbuffers);
void         (GL_APIENTRYP glGenTextures) (GLsizei n, GLuint* textures);
void         (GL_APIENTRYP glGetActiveAttrib) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
void         (GL_APIENTRYP glGetActiveUniform) (GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
void         (GL_APIENTRYP glGetAttachedShaders) (GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
int          (GL_APIENTRYP glGetAttribLocation) (GLuint program, const char* name);
void         (GL_APIENTRYP glGetBooleanv) (GLenum pname, GLboolean* params);
void         (GL_APIENTRYP glGetBufferParameteriv) (GLenum target, GLenum pname, GLint* params);
GLenum       (GL_APIENTRYP glGetError) (void);
void         (GL_APIENTRYP glGetFloatv) (GLenum pname, GLfloat* params);
void         (GL_APIENTRYP glGetFramebufferAttachmentParameteriv) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetIntegerv) (GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetProgramiv) (GLuint program, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetProgramInfoLog) (GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
void         (GL_APIENTRYP glGetRenderbufferParameteriv) (GLenum target, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetShaderiv) (GLuint shader, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetShaderInfoLog) (GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
void         (GL_APIENTRYP glGetShaderPrecisionFormat) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
void         (GL_APIENTRYP glGetShaderSource) (GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
const GLubyte*( GL_APIENTRYP glGetString) (GLenum name);
void         (GL_APIENTRYP glGetTexParameterfv) (GLenum target, GLenum pname, GLfloat* params);
void         (GL_APIENTRYP glGetTexParameteriv) (GLenum target, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetUniformfv) (GLuint program, GLint location, GLfloat* params);
void         (GL_APIENTRYP glGetUniformiv) (GLuint program, GLint location, GLint* params);
int          (GL_APIENTRYP glGetUniformLocation) (GLuint program, const char* name);
void         (GL_APIENTRYP glGetVertexAttribfv) (GLuint index, GLenum pname, GLfloat* params);
void         (GL_APIENTRYP glGetVertexAttribiv) (GLuint index, GLenum pname, GLint* params);
void         (GL_APIENTRYP glGetVertexAttribPointerv) (GLuint index, GLenum pname, void** pointer);
void         (GL_APIENTRYP glHint) (GLenum target, GLenum mode);
GLboolean    (GL_APIENTRYP glIsBuffer) (GLuint buffer);
GLboolean    (GL_APIENTRYP glIsEnabled) (GLenum cap);
GLboolean    (GL_APIENTRYP glIsFramebuffer) (GLuint framebuffer);
GLboolean    (GL_APIENTRYP glIsProgram) (GLuint program);
GLboolean    (GL_APIENTRYP glIsRenderbuffer) (GLuint renderbuffer);
GLboolean    (GL_APIENTRYP glIsShader) (GLuint shader);
GLboolean    (GL_APIENTRYP glIsTexture) (GLuint texture);
void         (GL_APIENTRYP glLineWidth) (GLfloat width);
void         (GL_APIENTRYP glLinkProgram) (GLuint program);
void         (GL_APIENTRYP glPixelStorei) (GLenum pname, GLint param);
void         (GL_APIENTRYP glPolygonOffset) (GLfloat factor, GLfloat units);
void         (GL_APIENTRYP glReadPixels) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
void         (GL_APIENTRYP glReleaseShaderCompiler) (void);
void         (GL_APIENTRYP glRenderbufferStorage) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void         (GL_APIENTRYP glSampleCoverage) (GLclampf value, GLboolean invert);
void         (GL_APIENTRYP glScissor) (GLint x, GLint y, GLsizei width, GLsizei height);
void         (GL_APIENTRYP glShaderBinary) (GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
void         (GL_APIENTRYP glShaderSource) (GLuint shader, GLsizei count, const char** string, const GLint* length);
void         (GL_APIENTRYP glStencilFunc) (GLenum func, GLint ref, GLuint mask);
void         (GL_APIENTRYP glStencilFuncSeparate) (GLenum face, GLenum func, GLint ref, GLuint mask);
void         (GL_APIENTRYP glStencilMask) (GLuint mask);
void         (GL_APIENTRYP glStencilMaskSeparate) (GLenum face, GLuint mask);
void         (GL_APIENTRYP glStencilOp) (GLenum fail, GLenum zfail, GLenum zpass);
void         (GL_APIENTRYP glStencilOpSeparate) (GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
void         (GL_APIENTRYP glTexImage2D) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
void         (GL_APIENTRYP glTexParameterf) (GLenum target, GLenum pname, GLfloat param);
void         (GL_APIENTRYP glTexParameterfv) (GLenum target, GLenum pname, const GLfloat* params);
void         (GL_APIENTRYP glTexParameteri) (GLenum target, GLenum pname, GLint param);
void         (GL_APIENTRYP glTexParameteriv) (GLenum target, GLenum pname, const GLint* params);
void         (GL_APIENTRYP glTexSubImage2D) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void         (GL_APIENTRYP glUniform1f) (GLint location, GLfloat x);
void         (GL_APIENTRYP glUniform1fv) (GLint location, GLsizei count, const GLfloat* v);
void         (GL_APIENTRYP glUniform1i) (GLint location, GLint x);
void         (GL_APIENTRYP glUniform1iv) (GLint location, GLsizei count, const GLint* v);
void         (GL_APIENTRYP glUniform2f) (GLint location, GLfloat x, GLfloat y);
void         (GL_APIENTRYP glUniform2fv) (GLint location, GLsizei count, const GLfloat* v);
void         (GL_APIENTRYP glUniform2i) (GLint location, GLint x, GLint y);
void         (GL_APIENTRYP glUniform2iv) (GLint location, GLsizei count, const GLint* v);
void         (GL_APIENTRYP glUniform3f) (GLint location, GLfloat x, GLfloat y, GLfloat z);
void         (GL_APIENTRYP glUniform3fv) (GLint location, GLsizei count, const GLfloat* v);
void         (GL_APIENTRYP glUniform3i) (GLint location, GLint x, GLint y, GLint z);
void         (GL_APIENTRYP glUniform3iv) (GLint location, GLsizei count, const GLint* v);
void         (GL_APIENTRYP glUniform4f) (GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void         (GL_APIENTRYP glUniform4fv) (GLint location, GLsizei count, const GLfloat* v);
void         (GL_APIENTRYP glUniform4i) (GLint location, GLint x, GLint y, GLint z, GLint w);
void         (GL_APIENTRYP glUniform4iv) (GLint location, GLsizei count, const GLint* v);
void         (GL_APIENTRYP glUniformMatrix2fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         (GL_APIENTRYP glUniformMatrix3fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         (GL_APIENTRYP glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void         (GL_APIENTRYP glUseProgram) (GLuint program);
void         (GL_APIENTRYP glValidateProgram) (GLuint program);
void         (GL_APIENTRYP glVertexAttrib1f) (GLuint indx, GLfloat x);
void         (GL_APIENTRYP glVertexAttrib1fv) (GLuint indx, const GLfloat* values);
void         (GL_APIENTRYP glVertexAttrib2f) (GLuint indx, GLfloat x, GLfloat y);
void         (GL_APIENTRYP glVertexAttrib2fv) (GLuint indx, const GLfloat* values);
void         (GL_APIENTRYP glVertexAttrib3f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z);
void         (GL_APIENTRYP glVertexAttrib3fv) (GLuint indx, const GLfloat* values);
void         (GL_APIENTRYP glVertexAttrib4f) (GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void         (GL_APIENTRYP glVertexAttrib4fv) (GLuint indx, const GLfloat* values);
void         (GL_APIENTRYP glVertexAttribPointer) (GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
void         (GL_APIENTRYP glViewport) (GLint x, GLint y, GLsizei width, GLsizei height);
#pragma warning(pop)

}//!namespace libgb