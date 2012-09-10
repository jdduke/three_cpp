#include <three/three.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

#include <thread>
#include <fstream>

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


int main ( int argc, char* argv[] ) {

    /*std::ofstream ctt("CON");
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );*/

    GLRenderer::Parameters parameters;

    auto quit = [&]() {
        SDL_Quit();
        exit( 0 );
    };

    if ( !initSDL( parameters ) || !initGLEW( parameters ) ) {
        quit();
    }

	auto renderer = three::GLRenderer::create( parameters );
    if ( !renderer ) {
        quit();
    }

    const auto frameTime = .016f;

    Clock clock;
    auto lastTime = clock.getElapsedTime();
    typedef std::function<void(void)> Callback;
    auto requestAnimFrame = [&]( Callback callback ) {

        auto time = clock.getElapsedTime();
        auto deltaTime = ( time - lastTime );
        auto sleepTime = frameTime - deltaTime;

        std::this_thread::sleep_for( Clock::Duration(sleepTime) );

        lastTime = time;

        callback();

    };

    bool done = false;

    auto update = [&]() {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch( event.type ) {
            case SDL_KEYDOWN:
                done = true;
                //handleKeyPress( &event.key.keysym );
                break;
            case SDL_QUIT:
                done = TRUE;
                break;
            default:
                break;
            };
        }
    };

    auto render = [&]() {
        SDL_GL_SwapBuffers( );
        //renderer->render();
    };

    while ( !done && clock.getElapsedTime() < 10 ) {

        requestAnimFrame( [&] {

            update();

            render();

        } );

    }

    quit();
}