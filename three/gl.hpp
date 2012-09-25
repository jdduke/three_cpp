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
#else
#  include <GL/gl.h>
#endif

//#if defined(THREE_SDL)
//#include <SDL_opengl.h>
//#endif

namespace three {

#ifdef _DEBUG
#define printGLError() glError(__FILE__, __LINE__)
#define GL_CALL(a) (a); printGLError();
#else
#define printGLError() glError(__FILE__, __LINE__)
#define GL_CALL(a) (a)
#endif

inline int glError( const char *file, int line ) {
	int retCode = 0;
	auto err = glGetError();
	if ( err != GL_NO_ERROR ) {
		console().warn() << "THREE: glError in file " << file
		                 << " @ line "                << line
		                 << ": "                      << gluErrorString(err);
		retCode = 1;
	}
	return retCode;
}

#ifndef THREE_GLES

inline GLuint glCreateBuffer() {
	GLuint buffer =	0;
	GL_CALL( glGenBuffers( 1, &buffer ) );
	return buffer;
}

inline void	glDeleteBuffer(	GLuint&	buffer ) {
	GL_CALL( glDeleteBuffers( 1, &buffer ) );
	buffer = 0;
}

inline GLuint glCreateTexture()	{
	GLuint texture = 0;
	GL_CALL( glGenTextures( 1, &texture ) );
	return texture;
}

inline void	glDeleteTexture( GLuint& texture ) {
	GL_CALL( glDeleteTextures( 1, &texture ) );
	texture	= 0;
}

inline GLuint glCreateFramebuffer()	{
	GLuint buffer =	0;
	GL_CALL( glGenFramebuffers( 1, &buffer ) );
	return buffer;
}

inline void glDeleteFramebuffer( GLuint& buffer ) {
	GL_CALL( glDeleteFramebuffers( 1, &buffer ) );
	buffer = 0;
}

inline GLuint glCreateRenderbuffer() {
	GLuint buffer =	0;
	GL_CALL( glGenRenderbuffers(	1, &buffer ) );
	return buffer;
}

inline void	glDeleteRenderbuffer( GLuint& buffer ) {
	GL_CALL( glDeleteRenderbuffers( 1, &buffer ) );
	buffer = 0;
}

GLint glGetParameteri( GLenum pname	) {
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
	glGetTexParameterfv( GL_TEXTURE_2D,	pname, &parameter );
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

bool glTrue( GLboolean b ) {
	return b ==	GL_TRUE;
}

#endif

template < typename	C >
void glBindAndBuffer(int target, unsigned buffer, const	C& container, int usage) {
	GL_CALL( glBindBuffer( target, buffer ) );
	GL_CALL( glBufferData( target, container.size() * sizeof(container[0]), container.data(), usage ) );
}

} // namespace three

#endif // THREE_GL_HPP
