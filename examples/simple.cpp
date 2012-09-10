#include <three/three.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

#include <thread>

using namespace three;

bool initSDL( GLRenderer::Parameters& parameters ) {

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
        console().serror() << "Error initializing sdl: " << SDL_GetError();
        return false;
    }

    console().slog() << "SDL Initialized";

    if ( SDL_SetVideoMode( parameters.width, parameters.height, parameters.precision, SDL_OPENGL ) == NULL ) {
        console().serror() << "Error setting video mode: " << SDL_GetError();
        return false;
    }

    console().slog() << "SDL Video Initialized";

    SDL_WM_SetCaption( "Three.cpp", NULL );

    return true;
}

bool initGLEW( GLRenderer::Parameters& parameters ) {

    auto err = glewInit();

    if ( GLEW_OK != err  ) {
        console().serror() << "Error initializing glew: " << glewGetErrorString( err );
        return false;
    }

    console().slog() << "GLEW Initialized";

    return true;
}

int quit() {
    SDL_Quit();
    exit( 0 );
    return 0;
}

int main ( int argc, char* argv[] ) {

	GLRenderer::Parameters parameters;

    if ( !initSDL( parameters ) || !initGLEW( parameters ) ) {
        quit();
    }

	auto renderer = three::GLRenderer::create( parameters );
    if ( !renderer ) {
        quit();
    }

    Clock clock;
    bool done = false;

    while ( !done && clock.getElapsedTime() < 10 ) {

        SDL_Event event;
        while ( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
            case SDL_ACTIVEEVENT:
                break;
            case SDL_VIDEORESIZE:
                break;
            case SDL_KEYDOWN:
                //handleKeyPress( &event.key.keysym );
                break;
            case SDL_QUIT:
                done = TRUE;
                break;
            default:
                break;
            }
        }

        //renderer->render

        SDL_GL_SwapBuffers( );

        // TODO: Automate
        std::this_thread::sleep_for( std::chrono::milliseconds(16) );

    }

	return quit();
}