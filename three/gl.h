#ifndef THREE_GL_H
#define THREE_GL_H

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
#  if defined(__APPLE__)
#    include <OpenGL/gl.h>
#  else
#    include <GL/gl.h>
#  endif
#endif

#ifndef NDEBUG
#define GL_CALL(a) do { (a); glError(__FILE__, __LINE__) } while (0)
#else
#define GL_CALL(a) (a)
#endif

namespace three {

int glError( const char* file, int line );
bool glTrue( GLboolean b );
GLuint glCreateBuffer();
void glDeleteBuffer( GLuint& buffer );
GLuint glCreateTexture();
void glDeleteTexture( GLuint& texture );
GLuint glCreateFramebuffer();
void glDeleteFramebuffer( GLuint& buffer );
GLuint glCreateRenderbuffer();
void glDeleteRenderbuffer( GLuint& buffer );
GLint glGetParameteri( GLenum pname );
GLfloat glGetParameterf( GLenum pname );
GLint glGetTexParameteri( GLenum pname );
GLfloat glGetTexParameterf( GLenum pname );
GLint glGetProgramParameter( GLuint program, GLenum pname );
GLint glGetShaderParameter( GLuint program, GLenum pname );
void glEnableVSync( bool enable );

template < typename C >
inline void glBindAndBuffer( GLenum target, unsigned buffer, const C& container, GLenum usage ) {
  glBindBuffer( target, buffer );
  glBufferData( target, container.size() * sizeof( container[0] ), container.data(), usage );
}

} // namespace three

#endif // THREE_GL_H
