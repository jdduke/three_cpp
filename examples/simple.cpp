#include "common.h"

#include "three/core/geometry.h"
#include "three/cameras/perspective_camera.h"
#include "three/lights/point_light.h"
#include "three/objects/mesh.h"
#include "three/extras/geometries/sphere_geometry.h"
#include "three/materials/mesh_lambert_material.h"

#include "examples/extras/stats.h"

using namespace three;
using namespace three_examples;

void simple( GLWindow& window, GLRenderer& renderer ) {

  // Camera
  auto camera = PerspectiveCamera::create(
    50, ( float )renderer.width() / renderer.height(), .1f, 1000.f
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


  // Events
  auto mouseX = 0.f, mouseY = 0.f;
  window.addEventListener(SDL_MOUSEMOTION, [&]( const SDL_Event& event ) {
    mouseX = 2.f * ((float)event.motion.x / renderer.width()  - 0.5f);
    mouseY = 2.f * ((float)event.motion.y / renderer.height() - 0.5f);
  });


  // Rendering

  window.animate( [&]( float dt ) -> bool {

    camera->position.x += (-100.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += ( 100.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  return RunExample( simple );

}
