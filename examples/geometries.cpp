#include "common.h"

#include "three/cameras/perspective_camera.h"
#include "three/core/geometry.h"
#include "three/lights/ambient_light.h"
#include "three/lights/directional_light.h"
#include "three/materials/mesh_lambert_material.h"
#include "three/objects/mesh.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"
#include "three/extras/image_utils.h"

#include "three/extras/geometries/box_geometry.h"
#include "three/extras/geometries/circle_geometry.h"
#include "three/extras/geometries/cylinder_geometry.h"
#include "three/extras/geometries/plane_geometry.h"
#include "three/extras/geometries/sphere_geometry.h"


using namespace three;
using namespace three_examples;

void geometries( GLWindow& window, GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    45, ( float )renderer.width() / renderer.height(), 10, 20000
  );
  camera->position().y = 700;

  auto scene = Scene::create();

  scene->add( AmbientLight::create( 0x404040 ) );

  auto light = DirectionalLight::create( 0xffffff );
  light->position().set( 0, 1, 0 );
  scene->add( light );

  auto map = ImageUtils::loadTexture( threeDataPath( "textures/UV_Grid_Sm.jpg" ) );
  map->wrapS = map->wrapT = THREE::RepeatWrapping;
  map->anisotropy = 16;

  auto material = MeshLambertMaterial::create( 
    Material::Parameters()
      .add( "ambient", Color( 0xbbbbbb ) )
      .add( "map", map )
      .add( "side", THREE::DoubleSide ) 
  );

  //

  auto sphere = Mesh::create( SphereGeometry::create( 75, 20, 10 ), material );
  sphere->position().set( -400, 0, 200 );
  scene->add( sphere );

  //object = new THREE.Mesh( new THREE.IcosahedronGeometry( 75, 1 ), material );
  //object.position.set( -200, 0, 200 );
  //scene.add( object );

  // object = new THREE.Mesh( new THREE.OctahedronGeometry( 75, 2 ), material );
  // object.position.set( 0, 0, 200 );
  // scene.add( object );

  // object = new THREE.Mesh( new THREE.TetrahedronGeometry( 75, 0 ), material );
  // object.position.set( 200, 0, 200 );
  // scene.add( object );

  //

    auto plane = Mesh::create( PlaneGeometry::create( 100, 100, 4, 4 ), material );
  plane->position().set( -400, 0, 0 );
  scene->add( plane );

  auto cube = Mesh::create( BoxGeometry::create( 100, 100, 100, 4, 4, 4 ), material );
  cube->position().set( -200, 0, 0 );
  scene->add( cube );

  auto circle = Mesh::create( CircleGeometry::create( 50, 20, 0, Math::PI() * 2 ), material );
  circle->position().set( 0, 0, 0 );
  scene->add( circle );

  // object = new THREE.Mesh( new THREE.RingGeometry( 10, 50, 20, 5, 0, Math.PI * 2 ), material );
  // object.position.set( 200, 0, 0 );
  // scene.add( object );

  auto cylinder = Mesh::create( CylinderGeometry::create( 25, 75, 100, 40, 5 ), material );
  cylinder->position().set( 400, 0, 0 );
  scene->add( cylinder );

  //

  // var points = [];

  // for ( var i = 0; i < 50; i ++ ) {

  //   points.push( new THREE.Vector3( Math.sin( i * 0.2 ) * Math.sin( i * 0.1 ) * 15 + 50, 0, ( i - 5 ) * 2 ) );

  // }

  // object = new THREE.Mesh( new THREE.LatheGeometry( points, 20 ), material );
  // object.position.set( -400, 0, -200 );
  // scene.add( object );

  // object = new THREE.Mesh( new THREE.TorusGeometry( 50, 20, 20, 20 ), material );
  // object.position.set( -200, 0, -200 );
  // scene.add( object );

  // object = new THREE.Mesh( new THREE.TorusKnotGeometry( 50, 10, 50, 20 ), material );
  // object.position.set( 0, 0, -200 );
  // scene.add( object );

  // object = new THREE.AxisHelper( 50 );
  // object.position.set( 200, 0, -200 );
  // scene.add( object );

  // object = new THREE.ArrowHelper( new THREE.Vector3( 0, 1, 0 ), new THREE.Vector3( 0, 0, 0 ), 50 );
  // object.position.set( 400, 0, -200 );
  // scene.add( object );


  /*renderer.sortObjects = false;

 

  auto scene = Scene::create();
  scene->fog = Fog::create( 0xffffff, 1, 10000 );

  auto material = MeshNormalMaterial::create();
  auto geometry = CubeGeometry::create( 100, 100, 100 );

  auto group = Object3D::create();
  for ( int i = 0; i < 1000; i ++ ) {

    auto mesh = Mesh::create( geometry, material );
    mesh->position() = Vector3( Math::random() * 2000 - 1000,
                                Math::random() * 2000 - 1000,
                                Math::random() * 2000 - 1000 );

    mesh->rotation() = Euler( Math::random() * 2 * Math::PI(),
                              Math::random() * 2 * Math::PI(),
                              0 );

    mesh->matrixAutoUpdate = false;
    mesh->updateMatrix();

    group->add( mesh );

  }
  scene->add( group );

  auto light = DirectionalLight::create( 0xFFFFFF );
  light->target = group;
  scene->add( light );*/

  /////////////////////////////////////////////////////////////////////////

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

    camera->position().x = Math::cos( time * 0.2 ) * 800;
    camera->position().z = Math::sin( time * 0.2 ) * 800;
    camera->lookAt( scene->position() );

    for ( size_t i = 0; i < scene->children.size(); i ++ ) {

      auto& object = scene->children[ i ];

      object->rotation().x = time * 1;
      object->rotation().y = time * 0.25;

    }

    renderer.render( *scene, *camera );

    return --benchmarkFrames > 0;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
//  parameters.clearAlpha = 1.f;
//  parameters.clearColor = Color(0xFFFFFF);

  parameters.vsync = false;

  return RunExample( geometries, parameters );

}
