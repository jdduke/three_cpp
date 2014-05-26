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

  virtual void __clone( Object3D::Ptr& cloned, bool recursive ) const THREE_OVERRIDE {

    if ( !cloned ) cloned = create( 0 );

    Light::__clone( cloned, recursive );

  }

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_H