#include "common.hpp"

#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/extras/image_utils.hpp>
#include <three/objects/mesh.hpp>
#include <three/materials/shader_material.hpp>
#include <three/objects/particle_system.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras/geometries/cube_geometry.hpp>
#include <three/extras/geometries/sphere_geometry.hpp>
#include <three/extras/geometry_utils.hpp>

const std::string vertexShader =
"attribute float size;\n"
"attribute vec3 ca;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  vColor = ca;\n"
"  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"  gl_PointSize = size * ( 300.0 / length( mvPosition.xyz ) );\n"
"  gl_Position = projectionMatrix * mvPosition;\n"
"}\n";

const std::string fragmentShader =
"uniform vec3 color;\n"
"uniform sampler2D texture;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  gl_FragColor = vec4( color * vColor, 1.0 );\n"
"  gl_FragColor = gl_FragColor * texture2D( texture, gl_PointCoord );\n"
"}\n";

using namespace three;

void shader( GLRenderer::Ptr renderer ) {

 auto camera = PerspectiveCamera::create(
    45, ( float )renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 300;

  auto scene = Scene::create();
  auto texture = ImageUtils::loadTexture( threeDataPath( "textures/sprites/disc.png" ) );

  Uniforms uniforms;
  uniforms[ "color" ]   = Uniform( THREE::c, Color( 0xffffff ) );
  uniforms[ "texture" ] = Uniform( THREE::t, texture.get() );
  texture->wrapS = texture->wrapT = THREE::RepeatWrapping;

  Attributes attributes;
  attributes[ "size" ] = Attribute( THREE::f );
  attributes[ "ca" ]   = Attribute( THREE::c );

  auto shaderMaterial = ShaderMaterial::create(
    vertexShader,
    fragmentShader,
    uniforms,
    attributes
  );

  // Geometries
  const auto radius = 100.f, segments = 68.f, rings = 38.f;
  auto geometry = SphereGeometry::create( radius, segments, rings );
  const auto vc1 = geometry->vertices.size();

  auto geometry2 = CubeGeometry::create( 0.8f * radius, 0.8f * radius, 0.8f * radius, 10, 10, 10 );

  GeometryUtils::merge( *geometry, *geometry2 );

  auto sphere = ParticleSystem::create( geometry, shaderMaterial );
  sphere->sortParticles = true;

  const auto& vertices = sphere->geometry->vertices;
  const auto vertexCount = vertices.size();

  std::vector<float> valuesSize( vertexCount );
  std::vector<Color> valuesColor( vertexCount );

  for ( size_t v = 0; v < vertexCount; v++ ) {

    valuesColor[ v ] = Color( 0xffffff );

    if ( v < vc1 ) {
      valuesSize[ v ] = 10;
      valuesColor[ v ].setHSV( 0.01f  + 0.1f * ( (float)v / vc1 ),
                               0.99f,
                               ( vertices[ v ].y + radius ) / ( 2.f * radius ) );
    } else {
      valuesSize[ v ] = 40;
      valuesColor[ v ].setHSV( 0.6f,
                               0.75f,
                               ( 0.5f + vertices[ v ].y ) / ( 0.8f * radius ) );
    }

  }

  auto& size  = shaderMaterial->attributes[ "size" ];
  auto& color = shaderMaterial->attributes[ "ca" ];

  size.value  = std::move(valuesSize);
  color.value = std::move(valuesColor);

  scene->add( sphere );

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

  sdl::addEventListener( SDL_VIDEORESIZE, [&]( const sdl::Event event ) {
    camera->aspect = ( float )event.resize.w / event.resize.h;
    camera->updateProjectionMatrix();
    renderer->setSize( event.resize.w, event.resize.h );
  } );

  /////////////////////////////////////////////////////////////////////////

  stats::Stats stats( *renderer );
  auto time = 0.f;

  anim::gameLoop( [&]( float dt ) -> bool {

    time += dt;
    sphere->rotation.y = time * .03f;
    sphere->rotation.z = time * .03f;

    auto& sizes = size.value.cast<std::vector<float>>();
    for( size_t i = 0; i < sizes.size(); i++ ) {
      if ( i < vc1 )
        sizes[ i ] = 16.f + 12.f * Math::sin( 0.1f * i + time * 3.f );
    }
    size.needsUpdate = true;

    renderer->render( *scene, *camera );

    stats.update( dt, renderStats );

    return running;

  }, 2000 );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  parameters.clearColor = Color( 0x000000 );
  parameters.clearAlpha = 1.f;
  parameters.vsync = false;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  shader( renderer );

  return 0;
}