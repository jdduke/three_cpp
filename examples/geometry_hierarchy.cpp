#include "common.h"

#include <three/cameras/perspective_camera.h>
#include <three/core/geometry.h>
#include <three/lights/directional_light.h>
#include <three/materials/mesh_normal_material.h>
#include <three/objects/mesh.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>
#include <three/scenes/fog.h>

#include <three/extras/geometries/cube_geometry.h>

using namespace three;

void geometry_hierarchy( GLRenderer::Ptr renderer ) {

  renderer->sortObjects = false;

  auto camera = PerspectiveCamera::create(
    60, ( float )renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 1000;

  auto scene = Scene::create();
  scene->fog = Fog::create( 0xffffff, 1, 1000 );

  auto material = MeshNormalMaterial::create();
  auto geometry = CubeGeometry::create( 100, 100, 100 );

  auto group = Object3D::create();
  for ( int i = 0; i < 1000; i ++ ) {

    auto mesh = Mesh::create( geometry, material );
    mesh->position.x = Math::random() * 2000 - 1000;
    mesh->position.y = Math::random() * 2000 - 1000;
    mesh->position.z = Math::random() * 2000 - 1000;

    mesh->rotation.x = Math::random() * 2 * Math::PI();
    mesh->rotation.y = Math::random() * 2 * Math::PI();

    mesh->matrixAutoUpdate = false;
    mesh->updateMatrix();

    group->add( mesh );

  }
  scene->add( group );

  auto light = DirectionalLight::create( 0xFFFFFF );
  light->target = group;
  scene->add( light );

  /////////////////////////////////////////////////////////////////////////

  auto running = true, renderStats = true;
  sdl::addEventListener( SDL_KEYDOWN, [&]( const sdl::Event& e ) {
    switch (e.key.keysym.sym) {
    case SDLK_q:
    case SDLK_ESCAPE:
      running = false; break;
    default:
      renderStats = !renderStats; break;
    };
  } );

  sdl::addEventListener( SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  } );

  auto mouseX = 0.f, mouseY = 0.f;
  sdl::addEventListener( SDL_MOUSEMOTION, [&]( const sdl::Event & event ) {
    mouseX = 2.f * ( ( float )event.motion.x / renderer->width()  - 0.5f );
    mouseY = 2.f * ( ( float )event.motion.y / renderer->height() - 0.5f );
  } );

  sdl::addEventListener( SDL_VIDEORESIZE, [&]( const sdl::Event event ) {
    camera->aspect = ( float )event.resize.w / event.resize.h;
    camera->updateProjectionMatrix();
    renderer->setSize( event.resize.w, event.resize.h );
  } );

  /////////////////////////////////////////////////////////////////////////

  stats::Stats stats( *renderer );
  auto time = 0.f;
  int benchmarkFrames = 100000;

  anim::gameLoop(

  [&]( float dt ) -> bool {

    time += dt;

    camera->position.x += ( -3000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += ( 3000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    auto rx = Math::sin( time * 0.7f ) * 0.5f,
    ry = Math::sin( time * 0.3f ) * 0.5f,
    rz = Math::sin( time * 0.2f ) * 0.5f;

    group->rotation.x = rx;
    group->rotation.y = ry;
    group->rotation.z = rz;

    renderer->render( *scene, *camera );

    stats.update( dt, renderStats );

    return running && (--benchmarkFrames > 0);

  }, 2000 );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  parameters.vsync = false;
  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  geometry_hierarchy( renderer );

  return 0;
}