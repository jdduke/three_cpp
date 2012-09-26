#ifndef THREE_HEMISPHERE_LIGHT_HPP
#define THREE_HEMISPHERE_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class HemisphereLight : public Light {
public:

  typedef std::shared_ptr<HemisphereLight> Ptr;

  static Ptr create( int skyColorHex, int groundColorHex, float intensity = 1 ) {
    return make_shared<HemisphereLight>( skyColorHex, groundColorHex, intensity );
  }

  virtual THREE::Type type() const { return THREE::HemisphereLight; }

  //////////////////////////////////////////////////////////////////////////

  Color groundColor;

  //////////////////////////////////////////////////////////////////////////

protected:

  HemisphereLight( int skyColorHex, int groundColorHex, float intensity )
    : Light( skyColorHex, intensity ),
      groundColor( groundColorHex ) {

    position = Vector3( 0, 100, 0 );

  }

};

} // namespace three

#endif // THREE_HEMISPHERE_LIGHT_HPP