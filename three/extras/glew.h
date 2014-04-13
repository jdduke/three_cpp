#ifndef THREE_GLEW_H
#define THREE_GLEW_H

#include <three/console.h>
#include <three/renderers/gl_renderer.h>

namespace three {
namespace glew {

inline bool init( RendererParameters& parameters ) {

  auto err = glewInit();

  if ( GLEW_OK != err ) {
    console().error() << "Error initializing glew: " << glewGetErrorString( err );
    return false;
  }

  console().log() << "GLEW initialized";

  return true;
}

} // namespace glew
} // namespace three

#endif // THREE_GLEW_H
