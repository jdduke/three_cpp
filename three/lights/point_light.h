#ifndef THREE_POINT_LIGHT_H
#define THREE_POINT_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class PointLight : public Light {

public:

  THREE_IMPL_OBJECT(PointLight);

  static Ptr create( int hex, float intensity = 1, float distance = 0 ) {

    return make_shared<PointLight>( hex, intensity, distance );

  }

protected:

  PointLight( int hex, float intensity, float distance )
    : Light( hex, intensity, distance ) {

    position().set( 0, 0, 0 );

  }


  virtual Object3D::Ptr __clone( Object3D::Ptr target, bool recursive ) const {

    Ptr light = target ? std::static_pointer_cast<PointLight>(target) : create( 0 );

    Light::__clone( light, recursive );

    return light;

  }

};

} // namespace three

#endif // THREE_POINT_LIGHT_H