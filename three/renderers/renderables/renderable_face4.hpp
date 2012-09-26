#ifndef THREE_RENDERABLE_FACE4_HPP
#define THREE_RENDERABLE_FACE4_HPP

#include <three/common.hpp>

namespace three {

class RenderableFace4 : public RenderableFace {
public:
  RenderableFace4() : RenderableFace( THREE::Face4 ) { }
};

} // namespace three

#endif // THREE_RENDERABLE_FACE4_HPP