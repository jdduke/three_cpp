#include "common.hpp"

#include <three/core/geometry.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/objects/mesh.hpp>
#include <three/materials/shader_material.hpp>
#include <three/materials/mesh_face_material.hpp>
#include <three/renderers/renderer_parameters.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras/image_utils.hpp>
#include <three/extras/geometries/cube_geometry.hpp>

const char* vertexShader() {
  return
"\
varying vec2 vUv;\
void main() {\
  vUv = uv;\
  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\
  gl_Position = projectionMatrix * mvPosition;\
}\
";
}

const char* fragmentShader1() {
  return
"\
uniform float time;\
varying vec2 vUv;\
void main(void) {\
  vec2 p = -1.0 + 2.0 * vUv;\
  float a = time*40.0;\
  float d,e,f,g=1.0/40.0,h,i,r,q;\
  e=400.0*(p.x*0.5+0.5);\
  f=400.0*(p.y*0.5+0.5);\
  i=200.0+sin(e*g+a/150.0)*20.0;\
  d=200.0+cos(f*g/2.0)*18.0+cos(e*g)*7.0;\
  r=sqrt(pow(i-e,2.0)+pow(d-f,2.0));\
  q=f/r;\
  e=(r*cos(q))-a/2.0;f=(r*sin(q))-a/2.0;\
  d=sin(e*g)*176.0+sin(e*g)*164.0+r;\
  h=((f+d)+a/2.0)*g;\
  i=cos(h+r*p.x/1.3)*(e+e+a)+cos(q*g*6.0)*(r+h/3.0);\
  h=sin(f*g)*144.0-sin(e*g)*212.0*p.x;\
  h=(h+(f-e)*q+sin(r-(a+h)/7.0)*10.0+i/4.0)*g;\
  i+=cos(h*2.3*sin(a/350.0-q))*184.0*sin(q-(r*4.3+a/12.0)*g)+tan(r*g+h)*184.0*cos(r*g+h);\
  i=mod(i/5.6,256.0)/64.0;\
  if(i<0.0) i+=4.0;\
  if(i>=2.0) i=4.0-i;\
  d=r/350.0;\
  d+=sin(d*d*8.0)*0.52;\
  f=(sin(a*g)+1.0)/2.0;\
  gl_FragColor=vec4(vec3(f*i/1.6,i/2.0+d/13.0,i)*d*p.x+vec3(i/1.3+d/8.0,i/2.0+d/18.0,i)*d*(1.0-p.x),1.0);\
}\
";}

const char* fragmentShader2() {
  return
"\
uniform float time;\
varying vec2 vUv;\
void main( void ) {\
  vec2 position = -1.0 + 2.0 * vUv;\
  float red = abs( sin( position.x * position.y + time / 5.0 ) );\
  float green = abs( sin( position.x * position.y + time / 4.0 ) );\
  float blue = abs( sin( position.x * position.y + time / 3.0 ) );\
  gl_FragColor = vec4( red, green, blue, 1.0 );\
}\
";
}

const char* fragmentShader3() {
  return
"\
uniform float time;\
varying vec2 vUv;\
void main( void ) {\
  vec2 position = vUv;\
  float color = 0.0;\
  color += sin( position.x * cos( time / 15.0 ) * 80.0 ) + cos( position.y * cos( time / 15.0 ) * 10.0 );\
  color += sin( position.y * sin( time / 10.0 ) * 40.0 ) + cos( position.x * sin( time / 25.0 ) * 40.0 );\
  color += sin( position.x * sin( time / 5.0 ) * 10.0 ) + sin( position.y * sin( time / 35.0 ) * 80.0 );\
  color *= sin( time / 10.0 ) * 0.5;\
  gl_FragColor = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );\
}\
";
}

const char* fragmentShader4() {
  return
"\
uniform float time;\
uniform sampler2D texture;\
varying vec2 vUv;\
void main( void ) {\
  vec2 position = -1.0 + 2.0 * vUv;\
  float a = atan( position.y, position.x );\
  float r = sqrt( dot( position, position ) );\
  vec2 uv;\
  uv.x = cos( a ) / r;\
  uv.y = sin( a ) / r;\
  uv /= 10.0;\
  uv += time * 0.05;\
  vec3 color = texture2D( texture, uv ).rgb;\
  gl_FragColor = vec4( color * r * 1.5, 1.0 );\
}\
";
}

using namespace three;

void shader2( GLRenderer::Ptr renderer ) {

  auto camera = PerspectiveCamera::create(
    40, (float)renderer->width() / renderer->height(), 1, 3000
  );
  camera->position.z = 6;

  auto scene = Scene::create();

  float time = 1;

  auto texture = ImageUtils::loadTexture( threeDataPath("textures/disturb.jpg") );
  texture->wrapS = texture->wrapT = THREE::RepeatWrapping;

  Uniforms uniforms1;
  uniforms1.add( "time", Uniform( THREE::f, time) );

  Uniforms uniforms2;
  uniforms2.add( "time", Uniform( THREE::f, time) )
           .add( "texture", Uniform( THREE::t, texture.get() ) );

  std::vector<Material::Ptr> mlib;
  std::vector<Mesh::Ptr> meshes;

  auto addCube = [&]( float size, float x, float y, const char* vertexShader, const char* fragmentShader, Uniforms& uniforms ) {

    auto material = ShaderMaterial::create(
      Material::Parameters().add( "uniforms", uniforms )
                            .add( "vertexShader", std::string(vertexShader) )
                            .add( "fragmentShader", std::string(fragmentShader) )
    );

    mlib.push_back( material );

    auto mesh = Mesh::create( CubeGeometry::create( size, size, size,
                                                    1, 1, 1,
                                                    std::vector<Material::Ptr>( 1, material ) ),
                              MeshFaceMaterial::create() );
    mesh->position.x = x;
    mesh->position.y = y;
    scene->add( mesh );

    meshes.push_back( mesh );

  };

  addCube( .75f, -1.5f, -.5f, vertexShader(), fragmentShader1(), uniforms1 );
  addCube( .75f, -0.5f,  .5f, vertexShader(), fragmentShader2(), uniforms1 );
  addCube( .75f,  0.5f, -.5f, vertexShader(), fragmentShader3(), uniforms1 );
  addCube( .75f,  1.5f,  .5f, vertexShader(), fragmentShader4(), uniforms2 );


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

    for ( auto& material : mlib )
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

  shader2( renderer );

  return 0;
}