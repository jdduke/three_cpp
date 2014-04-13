#include "common.h"

#include <three/cameras/perspective_camera.h>
#include <three/core/geometry.h>
#include <three/lights/point_light.h>
#include <three/materials/line_basic_material.h>
#include <three/objects/line.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>
#include <three/scenes/fog.h>

#include <array>

using namespace three;

void lines_sphere( GLRenderer::Ptr renderer ) {

  auto camera = PerspectiveCamera::create(
    80, (float)renderer->width() / renderer->height(), 1, 3000
  );
  camera->position.z = 1000;

  auto scene = Scene::create();

  auto geometry  = Geometry::create();

  const auto radius = 450.f;

  const auto vertexCount = 1500;
  geometry->vertices.reserve( vertexCount * 2 );

  for ( int i = 0; i < vertexCount; ++i ) {

    Vertex vertex( Math::random(-1.f, 1.f),
                   Math::random(-1.f, 1.f),
                   Math::random(-1.f, 1.f) );
    vertex.normalize();
    vertex.multiplyScalar( radius );

    Vertex vertex2( vertex );
    vertex2.multiplyScalar( Math::random() * 0.09f + 1.f );

    geometry->vertices.push_back( vertex );
    geometry->vertices.push_back( vertex2 );

  }


  auto addLine = [&]( float scale, int hex, float opacity, float width ) {

    auto material = LineBasicMaterial::create(
      Material::Parameters().add( "color", Color( hex ) )
                            .add( "opacity", opacity )
                            .add( "linewidth", width )
    );

    auto line = Line::create( geometry, material, enums::LinePieces );
    line->scale.x = line->scale.y = line->scale.z = scale;
    line->rotation().y( Math::random() * Math::PI() );
    line->updateMatrix();
    scene->add( line );

  };

  std::array<float, 9> originalScales = { 0.25f, 0.5f, 0.75f, 1.f, 1.25f, 3.f, 3.5f, 4.5f, 5.5f};

  addLine( originalScales[ 0 ], 0xff7700, 1.f, 2 );
  addLine( originalScales[ 1 ], 0xff9900, 1.f, 1 );
  addLine( originalScales[ 2 ], 0xffaa00, 0.75f, 1 );
  addLine( originalScales[ 3 ], 0xffaa00, 0.5f, 1 );
  addLine( originalScales[ 4 ], 0x000833, 0.8f, 1 );
  addLine( originalScales[ 5 ], 0xaaaaaa, 0.75f, 2 );
  addLine( originalScales[ 6 ], 0xffffff, 0.5f, 1 );
  addLine( originalScales[ 7 ], 0xffffff, 0.25f, 1 );
  addLine( originalScales[ 8 ], 0xffffff, 0.125f, 1 );

  //////////////////////////////////////////////////////////////////////////

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
  sdl::addEventListener(SDL_MOUSEMOTION, [&]( const sdl::Event& event ) {
    mouseX = 2.f * ((float)event.motion.x / renderer->width()  - 0.5f);
    mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
  });

  //////////////////////////////////////////////////////////////////////////

  stats::Stats stats( *renderer );
  auto time = 0.f;

  anim::gameLoop(

    [&]( float dt ) -> bool {

      time += dt * 0.1f;

      //camera->position.x += ( -500.f * mouseX - camera->position.x ) * 3 * dt;
      camera->position.y += (  500.f * mouseY - camera->position.y ) * 3 * dt;
      camera->lookAt( scene->position );

      for ( size_t i = 0; i < scene->children.size(); i++ ) {
        auto& object = *scene->children[i];
        if ( object.type() == enums::Line ) {
          object.rotation().y( time * ( i < 4 ? ( (float)i + 1.f ) : - ( (float)i + 1.f ) ) );
          if ( i < 5 )
            object.scale.x = object.scale.y
                           = object.scale.z
                           = originalScales[ i ] * ((float)i / 5.f + 1.f) * (1.f + 0.5f * Math::sin( 7.f * time ) );
        }
      }

      renderer->render( *scene, *camera );

      stats.update( dt, renderStats );

      return running;

    }

  );

}

int main ( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.antialias = true;

  RunExample( lines_sphere, parameters );

  return 0;
}
