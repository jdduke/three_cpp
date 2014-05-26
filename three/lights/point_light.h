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


  virtual void __clone( Object3D::Ptr& cloned, bool recursive ) const THREE_OVERRIDE {

    if ( !cloned ) cloned = create( 0 );

    Light::__clone( cloned, recursive );

  }

};

} // namespace three

#endif // THREE_POINT_LIGHT_H