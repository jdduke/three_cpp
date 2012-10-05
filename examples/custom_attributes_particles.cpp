#include "common.hpp"

#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/extras/image_utils.hpp>
#include <three/objects/mesh.hpp>
#include <three/materials/shader_material.hpp>
#include <three/objects/particle_system.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

const char* vertexShader() {
  return
"attribute float size;\n"
"attribute vec3 customColor;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  vColor = customColor;\n"
"  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"  //gl_PointSize = size;\n"
"  gl_PointSize = size * ( 300.0 / length( mvPosition.xyz ) );\n"
"  gl_Position = projectionMatrix * mvPosition;\n"
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
    60, ( float )renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 300;

  auto scene = Scene::create();
  auto texture = ImageUtils::loadTexture( threeDataPath( "textures/sprites/spark1.png" ) );

  Attributes attributes;
  attributes[ "size" ]        = Attribute( THREE::f );
  attributes[ "customColor" ] = Attribute( THREE::c );

  Uniforms uniforms;
  uniforms[ "color" ]      = Uniform( THREE::c, Color( 0xffffff ) );
  uniforms[ "texture" ]    = Uniform( THREE::t, texture.get() );

  auto shaderMaterial = ShaderMaterial::create(
    Material::Parameters().add( "uniforms", uniforms )
                          .add( "attributes", attributes )
                          .add( "vertexShader", std::string(vertexShader()) )
                          .add( "fragmentShader", std::string(fragmentShader()) )
                          .add( "blending", THREE::AdditiveBlending )
                          .add( "depthTest", false )
                          .add( "transparent", true )
  );

  // Geometries
  const auto radius = 200.f;
  const auto pointCount = 100000;

  auto geometry = Geometry::create();

  auto& vertices = geometry->vertices;
  vertices.reserve( pointCount );

  std::generate_n( std::back_inserter(vertices),
                   pointCount,
                   [=] { Vector3 vertex( Math::random(-1.f, 1.f),
                                         Math::random(-1.f, 1.f),
                                         Math::random(-1.f, 1.f) );
                         vertex.multiplyScalar( radius );
                         return vertex; } );

  auto sphere = ParticleSystem::create( geometry, shaderMaterial );
  sphere->geometry->dynamic = true;
  sphere->sortParticles = false;

  std::vector<float> values_size( pointCount );
  std::vector<Color> values_color( pointCount );

  for ( int v = 0; v < pointCount; v++ ) {

    values_size[ v ] = 10;
    values_color[ v ].setHex( 0xffaa00 );

    if ( vertices[ v ].x < 0 )
      values_color[ v ].setHSV( 0.5f + 0.1f * ( (float)v / pointCount ), 0.7f, 0.9f );
    else
      values_color[ v ].setHSV( 0.0f + 0.1f * ( (float)v / pointCount), 0.9f, 0.9f );

  }

  auto& size  = shaderMaterial->attributes[ "size" ];
  auto& color = shaderMaterial->attributes[ "customColor" ];

  size.value  = values_size;
  color.value = values_color;

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

  auto time = 0.f;

  anim::gameLoop( [&]( float dt ) -> bool {

    time += dt;
    sphere->rotation.z = time * .03f;

    auto& sizes = size.value.cast<std::vector<float>>();
    for( size_t i = 0; i < sizes.size(); i++ ) {
      sizes[ i ] = 14.f + 13.f * Math::sin( 0.1f * i + time * 3.f );
    }
    size.needsUpdate = true;

    renderer->render( *scene, *camera );

    return running;

  } );

}

int main( int argc, char* argv[] ) {

  auto onQuit = defer( sdl::quit );

  RendererParameters parameters;
  parameters.clearColor = Color( 0x000000 );
  parameters.clearAlpha = 1.f;
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