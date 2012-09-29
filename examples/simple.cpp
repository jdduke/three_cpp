//#include <three/three.hpp>
#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/lights/point_light.hpp>
#include <three/objects/mesh.hpp>
#include <three/extras/geometries/sphere_geometry.hpp>
#include <three/materials/mesh_lambert_material.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

using namespace three;

void simple( GLRenderer::Ptr renderer ) {

  // Camera
  auto camera = PerspectiveCamera::create(
    50, ( float )renderer->width() / renderer->height(), .1f, 1000.f
  );
  camera->position.z = 300;

  // Scene
  auto scene = Scene::create();
  scene->add( camera );

  // Lights
  auto pointLight = PointLight::create( 0xFFFFFF );
  pointLight->position = Vector3( 10, 50, 130 );
  scene->add( pointLight );

  // Materials
  auto sphereMaterial = MeshLambertMaterial::create(
    Material::Parameters().add( "color", Color( 0xcc0000 ) )
  );

  // Geometries
  float radius = 50, segments = 16, rings = 16;
  auto sphereGeometry = SphereGeometry::create( radius, segments, rings );

  auto sphere = Mesh::create( sphereGeometry, sphereMaterial );
  scene->add( sphere );

  auto running = true;
  sdl::addEventListener(SDL_KEYDOWN, [&]( const sdl::Event& ) {
    running = false;
  });
  sdl::addEventListener(SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  });

  auto mouseX = 0.f, mouseY = 0.f;
  sdl::addEventListener(SDL_MOUSEMOTION, [&]( const sdl::Event& event ) {
    mouseX = 2.f * ((float)event.motion.x / renderer->width()  - 0.5f);
    mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
  });

  // Rendering
  anim::gameLoop( [&]( float dt ) -> bool {

    sdl::processEvents();

    camera->position.x += ( 100.f * mouseX - camera->position.x ) * dt;
    camera->position.y += ( 100.f * mouseY - camera->position.y ) * dt;
    camera->lookAt( scene->position );

    renderer->render( *scene, *camera );

    sdl::swapBuffers();

    return running;

  } );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  simple( renderer );

  return 0;
}