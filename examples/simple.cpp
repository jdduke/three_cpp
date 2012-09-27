//#include <three/three.hpp>
#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/lights/point_light.hpp>
#include <three/extras/geometries/sphere_geometry.hpp>
#include <three/materials/mesh_lambert_material.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

using namespace three;

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
  return true;

};

void test0( three::GLRenderer& renderer ) {

  // Camera
  auto camera = PerspectiveCamera::create(
    50,
    (float)renderer.width() / renderer.height(),
    .1f, 1000.f
  );
  camera->position.z = 300;

  // Scene
  auto scene = Scene::create();
  scene->add ( camera );

  // Lights
  auto pointLight = PointLight::create( 0xFFFFFF );
  pointLight->position = Vector3( 10, 50, 130 );
  scene->add ( pointLight );

  // Materials
  auto sphereMaterial = MeshLambertMaterial::create(
    parameters( Parameter("color", Color(0xcc0000) ) )
  );

  // Geometries
  float radius = 50, segments = 16, rings = 16;
  auto sphereGeometry = SphereGeometry::create( radius, segments, rings );

  auto sphere = Mesh::create( sphereGeometry, sphereMaterial );
  scene->add( sphere );

  // Rendering
  anim::gameLoop ( gameUpdate, [&](float) -> bool {
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

  if ( !sdl::initSDL( parameters ) || !glew::initGLEW( parameters ) ) {
    return 0;
  }

  auto renderer = three::GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  test0( *renderer );

  return 0;
}