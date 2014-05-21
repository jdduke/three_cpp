#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/plane.h>
#include <three/math/impl/plane.cpp>
#include <three/math/matrix3.h>
#include <three/math/impl/matrix3.cpp>

using namespace three;

bool comparePlane( const Plane& a, const Plane& b, float threshold = 0.0001 ) {
	return ( a.normal.distanceTo( b.normal ) < threshold &&
	Math::abs( a.constant - b.constant ) < threshold );
}

TEST(math_plane_test, constructor) {
	auto a = Plane();
	EXPECT_TRUE( a.normal.x == 1 );
	EXPECT_TRUE( a.normal.y == 0 );
	EXPECT_TRUE( a.normal.z == 0 );
	EXPECT_TRUE( a.constant == 0 );

	a = Plane( one3.clone(), 0 );
	EXPECT_TRUE( a.normal.x == 1 );
	EXPECT_TRUE( a.normal.y == 1 );
	EXPECT_TRUE( a.normal.z == 1 );
	EXPECT_TRUE( a.constant == 0 );

	a = Plane( one3.clone(), 1 );
	EXPECT_TRUE( a.normal.x == 1 );
	EXPECT_TRUE( a.normal.y == 1 );
	EXPECT_TRUE( a.normal.z == 1 );
	EXPECT_TRUE( a.constant == 1 );
}

TEST(math_plane_test, copy) {
	auto a = Plane( Vector3( x, y, z ), w );
	auto b = Plane().copy( a );
	EXPECT_TRUE( b.normal.x == x );
	EXPECT_TRUE( b.normal.y == y );
	EXPECT_TRUE( b.normal.z == z );
	EXPECT_TRUE( b.constant == w );

	// ensure that it is a true copy
	a.normal.x = 0;
	a.normal.y = -1;
	a.normal.z = -2;
	a.constant = -3;
	EXPECT_TRUE( b.normal.x == x );
	EXPECT_TRUE( b.normal.y == y );
	EXPECT_TRUE( b.normal.z == z );
	EXPECT_TRUE( b.constant == w );
}

TEST(math_plane_test, set) {
	auto a = Plane();
	EXPECT_TRUE( a.normal.x == 1 );
	EXPECT_TRUE( a.normal.y == 0 );
	EXPECT_TRUE( a.normal.z == 0 );
	EXPECT_TRUE( a.constant == 0 );

	auto b = a.clone().set( Vector3( x, y, z ), w );
	EXPECT_TRUE( b.normal.x == x );
	EXPECT_TRUE( b.normal.y == y );
	EXPECT_TRUE( b.normal.z == z );
	EXPECT_TRUE( b.constant == w );
}

TEST(math_plane_test, setComponents) {
	auto a = Plane();
	EXPECT_TRUE( a.normal.x == 1 );
	EXPECT_TRUE( a.normal.y == 0 );
	EXPECT_TRUE( a.normal.z == 0 );
	EXPECT_TRUE( a.constant == 0 );

	auto b = a.clone().setComponents( x, y, z , w );
	EXPECT_TRUE( b.normal.x == x );
	EXPECT_TRUE( b.normal.y == y );
	EXPECT_TRUE( b.normal.z == z );
	EXPECT_TRUE( b.constant == w );
}

TEST(math_plane_test, setFromNormalAndCoplanarPoint) {
	auto normal = one3.clone().normalize();
	auto a = Plane().setFromNormalAndCoplanarPoint( normal, zero3 );
	
	EXPECT_TRUE( a.normal.equals( normal ) );
	EXPECT_TRUE( a.constant == 0 );
}

TEST(math_plane_test, normalize) {
	auto a = Plane( Vector3( 2, 0, 0 ), 2 );
	
	a.normalize();
	EXPECT_TRUE( a.normal.length() == 1 );
	EXPECT_TRUE( a.normal.equals( Vector3( 1, 0, 0 ) ) );
	EXPECT_TRUE( a.constant == 1 );
}

TEST(math_plane_test, negate_distanceToPoint) {
	auto a = Plane( Vector3( 2, 0, 0 ), -2 );
	
	a.normalize();
	EXPECT_TRUE( a.distanceToPoint( Vector3( 4, 0, 0 ) ) == 3 );
	EXPECT_TRUE( a.distanceToPoint( Vector3( 1, 0, 0 ) ) == 0 );

	a.negate();
	EXPECT_TRUE( a.distanceToPoint( Vector3( 4, 0, 0 ) ) == -3 );
	EXPECT_TRUE( a.distanceToPoint( Vector3( 1, 0, 0 ) ) == 0 );
}

TEST(math_plane_test, distanceToPoint) {
	auto a = Plane( Vector3( 2, 0, 0 ), -2 );
	
	a.normalize();
	EXPECT_TRUE( a.distanceToPoint( a.projectPoint( zero3.clone() ) ) == 0 );
	EXPECT_TRUE( a.distanceToPoint( Vector3( 4, 0, 0 ) ) == 3 );
}

TEST(math_plane_test, distanceToSphere) {
	auto a = Plane( Vector3( 1, 0, 0 ), 0 );

	auto b = Sphere( Vector3( 2, 0, 0 ), 1 );
	
	EXPECT_TRUE( a.distanceToSphere( b ) == 1 );

	a.set( Vector3( 1, 0, 0 ), 2 );
	EXPECT_TRUE( a.distanceToSphere( b ) == 3 );
	a.set( Vector3( 1, 0, 0 ), -2 );
	EXPECT_TRUE( a.distanceToSphere( b ) == -1 );
}

//TEST(math_plane_test, isIntersectionLine) {
//	auto a = Plane( Vector3( 1, 0, 0 ), 0 );
//
//	auto l1 = Line3( Vector3( -10, 0, 0 ), Vector3( 10, 0, 0 ) );
//	EXPECT_TRUE( a.isIntersectionLine( l1 ) );
//	EXPECT_TRUE( a.intersectLine( l1 )->equals( Vector3( 0, 0, 0 ) ) );
//
//	a = Plane( Vector3( 1, 0, 0 ), -3 );
//
//	EXPECT_TRUE( a.isIntersectionLine( l1 ) );
//	EXPECT_TRUE( a.intersectLine( l1 )->equals( Vector3( 3, 0, 0 ) ) );
//
//
//	a = Plane( Vector3( 1, 0, 0 ), -11 );
//
//	EXPECT_TRUE( ! a.isIntersectionLine( l1 ) );
//	EXPECT_TRUE( a.intersectLine( l1 ) == 0 );
//	
//	a = Plane( Vector3( 1, 0, 0 ), 11 );
//
//	EXPECT_TRUE( ! a.isIntersectionLine( l1 ) );
//	EXPECT_TRUE( a.intersectLine( l1 ) == 0 );
//
//}

TEST(math_plane_test, projectPoint) {
	auto a = Plane( Vector3( 1, 0, 0 ), 0 );

	EXPECT_TRUE( a.projectPoint( Vector3( 10, 0, 0 ) ).equals( zero3 ) );
	EXPECT_TRUE( a.projectPoint( Vector3( -10, 0, 0 ) ).equals( zero3 ) );

	a = Plane( Vector3( 0, 1, 0 ), -1 );
	EXPECT_TRUE( a.projectPoint( Vector3( 0, 0, 0 ) ).equals( Vector3( 0, 1, 0 ) ) );
	EXPECT_TRUE( a.projectPoint( Vector3( 0, 1, 0 ) ).equals( Vector3( 0, 1, 0 ) ) );
	
}

TEST(math_plane_test, orthoPoint) {
	auto a = Plane( Vector3( 1, 0, 0 ), 0 );

	EXPECT_TRUE( a.orthoPoint( Vector3( 10, 0, 0 ) ).equals( Vector3( 10, 0, 0 ) ) );
	EXPECT_TRUE( a.orthoPoint( Vector3( -10, 0, 0 ) ).equals( Vector3( -10, 0, 0 ) ) );
}

TEST(math_plane_test, coplanarPoint) {
	auto a = Plane( Vector3( 1, 0, 0 ), 0 );
	EXPECT_TRUE( a.distanceToPoint( a.coplanarPoint() ) == 0 );

	a = Plane( Vector3( 0, 1, 0 ), -1 );
	EXPECT_TRUE( a.distanceToPoint( a.coplanarPoint() ) == 0 );
}

TEST(math_plane_test, applyMatrix4_translate) {

	auto a = Plane( Vector3( 1, 0, 0 ), 0 );

	auto m = Matrix4();
	m.makeRotationZ( Math::PI() * 0.5 );

	EXPECT_TRUE( comparePlane( a.clone().applyMatrix4( m ), Plane( Vector3( 0, 1, 0 ), 0 ) ) );

	a = Plane( Vector3( 0, 1, 0 ), -1 );
	EXPECT_TRUE( comparePlane( a.clone().applyMatrix4( m ), Plane( Vector3( -1, 0, 0 ), -1 ) ) );

	m.makeTranslation( 1, 1, 1 );
	EXPECT_TRUE( comparePlane( a.clone().applyMatrix4( m ), a.clone().translate( Vector3( 1, 1, 1 ) ) ) );
}