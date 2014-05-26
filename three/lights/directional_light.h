#ifndef THREE_DIRECTIONAL_LIGHT_H
#define THREE_DIRECTIONAL_LIGHT_H

#include <three/common.h>
#include <three/cameras/camera.h>
#include <three/lights/light.h>

namespace three {

class DirectionalLight : public Light {

public:

  THREE_IMPL_OBJECT(DirectionalLight);

  static Ptr create( int hex, float intensity = 1, float distance = 0 ) {

    return make_shared<DirectionalLight>( hex, intensity, distance );

  }

  float shadowCameraNear;
  float shadowCameraFar;

  float shadowCameraLeft;
  float shadowCameraRight;
  float shadowCameraTop;
  float shadowCameraBottom;

  bool shadowCameraVisible;

  float shadowBias;
  float shadowDarkness;

  int shadowMapWidth;
  int shadowMapHeight;

  Vector3 shadowCascadeOffset;
  int shadowCascadeCount;

  std::array<int, 3> shadowCascadeBias;
  std::array<int, 3> shadowCascadeWidth;
  std::array<int, 3> shadowCascadeHeight;

  Vector3 shadowCascadeNearZ;
  Vector3 shadowCascadeFarZ;

  std::vector<Light::Ptr> shadowCascadeArray;

  Texture::Ptr shadowMap;
  std::array<int, 2> shadowMapSize;
  Camera::Ptr shadowCamera;
  Matrix4 shadowMatrix;

protected:

  DirectionalLight( int hex, float intensity, float distance )
    : Light( hex, intensity, distance ),
      shadowCameraNear( 50 ), shadowCameraFar( 5000 ),
      shadowCameraLeft( -500 ), shadowCameraRight( 500 ),
      shadowCameraTop( 500 ), shadowCameraBottom( -500 ),
      shadowCameraVisible( false ),
      shadowBias( 0 ),
      shadowDarkness( 0.5 ),
      shadowMapWidth( 512 ),
      shadowMapHeight( 512 ),
      shadowCascadeOffset( 0, 0, -1000 ),
      shadowCascadeCount( 2 ),
      shadowCascadeNearZ( -1.f,  0.99f, 0.998f ),
      shadowCascadeFarZ(  0.99f, 0.998f, 1.f ) {

    shadowCascadeBias.fill( 0 );
    shadowCascadeWidth.fill( 512 );
    shadowCascadeHeight.fill( 512 );

    target = Object3D::create();

    position().set( 0, 1, 0 );

  }

  virtual Object3D::Ptr __clone( Object3D::Ptr target, bool recursive ) const THREE_OVERRIDE {

    Ptr light = target ? std::static_pointer_cast<DirectionalLight>(target) : create( 0 );

    Light::__clone( light, recursive );

    light->shadowCameraNear = shadowCameraNear;
    light->shadowCameraFar = shadowCameraFar;

    light->shadowCameraLeft = shadowCameraLeft;
    light->shadowCameraRight = shadowCameraRight;
    light->shadowCameraTop = shadowCameraTop;
    light->shadowCameraBottom = shadowCameraBottom;

    light->shadowCameraVisible = shadowCameraVisible;

    light->shadowBias = shadowBias;
    light->shadowDarkness = shadowDarkness;

    light->shadowMapWidth = shadowMapWidth;
    light->shadowMapHeight = shadowMapHeight;

    light->shadowCascadeOffset = shadowCascadeOffset;
    light->shadowCascadeCount = light->shadowCascadeCount;

    light->shadowCascadeBias = shadowCascadeBias;
    light->shadowCascadeWidth = shadowCascadeWidth;
    light->shadowCascadeHeight = shadowCascadeHeight;

    light->shadowCascadeNearZ = shadowCascadeNearZ;
    light->shadowCascadeFarZ = shadowCascadeFarZ;

    light->shadowCascadeArray = shadowCascadeArray;

    light->shadowMap = shadowMap;
    light->shadowMapSize = shadowMapSize;
    light->shadowCamera = shadowCamera;
    light->shadowMatrix = shadowMatrix;

    return light;

  }

};

} // namespace three

#endif // THREE_DIRECTIONAL_LIGHT_H