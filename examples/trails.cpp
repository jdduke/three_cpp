#include <three/three.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

#include <thread>
#include <fstream>

using namespace three;

void trails( three::GLRenderer& renderer ) {

  // Camera
  auto camera = PerspectiveCamera::create(
    60,
    (float)renderer.width() / renderer.height(),
    1, 10000
  );
  camera->position.set( 100000, 0, 3200 );

  // Scene
  auto scene = Scene::create();

  // Geometries
  std::array<Color, 4> colors = { Color(0x000000), Color(0xff0080), Color(0x8000ff), Color(0xffffff) };
  auto geometry = Geometry::create();

  for ( int i = 0; i < 2000; i ++ ) {
    Vertex vertex;
    vertex.position.x = Math::random() * 4000 - 2000;
    vertex.position.y = Math::random() * 4000 - 2000;
    vertex.position.z = Math::random() * 4000 - 2000;
    geometry->vertices.push_back( vertex );
    geometry->colors.push_back( colors[ (int)Math::floor( Math::random() * colors.size() ) ] );
  }

  // Materials
  auto material = ParticleBasicMaterial::create(
    parameters( Parameter("color", Color(0xcc0000)),
                Parameter("size", 1.0f),
                Parameter("vertexColors", THREE::VertexColors),
                Parameter("depthTest", false),
                Parameter("opacity", 0.5f),
                Parameter("sizeAttenuation", false) )
  );

  auto mesh = ParticleSystem::create( geometry, material );
  scene->add( mesh );

  // Rendering
  anim::gameLoop (

    [&](float) -> bool {
      float mouseX = 0, mouseY = 0;

      SDL_Event event;
      while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
          case SDL_KEYDOWN:
          case SDL_QUIT:
            return false;
          case SDL_MOUSEMOTION:
            mouseX = (float)event.motion.xrel;
            mouseY = (float)event.motion.yrel;
          default:
          break;
        };
      }

      camera->position.x += ( mouseX  - camera->position.x ) * .05f;
      camera->position.y += ( -mouseY - camera->position.y ) * .05f;
      camera->lookAt( scene->position );
      return true;

  }, [&](float) -> bool {

      renderer.render( *scene, *camera );
      sdl::swapBuffers();
      return true;

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
  parameters.preserveDrawingBuffer = true;

  if ( !sdl::initSDL( parameters ) || !glew::initGLEW( parameters ) ) {
    return 0;
  }

  auto renderer = three::GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  renderer->sortObjects = false;
  renderer->autoClearColor = false;

  trails( *renderer );

  return 0;
}