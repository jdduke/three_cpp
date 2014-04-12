#include "common.h"

#include <three/core/geometry.h>
#include <three/cameras/perspective_camera.h>
#include <three/objects/mesh.h>
#include <three/extras/geometries/sphere_geometry.h>
#include <three/materials/shader_material.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>

const std::string vertexShader =
"\
uniform float time;\
uniform float scale;\
varying vec3 vTexCoord3D;\
varying vec3 vNormal;\
varying vec3 vViewPosition;\
void main( void ) {\
  vec4 mPosition = modelViewMatrix * vec4( position, 1.0 );\
  vNormal = normalize( normalMatrix * normal );\
  vViewPosition = cameraPosition - mPosition.xyz;\
  vTexCoord3D = scale * ( position.xyz + vec3( 0.0, 0.0, -time ) );\
  gl_Position = projectionMatrix * mPosition;\
}\
";

const std::string fragmentShader =
//
      // Description : Array and textureless GLSL 3D simplex noise function.
      //      Author : Ian McEwan, Ashima Arts.
      //  Maintainer : ijm
      //     Lastmod : 20110409 (stegu)
      //     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
      //               Distributed under the MIT License. See LICENSE file.
      //
"\
uniform float time;\
varying vec3 vTexCoord3D;\
varying vec3 vNormal;\
varying vec3 vViewPosition;\
vec4 permute( vec4 x ) {\
  return mod( ( ( x * 34.0 ) + 1.0 ) * x, 289.0 );\
}\
vec4 taylorInvSqrt( vec4 r ) {\
  return 1.79284291400159 - 0.85373472095314 * r;\
}\
float snoise( vec3 v ) {\
  const vec2 C = vec2( 1.0 / 6.0, 1.0 / 3.0 );\
  const vec4 D = vec4( 0.0, 0.5, 1.0, 2.0 );\
  vec3 i  = floor( v + dot( v, C.yyy ) );\
  vec3 x0 = v - i + dot( i, C.xxx );\
  vec3 g = step( x0.yzx, x0.xyz );\
  vec3 l = 1.0 - g;\
  vec3 i1 = min( g.xyz, l.zxy );\
  vec3 i2 = max( g.xyz, l.zxy );\
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;\
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;\
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;\
  i = mod( i, 289.0 );\
  vec4 p = permute( permute( permute(\
       i.z + vec4( 0.0, i1.z, i2.z, 1.0 ) )\
       + i.y + vec4( 0.0, i1.y, i2.y, 1.0 ) )\
       + i.x + vec4( 0.0, i1.x, i2.x, 1.0 ) );\
  float n_ = 1.0 / 7.0;\
  vec3 ns = n_ * D.wyz - D.xzx;\
  vec4 j = p - 49.0 * floor( p * ns.z *ns.z );\
  vec4 x_ = floor( j * ns.z );\
  vec4 y_ = floor( j - 7.0 * x_ );\
  vec4 x = x_ *ns.x + ns.yyyy;\
  vec4 y = y_ *ns.x + ns.yyyy;\
  vec4 h = 1.0 - abs( x ) - abs( y );\
  vec4 b0 = vec4( x.xy, y.xy );\
  vec4 b1 = vec4( x.zw, y.zw );\
  vec4 s0 = floor( b0 ) * 2.0 + 1.0;\
  vec4 s1 = floor( b1 ) * 2.0 + 1.0;\
  vec4 sh = -step( h, vec4( 0.0 ) );\
  vec4 a0 = b0.xzyw + s0.xzyw * sh.xxyy;\
  vec4 a1 = b1.xzyw + s1.xzyw * sh.zzww;\
  vec3 p0 = vec3( a0.xy, h.x );\
  vec3 p1 = vec3( a0.zw, h.y );\
  vec3 p2 = vec3( a1.xy, h.z );\
  vec3 p3 = vec3( a1.zw, h.w );\
  vec4 norm = taylorInvSqrt( vec4( dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3) ) );\
  p0 *= norm.x;\
  p1 *= norm.y;\
  p2 *= norm.z;\
  p3 *= norm.w;\
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3) ), 0.0 );\
  m = m * m;\
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),\
                dot(p2,x2), dot(p3,x3) ) );\
}\
float heightMap( vec3 coord ) {\
  float n = abs( snoise( coord ) );\
  n += 0.25   * abs( snoise( coord * 2.0 ) );\
  n += 0.25   * abs( snoise( coord * 4.0 ) );\
  n += 0.125  * abs( snoise( coord * 8.0 ) );\
  n += 0.0625 * abs( snoise( coord * 16.0 ) );\
  return n;\
}\
void main( void ) {\
  float n = heightMap( vTexCoord3D );\
  gl_FragColor = vec4( vec3( 1.5 - n, 1.0 - n, 0.5 - n ), 1.0 );\
  const float e = 0.001;\
  float nx = heightMap( vTexCoord3D + vec3( e, 0.0, 0.0 ) );\
  float ny = heightMap( vTexCoord3D + vec3( 0.0, e, 0.0 ) );\
  float nz = heightMap( vTexCoord3D + vec3( 0.0, 0.0, e ) );\
  vec3 normal = normalize( vNormal + 0.05 * vec3( n - nx, n - ny, n - nz ) / e );\
  vec3 vLightWeighting = vec3( 0.1 );\
  vec4 lDirection = viewMatrix * vec4( normalize( vec3( 1.0, 0.0, 0.5 ) ), 0.0 );\
  float directionalLightWeighting = dot( normal, normalize( lDirection.xyz ) ) * 0.25 + 0.75;\
  vLightWeighting += vec3( 1.0 ) * directionalLightWeighting;\
  vec3 dirHalfVector = normalize( lDirection.xyz + normalize( vViewPosition ) );\
  float dirDotNormalHalf = dot( normal, dirHalfVector );\
  float dirSpecularWeight = 0.0;\
  if ( dirDotNormalHalf >= 0.0 )\
    dirSpecularWeight = ( 1.0 - n ) * pow( dirDotNormalHalf, 5.0 );\
  vLightWeighting += vec3( 1.0, 0.5, 0.0 ) * dirSpecularWeight * n * 2.0;\
  gl_FragColor *= vec4( vLightWeighting, 1.0 );\
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

  auto material = ShaderMaterial::create(
    vertexShader,
    fragmentShader,
    Uniforms().add("time",  Uniform( enums::f, time ))
              .add("scale", Uniform( enums::f, 1.5f ))
  );

  // Geometries
  auto mesh = Mesh::create( SphereGeometry::create( 0.75f, 64, 32 ), material );
  scene->add( mesh );

  renderer->setClearColorHex( 0x050505, 0 );

  /////////////////////////////////////////////////////////////////////////

  auto running = true;
  sdl::addEventListener(SDL_KEYDOWN, [&]( const sdl::Event& ) {
    running = false;
  });
  sdl::addEventListener(SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  });

  /////////////////////////////////////////////////////////////////////////

  anim::gameLoop( [&]( float dt ) -> bool {

    time += dt;
    material->uniforms[ "time" ].value = time;

    mesh->rotation().y( mesh->rotation().y() + 0.5f * dt );
    mesh->rotation().x( mesh->rotation().x() + 0.1f * dt );

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