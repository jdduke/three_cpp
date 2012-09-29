#ifndef THREE_RENDERER_PARAMETERS_HPP
#define THREE_RENDERER_PARAMETERS_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>

namespace three {

struct RendererParameters {
  RendererParameters()
  : width( 1024 ), height( 768 ),
    vsync ( true ),
    precision( THREE::PrecisionHigh ),
    alpha( true ),
    premultipliedAlpha( true ),
    antialias( false ),
    stencil( true ),
    preserveDrawingBuffer( false ),
    clearColor( 0 ),
    clearAlpha( 0 ),
    maxLights( 4 ) { }

  int width, height;
  bool vsync;
  THREE::PrecisionType precision;
  bool alpha;
  bool premultipliedAlpha;
  bool antialias;
  bool stencil;
  bool preserveDrawingBuffer;
  Color clearColor;
  float clearAlpha;
  int maxLights;
};

} // namespace three

#endif // THREE_RENDERER_PARAMETERS_HPP