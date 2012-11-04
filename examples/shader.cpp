#include "common.hpp"

#include <three/core/geometry.hpp>
#include <three/cameras/camera.hpp>
#include <three/objects/mesh.hpp>
#include <three/extras/geometries/plane_geometry.hpp>
#include <three/materials/shader_material.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

std::string vertexShader() {
  return std::string(
"\
void main() { \
  gl_Position = vec4( position, 1.0 ); \
}\
");
}

std::string fragmentShader() {
  return std::string(
"\
uniform vec2 resolution; \
uniform float time; \
void main() { \
  vec2 p = -1.0 + 2.0 * gl_FragCoord.xy / resolution.xy; \
  float a = time*40.0; \
  float d,e,f,g=1.0/40.0,h,i,r,q; \
  e=400.0*(p.x*0.5+0.5); \
  f=400.0*(p.y*0.5+0.5); \
  i=200.0+sin(e*g+a/150.0)*20.0; \
  d=200.0+cos(f*g/2.0)*18.0+cos(e*g)*7.0; \
  r=sqrt(pow(i-e,2.0)+pow(d-f,2.0)); \
  q=f/r; \
  e=(r*cos(q))-a/2.0;f=(r*sin(q))-a/2.0; \
  d=sin(e*g)*176.0+sin(e*g)*164.0+r; \
  h=((f+d)+a/2.0)*g; \
  i=cos(h+r*p.x/1.3)*(e+e+a)+cos(q*g*6.0)*(r+h/3.0); \
  h=sin(f*g)*144.0-sin(e*g)*212.0*p.x; \
  h=(h+(f-e)*q+sin(r-(a+h)/7.0)*10.0+i/4.0)*g; \
  i+=cos(h*2.3*sin(a/350.0-q))*184.0*sin(q-(r*4.3+a/12.0)*g)+tan(r*g+h)*184.0*cos(r*g+h); \
  i=mod(i/5.6,256.0)/64.0; \
  if(i<0.0) i+=4.0; \
  if(i>=2.0) i=4.0-i; \
  d=r/350.0; \
  d+=sin(d*d*8.0)*0.52; \
  f=(sin(a*g)+1.0)/2.0; \
  gl_FragColor=vec4(vec3(f*i/1.6,i/2.0+d/13.0,i)*d*p.x+vec3(i/1.3+d/8.0,i/2.0+d/18.0,i)*d*(1.0-p.x),1.0); \
}\
");

}

using namespace three;

void shader( GLRenderer::Ptr renderer ) {

  auto camera = Camera::create();
  camera->position.z = 1;

  auto scene = Scene::create();

  float time = 1;

  Uniforms uniforms;
  uniforms[ "time" ]       = Uniform( THREE::f, time);
  uniforms[ "resolution" ] = Uniform( THREE::v2, Vector2( (float)renderer->width(),
                                                          (float)renderer->height()) );

  auto material = ShaderMaterial::create(
    vertexShader(),
    fragmentShader(),
    uniforms
  );

  // Geometries
  auto mesh = Mesh::create( PlaneGeometry::create( 2, 2 ), material );
  scene->add( mesh );

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

  shader( renderer );

  return 0;
}