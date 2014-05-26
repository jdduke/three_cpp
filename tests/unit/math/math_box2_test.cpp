#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/box2.h>

using namespace three;

TEST(math_box2_test, constructor) {
  auto a = Box2();
  EXPECT_TRUE( a.min.equals( posInf2 ) );
  EXPECT_TRUE( a.max.equals( negInf2 ) );

  a = Box2( zero2.clone(), zero2.clone() );
  EXPECT_TRUE( a.min.equals( zero2 ) );
  EXPECT_TRUE( a.max.equals( zero2 ) );

  a = Box2( zero2.clone(), one2.clone() );
  EXPECT_TRUE( a.min.equals( zero2 ) );
  EXPECT_TRUE( a.max.equals( one2 ) );
}

TEST(math_box2_test, copy) {
  auto a = Box2( zero2.clone(), one2.clone() );
  auto b = Box2().copy( a );
  EXPECT_TRUE( b.min.equals( zero2 ) );
  EXPECT_TRUE( b.max.equals( one2 ) );

  // ensure that it is a true copy
  a.min = zero2;
  a.max = one2;
  EXPECT_TRUE( b.min.equals( zero2 ) );
  EXPECT_TRUE( b.max.equals( one2 ) );
}

TEST(math_box2_test, set) {
  auto a = Box2();

  a.set( zero2, one2 );
  EXPECT_TRUE( a.min.equals( zero2 ) );
  EXPECT_TRUE( a.max.equals( one2 ) );
}

TEST(math_box2_test, empty_makeEmpty) {
  auto a = Box2();

  EXPECT_TRUE( a.empty() );

  a = Box2( zero2.clone(), one2.clone() );
  EXPECT_TRUE( ! a.empty() );

  a.makeEmpty();
  EXPECT_TRUE( a.empty() );
}

TEST(math_box2_test, center) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  EXPECT_TRUE( a.center().equals( zero2 ) );

  a = Box2( zero2, one2 );
  auto midpoint = one2.clone().multiplyScalar( 0.5 );
  EXPECT_TRUE( a.center().equals( midpoint ) );
}

TEST(math_box2_test, size) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  EXPECT_TRUE( a.size().equals( zero2 ) );

  a = Box2( zero2.clone(), one2.clone() );
  EXPECT_TRUE( a.size().equals( one2 ) );
}

TEST(math_box2_test, expandByPoint) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  a.expandByPoint( zero2 );
  EXPECT_TRUE( a.size().equals( zero2 ) );

  a.expandByPoint( one2 );
  EXPECT_TRUE( a.size().equals( one2 ) );

  a.expandByPoint( one2.clone().negate() );
  EXPECT_TRUE( a.size().equals( one2.clone().multiplyScalar( 2 ) ) );
  EXPECT_TRUE( a.center().equals( zero2 ) );
}

TEST(math_box2_test, expandByVector) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  a.expandByVector( zero2 );
  EXPECT_TRUE( a.size().equals( zero2 ) );

  a.expandByVector( one2 );
  EXPECT_TRUE( a.size().equals( one2.clone().multiplyScalar( 2 ) ) );
  EXPECT_TRUE( a.center().equals( zero2 ) );
}

TEST(math_box2_test, expandByScalar) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  a.expandByScalar( 0 );
  EXPECT_TRUE( a.size().equals( zero2 ) );

  a.expandByScalar( 1 );
  EXPECT_TRUE( a.size().equals( one2.clone().multiplyScalar( 2 ) ) );
  EXPECT_TRUE( a.center().equals( zero2 ) );
}

TEST(math_box2_test, containsPoint) {
  auto a = Box2( zero2.clone(), zero2.clone() );

  EXPECT_TRUE( a.containsPoint( zero2 ) );
  EXPECT_TRUE( ! a.containsPoint( one2 ) );

  a.expandByScalar( 1 );
  EXPECT_TRUE( a.containsPoint( zero2 ) );
  EXPECT_TRUE( a.containsPoint( one2 ) );
  EXPECT_TRUE( a.containsPoint( one2.clone().negate() ) );
}

TEST(math_box2_test, containsBox) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( zero2.clone(), one2.clone() );
  auto c = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.containsBox( a ) );
  EXPECT_TRUE( ! a.containsBox( b ) );
  EXPECT_TRUE( ! a.containsBox( c ) );

  EXPECT_TRUE( b.containsBox( a ) );
  EXPECT_TRUE( c.containsBox( a ) );
  EXPECT_TRUE( ! b.containsBox( c ) );
}

TEST(math_box2_test, getParameter) {
  auto a = Box2( zero2.clone(), one2.clone() );
  auto b = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.getParameter( Vector2( 0, 0 ) ).equals( Vector2( 0, 0 ) ) );
  EXPECT_TRUE( a.getParameter( Vector2( 1, 1 ) ).equals( Vector2( 1, 1 ) ) );

  EXPECT_TRUE( b.getParameter( Vector2( -1, -1 ) ).equals( Vector2( 0, 0 ) ) );
  EXPECT_TRUE( b.getParameter( Vector2( 0, 0 ) ).equals( Vector2( 0.5, 0.5 ) ) );
  EXPECT_TRUE( b.getParameter( Vector2( 1, 1 ) ).equals( Vector2( 1, 1 ) ) );
}

TEST(math_box2_test, clampPoint) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.clampPoint( Vector2( 0, 0 ) ).equals( Vector2( 0, 0 ) ) );
  EXPECT_TRUE( a.clampPoint( Vector2( 1, 1 ) ).equals( Vector2( 0, 0 ) ) );
  EXPECT_TRUE( a.clampPoint( Vector2( -1, -1 ) ).equals( Vector2( 0, 0 ) ) );

  EXPECT_TRUE( b.clampPoint( Vector2( 2, 2 ) ).equals( Vector2( 1, 1 ) ) );
  EXPECT_TRUE( b.clampPoint( Vector2( 1, 1 ) ).equals( Vector2( 1, 1 ) ) );
  EXPECT_TRUE( b.clampPoint( Vector2( 0, 0 ) ).equals( Vector2( 0, 0 ) ) );
  EXPECT_TRUE( b.clampPoint( Vector2( -1, -1 ) ).equals( Vector2( -1, -1 ) ) );
  EXPECT_TRUE( b.clampPoint( Vector2( -2, -2 ) ).equals( Vector2( -1, -1 ) ) );
}

TEST(math_box2_test, distanceToPoint) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.distanceToPoint( Vector2( 0, 0 ) ) == 0 );
  EXPECT_TRUE( a.distanceToPoint( Vector2( 1, 1 ) ) == Math::sqrt( 2.f ) );
  EXPECT_TRUE( a.distanceToPoint( Vector2( -1, -1 ) ) == Math::sqrt( 2.f ) );

  EXPECT_TRUE( b.distanceToPoint( Vector2( 2, 2 ) ) == Math::sqrt( 2.f ) );
  EXPECT_TRUE( b.distanceToPoint( Vector2( 1, 1 ) ) == 0 );
  EXPECT_TRUE( b.distanceToPoint( Vector2( 0, 0 ) ) == 0 );
  EXPECT_TRUE( b.distanceToPoint( Vector2( -1, -1 ) ) == 0 );
  EXPECT_TRUE( b.distanceToPoint( Vector2( -2, -2 ) ) == Math::sqrt( 2.f ) );
}

TEST(math_box2_test, isIntersectionBox) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( zero2.clone(), one2.clone() );
  auto c = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.isIntersectionBox( a ) );
  EXPECT_TRUE( a.isIntersectionBox( b ) );
  EXPECT_TRUE( a.isIntersectionBox( c ) );

  EXPECT_TRUE( b.isIntersectionBox( a ) );
  EXPECT_TRUE( c.isIntersectionBox( a ) );
  EXPECT_TRUE( b.isIntersectionBox( c ) );

  b.translate( Vector2( 2, 2 ) );
  EXPECT_TRUE( ! a.isIntersectionBox( b ) );
  EXPECT_TRUE( ! b.isIntersectionBox( a ) );
  EXPECT_TRUE( ! b.isIntersectionBox( c ) );
}

TEST(math_box2_test, intersect) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( zero2.clone(), one2.clone() );
  auto c = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.clone().intersect( a ).equals( a ) );
  EXPECT_TRUE( a.clone().intersect( b ).equals( a ) );
  EXPECT_TRUE( b.clone().intersect( b ).equals( b ) );
  EXPECT_TRUE( a.clone().intersect( c ).equals( a ) );
  EXPECT_TRUE( b.clone().intersect( c ).equals( b ) );
  EXPECT_TRUE( c.clone().intersect( c ).equals( c ) );
}

TEST(math_box2_test, unionFunc) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( zero2.clone(), one2.clone() );
  auto c = Box2( one2.clone().negate(), one2.clone() );

  EXPECT_TRUE( a.clone().unionBox( a ).equals( a ) );
  EXPECT_TRUE( a.clone().unionBox( b ).equals( b ) );
  EXPECT_TRUE( a.clone().unionBox( c ).equals( c ) );
  EXPECT_TRUE( b.clone().unionBox( c ).equals( c ) );
}

TEST(math_box2_test, translate) {
  auto a = Box2( zero2.clone(), zero2.clone() );
  auto b = Box2( zero2.clone(), one2.clone() );
  //auto c = Box2( one2.clone().negate(), one2.clone() );
  auto d = Box2( one2.clone().negate(), zero2.clone() );

  EXPECT_TRUE( a.clone().translate( one2 ).equals( Box2( one2, one2 ) ) );
  EXPECT_TRUE( a.clone().translate( one2 ).translate( one2.clone().negate() ).equals( a ) );
  EXPECT_TRUE( d.clone().translate( one2 ).equals( b ) );
  EXPECT_TRUE( b.clone().translate( one2.clone().negate() ).equals( d ) );
}