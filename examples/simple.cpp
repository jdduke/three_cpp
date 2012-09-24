#include <three/three.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

#include <thread>
#include <fstream>

using namespace three;

typedef std::function<bool(float)> Update;
typedef std::function<bool(float)> Render;

void gameLoop(Update update, Render render) {

    anim::AnimFrameRequest requestAnimFrame;

    while ( requestAnimFrame( [&]( float deltaTime ) {
        return update( deltaTime ) && render( deltaTime );
    } ) );

}

bool gameUpdate( float /* deltaTime */ ) {

    SDL_Event event;
    while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
        case SDL_KEYDOWN:
        case SDL_QUIT:
            return false;
        default:
            break;
        };
    }
    //if ( requestAnimFrame.elapsedTime() > 10 ) {
    //    return false;
    //}
    return true;

};

void test0( three::GLRenderer& renderer ) {

    auto scene = Scene::create();

    auto camera = PerspectiveCamera::create( 50,
                                             (float)renderer.width() / renderer.height(),
                                             .1f, 1000.f );

    scene->add ( camera );

    camera->position.z = 300;

    gameLoop ( gameUpdate, [&](float) -> bool {
        SDL_GL_SwapBuffers();
        return true;
        //renderer->renderBuffer()
    } );

}

void test1( three::GLRenderer& renderer ) {

    gameLoop ( gameUpdate, [&](float) -> bool {
        SDL_GL_SwapBuffers();
        return true;
        //renderer->renderBuffer()
    } );

}

int main ( int argc, char* argv[] ) {

    /*std::ofstream ctt("CON");
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );*/

    struct OnQuit {
        ~OnQuit() {
            SDL_Quit();
        }
    } onQuit;

    GLRenderer::Parameters parameters;

    if ( !sdl::initSDL( parameters ) || !glew::initGLEW( parameters ) ) {
        return 0;
    }

    auto renderer = three::GLRenderer::create( parameters );
    if ( !renderer ) {
        return 0;
    }

    test0( *renderer );

    test1( *renderer );

    return 0;
}