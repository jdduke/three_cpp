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

const char* vertexShader() {
  return
"attribute float size;\n"
"attribute vec3 ca;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  vColor = ca;\n"
"  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"  gl_PointSize = size * ( 300.0 / length( mvPosition.xyz ) );\n"
"}\n";
}

const char* fragmentShader() {
  return
"\n"
"uniform vec3 color;\n"
"uniform sampler2D texture;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  gl_FragColor = vec4( color * vColor, 1.0 );\n"
"  gl_FragColor = gl_FragColor * texture2D( texture, gl_PointCoord );\n"
"}\n";
}

using namespace three;

void shader( GLRenderer::Ptr renderer ) {

 auto camera = PerspectiveCamera::create(
    45, ( float )renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 300;

  auto scene = Scene::create();
  auto texture = ImageUtils::loadTexture( threeDataPath( "textures/sprites/disc.png" ) );

  Attributes attributes;
  attributes[ "size" ] = Attribute( THREE::f );
  attributes[ "ca" ]   = Attribute( THREE::c );

  Uniforms uniforms;
  uniforms[ "color" ]      = Uniform( THREE::c, Color( 0xffffff ) );
  uniforms[ "texture" ]    = Uniform( THREE::t, texture.get() );

  texture->wrapS = texture->wrapT = THREE::RepeatWrapping;

  auto shaderMaterial = ShaderMaterial::create(
    Material::Parameters().add( "uniforms", uniforms )
                          .add( "attributes", attributes )
                          .add( "vertexShader", std::string(vertexShader()) )
                          .add( "fragmentShader", std::string(fragmentShader()) )
  );

  // Geometries
  const auto radius = 100.f, segments = 68.f, rings = 38.f;
  auto geometry = SphereGeometry::create( radius, segments, rings );

  auto geometry2 = CubeGeometry::create( 0.8f * radius, 0.8f * radius, 0.8f * radius, 10, 10, 10 );

  GeometryUtils::merge( *geometry, *geometry2 );

  auto sphere = ParticleSystem::create( geometry, shaderMaterial );
  //sphere->dynamic = true;
  sphere->sortParticles = true;

  const auto& vertices = sphere->geometry->vertices;

  const auto vc1 = geometry->vertices.size();
  const auto vertexCount = sphere->geometry->vertices.size();

  std::vector<float> values_size( vertexCount );
  std::vector<Color> values_color( vertexCount );

  for ( size_t v = 0; v < vertexCount; v++ ) {

    if ( v < vc1 ) {
      values_size[ v ] = 10;
      values_color[ v ].setHSV( 0.1f  + 0.1f * ( (float)v / vc1 ),
                                0.99f,
                                ( vertices[ v ].y + radius ) / ( 2.f * radius ) );
    } else {
      values_size[ v ] = 40;
      values_color[ v ].setHSV( 0.6f,
                                0.75f,
                                ( 0.5f + vertices[ v ].y ) / ( 0.8f * radius ) );
    }

  }

  auto& size  = shaderMaterial->attributes[ "size" ];
  auto& color = shaderMaterial->attributes[ "ca" ];

  size.value  = std::move(values_size);
  color.value = std::move(values_color);

  scene->add( sphere );

  /////////////////////////////////////////////////////////////////////////

  auto running = true;
  sdl::addEventListener(SDL_KEYDOWN, [&]( const sdl::Event& ) {
    running = false;
  });
  sdl::addEventListener(SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  });

  sdl::addEventListener( SDL_VIDEORESIZE, [&]( const sdl::Event event ) {
    camera->aspect = ( float )event.resize.w / event.resize.h;
    camera->updateProjectionMatrix();
    renderer->setSize( event.resize.w, event.resize.h );
  } );

  /////////////////////////////////////////////////////////////////////////

  stats::Stats stats;

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

    stats.update( dt, *renderer );

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