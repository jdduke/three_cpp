#ifndef THREE_EXAMPLES_COMMON_H
#define THREE_EXAMPLES_COMMON_H

#include <three/config.h>
#include <three/gl.h>

#include <three/renderers/gl_renderer.h>
#include <three/renderers/renderer_parameters.h>

#include <three/utils/conversion.h>
#include <three/utils/template.h>

#if defined(THREE_GLEW)
#include <three/extras/glew.h>
#endif

#include "examples/extras/anim.h"
#include "examples/extras/sdl.h"
#include "examples/extras/stats.h"

namespace three {
namespace {

struct ExampleSession {

  ExampleSession(  three::RendererParameters parameters = three::RendererParameters() ) {
    if ( !three::sdl::init( parameters ) )
      return;
#if defined(THREE_GLEW)
    if ( !three::glew::init( parameters ) )
      return;
#endif
    renderer = three::GLRenderer::create( parameters );
  }

  ~ExampleSession() {
    three::sdl::quit();
  }

  template < typename Example >
  void run( Example example ) {
    if ( renderer )
      example( renderer );
  }

private:
  ExampleSession(ExampleSession&);
  ExampleSession& operator=(ExampleSession&);

  three::GLRenderer::Ptr renderer;
};

} // namespace

template <typename Example >
void RunExample( Example example,
                 three::RendererParameters parameters = three::RendererParameters() ) {
  ExampleSession session( parameters );
  session.run( example );
}

} // namespace three

#endif // THREE_EXAMPLES_COMMON_H
