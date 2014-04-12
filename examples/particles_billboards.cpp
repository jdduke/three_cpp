#include "common.h"

#include <three/cameras/perspective_camera.h>
#include <three/core/geometry.h>
#include <three/materials/particle_system_material.h>
#include <three/objects/particle_system.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>
#include <three/scenes/fog_exp2.h>

#include <three/extras/image_utils.h>

using namespace three;

void particles_billboards( const GLRenderer::Ptr& renderer ) {

  auto camera = PerspectiveCamera::create(
    55, ( float )renderer->width() / renderer->height(), 2.f, 2000
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
  );
  material->color.setHSL( 1.f, 0.3f, 0.7f );

  auto particles = ParticleSystem::create( geometry, material );
  particles->sortParticles = true;
  scene->add( particles );

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

  anim::gameLoop(

  [&]( float dt ) -> bool {

    time += dt * .05f;

    camera->position.x += ( -1000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += (  1000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    const auto h = Math::fmod( 360.f * ( 1.f + time ), 360.f ) / 360.f;
    material->color.setHSL( h, 0.5f, 0.5f );

    renderer->render( *scene, *camera );

    stats.update( dt, renderStats );

    return running;

  } );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  parameters.clearAlpha = 1;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  particles_billboards( renderer );

  return 0;
}