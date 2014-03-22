#include "common.h"

#include <three/cameras/perspective_camera.h>
#include <three/core/geometry.h>
#include <three/materials/particle_basic_material.h>
#include <three/objects/particle_system.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>
#include <three/scenes/fog_exp2.h>

#include <three/extras/image_utils.h>

using namespace three;

void particles_billboards_colors( const GLRenderer::Ptr& renderer ) {

  auto camera = PerspectiveCamera::create(
    50, ( float )renderer->width() / renderer->height(), 1.f, 3000
  );
  camera->position.z = 1400;

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
    color.setHSV( (vertex.x + 1000.f) / 2000.f, 1.f, 1.f );
    geometry->colors.push_back( color );

  }

  auto sprite = ImageUtils::loadTexture(
    threeDataPath("textures/sprites/ball.png")
  );

  auto material = ParticleBasicMaterial::create(
    Material::Parameters().add( "size", 85.f )
                          .add( "map", sprite )
                          .add( "vertexColors", enums::VertexColors )
  );
  material->color.setHSV( 1.f, 0.2f, 0.8f );
  //material->depthTest = false;
  //material->alphaTest = .01f;

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

    time += dt * .15f;

    camera->position.x += ( -1000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += (  1000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    const auto h = Math::fmod( 360.f * ( 1.f + time ), 360.f ) / 360.f;
    material->color.setHSV( h, 0.8f, 1.f );

    renderer->render( *scene, *camera );

    stats.update( dt, renderStats );

    return running;

  }, 2000 );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  parameters.clearAlpha = 1;
  parameters.vsync = false;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  particles_billboards_colors( renderer );

  return 0;
}