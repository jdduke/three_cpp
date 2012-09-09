#include <three/three.hpp>

using namespace three;

int main ( int argc, char* argv[] ) {

	auto err = glewInit();
	if ( GLEW_OK != err  ) {
		console().serror() << "Error initializing glew: " << glewGetErrorString( err );
		return 0;
	}

	auto renderer = three::GLRenderer::create();

	return 0;
}