#ifndef THREE_LIGHT_H
#define THREE_LIGHT_H

#include <three/common.h>

#include <three/visitor.h>
#include <three/math/color.h>
#include <three/core/object3d.h>

namespace three {

class Light : public Object3D {

public:

  THREE_IMPL_OBJECT(Light);

  Color color;
  float intensity;
  float distance;

  Object3D::Ptr target;

  bool castShadow, onlyShadow, shadowCascade;

protected:

  explicit Light( int hex, float intensity = 1, float distance = 0 )
    : Object3D(),
      color( hex ),
      intensity( intensity ),
      distance( distance ),
      target( nullptr ),
      castShadow( false ),
      onlyShadow( false ),
      shadowCascade( false ) { }

  virtual Object3D::Ptr __clone( Object3D::Ptr target, bool recursive ) const THREE_OVERRIDE {

    if ( !target )
      THREE_ASSERT( false );

    Object3D::__clone( target, recursive );

    auto light = std::static_pointer_cast<Light>( target );

    light->color = color;
    light->intensity = intensity;
    light->distance = distance;

    light->target = target;

    light->castShadow = castShadow;
    light->onlyShadow = onlyShadow;
    light->shadowCascade = shadowCascade;

    return light;
  }

};

} // namespace three

#endif // THREE_LIGHT_H