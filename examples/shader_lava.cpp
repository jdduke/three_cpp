#include "common.hpp"

#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/objects/mesh.hpp>
#include <three/materials/shader_material.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras/geometries/torus_geometry.hpp>
#include <three/extras/image_utils.hpp>

const std::string vertexShader =
"\
uniform vec2 uvScale;\
varying vec2 vUv;\
void main() {\
  vUv = uvScale * uv;\
  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\
  gl_Position = projectionMatrix * mvPosition;\
}\
";

const std::string fragmentShader =
"\
uniform float time;\
\
uniform float fogDensity;\
uniform vec3 fogColor;\
\
uniform sampler2D texture1;\
uniform sampler2D texture2;\
\
varying vec2 vUv;\
\
void main( void ) {\
  vec2 position = -1.0 + 2.0 * vUv;\
\
  vec4 noise = texture2D( texture1, vUv );\
  vec2 T1 = vUv + vec2( 1.5, -1.5 ) * time  *0.02;\
  vec2 T2 = vUv + vec2( -0.5, 2.0 ) * time * 0.01;\
\
  T1.x += noise.x * 2.0;\
  T1.y += noise.y * 2.0;\
  T2.x -= noise.y * 0.2;\
  T2.y += noise.z * 0.2;\
\
  float p = texture2D( texture1, T1 * 2.0 ).a;\
\
  vec4 color = texture2D( texture2, T2 * 2.0 );\
  vec4 temp = color * ( vec4( p, p, p, p ) * 2.0 ) + ( color * color - 0.1 );\
\
  if( temp.r > 1.0 ){ temp.bg += clamp( temp.r - 2.0, 0.0, 100.0 ); }\
  if( temp.g > 1.0 ){ temp.rb += temp.g - 1.0; }\
  if( temp.b > 1.0 ){ temp.rg += temp.b - 1.0; }\
\
  gl_FragColor = temp;\
\
  float depth = gl_FragCoord.z / gl_FragCoord.w;\
  const float LOG2 = 1.442695;\
  float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\
  fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );\
\
  gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );\
}\
";

using namespace three;

void shader_fireball( GLRenderer::Ptr renderer ) {

  auto camera = PerspectiveCamera::create(
    40, (float)renderer->width() / renderer->height(), 1, 3000
  );
  camera->position.z = 4;

  auto scene = Scene::create();

  float time = 1;

  auto texture1 = ImageUtils::loadTexture( threeDataPath("textures/lava/cloud.png") );
  auto texture2 = ImageUtils::loadTexture( threeDataPath("textures/lava/lavatile.jpg") );
  texture1->wrapS = texture1->wrapT = enums::RepeatWrapping;
  texture2->wrapS = texture2->wrapT = enums::RepeatWrapping;

  auto material = ShaderMaterial::create(
    vertexShader,
    fragmentShader,
    Uniforms().add("fogDensity", Uniform( enums::f, .45f ))
              .add("fogColor",   Uniform( enums::v3, Vector3(0, 0, 0)))
              .add("time",       Uniform( enums::f, time ))
              .add("uvScale",    Uniform( enums::v2, Vector2( 3.f, 1.f )))
              .add("texture1",   Uniform( enums::t, texture1.get()))
              .add("texture2",   Uniform( enums::t, texture2.get()))
  );

  // Geometries
  auto mesh = Mesh::create( TorusGeometry::create( 0.65f, 0.3f, 30, 30 ), material );
  mesh->rotation.x = 0.3f;
  scene->add( mesh );

  /////////////////////////////////////////////////////////////////////////

  auto running = true;
  sdl::addEventListener(SDL_KEYDOWN, [&]( const sdl::Event& ) {
    running = false;
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

  anim::gameLoop( [&]( float dt ) -> bool {

    camera->position.x += (-2.f * mouseX - camera->position.x ) * 3 * dt;
    camera->position.y += ( 2.f * mouseY - camera->position.y ) * 3 * dt;
    camera->lookAt( scene->position );

    time += dt;
    material->uniforms[ "time" ].value = time;

    mesh->rotation.y += 0.0375f * dt;
    mesh->rotation.x += 0.15f * dt;

    renderer->render( *scene, *camera );

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

  shader_fireball( renderer );

  return 0;
}