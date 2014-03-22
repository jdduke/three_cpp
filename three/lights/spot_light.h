#ifndef THREE_SPOT_LIGHT_HPP
#define THREE_SPOT_LIGHT_HPP

#include <three/common.h>

#include <three/lights/light.h>

namespace three {

class SpotLight : public Light {

public:

  typedef std::shared_ptr<SpotLight> Ptr;

  static Ptr create( int hex, float intensity = 1, float distance = 0, float angle = Math::PI() / 3, float exponent = 10 ) {

    return make_shared<SpotLight>( hex, intensity, distance, angle, exponent );

  }

  virtual enums::Type type() const { 

    return enums::SpotLight; 

  }

  float angle;
  float exponent;

  float shadowCameraNear;
  float shadowCameraFar;
  float shadowCameraFov;

  bool shadowCameraVisible;

  float shadowBias;
  float shadowDarkness;

  int shadowMapWidth;
  int shadowMapHeight;

  Texture::Ptr shadowMap;
  std::array<int, 2> shadowMapSize;
  Camera::Ptr shadowCamera;
  Matrix4 shadowMatrix;

protected:

  SpotLight( int hex, float intensity, float distance, float angle, float exponent )
    : Light( hex, intensity, distance ),
      angle( angle ), exponent( exponent ),
      shadowCameraNear( 50 ), 
      shadowCameraFar( 5000 ), 
      shadowCameraFov( 50 ),
      shadowCameraVisible( false ),
      shadowBias( 0 ),
      shadowDarkness( 0.5 ),
      shadowMapWidth( 512 ),
      shadowMapHeight( 512 ) {

    position.set( 0, 1, 0 );

  }

};

} // namespace three

#endif // THREE_SPOT_LIGHT_HPP