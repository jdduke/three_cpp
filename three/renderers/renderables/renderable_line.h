#ifndef THREE_RENDERABLE_LINE_HPP
#define THREE_RENDERABLE_LINE_HPP

#include <three/common.hpp>

#include <three/renderers/renderables/renderable.hpp>
#include <three/renderers/renderables/renderable_vertex.hpp>

namespace three {

class RenderableLine : public Renderable {
public:

  RenderableLine() : Renderable( 0 ), material( nullptr ) { }

  RenderableVertex v1, v2;
  Material* material;
};

} // namespace three

#endif // THREE_RENDERABLE_LINE_HPP