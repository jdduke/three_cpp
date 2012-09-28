#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/lights/directional_light.hpp>
#include <three/extras/geometries/cube_geometry.hpp>
#include <three/materials/mesh_normal_material.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

using namespace three;

void geometry_hierarchy( three::GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    60,
    (float)renderer.width() / renderer.height(),
    1, 10000
  );
  camera->position.z = 500;

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
  light->target = group.get();
  scene->add ( light );

  renderer.sortObjects = false;

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
            mouseX = (float)event.motion.x;
            mouseY = (float)event.motion.y;
          default:
          break;
        };
      }

      camera->position.x += (  mouseX - camera->position.x );
      camera->position.y += (  mouseY - camera->position.y );
      camera->lookAt( scene->position );

      auto rx = Math::sin( time * 0.7f ) * 0.5f,
           ry = Math::sin( time * 0.3f ) * 0.5f,
           rz = Math::sin( time * 0.2f ) * 0.5f;

      group->rotation.x = rx;
      group->rotation.y = ry;
      group->rotation.z = rz;

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

  geometry_hierarchy( *renderer );

  return 0;
}