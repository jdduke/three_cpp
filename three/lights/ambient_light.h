#ifndef THREE_AMBIENT_LIGHT_H
#define THREE_AMBIENT_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class AmbientLight : public Light {

public:

  THREE_IMPL_OBJECT(AmbientLight);

  static Ptr create( int hex ) {

    return make_shared<AmbientLight>( hex );

  }

protected:

  AmbientLight( int hex )
    : Light( hex ) {}

  virtual Object3D::Ptr __clone( Object3D::Ptr target, bool recursive ) const THREE_OVERRIDE {

    Ptr light = target ? std::static_pointer_cast<AmbientLight>(target) : create( 0 );

    Light::__clone( target, recursive );

    return target;
  }

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_H