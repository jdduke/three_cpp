#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/box3.h>
#include <three/math/vector2.h>
#include <three/math/matrix4.h>

using namespace three;

bool compareBox( const Box3& a, const Box3& b, float threshold = 0.0001 ) {
  return ( a.min.distanceTo( b.min ) < threshold && a.max.distanceTo( b.max ) < threshold );
};

TEST(math_box3_test, constructor) {
  auto a = Box3();
  EXPECT_TRUE( a.min.equals( posInf3 ));
  EXPECT_TRUE( a.max.equals( negInf3 ));

  a = Box3( zero3.clone(), zero3.clone() );
  EXPECT_TRUE( a.min.equals( zero3 ));
  EXPECT_TRUE( a.max.equals( zero3 ));

  a = Box3( zero3.clone(), one3.clone() );
  EXPECT_TRUE( a.min.equals( zero3 ));
  EXPECT_TRUE( a.max.equals( one3 ));
}

TEST(math_box3_test, copy) {
  auto a = Box3( zero3.clone(), one3.clone() );
  auto b = Box3().copy( a );
  EXPECT_TRUE( b.min.equals( zero3 ));
  EXPECT_TRUE( b.max.equals( one3 ));

  // ensure that it is a true copy
  a.min = zero3;
  a.max = one3;
  EXPECT_TRUE( b.min.equals( zero3 ));
  EXPECT_TRUE( b.max.equals( one3 ));
}

TEST(math_box3_test, set) {
  auto a = Box3();

  a.set( zero3, one3 );
  EXPECT_TRUE( a.min.equals( zero3 ));
  EXPECT_TRUE( a.max.equals( one3 ));
}

TEST(math_box3_test, empty_makeEmpty) {
  auto a = Box3();

  EXPECT_TRUE( a.empty());

  a = Box3( zero3.clone(), one3.clone() );
  EXPECT_TRUE( ! a.empty());

  a.makeEmpty();
  EXPECT_TRUE( a.empty());
}

TEST(math_box3_test, center) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  EXPECT_TRUE( a.center().equals( zero3 ));

  a = Box3( zero3.clone(), one3.clone() );
  auto midpoint = one3.clone().multiplyScalar( 0.5 );
  EXPECT_TRUE( a.center().equals( midpoint ));
}

TEST(math_box3_test, size) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  EXPECT_TRUE( a.size().equals( zero3 ));

  a = Box3( zero3.clone(), one3.clone() );
  EXPECT_TRUE( a.size().equals( one3 ));
}

TEST(math_box3_test, expandByPoint) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  a.expandByPoint( zero3 );
  EXPECT_TRUE( a.size().equals( zero3 ));

  a.expandByPoint( one3 );
  EXPECT_TRUE( a.size().equals( one3 ));

  a.expandByPoint( one3.clone().negate() );
  EXPECT_TRUE( a.size().equals( one3.clone().multiplyScalar( 2 ) ));
  EXPECT_TRUE( a.center().equals( zero3 ));
}

TEST(math_box3_test, expandByVector) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  a.expandByVector( zero3 );
  EXPECT_TRUE( a.size().equals( zero3 ));

  a.expandByVector( one3 );
  EXPECT_TRUE( a.size().equals( one3.clone().multiplyScalar( 2 ) ));
  EXPECT_TRUE( a.center().equals( zero3 ));
}

TEST(math_box3_test, expandByScalar) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  a.expandByScalar( 0 );
  EXPECT_TRUE( a.size().equals( zero3 ));

  a.expandByScalar( 1 );
  EXPECT_TRUE( a.size().equals( one3.clone().multiplyScalar( 2 ) ));
  EXPECT_TRUE( a.center().equals( zero3 ));
}

TEST(math_box3_test, containsPoint) {
  auto a = Box3( zero3.clone(), zero3.clone() );

  EXPECT_TRUE( a.containsPoint( zero3 ));
  EXPECT_TRUE( ! a.containsPoint( one3 ));

  a.expandByScalar( 1 );
  EXPECT_TRUE( a.containsPoint( zero3 ));
  EXPECT_TRUE( a.containsPoint( one3 ));
  EXPECT_TRUE( a.containsPoint( one3.clone().negate() ));
}

TEST(math_box3_test, containsBox) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.containsBox( a ));
  EXPECT_TRUE( ! a.containsBox( b ));
  EXPECT_TRUE( ! a.containsBox( c ));

  EXPECT_TRUE( b.containsBox( a ));
  EXPECT_TRUE( c.containsBox( a ));
  EXPECT_TRUE( ! b.containsBox( c ));
}

TEST(math_box3_test, getParameter) {
  auto a = Box3( zero3.clone(), one3.clone() );
  auto b = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.getParameter( Vector3( 0, 0, 0 ) )->equals( Vector3( 0, 0, 0 ) ));
  EXPECT_TRUE( a.getParameter( Vector3( 1, 1, 1 ) )->equals( Vector3( 1, 1, 1 ) ));

  EXPECT_TRUE( b.getParameter( Vector3( -1, -1, -1 ) )->equals( Vector3( 0, 0, 0 ) ));
  EXPECT_TRUE( b.getParameter( Vector3( 0, 0, 0 ) )->equals( Vector3( 0.5, 0.5, 0.5 ) ));
  EXPECT_TRUE( b.getParameter( Vector3( 1, 1, 1 ) )->equals( Vector3( 1, 1, 1 ) ));
}

TEST(math_box3_test, clampPoint) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.clampPoint( Vector3( 0, 0, 0 ) ).equals( Vector3( 0, 0, 0 ) ));
  EXPECT_TRUE( a.clampPoint( Vector3( 1, 1, 1 ) ).equals( Vector3( 0, 0, 0 ) ));
  EXPECT_TRUE( a.clampPoint( Vector3( -1, -1, -1 ) ).equals( Vector3( 0, 0, 0 ) ));

  EXPECT_TRUE( b.clampPoint( Vector3( 2, 2, 2 ) ).equals( Vector3( 1, 1, 1 ) ));
  EXPECT_TRUE( b.clampPoint( Vector3( 1, 1, 1 ) ).equals( Vector3( 1, 1, 1 ) ));
  EXPECT_TRUE( b.clampPoint( Vector3( 0, 0, 0 ) ).equals( Vector3( 0, 0, 0 ) ));
  EXPECT_TRUE( b.clampPoint( Vector3( -1, -1, -1 ) ).equals( Vector3( -1, -1, -1 ) ));
  EXPECT_TRUE( b.clampPoint( Vector3( -2, -2, -2 ) ).equals( Vector3( -1, -1, -1 ) ));
}

TEST(math_box3_test, distanceToPoint) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.distanceToPoint( Vector3( 0, 0, 0 ) ) == 0);
  EXPECT_TRUE( a.distanceToPoint( Vector3( 1, 1, 1 ) ) == Math::sqrt( 3.f ));
  EXPECT_TRUE( a.distanceToPoint( Vector3( -1, -1, -1 ) ) == Math::sqrt( 3.f ));

  EXPECT_TRUE( b.distanceToPoint( Vector3( 2, 2, 2 ) ) == Math::sqrt( 3.f ));
  EXPECT_TRUE( b.distanceToPoint( Vector3( 1, 1, 1 ) ) == 0);
  EXPECT_TRUE( b.distanceToPoint( Vector3( 0, 0, 0 ) ) == 0);
  EXPECT_TRUE( b.distanceToPoint( Vector3( -1, -1, -1 ) ) == 0);
  EXPECT_TRUE( b.distanceToPoint( Vector3( -2, -2, -2 ) ) == Math::sqrt( 3.f ));
}

TEST(math_box3_test, isIntersectionBox) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.isIntersectionBox( a ));
  EXPECT_TRUE( a.isIntersectionBox( b ));
  EXPECT_TRUE( a.isIntersectionBox( c ));

  EXPECT_TRUE( b.isIntersectionBox( a ));
  EXPECT_TRUE( c.isIntersectionBox( a ));
  EXPECT_TRUE( b.isIntersectionBox( c ));

  b.translate( Vector3( 2, 2, 2 ) );
  EXPECT_TRUE( ! a.isIntersectionBox( b ));
  EXPECT_TRUE( ! b.isIntersectionBox( a ));
  EXPECT_TRUE( ! b.isIntersectionBox( c ));
}

TEST(math_box3_test, getBoundingSphere) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.getBoundingSphere().equals( Sphere( zero3, 0.f ) ));
  EXPECT_TRUE( b.getBoundingSphere().equals( Sphere( one3.clone().multiplyScalar( 0.5f ), Math::sqrt( 3.f ) * 0.5f ) ));
  EXPECT_TRUE( c.getBoundingSphere().equals( Sphere( zero3, Math::sqrt( 12.f ) * 0.5f ) ));
}

TEST(math_box3_test, intersect) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.clone().intersect( a ).equals( a ));
  EXPECT_TRUE( a.clone().intersect( b ).equals( a ));
  EXPECT_TRUE( b.clone().intersect( b ).equals( b ));
  EXPECT_TRUE( a.clone().intersect( c ).equals( a ));
  EXPECT_TRUE( b.clone().intersect( c ).equals( b ));
  EXPECT_TRUE( c.clone().intersect( c ).equals( c ));
}

TEST(math_box3_test, unionBox) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );

  EXPECT_TRUE( a.clone().unionBox( a ).equals( a ));
  EXPECT_TRUE( a.clone().unionBox( b ).equals( b ));
  EXPECT_TRUE( a.clone().unionBox( c ).equals( c ));
  EXPECT_TRUE( b.clone().unionBox( c ).equals( c ));
}

TEST(math_box3_test, applyMatrix4test) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  auto c = Box3( one3.clone().negate(), one3.clone() );
  auto d = Box3( one3.clone().negate(), zero3.clone() );

  auto m = Matrix4().makeTranslation( 1, -2, 1 );
  auto t1 = Vector3( 1, -2, 1 );

   auto aresult = compareBox( a.clone().applyMatrix4( m ), a.clone().translate( t1 ) );
  EXPECT_TRUE( aresult );

  auto bresult = compareBox( b.clone().applyMatrix4( m ), b.clone().translate( t1 ) );
  EXPECT_TRUE( bresult );

   auto cresult = compareBox( c.clone().applyMatrix4( m ), c.clone().translate( t1 ) );
  EXPECT_TRUE( cresult);

  auto dresult = compareBox( d.clone().applyMatrix4( m ), d.clone().translate( t1 ) );
  EXPECT_TRUE( dresult );
}

TEST(math_box3_test, translate) {
  auto a = Box3( zero3.clone(), zero3.clone() );
  auto b = Box3( zero3.clone(), one3.clone() );
  //auto c = Box3( one3.clone().negate(), one3.clone() );
  auto d = Box3( one3.clone().negate(), zero3.clone() );

  EXPECT_TRUE( a.clone().translate( one3 ).equals( Box3( one3, one3 ) ));
  EXPECT_TRUE( a.clone().translate( one3 ).translate( one3.clone().negate() ).equals( a ));
  EXPECT_TRUE( d.clone().translate( one3 ).equals( b ));
  EXPECT_TRUE( b.clone().translate( one3.clone().negate() ).equals( d ));
}