#include "common.h"

#include "three/cameras/perspective_camera.h"
#include "three/core/geometry.h"
#include "three/extras/image_utils.h"
#include "three/objects/mesh.h"
#include "three/materials/shader_material.h"
#include "three/objects/particle_system.h"
#include "three/renderers/renderer_parameters.h"
#include "three/renderers/gl_renderer.h"

const std::string vertexShader =
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

const std::string fragmentShader =
"\n"
"uniform vec3 color;\n"
"uniform sampler2D texture;\n"
"varying vec3 vColor;\n"
"void main() {\n"
"  gl_FragColor = vec4( color * vColor, 1.0 );\n"
"  gl_FragColor = gl_FragColor * texture2D( texture, gl_PointCoord );\n"
"}\n";

using namespace three;
using namespace three_examples;

void shader( GLWindow& window, GLRenderer& renderer ) {

 auto camera = PerspectiveCamera::create(
    60, ( float )renderer.width() / renderer.height(), 1, 10000
  );
  camera->position.z = 300;

  auto scene = Scene::create();
  auto texture = ImageUtils::loadTexture( threeDataPath( "textures/sprites/spark1.png" ) );

  Uniforms uniforms;
  uniforms[ "color" ]      = Uniform( enums::c, Color( 0xffffff ) );
  uniforms[ "texture" ]    = Uniform( enums::t, texture.get() );

  Attributes attributes;
  attributes[ "size" ]        = Attribute( enums::f );
  attributes[ "customColor" ] = Attribute( enums::c );

  auto shaderMaterial = ShaderMaterial::create(
    vertexShader,
    fragmentShader,
    uniforms,
    attributes,
    Material::Parameters().add( "blending", enums::AdditiveBlending )
                          .add( "depthTest", false )
                          .add( "transparent", true )
  );

  // Geometries
  const auto radius = 200.f;
  const auto pointCount = 100000;

  auto geometry = Geometry::create();

  auto& vertices = geometry->vertices;
  vertices.reserve( pointCount );

  std::generate_n(
    std::back_inserter(vertices),
    pointCount,
    [=]() -> Vector3 {
      return Vector3( Math::random(-1.f, 1.f),
                      Math::random(-1.f, 1.f),
                      Math::random(-1.f, 1.f) ).multiplyScalar( radius );
    }
  );

  auto sphere = ParticleSystem::create( geometry, shaderMaterial );
  sphere->geometry->dynamic = true;
  sphere->sortParticles = false;

  std::vector<float> values_size( pointCount );
  std::vector<Color> values_color( pointCount );

  for ( int v = 0; v < pointCount; v++ ) {

    values_size[ v ] = 10;
    values_color[ v ].set( 0xffaa00 );

    if ( vertices[ v ].x < 0 )
      values_color[ v ].setHSL( 0.5f + 0.1f * ( (float)v / pointCount ), 0.7f, 0.5f );
    else
      values_color[ v ].setHSL( 0.0f + 0.1f * ( (float)v / pointCount), 0.9f, 0.5f );

  }

  auto& size  = shaderMaterial->attributes[ "size" ];
  auto& color = shaderMaterial->attributes[ "customColor" ];

  size.value  = values_size;
  color.value = values_color;

  scene->add( sphere );

  /////////////////////////////////////////////////////////////////////////

  window.addEventListener( SDL_WINDOWEVENT, [&]( const SDL_Event& event ) {
    if (event.window.event != SDL_WINDOWEVENT_RESIZED) return;
    camera->aspect = ( float )event.window.data1 / event.window.data2;
    camera->updateProjectionMatrix();
    renderer.setSize( event.window.data1, event.window.data2 );
  } );

  /////////////////////////////////////////////////////////////////////////

  auto time = 0.f;

  window.animate( [&]( float dt ) -> bool {

    time += dt;
    sphere->rotation().z = time * .03f;

    auto& sizes = size.value.cast<std::vector<float>>();
    for( size_t i = 0; i < sizes.size(); i++ ) {
      sizes[ i ] = 10.f + 9.f * Math::sin( 0.1f * i + time * 3.f );
    }
    size.needsUpdate = true;

    renderer.render( *scene, *camera );

    return true;

  } );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearColor = Color( 0x000000 );
  parameters.clearAlpha = 1.f;
  parameters.vsync = false;

  return RunExample( shader, parameters );

}
