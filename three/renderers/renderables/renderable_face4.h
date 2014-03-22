#ifndef THREE_RENDERABLE_FACE4_H
#define THREE_RENDERABLE_FACE4_H

#include <three/common.h>

namespace three {

class RenderableFace4 : public RenderableFace {
public:
  RenderableFace4() : RenderableFace( enums::Face4 ) { }
};

} // namespace three

#endif // THREE_RENDERABLE_FACE4_H