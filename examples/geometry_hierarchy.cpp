#include "common.h"

#include "three/cameras/perspective_camera.h"
#include "three/core/geometry.h"
#include "three/lights/directional_light.h"
#include "three/materials/mesh_normal_material.h"
#include "three/objects/mesh.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"
#include "three/scenes/fog.h"

#include "three/extras/geometries/cube_geometry.h"

using namespace three;
using namespace three_examples;

void geometry_hierarchy( GLWindow& window, GLRenderer& renderer ) {

  renderer.sortObjects = false;

  auto camera = PerspectiveCamera::create(
    60, ( float )renderer.width() / renderer.height(), 1, 10000
  );
  camera->position.z = 500;

  auto scene = Scene::create();
  scene->fog = Fog::create( 0xffffff, 1, 10000 );

  auto material = MeshNormalMaterial::create();
  auto geometry = CubeGeometry::create( 100, 100, 100 );

  auto group = Object3D::create();
  for ( int i = 0; i < 1000; i ++ ) {

    auto mesh = Mesh::create( geometry, material );
    mesh->position.x = Math::random() * 2000 - 1000;
    mesh->position.y = Math::random() * 2000 - 1000;
    mesh->position.z = Math::random() * 2000 - 1000;

    mesh->rotation().x = Math::random() * 2 * Math::PI();
    mesh->rotation().y = Math::random() * 2 * Math::PI();

    mesh->matrixAutoUpdate = false;
    mesh->updateMatrix();

    group->add( mesh );

  }
  scene->add( group );

  auto light = DirectionalLight::create( 0xFFFFFF );
  light->target = group;
  scene->add( light );

  /////////////////////////////////////////////////////////////////////////

  auto mouseX = 0.f, mouseY = 0.f;
  window.addEventListener( SDL_MOUSEMOTION, [&]( const SDL_Event & event ) {
    mouseX = 2.f * ( ( float )event.motion.x / renderer.width()  - 0.5f );
    mouseY = 2.f * ( ( float )event.motion.y / renderer.height() - 0.5f );
  } );

  window.addEventListener( SDL_WINDOWEVENT, [&]( const SDL_Event& event ) {
    if (event.window.event != SDL_WINDOWEVENT_RESIZED) return;
    camera->aspect = ( float )event.window.data1 / event.window.data2;
    camera->updateProjectionMatrix();
    renderer.setSize( event.window.data1, event.window.data2 );
  } );

  /////////////////////////////////////////////////////////////////////////

  auto time = 0.f;
  int benchmarkFrames = 100000;

  window.animate( [&]( float dt ) -> bool {

    time += dt;

    camera->position.x += ( -3000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += ( 3000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    auto rx = Math::sin( time * 0.7f ) * 0.5f,
    ry = Math::sin( time * 0.3f ) * 0.5f,
    rz = Math::sin( time * 0.2f ) * 0.5f;

    group->rotation().set( rx, ry, rz );

    renderer.render( *scene, *camera );

    return --benchmarkFrames > 0;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearAlpha = 1.f;
  parameters.clearColor = Color(0xFFFFFF);
    
  parameters.vsync = false;

  return RunExample( geometry_hierarchy, parameters );

}
