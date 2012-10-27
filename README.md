three.cpp
=========

#### A port of three.js to C++ #####

three.js is a popular and accessible 3D library (credits to mdroob and alteredq)
 The goal with three.cpp is to fully implement the relevant portions of the library in native C++11, up to and including revision 50.

### Status ###

Much of the core functionality has been implemented to some degree, excepting
morph targets and animation of any sort. It's not pretty; I tried to preserve
the style/syntax/structure of the original library while not completely
obviating the merits of native code . To be sure, this is as direct a port as
possible; you will find things like public member variables, tight coupling and
likely other code snippets that may or may not go against all you have come to
believe in.  C++ is *not* Javascript.

For the examples that have been ported, perf is anywhere from 2x-10x that of
three.js, average 4-5x, on my i7, GT 650m laptop (with vsync off on both, of course).
No thorough profiling or meaningful optimization has been done, and the graphics
path is more or less identical to that found in three.js.

TODO: Test with GLES (Android/iOS/NACL)


### Usage ###

three.cpp is optionally header-only; you can simply drop the
"three" folder in your project and get started, or you can compile it into a
static library (dynamic to come...).  The design is more or less
that found in [Boost Asio](http://www.boost.org/doc/libs/1_51_0/doc/html/boost_asio.html):

# As header-only library #
Checkout.  All relevant files are in the base **three** folder. Include and use.

# As compiled library #
Checkout.  Run CMake with **THREE_HEADER_ONLY** set to false.  This will define
**THREE_SEPARATE_COMPILATION**.  Compile.  A static library `three_impl.{a/lib}` will
be created in **lib**.  Any code using this library must also define
**THREE_SEPARATE_COMPILATION** when using three.cpp.


#### Sample code ####

This code creates a renderer, then a scene and camera, adds the camera and cube to the scene, then
starts the rendering loop.

```c++

void scene() {

  using namespace three;

  auto renderer = GLRenderer::create();

  auto camera = PerspectiveCamera::create( 75, renderer->width() / renderer->height(), 1, 10000 );
  camera->position.z = 1000;

  auto scene = Scene::create();

  auto geometry = CubeGeometry::create( 200, 200, 200 );
  auto material = MeshBasicMaterial::create(
    Material::Paramaters().add( "color", Color(0xff0000) )
                          .add( "wireframe", true )
  );

  auto mesh = Mesh::create( geometry, material );
  scene->add( mesh );

  anim::gameLoop( [&]( float dt ) -> bool {

    mesh->rotation.x += 0.01f;
    mesh->rotation.y += 0.02f;

    renderer->render( *scene, *camera );

  } );

}

```


### Examples ###

Run CMake with BUILD_THREE_EXAMPLES enabled.  Compile and run from bin/*.  The
examples will use either the header-only or compiled library, depending on the
THREE_HEADER_ONLY condition.

Working examples:
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_custom_attributes_particles.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_custom_attributes_particles2.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_custom_attributes_particles3.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_geometry_hierarchy.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_geometry_hierarchy2.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_geometry_minecraft.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_lines_colors.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_lines_cubes.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_lines_sphere.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_particles_billboards.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_particles_billboards_colorspng">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_particles_random.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_particles_sprites.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_shader.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_shader2.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_shader_lava.png">
<img src="https://raw.github.com/jdduke/three_cpp/master/data/thumbs/webgl_test_memory.png">


### Compatibility ###

You'll need a sufficiently modern C++11 compiler; Clang 3.1, GCC 4.6.3+, or
(debateable) MSVC 2012.

Most functionality tested and working on Mint 13 with GCC 4.6.3, Windows 7 with
both MSVC 2012 and MinGW (GCC 4.8 and 4.7) and OSX with Clang 3.1.  Some examples
might be a little flaky on any given platform.  MSVC 2012 is really a pain to
support (no variadics, no initializer lists, no default/deleted functions, no
constexpr etc...), and support may be dropped at some point in the future.


### FAQ ###

* Why on earth would you do this?
** It started as an exercise to 1) dive into a popular Javascript library of medium size, 2) play with C++11 and 3) offend as many programmers as possible
* But, but, C++ is lame!?
** Totally!
* And Javascript is the devil!
** Clearly!


