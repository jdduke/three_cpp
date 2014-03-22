#ifndef THREE_AREA_LIGHT_HPP
#define THREE_AREA_LIGHT_HPP

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class AreaLight : public Light {

public:

	typedef std::shared_ptr<AreaLight> Ptr;

  static Ptr create( int hex , float intensity = 1 ) {

    return make_shared<AreaLight>( hex );

  }

  Vector3 normal;
  Vector3 right;

  float width;
  float height;

  float constantAttenuation;
  float linearAttenuation;
  float quadraticAttenuation;

  virtual enums::Type type() const { 

    return enums::AreaLight;

  }

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

};

}

#endif // THREE_AREA_LIGHT_HPP