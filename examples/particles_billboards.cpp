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

void particles_billboards( GLWindow& window, GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    55, ( float )renderer.width() / renderer.height(), 2.f, 2000
  );
  camera->position.z = 1000;

  auto scene = Scene::create();
  scene->fog = FogExp2::create( 0x000000, .001f );

  auto geometry = Geometry::create();

  const auto particleCount = 10000;
  geometry->vertices.reserve( particleCount );

  std::generate_n( std::back_inserter( geometry->vertices ),
                   particleCount,
                   [] { return Vector3( Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f) ); } );

  auto sprite = ImageUtils::loadTexture(
    threeDataPath("textures/sprites/disc.png")
  );

  auto material = ParticleSystemMaterial::create(
    Material::Parameters().add( "size", 35.f )
                          .add( "map", sprite )
                          .add( "sizeAttenuation", false )
                          .add( "transparent", true)
  );
  material->color.setHSL( 1.f, 0.3f, 0.7f );

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

    time += dt * .05f;

    camera->position.x += ( -1000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += (  1000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    const auto h = Math::fmod( 360.f * ( 1.f + time ), 360.f ) / 360.f;
    material->color.setHSL( h, 0.5f, 0.5f );

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearAlpha = 1;

  return RunExample( particles_billboards, parameters );

}
