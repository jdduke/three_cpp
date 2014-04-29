#include <three/gl.h>

#include <three/console.h>

namespace three {

namespace {
const char* errorStringImpl( GLenum err ) {
  switch(err) {
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
#if !defined(THREE_GLES)
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
#endif
    default:                   return "Unknown error";
  }
}

} // namespace

bool GLInterface::validate() const {
#define CHECK_PTR(FUNC)                                                  \
  if (!(FUNC)) {                                                         \
    console().warn() << "THREE: gl function does not exist - " << #FUNC; \
    return false;                                                        \
  }
#define GL_FUNC_DECL(PFUNC, FUNC) CHECK_PTR(FUNC)
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) CHECK_PTR(FUNC)
#include "three/gl_functions.h"
#undef GL_FUNC_DECL
#undef GL_FUNC_EXT_DECL
#undef CHECK_PTR

  return true;
}

GLInterfaceWrapper::GLInterfaceWrapper( const GLInterface& gl )
    : GLInterface( gl ) {}

int GLInterfaceWrapper::Error( const char* file, int line ) const {
  int retCode = 0;
  auto err = GetError();
  if ( err != GL_NO_ERROR ) {
    console().warn() << "THREE: gl error in file " << file
                     << " @ line "                 << line
                     << ": "                       << errorStringImpl( err );
    retCode = 1;
  }
  return retCode;
}

GLuint GLInterfaceWrapper::CreateBuffer() const {
  GLuint buffer = 0;
  GenBuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteBuffer( GLuint& buffer ) const {
  DeleteBuffers( 1, &buffer );
  buffer = 0;
}

GLuint GLInterfaceWrapper::CreateTexture() const {
  GLuint texture = 0;
  GenTextures( 1, &texture );
  return texture;
}

void GLInterfaceWrapper::DeleteTexture( GLuint& texture ) const {
  DeleteTextures( 1, &texture );
  texture = 0;
}

GLuint GLInterfaceWrapper::CreateFramebuffer() const {
  GLuint buffer = 0;
  GenFramebuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteFramebuffer( GLuint& buffer ) const {
  DeleteFramebuffers( 1, &buffer );
  buffer = 0;
}

GLuint GLInterfaceWrapper::CreateRenderbuffer() const {
  GLuint buffer = 0;
  GenRenderbuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteRenderbuffer( GLuint& buffer ) const {
  DeleteRenderbuffers( 1, &buffer );
  buffer = 0;
}

GLint GLInterfaceWrapper::GetParameteri( GLenum pname ) const {
  GLint parameter = 0;
  GetIntegerv( pname, &parameter );
  return parameter;
}

GLfloat GLInterfaceWrapper::GetParameterf( GLenum pname ) const {
  GLfloat parameter = 0;
  GetFloatv( pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetTexParameteri( GLenum pname ) const {
  GLint parameter = 0;
  GetTexParameteriv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLfloat GLInterfaceWrapper::GetTexParameterf( GLenum pname ) const {
  GLfloat parameter = 0;
  GetTexParameterfv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetProgramParameter( GLuint program, GLenum pname ) const {
  GLint parameter = 0;
  GetProgramiv( program, pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetShaderParameter( GLuint program, GLenum pname ) const {
  GLint parameter = 0;
  GetShaderiv( program, pname, &parameter );
  return parameter;
}

} // namespace three
