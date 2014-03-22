#ifndef THREE_RENDERABLE_FACE3_H
#define THREE_RENDERABLE_FACE3_H

#include <three/common.h>

namespace three {

class RenderableFace3 : public RenderableFace {
public:
  RenderableFace3() : RenderableFace( enums::Face3 ) { }
};

} // namespace three

#endif // THREE_RENDERABLE_FACE3_H