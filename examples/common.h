#ifndef THREE_EXAMPLES_COMMON_H
#define THREE_EXAMPLES_COMMON_H

#include <three/config.h>

#include <three/extras/anim.h>
#include <three/extras/glew.h>
#include <three/extras/sdl.h>
#include <three/extras/stats.h>

#include <three/renderers/gl_renderer.h>
#include <three/renderers/renderer_parameters.h>

#include <three/utils/conversion.h>
#include <three/utils/template.h>

namespace three {

struct ExampleSession {

  ExampleSession(  three::RendererParameters parameters = three::RendererParameters() ) {
    if ( !three::sdl::init( parameters ) || !three::glew::init( parameters ) ) {
      return;
    }
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

} // namespace three

#endif // THREE_EXAMPLES_COMMON_H