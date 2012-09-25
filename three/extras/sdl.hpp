#ifndef THREE_SDL_HPP
#define THREE_SDL_HPP

#include <three/renderers/gl_renderer.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

namespace three {
namespace sdl {

bool initSDL( GLRenderer::Parameters& parameters ) {

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		console().error() << "Error initializing SDL: " << SDL_GetError();
		return false;
	}

	console().log() << "SDL initialized";

	if ( SDL_SetVideoMode( parameters.width, parameters.height, parameters.precision, SDL_OPENGL ) == NULL ) {
		console().error() << "Error setting SDL video mode: " << SDL_GetError();
		return false;
	}

	console().log() << "SDL video initialized";

	SDL_WM_SetCaption( "Three.cpp", NULL );

	return true;
}

bool swapBuffers() {
	SDL_GL_SwapBuffers();
	return true;
}

} // namespace sdl
} // namespace three

#endif // THREE_SDL_HPP