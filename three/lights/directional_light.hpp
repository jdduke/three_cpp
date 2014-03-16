#ifndef THREE_DIRECTIONAL_LIGHT_HPP
#define THREE_DIRECTIONAL_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class DirectionalLight : public Light {
public:

  typedef std::shared_ptr<DirectionalLight> Ptr;

  static Ptr create( int hex, float intensity = 1, float distance = 0 ) {
    return make_shared<DirectionalLight>( hex, intensity, distance );
  }

  virtual enums::Type type() const { return enums::DirectionalLight; }

  /////////////////////////////////////////////////////////////////////////

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

  //

  Texture::Ptr shadowMap;
  std::array<int, 2> shadowMapSize;
  Camera::Ptr shadowCamera;
  Matrix4 shadowMatrix;

  /////////////////////////////////////////////////////////////////////////

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

    position.set( 0, 1, 0 );

  }

  virtual void visit( Visitor& v ) { v( *this ); }
  virtual void visit( ConstVisitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_DIRECTIONAL_LIGHT_HPP