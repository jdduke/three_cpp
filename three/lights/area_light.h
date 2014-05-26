#ifndef THREE_AREA_LIGHT_H
#define THREE_AREA_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class AreaLight : public Light {

public:

  THREE_IMPL_OBJECT(AreaLight);

  static Ptr create( int hex , float intensity = 1 ) {

    return make_shared<AreaLight>( hex, intensity );

  }

  Vector3 normal;
  Vector3 right;

  float width;
  float height;

  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;

protected:

  AreaLight( int hex , float intensity )
    : Light( hex, intensity ),
      normal( 0, -1, 0 ),
      right( 1, 0, 0 ),
      width( 1 ),
      height( 1 ),
      constantAttenuation( 1.5 ),
      linearAttenuation( 0.5 ),
      quadraticAttenuation( 0.1 ) {}


  virtual void __clone( Object3D::Ptr& cloned, bool recursive ) const THREE_OVERRIDE {

    if ( !cloned ) cloned = create( 0 );

    Light::__clone( cloned, recursive );

    auto light = static_cast<AreaLight&>( *cloned );
    light.normal = normal;
    light.right = right;

    light.right = width;
    light.right = height;

    light.constantAttenuation = constantAttenuation;
    light.linearAttenuation = linearAttenuation;
    light.quadraticAttenuation = quadraticAttenuation;

  }

};

}

#endif // THREE_AREA_LIGHT_H