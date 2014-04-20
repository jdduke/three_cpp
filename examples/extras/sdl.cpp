#include "examples/extras/sdl.h"

#include "three/console.h"
#include "three/core/clock.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"

#include "examples/extras/stats.h"

// TODO(jdduke): Include gles where appropriate.
#include "SDL_assert.h"
#include "SDL_timer.h"

using namespace three;

namespace three_examples {
namespace {

struct SDLGLInterface : public GLInterface {
  SDLGLInterface() {
    BindTexture = glBindTexture;
    BlendFunc = glBlendFunc;
    Clear = glClear;
    ClearColor = glClearColor;
    ClearDepth = glClearDepth;
    ClearStencil = glClearStencil;
    ColorMask = glColorMask;
    CullFace = glCullFace;
    DeleteTextures = glDeleteTextures;
    DepthFunc = glDepthFunc;
    DepthMask = glDepthMask;
    Disable = glDisable;
    DrawArrays = glDrawArrays;
    DrawElements = glDrawElements;
    Enable = glEnable;
    FrontFace = glFrontFace;
    Finish = glFinish;
    Flush = glFlush;
    GenTextures = glGenTextures;
    GetError = glGetError;
    GetFloatv = glGetFloatv;
    GetIntegerv = glGetIntegerv;
    GetTexParameteriv = glGetTexParameteriv;
    GetTexParameterfv = glGetTexParameterfv;
    GetString = glGetString;
    LineWidth = glLineWidth;
    PolygonOffset = glPolygonOffset;
    Scissor = glScissor;
    StencilFunc = glStencilFunc;
    StencilMask = glStencilMask;
    StencilOp = glStencilOp;
    TexImage2D = glTexImage2D;
    TexParameterf = glTexParameterf;
    TexParameterfv = glTexParameterfv;
    TexParameteri = glTexParameteri;
    TexParameteriv = glTexParameteriv;
    TexSubImage2D = glTexSubImage2D;
    Viewport = glViewport;

  #define M_CONC(A, B) M_CONC_(A, B)
  #define M_CONC_(A, B) A##B
  #if defined(__APPLE__) || defined(THREE_GLES)
  #define SDL_GL_GET_PROC(F) F = gl ## F;
  #else
  #define SDL_GL_GET_PROC(F) F = (GL_ ## F ## _Func) SDL_GL_GetProcAddress("gl" #F);
  #endif

    SDL_GL_GET_PROC( ActiveTexture );
    SDL_GL_GET_PROC( AttachShader );
    SDL_GL_GET_PROC( BindAttribLocation );
    SDL_GL_GET_PROC( BindBuffer );
    SDL_GL_GET_PROC( BlendEquation );
    SDL_GL_GET_PROC( BlendEquationSeparate );
    SDL_GL_GET_PROC( BlendFuncSeparate );
    SDL_GL_GET_PROC( BufferData );
    SDL_GL_GET_PROC( BufferSubData );
    SDL_GL_GET_PROC( CompileShader );
    SDL_GL_GET_PROC( CreateProgram );
    SDL_GL_GET_PROC( CreateShader );
    SDL_GL_GET_PROC( DeleteBuffers );
    SDL_GL_GET_PROC( DeleteFramebuffers );
    SDL_GL_GET_PROC( DeleteProgram );
    SDL_GL_GET_PROC( DeleteRenderbuffers );
    SDL_GL_GET_PROC( DeleteShader );
    SDL_GL_GET_PROC( DisableVertexAttribArray );
    SDL_GL_GET_PROC( EnableVertexAttribArray );
    SDL_GL_GET_PROC( GenBuffers );
    SDL_GL_GET_PROC( GenFramebuffers );
    SDL_GL_GET_PROC( GenRenderbuffers );
    SDL_GL_GET_PROC( GetAttribLocation );
    SDL_GL_GET_PROC( GetBufferParameteriv );
    SDL_GL_GET_PROC( GenerateMipmap );
    SDL_GL_GET_PROC( GetProgramInfoLog );
    SDL_GL_GET_PROC( GetProgramiv );
    SDL_GL_GET_PROC( GetShaderInfoLog );
    SDL_GL_GET_PROC( GetShaderiv );
    SDL_GL_GET_PROC( GetUniformLocation );
    SDL_GL_GET_PROC( LinkProgram );
    SDL_GL_GET_PROC( ShaderSource );
    SDL_GL_GET_PROC( Uniform1f );
    SDL_GL_GET_PROC( Uniform1i );
    SDL_GL_GET_PROC( Uniform1fv );
    SDL_GL_GET_PROC( Uniform1iv );
    SDL_GL_GET_PROC( Uniform2f );
    SDL_GL_GET_PROC( Uniform2i );
    SDL_GL_GET_PROC( Uniform2fv );
    SDL_GL_GET_PROC( Uniform2iv );
    SDL_GL_GET_PROC( Uniform3f );
    SDL_GL_GET_PROC( Uniform3i );
    SDL_GL_GET_PROC( Uniform3fv );
    SDL_GL_GET_PROC( Uniform3iv );
    SDL_GL_GET_PROC( Uniform4f );
    SDL_GL_GET_PROC( Uniform4i );
    SDL_GL_GET_PROC( Uniform4fv );
    SDL_GL_GET_PROC( Uniform4iv );
    SDL_GL_GET_PROC( UniformMatrix2fv );
    SDL_GL_GET_PROC( UniformMatrix3fv );
    SDL_GL_GET_PROC( UniformMatrix4fv );
    SDL_GL_GET_PROC( UseProgram );
    SDL_GL_GET_PROC( VertexAttribPointer );
    SDL_GL_GET_PROC( BindFramebuffer );
    SDL_GL_GET_PROC( FramebufferTexture2D );
    SDL_GL_GET_PROC( DeleteFramebuffers );
    SDL_GL_GET_PROC( RenderbufferStorage );
    SDL_GL_GET_PROC( GenRenderbuffers );
    SDL_GL_GET_PROC( DeleteRenderbuffers );
    SDL_GL_GET_PROC( FramebufferRenderbuffer );
    SDL_GL_GET_PROC( BindRenderbuffer );
  }
};

} // namepace

GLWindow::GLWindow( const RendererParameters& parameters )
    : window( nullptr ),
      context( nullptr ),
      renderStats( true ) {

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

  if ( parameters.vsync )
    SDL_GL_SetSwapInterval( 1 );

  /*
  if ( parameters.antialias ) {
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
    SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
  }
  */

  unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
  window = SDL_CreateWindow( "three.cpp",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             parameters.width,
                             parameters.height,
                             flags );
  if ( !window ) {
    console().error() << "Error creating window: " << SDL_GetError();
    return;
  }

  context = SDL_GL_CreateContext( window );
  if ( !context ) {
    console().error() << "Error creating GL context: " << SDL_GetError();
    return;
  }

  console().log() << "SDL initialized";
}

GLWindow::~GLWindow() {
  if ( context )
    SDL_GL_DeleteContext( context );
  if ( window )
    SDL_DestroyWindow( window );
}

three::GLInterface GLWindow::createGLInterface() {
  return SDLGLInterface();
}

void GLWindow::addEventListener( EventType eventType, EventListener eventListener ) {
  listeners[ eventType ].emplace_back( std::move(eventListener) );
}

void GLWindow::animate( Update update ) {
  SDL_assert( valid() );

/*
  RendererParameters statsRendererParams;
  statsRendererParams.preserveDrawingBuffer = true;
  auto statsRenderer = GLRenderer::create( statsRendererParams, createGLInterface() );
  Stats stats( *statsRenderer );
*/
  Clock clock;

  auto lastUpdateTime = clock.getElapsedTime();

  while ( true ) {

    const auto currentTime = clock.getElapsedTime();
    const auto deltaTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    if ( !processEvents() )
      return;

    if ( !update( deltaTime ) )
      return;

    // stats.update( deltaTime, renderStats );

    swapBuffers();
  }
}

bool GLWindow::valid() const {
  return window && context;
}

void GLWindow::swapBuffers() {
  SDL_GL_SwapWindow( window );
}

bool GLWindow::processEvents() {
  SDL_Event event;
  while ( SDL_PollEvent( &event ) ) {
    if ( event.type == SDL_QUIT )
      return false;

    if ( event.type == SDL_KEYDOWN ) {
      switch ( event.key.keysym.sym ) {
        case SDLK_q:
        case SDLK_ESCAPE:
          return false;
        default:
          renderStats = !renderStats;
          break;
      };
    }

    auto eventTypeListenersIt = listeners.find( static_cast<EventType>( event.type ) );
    if ( eventTypeListenersIt == listeners.end() )
      continue;

    for ( const auto& listener : eventTypeListenersIt->second )
      listener( event );
  }
  return true;
}

} // namespace three_examples

