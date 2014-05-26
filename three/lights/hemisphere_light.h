#ifndef THREE_HEMISPHERE_LIGHT_H
#define THREE_HEMISPHERE_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class HemisphereLight : public Light {

public:

  THREE_IMPL_OBJECT(HemisphereLight);

  static Ptr create( int skyColorHex, int groundColorHex, float intensity = 1 ) {

    return make_shared<HemisphereLight>( skyColorHex, groundColorHex, intensity );

  }

  Color groundColor;

protected:

  HemisphereLight( int skyColorHex, int groundColorHex, float intensity )
    : Light( skyColorHex, intensity ),
      groundColor( groundColorHex ) {

    position().set( 0, 100, 0 );

  }

  virtual void __clone( Object3D::Ptr& cloned, bool recursive ) const THREE_OVERRIDE {

    if ( !cloned ) cloned = create( 0, 0 );

    Light::__clone( cloned, recursive );

    static_cast<HemisphereLight&>( *cloned ).groundColor = groundColor;

  }

};

} // namespace three

#endif // THREE_HEMISPHERE_LIGHT_H