#ifndef THREE_RENDERABLE_FACE3_HPP
#define THREE_RENDERABLE_FACE3_HPP

#include <three/common.hpp>

namespace three {

class RenderableFace3 : public RenderableFace {
public:
  RenderableFace3() : RenderableFace( THREE::Face3 ) { }
};

} // namespace three

#endif // THREE_RENDERABLE_FACE3_HPP