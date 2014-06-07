#include "common.h"

#include "three/cameras/perspective_camera.h"
#include "three/core/geometry.h"
#include "three/materials/particle_system_material.h"
#include "three/objects/particle_system.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"
#include "three/scenes/fog_exp2.h"

using namespace three;
using namespace three_examples;

void particles_random( GLWindow& window, GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    75, ( float )renderer.width() / renderer.height(), 1.f, 3000
  );
  camera->position().z = 1000;

  auto scene = Scene::create();
  scene->fog = FogExp2::create( 0x000000, .0007f );

  auto geometry = Geometry::create();

  const auto particleCount = 20000;
  geometry->vertices.reserve( particleCount );
  std::generate_n( std::back_inserter( geometry->vertices ), particleCount,
                   [] { return Vector3( Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f) ); } );

  std::vector<Material::Ptr> materials;
  auto addParticleSystem = [&]( const Vector3& color, float size ) {

    //materials[i] = new THREE.ParticleBasicMaterial( { color: color, size: size } );
    auto material = ParticleSystemMaterial::create(
      Material::Parameters().add( "size", size )
    );

    materials.push_back( material );
    material->color.setHSL( color[0], color[1], color[2] );

    auto particles = ParticleSystem::create( geometry, material );

    particles->rotation() = Euler( Math::random() * 6,
                                   Math::random() * 6,
                                   Math::random() * 6 );

    scene->add( particles );
  };

  typedef std::pair<Vector3, float> ColorSize;
  std::array<ColorSize, 5> params = {
    ColorSize( Vector3(  1.f, 1.f, 0.5f), 5.f ),
    ColorSize( Vector3(0.95f, 1.f, 0.5f), 4.f ),
    ColorSize( Vector3(0.90f, 1.f, 0.5f), 3.f ),
    ColorSize( Vector3(0.85f, 1.f, 0.5f), 2.f ),
    ColorSize( Vector3(0.80f, 1.f, 0.5f), 1.f )
  };

  for ( const auto& param : params ) {
    addParticleSystem( param.first, param.second );
  }

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

    camera->position().x += ( -1000.f * mouseX - camera->position().x ) * 3 * dt;
    camera->position().y += (  1000.f * mouseY - camera->position().y ) * 3 * dt;
    camera->lookAt( scene->position() );

    for ( size_t i = 0; i < scene->children.size(); ++i ) {
      auto& object = *scene->children[ i ];
      if ( object.type() == THREE::ParticleSystem ) {
        object.rotation().y = time * ( i < 4 ? i + 1 : - ( (int)i + 1 ) );
      }
    }

    for ( size_t i = 0; i < materials.size(); ++i ) {
      auto& color = params[ i ].first;
      const auto h = Math::fmod( 360.f * ( color[0] + time ), 360.f ) / 360.f;
      materials[ i ]->color.setHSL( h, color[ 1 ], color[ 2 ] );
    }

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  return RunExample( particles_random );

}
