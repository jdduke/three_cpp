#ifndef THREE_RENDERABLE_VERTEX_H
#define THREE_RENDERABLE_VERTEX_H

#include <three/common.h>

namespace three {

class RenderableVertex {

public:

  RenderableVertex() : visible( true ) { }

  Vector3 position;
  Vector3 positionWorld;
  Vector4 positionScreen;

  bool visible;

  RenderableVertex& copy( const RenderableVertex& vertex ) {

    positionWorld.copy( vertex.positionWorld );
	positionScreen.copy( vertex.positionScreen );

	return *this;

  }
};

} // namespace three

#endif // THREE_RENDERABLE_VERTEX_H