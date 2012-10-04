#include "common.hpp"

#include <three/cameras/perspective_camera.hpp>
#include <three/core/geometry.hpp>
#include <three/materials/particle_basic_material.hpp>
#include <three/objects/particle_system.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/scenes/fog_exp2.hpp>

using namespace three;

void particles_random( const GLRenderer::Ptr& renderer ) {

  auto camera = PerspectiveCamera::create(
    75, ( float )renderer->width() / renderer->height(), 1.f, 3000
  );
  camera->position.z = 1000;

  auto scene = Scene::create();
  scene->fog = FogExp2::create( 0x000000, .0007f );

  auto geometry = Geometry::create();

  const auto particles = 20000;
  geometry->vertices.reserve( particles );
  std::generate_n( std::back_inserter( geometry->vertices ), particles,
                   [] { return Vector3( Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f) ); } );

  std::vector<Material::Ptr> materials;
  auto addParticleSystem = [&]( const Vector3& color, float size ) {

    //materials[i] = new THREE.ParticleBasicMaterial( { color: color, size: size } );
    auto material = ParticleBasicMaterial::create(
      Material::Parameters().add( "size", size * .5f )
    );

    materials.push_back( material );
    material->color.setHSV( color[0], color[1], color[2] );

    auto particles = ParticleSystem::create( geometry, material );

    particles->rotation.x = Math::random() * 6;
    particles->rotation.y = Math::random() * 6;
    particles->rotation.z = Math::random() * 6;

    scene->add( particles );
  };

  typedef std::pair<Vector3, float> ColorSize;
  std::array<ColorSize, 5> params = {
    ColorSize( Vector3(  1.f, 1.f, 1.f), 5.f ),
    ColorSize( Vector3(0.95f, 1.f, 1.f), 4.f ),
    ColorSize( Vector3(0.90f, 1.f, 1.f), 3.f ),
    ColorSize( Vector3(0.85f, 1.f, 1.f), 2.f ),
    ColorSize( Vector3(0.80f, 1.f, 1.f), 1.f )
  };

  for ( const auto& param : params ) {
    addParticleSystem( param.first, param.second );
  }

  /////////////////////////////////////////////////////////////////////////

  auto running = true;
  sdl::addEventListener( SDL_KEYDOWN, [&]( const sdl::Event& ) {
    running = false;
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

  stats::Stats stats;

  auto time = 0.f;

  anim::gameLoop(

  [&]( float dt ) -> bool {

    time += dt * .05f;

    camera->position.x += ( -1000.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += (  1000.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    for ( size_t i = 0; i < scene->children.size(); ++i ) {
      auto& object = *scene->children[ i ];
      if ( object.type() == THREE::ParticleSystem ) {
        object.rotation.y = time * ( i < 4 ? i + 1 : - ( (int)i + 1 ) );
      }
    }

    for ( size_t i = 0; i < materials.size(); ++i ) {
      auto& color = params[ i ].first;
      const auto h = Math::fmod( 360.f * ( color[0] + time ), 360.f ) / 360.f;
      materials[ i ]->color.setHSV( h, color[ 1 ], color[ 2 ] );
    }

    renderer->render( *scene, *camera );

    //stats.update( dt, *renderer );

    return running;

  } );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  particles_random( renderer );

  return 0;
}