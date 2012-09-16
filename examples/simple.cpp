#include <three/three.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

#include <thread>
#include <fstream>

using namespace three;

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

bool initGLEW( GLRenderer::Parameters& parameters ) {

    auto err = glewInit();

    if ( GLEW_OK != err  ) {
        console().error() << "Error initializing glew: " << glewGetErrorString( err );
        return false;
    }

    console().log() << "GLEW initialized";

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

        std::this_thread::sleep_for( std::chrono::milliseconds( (int)(sleepTime * 1000) ) );

        lastTime = time;

        callback();

    };

    bool done = false;

    auto update = [&]() {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch( event.type ) {
            case SDL_KEYDOWN:
            case SDL_QUIT:
                done = true;
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

    return 0;
}