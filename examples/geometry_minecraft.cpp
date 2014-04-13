#include "common.h"

#include <three/cameras/perspective_camera.h>
#include <three/core/geometry.h>
#include <three/lights/ambient_light.h>
#include <three/lights/directional_light.h>
#include <three/materials/mesh_lambert_material.h>
#include <three/materials/mesh_face_material.h>
#include <three/objects/mesh.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>

#include <three/extras/geometries/cube_geometry.h>
#include <three/extras/geometry_utils.h>
#include <three/extras/image_utils.h>
#include <three/extras/scene_utils.h>
#include <three/extras/utils/improved_noise.h>

using namespace three;

std::vector<float> generateHeight( int width, int height ) {

  ImprovedNoise perlin;
  const auto size = width * height;
  const auto z = Math::random() * 100;
  auto quality = 2.f;
  std::vector<float> data( size, 0.f );

  for ( int j = 0; j < 4; j ++ ) {
    for ( int i = 0; i < size; i ++ ) {
      const auto x = i % width;
      const auto y = i / width;
      data[ i ] += perlin.noise( (float)x / quality, (float)y / quality, (float)z ) * quality;
    }
    quality *= 4.f;
  }

  return data;
}


void geometry_minecraft( const GLRenderer::Ptr& renderer ) {

  const auto worldWidth = 128;
  const auto worldDepth = 128;
  const auto worldHalfWidth = worldWidth / 2;
  const auto worldHalfDepth = worldDepth / 2;
  const auto data = generateHeight( worldWidth, worldDepth );

  /////////////////////////////////////////////////////////////////////////

  auto camera = PerspectiveCamera::create(
    60, (float)renderer->width() / renderer->height(), 1, 20000
  );

  auto scene = Scene::create();

  /////////////////////////////////////////////////////////////////////////

  auto loadTexture = []( const char* path ) -> Material::Ptr {
    auto texture  = ImageUtils::loadTexture( threeDataPath( path ) );
    return MeshLambertMaterial::create(
      Material::Parameters().add( "map", texture )
                            .add( "ambient", Color(0xbbbbbb) )
    );
  };

  auto grassDirt = loadTexture( "textures/minecraft/grass_dirt.png" );
  auto grass     = loadTexture( "textures/minecraft/grass.png" );
  auto dirt      = loadTexture( "textures/minecraft/dirt.png" );

  std::array<Material::Ptr, 6> materialArray = {
    grassDirt,
    grassDirt,
    grass,
    dirt,
    grassDirt,
    grassDirt
  };
  std::vector<Material::Ptr> materials( materialArray.begin(), materialArray.end() );

  /////////////////////////////////////////////////////////////////////////

  std::vector<CubeGeometry::Ptr> cubes;

  for ( int i = 0; i < 16; i++ ) {
    CubeGeometry::Sides sides;
    sides.px = ( i & 8 ) == 8;
    sides.nx = ( i & 4 ) == 4;
    sides.py = true;
    sides.ny = false;
    sides.pz = ( i & 2 ) == 2;
    sides.nz = ( i & 1 ) == 1;
    cubes.push_back( CubeGeometry::create( 100, 100, 100,
                                           1, 1, 1,
                                           materials,
                                           sides ) );
  }

  /////////////////////////////////////////////////////////////////////////

  auto geometry = Geometry::create();

  auto getY = [&]( int x, int z ) {
    return Math::floor( data[ (x + z * worldWidth) % data.size() ] * 0.2f );
  };

  camera->position.y = getY( worldHalfWidth, worldHalfDepth ) * 100 + 100;

  /////////////////////////////////////////////////////////////////////////

  for ( int z = 0; z < worldDepth; z ++ ) {
    for ( int x = 0; x < worldWidth; x ++ ) {

      int px = 0, nx = 0, pz = 0, nz = 0;

      const auto h = getY( x, z );

      auto h2 = getY( x + 1, z );
      px = ( ( h2 != h && h2 != h + 1 ) || x == 0 ) ? 1 : 0;

      h2 = getY( x - 1, z );
      nx = ( ( h2 != h && h2 != h + 1 ) || x == worldWidth - 1 ) ? 1 : 0;

      h2 = getY( x, z + 1 );
      pz = ( ( h2 != h && h2 != h + 1 ) || z == worldDepth - 1 ) ? 1 : 0;

      h2 = getY( x, z - 1 );
      nz = ( ( h2 != h && h2 != h + 1 ) || z == 0 ) ? 1 : 0;

      auto mesh = Mesh::create( cubes[ px * 8 + nx * 4 + pz * 2 + nz ], Material::Ptr() );

      mesh->position.x = x * 100.f - worldHalfWidth * 100.f;
      mesh->position.y = h * 100.f;
      mesh->position.z = z * 100.f - worldHalfDepth * 100.f;

      GeometryUtils::merge( *geometry, *mesh );

    }

  }

  /////////////////////////////////////////////////////////////////////////

  auto mesh = Mesh::create( geometry, MeshFaceMaterial::create() );
  scene->add( mesh );

  auto ambientLight = AmbientLight::create( 0xcccccc );
  scene->add( ambientLight );

  auto directionalLight = DirectionalLight::create( 0xffffff, 2 );
  directionalLight->position.set( 1, 1, 0.5 ).normalize();
  scene->add( directionalLight );

  /////////////////////////////////////////////////////////////////////////

  auto running = true, renderStats = true;
  sdl::addEventListener(SDL_KEYDOWN, [&]( const sdl::Event& e ) {
    switch (e.key.keysym.sym) {
    case SDLK_q:
    case SDLK_ESCAPE:
      running = false; break;
    default:
      renderStats = !renderStats; break;
    };
  });

  sdl::addEventListener(SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  });

  auto mouseX = 0.f, mouseY = 0.f;
  sdl::addEventListener(SDL_MOUSEMOTION, [&]( const sdl::Event& event ) {
    mouseX = 2.f * ((float)event.motion.x / renderer->width()  - 0.5f);
    mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
  });

  /////////////////////////////////////////////////////////////////////////

  stats::Stats stats( *renderer );
  auto time = 0.f;

  anim::gameLoop (

    [&]( float dt ) -> bool {

      time += dt;

      camera->position.x += (-3000.f * mouseX - camera->position.x ) * 3 * dt;
      camera->position.y += ( 3000.f * mouseY - camera->position.y ) * 3 * dt;
      camera->lookAt( scene->position );

      renderer->render( *scene, *camera );

      stats.update( dt, renderStats );

      return running;

  }, 2000 );

}

int main ( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.vsync = false;

  RunExample( geometry_minecraft, parameters );

  return 0;
}
