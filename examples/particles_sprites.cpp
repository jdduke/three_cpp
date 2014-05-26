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

void particles_sprites( GLWindow& window, GLRenderer& renderer ) {

  auto camera = PerspectiveCamera::create(
    75, ( float )renderer.width() / renderer.height(), 1.f, 2000
  );
  camera->position().z = 1000;

  auto scene = Scene::create();
  scene->fog = FogExp2::create( 0x000000, .0008f );

  auto geometry = Geometry::create();

  auto sprite1 = ImageUtils::loadTexture( threeDataPath("textures/sprites/snowflake1.png") );
  auto sprite2 = ImageUtils::loadTexture( threeDataPath("textures/sprites/snowflake2.png") );
  auto sprite3 = ImageUtils::loadTexture( threeDataPath("textures/sprites/snowflake3.png") );
  auto sprite4 = ImageUtils::loadTexture( threeDataPath("textures/sprites/snowflake4.png") );
  auto sprite5 = ImageUtils::loadTexture( threeDataPath("textures/sprites/snowflake5.png") );

  const auto particleCount = 10000;
  geometry->vertices.reserve( particleCount );

  std::generate_n( std::back_inserter( geometry->vertices ),
                   particleCount,
                   [] { return Vector3( Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f),
                                        Math::random(-1000.f, 1000.f) ); } );

  std::vector<Material::Ptr> materials;
  auto addParticleSystem = [&]( const Vector3& color, const Texture::Ptr& sprite, float size ) {

    auto material = ParticleSystemMaterial::create(
      Material::Parameters().add( "size", size )
                            .add( "map", sprite )
                            .add( "blending", enums::AdditiveBlending )
                            .add( "depthTest", false )
                            .add( "transparent", true )
    );

    materials.push_back( material );
    material->color.setHSL( color[0], color[1], color[2] );

    auto particles = ParticleSystem::create( geometry, material );

    particles->rotation() = Euler( Math::random() * 6,
                                   Math::random() * 6,
                                   Math::random() * 6 );

    scene->add( particles );
  };

  typedef std::tuple<Vector3, Texture::Ptr, float> ColorSpriteSize;
  std::array<ColorSpriteSize, 5> params = {
    ColorSpriteSize( Vector3(  1.f, 0.2f,  0.5f), sprite2, 20.f ),
    ColorSpriteSize( Vector3(0.95f, 0.1f,  0.5f), sprite3, 13.f ),
    ColorSpriteSize( Vector3(0.90f, 0.05f, 0.5f), sprite1, 10.f ),
    ColorSpriteSize( Vector3(0.85f, 0.f,   0.5f), sprite5, 8.f ),
    ColorSpriteSize( Vector3(0.80f, 0.f,   0.5f), sprite4, 5.f )
  };
  for ( const auto& param : params ) {
    addParticleSystem( std::get<0>(param), std::get<1>(param), std::get<2>(param) );
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
      if ( object.type() == enums::ParticleSystem ) {
        object.rotation().y = time * ( i < 4 ? i + 1 : - ( (int)i + 1 ) );
      }
    }

    for ( size_t i = 0; i < materials.size(); ++i ) {
      auto& color = std::get<0>(params[ i ]);
      const auto h = Math::fmod( 360.f * ( color[0] + time ), 360.f ) / 360.f;
      materials[ i ]->color.setHSL( h, color[ 1 ], color[ 2 ] );
    }

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearAlpha = 1;

  return RunExample( particles_sprites, parameters );

}
