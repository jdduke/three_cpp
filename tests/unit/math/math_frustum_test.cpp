#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/frustum.h>
#include <three/math/impl/frustum.cpp>
#include <iostream>

using namespace three;

bool planeEquals( const Plane& a, const Plane& b, const float tolerance ) {
    
	if( a.normal.distanceTo( b.normal ) > tolerance ) {
		return false;
	}
	if( Math::abs( a.constant - b.constant ) > tolerance ) {
		return false;
	}
	return true;
};

TEST(math_frustum_test, constructor) {
    
	Frustum a;
    
	EXPECT_TRUE( a.planes.size() == 6);
    
	auto pDefault = Plane();
	for( int i = 0; i < 6; i ++ ) {
		EXPECT_TRUE( a.planes[i].equals( pDefault ) );
	}
    
	auto p0 = Plane( unit3, -1 );
	auto p1 = Plane( unit3, 1 );
	auto p2 = Plane( unit3, 2 );
	auto p3 = Plane( unit3, 3 );
	auto p4 = Plane( unit3, 4 );
	auto p5 = Plane( unit3, 5 );
    
	a = Frustum( p0, p1, p2, p3, p4, p5 );
	EXPECT_TRUE( a.planes[0].equals( p0 ) );
	EXPECT_TRUE( a.planes[1].equals( p1 ) );
	EXPECT_TRUE( a.planes[2].equals( p2 ) );
	EXPECT_TRUE( a.planes[3].equals( p3 ) );
	EXPECT_TRUE( a.planes[4].equals( p4 ) );
	EXPECT_TRUE( a.planes[5].equals( p5 ) );
    
}

TEST(math_frustum_test, copy) {
    
	auto p0 = Plane( unit3, -1 );
	auto p1 = Plane( unit3, 1 );
	auto p2 = Plane( unit3, 2 );
	auto p3 = Plane( unit3, 3 );
	auto p4 = Plane( unit3, 4 );
	auto p5 = Plane( unit3, 5 );
    
	auto b = Frustum( p0, p1, p2, p3, p4, p5 );
	auto a = Frustum().copy(b);
    
	EXPECT_TRUE( a.planes[0].equals( p0 ) );
	EXPECT_TRUE( a.planes[1].equals( p1 ) );
	EXPECT_TRUE( a.planes[2].equals( p2 ) );
	EXPECT_TRUE( a.planes[3].equals( p3 ) );
	EXPECT_TRUE( a.planes[4].equals( p4 ) );
	EXPECT_TRUE( a.planes[5].equals( p5 ) );
    
	// ensure it is a true copy by modifying source
	b.planes[0] = p1;
	EXPECT_TRUE( a.planes[0].equals( p0 ) );
};

TEST(math_frustum_test, setFromMatrix_makeOrthographic_containsPoint) {
	auto m = Matrix4().makeOrthographic( -1, 1, -1, 1, 1, 100 );
	auto a = Frustum().setFromMatrix( m );
  
	EXPECT_TRUE( ! a.containsPoint( Vector3( 0, 0, 0 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -50 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( -1, -1, -1.001 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( -1.1, -1.1, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 1, 1, -1.001 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 1.1, 1.1, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -100 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( -1, -1, -100 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( -1.1, -1.1, -100.1 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 1, 1, -100 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 1.1, 1.1, -100.1 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 0, 0, -101 ) ) );
    
}

TEST(math_frustum_test, setFromMatrix_makeFrustum_containsPoint) {
	auto m = Matrix4().makeFrustum( -1, 1, -1, 1, 1, 100 );
	auto a = Frustum().setFromMatrix( m );
    
	EXPECT_TRUE( ! a.containsPoint( Vector3( 0, 0, 0 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -50 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( -1, -1, -1.001 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( -1.1, -1.1, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 1, 1, -1.001 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 1.1, 1.1, -1.001 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 0, 0, -99.999 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( -99.999, -99.999, -99.999 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( -100.1, -100.1, -100.1 ) ) );
	EXPECT_TRUE( a.containsPoint( Vector3( 99.999, 99.999, -99.999 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 100.1, 100.1, -100.1 ) ) );
	EXPECT_TRUE( ! a.containsPoint( Vector3( 0, 0, -101 ) ) );
}

TEST(math_frustum_test, setFromMatrix_makeFrustum_intersectsSphere) {
	auto m = Matrix4().makeFrustum( -1, 1, -1, 1, 1, 100 );
	auto a = Frustum().setFromMatrix( m );
    
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( 0, 0, 0 ), 0 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( 0, 0, 0 ), 0.9 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 0, 0, 0 ), 1.1 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 0, 0, -50 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 0, 0, -1.001 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( -1, -1, -1.001 ), 0 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( -1.1, -1.1, -1.001 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( -1.1, -1.1, -1.001 ), 0.5 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 1, 1, -1.001 ), 0 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( 1.1, 1.1, -1.001 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 1.1, 1.1, -1.001 ), 0.5 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 0, 0, -99.999 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( -99.999, -99.999, -99.999 ), 0 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( -100.1, -100.1, -100.1 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( -100.1, -100.1, -100.1 ), 0.5 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 99.999, 99.999, -99.999 ), 0 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( 100.1, 100.1, -100.1 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 100.1, 100.1, -100.1 ), 0.2 ) ) );
	EXPECT_TRUE( ! a.intersectsSphere( Sphere( Vector3( 0, 0, -101 ), 0 ) ) );
	EXPECT_TRUE( a.intersectsSphere( Sphere( Vector3( 0, 0, -101 ), 1.1 ) ) );
}

TEST(math_frustum_test, clone) {
    
	auto p0 = Plane( unit3, -1 );
	auto p1 = Plane( unit3, 1 );
	auto p2 = Plane( unit3, 2 );
	auto p3 = Plane( unit3, 3 );
	auto p4 = Plane( unit3, 4 );
	auto p5 = Plane( unit3, 5 );
    
	auto b = Frustum( p0, p1, p2, p3, p4, p5 );
	auto a = b.clone();
	EXPECT_TRUE( a.planes[0].equals( p0 ) );
	EXPECT_TRUE( a.planes[1].equals( p1 ) );
	EXPECT_TRUE( a.planes[2].equals( p2 ) );
	EXPECT_TRUE( a.planes[3].equals( p3 ) );
	EXPECT_TRUE( a.planes[4].equals( p4 ) );
	EXPECT_TRUE( a.planes[5].equals( p5 ) );
    
	// ensure it is a true copy by modifying source
	a.planes[0].copy( p1 );
	EXPECT_TRUE( b.planes[0].equals( p0 ) );
}