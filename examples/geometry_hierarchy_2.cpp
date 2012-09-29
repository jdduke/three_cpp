#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/lights/directional_light.hpp>
#include <three/objects/mesh.hpp>
#include <three/extras/geometries/cube_geometry.hpp>
#include <three/materials/mesh_normal_material.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

using namespace three;

void geometry_hierarchy_2( GLRenderer::Ptr renderer ) {

  renderer->sortObjects = false;

  auto camera = PerspectiveCamera::create(
    60, (float)renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 3000;

  auto scene = Scene::create();
  auto material = MeshNormalMaterial::create();
  auto geometry = CubeGeometry::create( 100, 100, 100 );

  auto root = Object3D::create();
  root->position.x = 1000;
  scene->add( root );

  auto amount = 100;
  auto parent = root;

  auto add = [&geometry, &material]( Object3D::Ptr parent,
                                     int amount,
                                     Vector3 offset ) {
    for ( int i = 0; i < amount; i ++ ) {

      auto object = Mesh::create( geometry, material );
      object->position = offset;

      parent->add( object );
      parent = object;

    }
  };

  add( root, amount, Vector3(-100,    0,    0) );
  add( root, amount, Vector3( 100,    0,    0) );
  add( root, amount, Vector3(   0, -100,    0) );
  add( root, amount, Vector3(   0,  100,    0) );
  add( root, amount, Vector3(   0,    0, -100) );
  add( root, amount, Vector3(   0,    0,  100) );

  auto time = 0.f, mouseX = 0.f, mouseY = 0.f;

  anim::gameLoop (

    [&]( float dt ) -> bool {

      time += dt;

      SDL_Event event;
      while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
          case SDL_KEYDOWN:
          case SDL_QUIT:
            return false;
          case SDL_MOUSEMOTION:
            mouseX = 2.f * ((float)event.motion.x / renderer->width()  - 0.5f);
            mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
          default:
          break;
        };
      }

      camera->position.x += ( 3000.f * mouseX - camera->position.x ) * dt;
      camera->position.y += ( 3000.f * mouseY - camera->position.y ) * dt;
      camera->lookAt( scene->position );

      auto rx = Math::sin( time * 0.7f ) * 0.2f,
           ry = Math::sin( time * 0.3f ) * 0.1f,
           rz = Math::sin( time * 0.2f ) * 0.1f;

      SceneUtils::traverseHierarchy( *root, [rx,ry,rz]( Object3D& object ) {
        object.rotation.x = rx;
        object.rotation.y = ry;
        object.rotation.z = rz;
      } );

      renderer->render( *scene, *camera );
      sdl::swapBuffers();
      return true;

  }, 2000 );

}

int main ( int argc, char* argv[] ) {

  auto onQuit = defer( SDL_Quit );

  RendererParameters parameters;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  geometry_hierarchy_2( renderer );

  return 0;
}