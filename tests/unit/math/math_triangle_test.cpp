#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/triangle.h>

using namespace three;

TEST(math_triangle_test, constructor) {
  auto a = Triangle();
  EXPECT_TRUE( a.a.equals( zero3 ) );
  EXPECT_TRUE( a.b.equals( zero3 ) );
  EXPECT_TRUE( a.c.equals( zero3 ) );

  a = Triangle( one3.clone().negate(), one3.clone(), two3.clone() );
  EXPECT_TRUE( a.a.equals( one3.clone().negate() ) );
  EXPECT_TRUE( a.b.equals( one3 ) );
  EXPECT_TRUE( a.c.equals( two3 ) );
}

TEST(math_triangle_test, copy) {
  auto a = Triangle( one3.clone().negate(), one3.clone(), two3.clone() );
  auto b = Triangle().copy( a );
  EXPECT_TRUE( b.a.equals( one3.clone().negate() ) );
  EXPECT_TRUE( b.b.equals( one3 ) );
  EXPECT_TRUE( b.c.equals( two3 ) );

  // ensure that it is a true copy
  a.a = one3;
  a.b = zero3;
  a.c = zero3;
  EXPECT_TRUE( b.a.equals( one3.clone().negate() ) );
  EXPECT_TRUE( b.b.equals( one3 ) );
  EXPECT_TRUE( b.c.equals( two3 ) );
}

TEST(math_triangle_test, setFromPointsAndIndices) {
  auto a = Triangle();

  std::array<Vector3, 3> points = { one3, one3.clone().negate(), two3 };
  a.setFromPointsAndIndices( points, 1, 0, 2 );
  EXPECT_TRUE( a.a.equals( one3.clone().negate() ) );
  EXPECT_TRUE( a.b.equals( one3 ) );
  EXPECT_TRUE( a.c.equals( two3 ) );

}

TEST(math_triangle_test, set) {
  auto a = Triangle();

  a.set( one3.clone().negate(), one3, two3 );
  EXPECT_TRUE( a.a.equals( one3.clone().negate() ) );
  EXPECT_TRUE( a.b.equals( one3 ) );
  EXPECT_TRUE( a.c.equals( two3 ) );

}

TEST(math_triangle_test, area) {
  auto a = Triangle();

  EXPECT_TRUE( a.area() == 0.f );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.area() == 0.5 );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.area() == 2.f );

  // colinear triangle.
  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 3, 0, 0 ) );
  EXPECT_TRUE( a.area() == 0.f );
}

TEST(math_triangle_test, midpoint) {
  auto a = Triangle();

  EXPECT_TRUE( a.midpoint().equals( Vector3( 0, 0, 0 ) ) );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.midpoint().equals( Vector3( 1.f/3, 1.f/3, 0 ) ) );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.midpoint().equals( Vector3( 2.f/3, 0, 2.f/3 ) ) );
}

TEST(math_triangle_test, normal) {
  auto a = Triangle();

  EXPECT_TRUE( a.normal().equals( Vector3( 0, 0, 0 ) ) );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.normal().equals( Vector3( 0, 0, 1 ) ) );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.normal().equals( Vector3( 0, 1, 0 ) ) );
}

TEST(math_triangle_test, plane) {
  auto a = Triangle();

  // artificial normal is created in this case.
  EXPECT_TRUE( a.plane().distanceToPoint( a.a ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.b ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.c ) == 0 );
  EXPECT_TRUE( a.plane().normal.equals( a.normal() ) );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.plane().distanceToPoint( a.a ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.b ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.c ) == 0 );
  EXPECT_TRUE( a.plane().normal.equals( a.normal() ) );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.plane().distanceToPoint( a.a ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.b ) == 0 );
  EXPECT_TRUE( a.plane().distanceToPoint( a.c ) == 0 );
  EXPECT_TRUE( a.plane().normal.clone().normalize().equals( a.normal() ) );
}

TEST(math_triangle_test, barycoordFromPoint) {
  auto a = Triangle();

  auto bad = Vector3( -2, -1, -1 );

  EXPECT_TRUE( a.barycoordFromPoint( a.a ).equals( bad ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.b ).equals( bad ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.c ).equals( bad ) );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.a ).equals( Vector3( 1, 0, 0 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.b ).equals( Vector3( 0, 1, 0 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.c ).equals( Vector3( 0, 0, 1 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.midpoint() ).distanceTo( Vector3( 1.f/3, 1.f/3, 1.f/3 ) ) < 0.0001 );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.a ).equals( Vector3( 1, 0, 0 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.b ).equals( Vector3( 0, 1, 0 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.c ).equals( Vector3( 0, 0, 1 ) ) );
  EXPECT_TRUE( a.barycoordFromPoint( a.midpoint() ).distanceTo( Vector3( 1.f/3, 1.f/3, 1.f/3 ) ) < 0.0001 );
}

TEST(math_triangle_test, containsPoint) {
  auto a = Triangle();

  EXPECT_TRUE( ! a.containsPoint( a.a ) );
  EXPECT_TRUE( ! a.containsPoint( a.b ) );
  EXPECT_TRUE( ! a.containsPoint( a.c ) );

  a = Triangle( Vector3( 0, 0, 0 ), Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ) );
  EXPECT_TRUE( a.containsPoint( a.a ) );
  EXPECT_TRUE( a.containsPoint( a.b ) );
  EXPECT_TRUE( a.containsPoint( a.c ) );
  EXPECT_TRUE( a.containsPoint( a.midpoint() ) );
  EXPECT_TRUE( ! a.containsPoint( Vector3( -1, -1, -1 ) ) );

  a = Triangle( Vector3( 2, 0, 0 ), Vector3( 0, 0, 0 ), Vector3( 0, 0, 2 ) );
  EXPECT_TRUE( a.containsPoint( a.a ) );
  EXPECT_TRUE( a.containsPoint( a.b ) );
  EXPECT_TRUE( a.containsPoint( a.c ) );
  EXPECT_TRUE( a.containsPoint( a.midpoint() ) );
  EXPECT_TRUE( ! a.containsPoint( Vector3( -1, -1, -1 ) ) );
}