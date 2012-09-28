#include <three/core/geometry.hpp>
#include <three/objects/particle_system.hpp>
#include <three/cameras/perspective_camera.hpp>
#include <three/materials/particle_basic_material.hpp>
#include <three/renderers/gl_renderer.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

using namespace three;

void trails( GLRenderer::Ptr renderer ) {

  renderer->sortObjects = false;
  renderer->autoClearColor = false;

  // Camera
  auto camera = PerspectiveCamera::create(
    60,
    (float)renderer->width() / renderer->height(),
    1, 10000
  );
  camera->position.set( 100000, 0, 3200 );

  // Scene
  auto scene = Scene::create();

  // Geometries
  std::array<Color, 4> colors = { Color(0x000000), Color(0xff0080), Color(0x8000ff), Color(0xffffff) };
  auto geometry = Geometry::create();

  for ( int i = 0; i < 2000; i ++ ) {
    Vertex vertex;
    vertex.position.x = Math::random(-2000.f, 2000.f);
    vertex.position.y = Math::random(-2000.f, 2000.f);
    vertex.position.z = Math::random(-2000.f, 2000.f);
    geometry->vertices.push_back( vertex );
    geometry->colors.push_back( colors[ (int)Math::floor( Math::random() * colors.size() ) ] );
  }

  // Materials
  auto material = ParticleBasicMaterial::create(
    Material::Parameters().add("color", Color(0xcccccc))
                          .add("size", 1.0f)
                          .add("vertexColors", THREE::VertexColors)
                          .add("depthTest", false)
                          .add("opacity", 0.5f)
                          .add("sizeAttenuation", false)
  );

  auto mesh = ParticleSystem::create( geometry, material );
  scene->add( mesh );

  auto time = 0.f, mouseX = 0.f, mouseY = 0.f;

  // Rendering
  anim::gameLoop (

    [&](float) -> bool {

      SDL_Event event;
      while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
          case SDL_KEYDOWN:
          case SDL_QUIT:
            return false;
          case SDL_MOUSEMOTION:
            mouseX = 2.f * ((float)event.motion.x / renderer->width()  - 0.5f);
            mouseY = 2.f * ((float)event.motion.y / renderer->height() - 0.5f);
          default:
          break;
        };
      }

      camera->position.x += ( 1000.f * mouseX - camera->position.x );
      camera->position.y += ( 1000.f * mouseY - camera->position.y );
      camera->lookAt( scene->position );

      renderer->render( *scene, *camera );
      sdl::swapBuffers();
      return true;

  } );

}

int main ( int argc, char* argv[] ) {

  auto onQuit = defer( SDL_Quit );

  GLRenderer::Parameters parameters;
  parameters.preserveDrawingBuffer = true;

  if ( !sdl::init( parameters ) || !glew::init( parameters ) ) {
    return 0;
  }

  auto renderer = GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  trails( renderer );

  return 0;
}