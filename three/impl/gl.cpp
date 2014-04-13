#include <three/gl.h>

#include <three/console.h>

namespace three {

namespace {
const char* glErrorStringImpl( GLenum err ) {
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

int glError( const char* file, int line ) {
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

bool glTrue( GLboolean b ) {
  return b == GL_TRUE;
}

GLuint glCreateBuffer() {
  GLuint buffer = 0;
  glGenBuffers( 1, &buffer );
  return buffer;
}

void glDeleteBuffer( GLuint& buffer ) {
  glDeleteBuffers( 1, &buffer );
  buffer = 0;
}

GLuint glCreateTexture() {
  GLuint texture = 0;
  glGenTextures( 1, &texture );
  return texture;
}

void glDeleteTexture( GLuint& texture ) {
  glDeleteTextures( 1, &texture );
  texture = 0;
}

GLuint glCreateFramebuffer() {
  GLuint buffer = 0;
  glGenFramebuffers( 1, &buffer );
  return buffer;
}

void glDeleteFramebuffer( GLuint& buffer ) {
  glDeleteFramebuffers( 1, &buffer );
  buffer = 0;
}

GLuint glCreateRenderbuffer() {
  GLuint buffer = 0;
  glGenRenderbuffers( 1, &buffer );
  return buffer;
}

void glDeleteRenderbuffer( GLuint& buffer ) {
  glDeleteRenderbuffers( 1, &buffer );
  buffer = 0;
}

GLint glGetParameteri( GLenum pname ) {
  GLint parameter = 0;
  glGetIntegerv( pname, &parameter );
  return parameter;
}

GLfloat glGetParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  glGetFloatv( pname, &parameter );
  return parameter;
}

GLint glGetTexParameteri( GLenum pname ) {
  GLint parameter = 0;
  glGetTexParameteriv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLfloat glGetTexParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  glGetTexParameterfv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLint glGetProgramParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  glGetProgramiv( program, pname, &parameter );
  return parameter;
}

GLint glGetShaderParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  glGetShaderiv( program, pname, &parameter );
  return parameter;
}

void glEnableVSync( bool enable ) {
#if defined (_WIN32)
  if (wglewIsSupported("WGL_EXT_swap_control")) {
    wglSwapIntervalEXT( enable ? 1 : 0 );
  }
#endif
}

} // namespace three
