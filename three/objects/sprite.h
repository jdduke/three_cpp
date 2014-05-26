#ifndef THREE_SPRITE_H
#define THREE_SPRITE_H

#include <three/common.h>

#include <three/core/object3d.h>
#include <three/materials/material.h>

namespace three {

// TODO "Impl"

class Sprite : public Object3D {
public:

  THREE_IMPL_OBJECT(Sprite);

  static Ptr create( const Material::Ptr& material ) {
    return three::make_shared<Sprite>( material );
  }

protected:

  Sprite( const Material::Ptr& material )
    : Object3D( material ) { }

  // TODO "Sprite::_clone"

};

} // namespace three

#endif // THREE_SPRITE_H