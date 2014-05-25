#ifndef THREE_RENDERABLE_SPRITE_H
#define THREE_RENDERABLE_SPRITE_H

#include <three/common.h>

#include <three/math/vector2.h>

#include <three/renderers/renderables/renderable.h>

namespace three {

class RenderableSprite : public Renderable {

public:

  RenderableSprite()
    : Renderable( 0 ), object( nullptr ), x( 0 ), y( 0 ), rotation( 0 ), material( nullptr ) { }

  Object3D* object;

  float x, y;

  float rotation;
  Vector2 scale;

  Material* material;

};

} // namespace three

#endif // THREE_RENDERABLE_LINE_H