#ifndef THREE_RENDERABLE_OBJECT_HPP
#define THREE_RENDERABLE_OBJECT_HPP

#include <three/common.hpp>

#include <three/renderers/renderables/renderable.hpp>

namespace three {

class RenderableObject : public Renderable {
public:

  Object3D* object;

  explicit RenderableObject( Object3D* object = nullptr, float z = 0 )
    : Renderable( z ), object( object ) { }
};

struct PainterSort {
  bool operator()( const RenderableObject& a, const RenderableObject& b ) const {
    return b.z < a.z;
  }
  bool operator()( const Renderable* a, const Renderable* b ) const {
    return b->z < a->z;
  }
};

} // namespace three

#endif // THREE_RENDERABLE_OBJECT_HPP