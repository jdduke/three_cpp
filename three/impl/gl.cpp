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
    case GL_STACK_OVERFLOW:    return "Stack overflow";
    case GL_STACK_UNDERFLOW:   return "Stack underflow";
    default:                   return "Unknown error";
  }
}

} // namespace

bool GLInterface::validate() const {
#define CHECK_PTR(PTR)                                                  \
  if (!(PTR)) {                                                         \
    console().warn() << "THREE: gl function does not exist - " << #PTR; \
    return false;                                                       \
  }
  CHECK_PTR( ActiveTexture );
  CHECK_PTR( AttachShader );
  CHECK_PTR( BindAttribLocation );
  CHECK_PTR( BindBuffer );
  CHECK_PTR( BindFramebuffer );
  CHECK_PTR( BlendEquation );
  CHECK_PTR( BindRenderbuffer );
  CHECK_PTR( BindTexture );
  CHECK_PTR( BlendEquationSeparate );
  CHECK_PTR( BlendFunc );
  CHECK_PTR( BlendFuncSeparate );
  CHECK_PTR( BufferData );
  CHECK_PTR( BufferSubData );
  CHECK_PTR( Clear );
  CHECK_PTR( ClearColor );
  CHECK_PTR( ClearDepth );
  CHECK_PTR( ClearStencil );
  CHECK_PTR( ColorMask );
  CHECK_PTR( CompileShader );
  CHECK_PTR( CreateProgram );
  CHECK_PTR( CreateShader );
  CHECK_PTR( CullFace );
  CHECK_PTR( DeleteBuffers );
  CHECK_PTR( DeleteFramebuffers );
  CHECK_PTR( DeleteProgram );
  CHECK_PTR( DeleteRenderbuffers );
  CHECK_PTR( DeleteShader );
  CHECK_PTR( DeleteTextures );
  CHECK_PTR( DepthFunc );
  CHECK_PTR( DepthMask );
  CHECK_PTR( Disable );
  CHECK_PTR( DisableVertexAttribArray );
  CHECK_PTR( DrawArrays );
  CHECK_PTR( DrawElements );
  CHECK_PTR( Enable );
  CHECK_PTR( EnableVertexAttribArray );
  CHECK_PTR( Finish );
  CHECK_PTR( Flush );
  CHECK_PTR( FramebufferRenderbuffer );
  CHECK_PTR( FramebufferTexture2D );
  CHECK_PTR( FrontFace );
  CHECK_PTR( GenBuffers );
  CHECK_PTR( GenerateMipmap );
  CHECK_PTR( GenFramebuffers );
  CHECK_PTR( GenRenderbuffers );
  CHECK_PTR( GenTextures );
  CHECK_PTR( GetAttribLocation );
  CHECK_PTR( GetBufferParameteriv );
  CHECK_PTR( GetError );
  CHECK_PTR( GetFloatv );
  CHECK_PTR( GetIntegerv );
  CHECK_PTR( GetProgramInfoLog );
  CHECK_PTR( GetProgramiv );
  CHECK_PTR( GetShaderInfoLog );
  CHECK_PTR( GetShaderiv );
  CHECK_PTR( GetString );
  CHECK_PTR( GetTexParameterfv );
  CHECK_PTR( GetTexParameteriv );
  CHECK_PTR( GetUniformLocation );
  CHECK_PTR( LineWidth );
  CHECK_PTR( LinkProgram );
  CHECK_PTR( PolygonOffset );
  CHECK_PTR( RenderbufferStorage );
  CHECK_PTR( Scissor );
  CHECK_PTR( StencilFunc );
  CHECK_PTR( StencilMask );
  CHECK_PTR( StencilOp );
  CHECK_PTR( ShaderSource );
  CHECK_PTR( ShaderSource );
  CHECK_PTR( ShaderSource );
  CHECK_PTR( TexImage2D );
  CHECK_PTR( TexParameterf );
  CHECK_PTR( TexParameterfv );
  CHECK_PTR( TexParameteri );
  CHECK_PTR( TexParameteriv );
  CHECK_PTR( TexSubImage2D );
  CHECK_PTR( Uniform1f );
  CHECK_PTR( Uniform1i );
  CHECK_PTR( Uniform1fv );
  CHECK_PTR( Uniform1iv );
  CHECK_PTR( Uniform2f );
  CHECK_PTR( Uniform2i );
  CHECK_PTR( Uniform2fv );
  CHECK_PTR( Uniform2iv );
  CHECK_PTR( Uniform3f );
  CHECK_PTR( Uniform3i );
  CHECK_PTR( Uniform3fv );
  CHECK_PTR( Uniform3iv );
  CHECK_PTR( Uniform4f );
  CHECK_PTR( Uniform4i );
  CHECK_PTR( Uniform4fv );
  CHECK_PTR( Uniform4iv );
  CHECK_PTR( UniformMatrix2fv );
  CHECK_PTR( UniformMatrix3fv );
  CHECK_PTR( UniformMatrix4fv );
  CHECK_PTR( UseProgram );
  CHECK_PTR( VertexAttribPointer );
  CHECK_PTR( Viewport );
#undef CHECK_PTR
  return true;
}

GLInterfaceWrapper::GLInterfaceWrapper( const GLInterface& gl )
    : GLInterface( gl ) {}

int GLInterfaceWrapper::Error( const char* file, int line ) {
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

GLuint GLInterfaceWrapper::CreateBuffer() {
  GLuint buffer = 0;
  GenBuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteBuffer( GLuint& buffer ) {
  DeleteBuffers( 1, &buffer );
  buffer = 0;
}

GLuint GLInterfaceWrapper::CreateTexture() {
  GLuint texture = 0;
  GenTextures( 1, &texture );
  return texture;
}

void GLInterfaceWrapper::DeleteTexture( GLuint& texture ) {
  DeleteTextures( 1, &texture );
  texture = 0;
}

GLuint GLInterfaceWrapper::CreateFramebuffer() {
  GLuint buffer = 0;
  GenFramebuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteFramebuffer( GLuint& buffer ) {
  DeleteFramebuffers( 1, &buffer );
  buffer = 0;
}

GLuint GLInterfaceWrapper::CreateRenderbuffer() {
  GLuint buffer = 0;
  GenRenderbuffers( 1, &buffer );
  return buffer;
}

void GLInterfaceWrapper::DeleteRenderbuffer( GLuint& buffer ) {
  DeleteRenderbuffers( 1, &buffer );
  buffer = 0;
}

GLint GLInterfaceWrapper::GetParameteri( GLenum pname ) {
  GLint parameter = 0;
  GetIntegerv( pname, &parameter );
  return parameter;
}

GLfloat GLInterfaceWrapper::GetParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  GetFloatv( pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetTexParameteri( GLenum pname ) {
  GLint parameter = 0;
  GetTexParameteriv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLfloat GLInterfaceWrapper::GetTexParameterf( GLenum pname ) {
  GLfloat parameter = 0;
  GetTexParameterfv( GL_TEXTURE_2D, pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetProgramParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  GetProgramiv( program, pname, &parameter );
  return parameter;
}

GLint GLInterfaceWrapper::GetShaderParameter( GLuint program, GLenum pname ) {
  GLint parameter = 0;
  GetShaderiv( program, pname, &parameter );
  return parameter;
}

} // namespace three
