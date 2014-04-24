#ifndef THREE_RENDERABLE_OBJECT_H
#define THREE_RENDERABLE_OBJECT_H

#include <three/common.h>

#include <three/renderers/renderables/renderable.h>

namespace three {

class RenderableObject : public Renderable {

public:

  explicit RenderableObject( Object3D* object = nullptr, float z = 0 )
    : Renderable( z ), object( object ) { }

  Object3D* object;
  
};

struct PainterSort {

  bool operator()( const RenderableObject& a, const RenderableObject& b ) const {
    return b.z > a.z;
  }

  bool operator()( const Renderable* a, const Renderable* b ) const {
    return b->z > a->z;
  }

};

struct PainterSortStable {

  bool operator()( const RenderableObject& a, const RenderableObject& b ) const {
    if ( a.z != b.z ) {

		return b.z - a.z;

	} else {

		return a.id - b.id;

	}
  }

  bool operator()( const Renderable* a, const Renderable* b ) const {
    if ( a->z != b->z ) {

		return b->z - a->z;

	} else {

		return a->id - b->id;

	}
  }

};

struct NumericalSort {

  bool operator()( const std::pair<float, int>& a, const std::pair<float, int>& b ) const {
    return b.first - a.first;
  }

};

} // namespace three

#endif // THREE_RENDERABLE_OBJECT_H