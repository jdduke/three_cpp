#ifndef THREE_GLEW_HPP
#define THREE_GLEW_HPP

#include <three/renderers/gl_renderer.hpp>

namespace three {
namespace glew {

bool initGLEW( GLRenderer::Parameters& parameters ) {

	auto err = glewInit();

	if ( GLEW_OK != err  ) {
		console().error() << "Error initializing glew: " << glewGetErrorString( err );
		return false;
	}

	console().log() << "GLEW initialized";

	return true;
}

} // namespace glew
} // namespace three

#endif // THREE_GLEW_HPP