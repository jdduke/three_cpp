
#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/quaternion.h>
#include <three/math/euler.h>
#include <three/math/matrix4.h>
#include <three/constants.h>
#include <array>

using namespace three;

std::array<enums::EulerRotationOrder, 6> orders = {
    enums::EulerRotationOrder::XYZ,
    enums::EulerRotationOrder::YXZ,
    enums::EulerRotationOrder::ZXY,
    enums::EulerRotationOrder::ZYX,
    enums::EulerRotationOrder::YZX,
    enums::EulerRotationOrder::XZY
};

auto eulerAngles = Euler( 0.1, -0.3, 0.25 );

Quaternion qSub( const Quaternion& a, const Quaternion& b ) {
  auto result = Quaternion();
  result.copy( a );

  result.x = result.x - b.x;
  result.y = result.y - b.y;
  result.z = result.z - b.z;
  result.w = result.w - b.w;

  return result;

}

TEST(math_quaternion_test, constructor) {
  auto a = Quaternion();
  EXPECT_EQ( a.x, 0 );
  EXPECT_EQ( a.y, 0 );
  EXPECT_EQ( a.z, 0 );
  EXPECT_EQ( a.w, 1 );

  a = Quaternion( x, y, z, w );
  EXPECT_EQ( a.x, x );
  EXPECT_EQ( a.y, y );
  EXPECT_EQ( a.z, z );
  EXPECT_EQ( a.w, w );
}

TEST(math_quaternion_test, copy) {
  auto a = Quaternion( x, y, z, w );
  auto b = Quaternion().copy( a );
  EXPECT_EQ( b.x, x );
  EXPECT_EQ( b.y, y );
  EXPECT_EQ( b.z, z );
  EXPECT_EQ( b.w, w );

  // ensure that it is a true copy
  a.x = 0;
  a.y = -1;
  a.z = 0;
  a.w = -1;
  EXPECT_EQ( b.x, x );
  EXPECT_EQ( b.y, y );
}

TEST(math_quaternion_test, set) {
  auto a = Quaternion();
  EXPECT_EQ( a.x, 0 );
  EXPECT_EQ( a.y, 0 );
  EXPECT_EQ( a.z, 0 );
  EXPECT_EQ( a.w, 1 );

  a.set( x, y, z, w );
  EXPECT_EQ( a.x, x );
  EXPECT_EQ( a.y, y );
  EXPECT_EQ( a.z, z );
  EXPECT_EQ( a.w, w );
}

TEST(math_quaternion_test, setFromAxisAngle) {

  // TODO: find cases to validate.
  EXPECT_TRUE( true );

  auto zero = Quaternion();

  auto a = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), 0 );
  EXPECT_TRUE( a.equals( zero ) );
  a = Quaternion().setFromAxisAngle( Vector3( 0, 1, 0 ), 0 );
  EXPECT_TRUE( a.equals( zero ) );
  a = Quaternion().setFromAxisAngle( Vector3( 0, 0, 1 ), 0 );
  EXPECT_TRUE( a.equals( zero ) );

  auto b1 = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), Math::PI() );
  EXPECT_TRUE( ! a.equals( b1 ) );
  auto b2 = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), -Math::PI() );
  EXPECT_TRUE( ! a.equals( b2 ) );

  b1.multiply( b2 );
  EXPECT_TRUE( a.equals( b1 ) );
}


TEST(math_quaternion_test, setFromEuler_setFromQuaternion) {

  Vector3 angles[] = {
    Vector3( 1, 0, 0 ),
    Vector3( 0, 1, 0 ),
    Vector3( 0, 0, 1 )
  };

  // ensure euler conversion to/from Quaternion matches.
  for( const auto& order : orders ) {
    for( const auto& angle : angles ) {
      auto eulers2 = Euler().setFromQuaternion( Quaternion().setFromEuler( Euler( angle.x, angle.y, angle.z, order ) ), order );
      auto newAngle = Vector3( eulers2.x, eulers2.y, eulers2.z );
      EXPECT_LT( newAngle.distanceTo( angle ), 0.001 );
    }
  }

}

TEST(math_quaternion_test, setFromEuler_setFromRotationMatrix) {

  // ensure euler conversion for Quaternion matches that of Matrix4
  THREE_REVIEW("We don't use the order here?");
  for( const auto& order : orders ) {
    auto q = Quaternion().setFromEuler( eulerAngles ); // EA: 2nd param removed
    auto m = Matrix4().makeRotationFromEuler( eulerAngles ); // EA: 2nd param removed
    auto q2 = Quaternion().setFromRotationMatrix( m );

    EXPECT_LT( qSub( q, q2 ).length(), 0.001 );
  }

}

TEST(math_quaternion_test, normalize_length_lengthSq) {
  auto a = Quaternion( x, y, z, w );
  //auto b = Quaternion( -x, -y, -z, -w );

  EXPECT_NE( a.length(), 1);
  EXPECT_NE( a.lengthSq(), 1);
  a.normalize();
  EXPECT_EQ( a.length(), 1);
  EXPECT_EQ( a.lengthSq(), 1);

  a.set( 0, 0, 0, 0 );
  EXPECT_EQ( a.lengthSq(), 0);
  EXPECT_EQ( a.length(), 0);
  a.normalize();
  EXPECT_EQ( a.lengthSq(), 1);
  EXPECT_EQ( a.length(), 1);
}

TEST(math_quaternion_test, inverse_conjugate) {
  auto a = Quaternion( x, y, z, w );

  // TODO: add better validation here.

  auto b = a.clone().conjugate();

  EXPECT_EQ( a.x, -b.x );
  EXPECT_EQ( a.y, -b.y );
  EXPECT_EQ( a.z, -b.z );
  EXPECT_EQ( a.w, b.w );
}


TEST(math_quaternion_test, multiplyQuaternions_multiply) {

  Euler angles[] = {
    Euler( 1, 0, 0 ),
    Euler( 0, 1, 0 ),
    Euler( 0, 0, 1 )
  };

  auto q1 = Quaternion().setFromEuler( angles[0] );
  auto q2 = Quaternion().setFromEuler( angles[1] );
  auto q3 = Quaternion().setFromEuler( angles[2] );

  auto q = Quaternion().multiplyQuaternions( q1, q2 ).multiply( q3 );

  auto m1 = Matrix4().makeRotationFromEuler( angles[0] );
  auto m2 = Matrix4().makeRotationFromEuler( angles[1] );
  auto m3 = Matrix4().makeRotationFromEuler( angles[2] );

  auto m = Matrix4().multiplyMatrices( m1, m2 ).multiply( m3 );

  auto qFromM = Quaternion().setFromRotationMatrix( m );

  EXPECT_LT( qSub( q, qFromM ).length(), 0.001 );
}

TEST(math_quaternion_test, multiplyVector3) {

  Euler angles[] = {
    Euler( 1, 0, 0 ),
    Euler( 0, 1, 0 ),
    Euler( 0, 0, 1 )
  };

  // ensure euler conversion for Quaternion matches that of Matrix4
  THREE_REVIEW("We don't use the order here?");
  for( const auto& order : orders ) {
    for( const auto& angle : angles ) {
      auto q = Quaternion().setFromEuler( angle );
      auto m = Matrix4().makeRotationFromEuler( angle );

      auto v0 = Vector3(1, 0, 0);
      auto qv = v0.clone().applyQuaternion( q );
      auto mv = v0.clone().applyMatrix4( m );

      EXPECT_LT( qv.distanceTo( mv ), 0.001 );
    }
  }

}

TEST(math_quaternion_test, equals) {
  auto a = Quaternion( x, y, z, w );
  auto b = Quaternion( -x, -y, -z, -w );

  EXPECT_TRUE( a.x != b.x );
  EXPECT_TRUE( a.y != b.y );

  EXPECT_TRUE( ! a.equals( b ) );
  EXPECT_TRUE( ! b.equals( a ) );

  a.copy( b );
  EXPECT_EQ( a.x, b.x );
  EXPECT_EQ( a.y, b.y );

  EXPECT_TRUE( a.equals( b ) );
  EXPECT_TRUE( b.equals( a ) );
}

