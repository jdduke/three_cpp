#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/euler.h>
#include <three/math/impl/euler.cpp>
#include <three/math/matrix4.h>

using namespace three;

auto eulerZero = Euler( 0, 0, 0, enums::EulerRotationOrder::XYZ );
auto eulerAxyz = Euler( 1, 0, 0, enums::EulerRotationOrder::XYZ );
auto eulerAzyx = Euler( 0, 1, 0, enums::EulerRotationOrder::ZYX );

TEST(math_euler_test, constructor_equals) {
  auto a = Euler();
  EXPECT_TRUE( a.equals( eulerZero ) );
  EXPECT_TRUE( ! a.equals( eulerAxyz ) );
  EXPECT_TRUE( ! a.equals( eulerAzyx ) );

  a = Euler(x, y, z);
  EXPECT_EQ( a.x, x );
  EXPECT_EQ( a.y, y );
  EXPECT_EQ( a.z, z );

  auto b = Euler(x, y, z);
  EXPECT_EQ( b.x, x );
  EXPECT_EQ( b.y, y );
  EXPECT_EQ( b.z, z );
}

TEST(math_euler_test, clone_copy_equals) {
  auto a = eulerAxyz.clone();
  EXPECT_TRUE( a.equals( eulerAxyz ) );
  EXPECT_TRUE( ! a.equals( eulerZero ) );
  EXPECT_TRUE( ! a.equals( eulerAzyx ) );

  a.copy( eulerAzyx );
  EXPECT_TRUE( a.equals( eulerAzyx ) );
  EXPECT_TRUE( ! a.equals( eulerAxyz ) );
  EXPECT_TRUE( ! a.equals( eulerZero ) );

}

TEST(math_euler_test, set) {
  auto a = Euler();

  a.set( 0, 1, 0, enums::EulerRotationOrder::ZYX );
  EXPECT_TRUE( a.equals( eulerAzyx ) );
  EXPECT_TRUE( ! a.equals( eulerAxyz ) );
  EXPECT_TRUE( ! a.equals( eulerZero ) );
}

TEST(math_euler_test, Quaternion_setFromEuler_Euler_fromQuaternion) {
    std::array<Euler, 3> testValues = { eulerZero, eulerAxyz, eulerAzyx };
  for( auto i = 0; i < testValues.size(); i ++ ) {
    auto v = testValues[i];
    auto q = Quaternion().setFromEuler( v );

    auto v2 = Euler().setFromQuaternion( q, v.order() );
    auto q2 = Quaternion().setFromEuler( v2 );
    EXPECT_TRUE( q.equals( q2 ) );
  }
}


TEST(math_euler_test, Matrix4_setFromEuler_Euler_fromRotationMatrix) {
    std::array<Euler, 3> testValues = { eulerZero, eulerAxyz, eulerAzyx };
  for( auto i = 0; i < testValues.size(); i ++ ) {
    auto v = testValues[i];
    auto m = Matrix4().makeRotationFromEuler( v );

    auto v2 = Euler().setFromRotationMatrix( m, v.order() );
    auto m2 = Matrix4().makeRotationFromEuler( v2 );
    EXPECT_TRUE( matrixEquals4( m, m2, 0.0001 ) );
  }
}

TEST(math_euler_test, reorder) {
    std::array<Euler, 3> testValues = { eulerZero, eulerAxyz, eulerAzyx };
  for( auto i = 0; i < testValues.size(); i ++ ) {
    auto v = testValues[i];
    auto q = Quaternion().setFromEuler( v );

    v.reorder( enums::EulerRotationOrder::YZX );
    auto q2 = Quaternion().setFromEuler( v );
    EXPECT_TRUE( q.equals( q2 ) );

    v.reorder( enums::EulerRotationOrder::ZXY );
    auto q3 = Quaternion().setFromEuler( v );
    EXPECT_TRUE( q.equals( q3 ) );
  }
}

