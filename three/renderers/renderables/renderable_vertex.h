#ifndef THREE_RENDERABLE_VERTEX_H
#define THREE_RENDERABLE_VERTEX_H

#include <three/common.h>

namespace three {

class RenderableVertex {
public:

  Vector3 position;
  Vector3 positionWorld;
  Vector4 positionScreen;

  bool visible;

  RenderableVertex() : visible( true ) { }

  RenderableVertex& copy( const RenderableVertex& vertex ) {
    return ( *this ) = vertex;
  }
};

} // namespace three

#endif // THREE_RENDERABLE_VERTEX_H