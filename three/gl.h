#ifndef THREE_GL_HPP
#define THREE_GL_HPP

#ifndef THREE_GLES
#  ifndef THREE_GLEW
#    define THREE_GLEW
#  endif
#endif

#ifdef THREE_NO_GLEW
#  undef THREE_GLEW
#endif

#if defined(THREE_GLES) && defined(THREE_GLEW)
#  error THREE_GLES and THREE_GLEW cannot be simultaneously defined
#endif

#if defined(THREE_GLES)
#  include <GLES/gl.h>
#elif defined(THREE_GLEW)
#  define NO_SDL_GLEXT
#  include <GL/glew.h>
#  if defined(_WIN32)
#    if !defined(NOMINMAX)
#      define NOMINMAX
#    endif
#    include <GL/wglew.h>
#    undef near
#    undef far
#  endif
#else
#  include <GL/gl.h>
#endif

#include <three/console.hpp>

namespace three {

#ifdef _DEBUG
#define GL_CALL(a) (a); glError(__FILE__, __LINE__)
#else
#define GL_CALL(a) (a)
#endif

namespace {
inline const char* glErrorStringImpl( GLenum err ) {
  switch(err) {
    case GL_INVALID_ENUM:      return "Invalid enum";
    case GL_INVALID_OPERATION: return "Invalid operation";
    case GL_INVALID_VALUE:     return "Invalid value";
    case GL_OUT_OF_MEMORY:     return "Out of memory";
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    case GL_TABLE_TOO_LARGE:   return "Table too large";
    default:                   return "Unknown error";
  }
}
} // namespace

inline int glError( const char* file, int line ) {
  int retCode = 0;
  auto err = glGetError();
  if ( err != GL_NO_ERROR ) {
    console().warn() << "THREE: glError in file " << file
                     << " @ line "                << line
                     << ": "                      << glErrorStringImpl( err );
    retCode = 1;
  }
  return retCode;
}

inline GLuint glCreateBuffer() {
  GLuint buffer = 0;
  glGenBuffers( 1, &buffer );
  return buffer;
}

inline void glDeleteBuffer( GLuint& buffer ) {
  glDeleteBuffers( 1, &buffer );
  buffer = 0;
}

inline GLuint glCreateTexture() {
  GLuint texture = 0;
  glGenTextures( 1, &texture );
  return texture;
}

inline void glDeleteTexture( GLuint& texture ) {
  glDeleteTextures( 1, &texture );
  texture = 0;
}

inline GLuint glCreateFramebuffer() {
  GLuint buffer = 0;
  glGenFramebuffers( 1, &buffer );
  return buffer;
}

inline void glDeleteFramebuffer( GLuint& buffer ) {
  glDeleteFramebuffers( 1, &buffer );
  buffer = 0;
}

inline GLuint glCreateRenderbuffer() {
  GLuint buffer = 0;
  glGenRenderbuffers( 1, &buffer );
  return buffer;
}

inline void glDeleteRenderbuffer( GLuint& buffer ) {
  glDeleteRenderbuffers( 1, &buffer );
  buffer = 0;
}

inline GLint glGetParameteri( GLenum pname ) {
  GLint parameter = 0;
  glGetIntegerv( pname, &parameter );
  return parameter;
}

inline GLfloat glGetParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  glGetFloatv( pname, &parameter );
  return parameter;
}

inline GLint glGetTexParameteri( GLenum pname ) {
  GLint parameter = 0;
  glGetTexParameteriv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

inline GLfloat glGetTexParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  glGetTexParameterfv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

inline GLint glGetProgramParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  glGetProgramiv( program, pname, &parameter );
  return parameter;
}

inline GLint glGetShaderParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  glGetShaderiv( program, pname, &parameter );
  return parameter;
}

inline bool glTrue( GLboolean b ) {
  return b == GL_TRUE;
}

template < typename C >
inline void glBindAndBuffer( GLenum target, unsigned buffer, const C& container, GLenum usage ) {
  glBindBuffer( target, buffer );
  glBufferData( target, container.size() * sizeof( container[0] ), container.data(), usage );
}

inline void glEnableVSync( bool enable ) {
#if defined (_WIN32)
  if (wglewIsSupported("WGL_EXT_swap_control")) {
    wglSwapIntervalEXT( enable ? 1 : 0 );
  }
#endif
}

} // namespace three

#endif // THREE_GL_HPP
