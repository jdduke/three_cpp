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
#  include <GL/glew.h>
#else
#  include <GL/gl.h>
#endif

namespace three {

#ifndef THREE_GLES

inline GLuint glCreateBuffer() {
	GLuint buffer = 0;
	glGenBuffers( 1, &buffer );
	return buffer;
}

inline GLuint glDeleteBuffer( GLuint& buffer ) {
	glDeleteBuffers( 1, &buffer );
	buffer = 0;
}

#endif


}

#endif // THREE_GL_HPP
