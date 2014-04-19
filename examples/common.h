#ifndef THREE_EXAMPLES_COMMON_H
#define THREE_EXAMPLES_COMMON_H

#include "three/config.h"
#include "three/gl.h"

#include "three/renderers/gl_renderer.h"
#include "three/renderers/renderer_parameters.h"

#include "three/utils/conversion.h"
#include "three/utils/template.h"

#include "examples/extras/sdl.h"
#include "examples/extras/stats.h"

namespace three_examples {

template <typename Example>
int RunExample( Example example,
                three::RendererParameters parameters = three::RendererParameters() ) {
  GLWindow window( parameters );
  if ( !window.valid() )
    return EXIT_FAILURE;

  three::GLInterface interface = window.createGLInterface();
  if ( !interface.validate() )
    return EXIT_FAILURE;

  auto renderer = three::GLRenderer::create( parameters, interface );
  if ( !renderer )
    return EXIT_FAILURE;

  example( window, *renderer );
  return EXIT_SUCCESS;
}

} // namespace three_examples

#endif // THREE_EXAMPLES_COMMON_H
