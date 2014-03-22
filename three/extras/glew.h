#ifndef THREE_GLEW_HPP
#define THREE_GLEW_HPP

#include <three/console.hpp>
#include <three/renderers/gl_renderer.hpp>

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

#endif // THREE_GLEW_HPP