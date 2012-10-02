#ifndef THREE_EXAMPLES_COMMON_HPP
#define THREE_EXAMPLES_COMMON_HPP

#include <three/config.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/glew.hpp>
#include <three/extras/sdl.hpp>

#include <three/renderers/gl_renderer.hpp>
#include <three/renderers/renderer_parameters.hpp>

#ifndef THREE_DATA_DIR
#define THREE_DATA_DIR "."
#endif

namespace three {

inline const std::string& threeDataDir() {
  static std::string sThreeDataDir(THREE_DATA_DIR);
  return sThreeDataDir;
}

inline std::string threeDataPath( const std::string& relativePath ) {
  return threeDataDir() + "/" + relativePath;
}

struct ExampleSession {

  ExampleSession(  three::RendererParameters parameters = three::RendererParameters() ) {
    if ( !three::sdl::init( parameters ) || !three::glew::init( parameters ) ) {
      return;
    }
    renderer = three::GLRenderer::create( parameters );
  }

  ~ExampleSession() {
    three::sdl::quit();
  }

  template < typename Example >
  void run( Example example ) {
    if ( renderer )
      example( renderer );
  }

private:
  ExampleSession(ExampleSession&);
  ExampleSession& operator=(ExampleSession&);

  three::GLRenderer::Ptr renderer;
};

} // namespace three

#endif // THREE_EXAMPLES_COMMON_HPP