#ifndef THREE_RENDERABLE_LINE_HPP
#define THREE_RENDERABLE_LINE_HPP

#include <three/common.h>

#include <three/renderers/renderables/renderable.h>
#include <three/renderers/renderables/renderable_vertex.h>

namespace three {

class RenderableLine : public Renderable {
public:

  RenderableLine() : Renderable( 0 ), material( nullptr ) { }

  RenderableVertex v1, v2;
  Material* material;
};

} // namespace three

#endif // THREE_RENDERABLE_LINE_HPP