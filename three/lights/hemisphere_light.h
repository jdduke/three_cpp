#ifndef THREE_HEMISPHERE_LIGHT_H
#define THREE_HEMISPHERE_LIGHT_H

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class HemisphereLight : public Light {
  
public:

  typedef std::shared_ptr<HemisphereLight> Ptr;

  static Ptr create( int skyColorHex, int groundColorHex, float intensity = 1 ) {

    return make_shared<HemisphereLight>( skyColorHex, groundColorHex, intensity );

  }

  virtual enums::Type type() const { 

    return enums::HemisphereLight; 

  }

  Color groundColor;

protected:

  HemisphereLight( int skyColorHex, int groundColorHex, float intensity )
    : Light( skyColorHex, intensity ),
      groundColor( groundColorHex ) {

    position.set( 0, 100, 0 );

  }

};

} // namespace three

#endif // THREE_HEMISPHERE_LIGHT_H