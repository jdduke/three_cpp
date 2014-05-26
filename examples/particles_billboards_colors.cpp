#include "common.h"

#include "three/cameras/perspective_camera.h"
#include "three/core/geometry.h"
#include "three/materials/particle_system_material.h"
#include "three/objects/particle_system.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"
#include "three/scenes/fog_exp2.h"

#include "three/extras/image_utils.h"

using namespace three;
using namespace three_examples;

void particles_billboards_colors( GLWindow& window, GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    50, ( float )renderer.width() / renderer.height(), 1.f, 3000
  );
  camera->position().z = 1400;

  auto scene = Scene::create();
  scene->fog = FogExp2::create( 0x000000, .0009f );

  auto geometry = Geometry::create();

  const auto particleCount = 5000;
  geometry->vertices.reserve( particleCount );
  geometry->colors.reserve( particleCount );

  for ( auto i = 0; i < particleCount; i++ ) {

    Vector3 vertex( Math::random(-1000.f, 1000.f),
                    Math::random(-1000.f, 1000.f),
                    Math::random(-1000.f, 1000.f) );
    geometry->vertices.push_back( vertex );

    Color color( 0xffffff );
    color.setHSL( (vertex.x + 1000.f) / 2000.f, 1.f, 0.5f );
    geometry->colors.push_back( color );

  }

  auto sprite = ImageUtils::loadTexture(
    threeDataPath("textures/sprites/ball.png")
  );

  auto material = ParticleSystemMaterial::create(
    Material::Parameters().add( "size", 85.f )
                          .add( "map", sprite )
                          .add( "vertexColors", enums::VertexColors )
                          .add( "transparent", true)
  );
  material->color.setHSL( 1.f, 0.2f, 0.7f );
  //material->depthTest = false;
  //material->alphaTest = .01f;

  auto particles = ParticleSystem::create( geometry, material );
  particles->sortParticles = true;
  scene->add( particles );

  /////////////////////////////////////////////////////////////////////////

  auto mouseX = 0.f, mouseY = 0.f;
  window.addEventListener( SDL_MOUSEMOTION, [&]( const SDL_Event& event ) {
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

  window.animate( [&]( float dt ) -> bool {

    time += dt * .15f;

    camera->position().x += ( -1000.f * mouseX - camera->position().x ) * 3 * dt;
    camera->position().y += (  1000.f * mouseY - camera->position().y ) * 3 * dt;
    camera->lookAt( scene->position() );

    const auto h = Math::fmod( 360.f * ( 1.f + time ), 360.f ) / 360.f;
    material->color.setHSL( h, 1.f, 0.6f );

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearAlpha = 1;
  parameters.vsync = false;

  return RunExample( particles_billboards_colors, parameters );

}
