#include "common.hpp"

#include <three/cameras/perspective_camera.hpp>
#include <three/core/geometry.hpp>
#include <three/lights/directional_light.hpp>
#include <three/materials/mesh_normal_material.hpp>
#include <three/objects/mesh.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog.hpp>

#include <three/extras/geometries/cube_geometry.hpp>
#include <three/extras/utils/font.hpp>

using namespace three;
using namespace three_examples;

void geometry_hierarchy( GLRenderer::Ptr renderer ) {

  renderer->sortObjects = false;

  auto camera = PerspectiveCamera::create(
    60, (float)renderer->width() / renderer->height(), 1, 10000
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

  auto font = utils::Font::create( threeDataPath("fonts/consolas.ttf") );

  auto time = 0.f, lastDt = 0.f, fps = 60.f;
  anim::gameLoop (

    [&]( float dt ) -> bool {

      time += dt;

      sdl::processEvents();

      camera->position.x += (-3000.f * mouseX - camera->position.x ) * 3 * dt;
      camera->position.y += ( 3000.f * mouseY - camera->position.y ) * 3 * dt;
      camera->lookAt( scene->position );

      auto rx = Math::sin( time * 0.7f ) * 0.5f,
           ry = Math::sin( time * 0.3f ) * 0.5f,
           rz = Math::sin( time * 0.2f ) * 0.5f;

      group->rotation.x = rx;
      group->rotation.y = ry;
      group->rotation.z = rz;

      renderer->render( *scene, *camera );

      if ( font ) {
        static int count = 0;
        if ( count++ % 60 == 0 ) {
          fps = Math::round( 1.f / (.9f * dt + 0.1f * lastDt) );
        }
        lastDt = dt;

        std::stringstream ss; ss << "FPS: " << (int)fps;
        font->render( ss.str().c_str(),
                      10.f, (float)renderer->height() - 30.f,
                      (float)renderer->width(),
                      (float)renderer->height(),
                      Color(0x00FF00) );
        renderer->resetStates();
      }

      sdl::swapBuffers();

      return running;

  } );

}

int main ( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;

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