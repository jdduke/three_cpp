#include "examples/extras/sdl.h"

#include "three/console.h"
#include "three/core/clock.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"

#include "examples/extras/stats.h"

// TODO(jdduke): Include gles where appropriate.
#include <SDL2/SDL_assert.h>
#include <SDL2/SDL_timer.h>

#define M_CONC(A, B) M_CONC_(A, B)
#define M_CONC_(A, B) A##B

using namespace three;

namespace three_examples {
namespace {

struct SDLGLInterface : public GLInterface {
  SDLGLInterface() {
#define GL_FUNC_DECL(PFUNC, FUNC) FUNC = gl ## FUNC;
#if defined(__APPLE__) || defined(THREE_GLES)
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) GL_FUNC_DECL(PFUNC, FUNC)
#else
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) FUNC = (PFUNC) SDL_GL_GetProcAddress("gl" #FUNC);
#endif
#include "three/gl_functions.h"
#undef GL_FUNC_DECL
#undef GL_FUNC_EXT_DECL
  }
};

} // namepace

GLWindow::GLWindow( const RendererParameters& parameters )
    : window( nullptr ),
      context( nullptr ),
      renderStats( true ) {
          
  if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS ) < 0 ) {
    console().error() << "Unable to initialize SDL: " << SDL_GetError();
    return;
  }

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
  
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

    auto type = (unsigned int)event.type;
      
    auto eventTypeListenersIt = listeners.find( type  );
    if ( eventTypeListenersIt == listeners.end() ) {
      continue;
    }
      
    for ( const auto& listener : eventTypeListenersIt->second ) {
      listener( event );
    }
  }
  return true;
}

} // namespace three_examples

