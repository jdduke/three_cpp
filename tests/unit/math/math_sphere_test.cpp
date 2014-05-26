#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/sphere.h>
#include <three/math/box3.h>
#include <three/math/matrix4.h>

using namespace three;

TEST(math_sphere_test, constructor) {
	auto a = Sphere();
	EXPECT_TRUE( a.center.equals( zero3 ) );
	EXPECT_TRUE( a.radius == 0 );

	a = Sphere( one3.clone(), 1 );
	EXPECT_TRUE( a.center.equals( one3 ) );
	EXPECT_TRUE( a.radius == 1 );
}

TEST(math_sphere_test, copy) {
	auto a = Sphere( one3.clone(), 1 );
	auto b = Sphere().copy( a );

	EXPECT_TRUE( b.center.equals( one3 ) );
	EXPECT_TRUE( b.radius == 1 );

	// ensure that it is a true copy
	a.center = zero3;
	a.radius = 0;
	EXPECT_TRUE( b.center.equals( one3 ) );
	EXPECT_TRUE( b.radius == 1 );
}

TEST(math_sphere_test, set) {
	auto a = Sphere();
	EXPECT_TRUE( a.center.equals( zero3 ) );
	EXPECT_TRUE( a.radius == 0 );

	a.set( one3, 1 );
	EXPECT_TRUE( a.center.equals( one3 ) );
	EXPECT_TRUE( a.radius == 1 );
}

TEST(math_sphere_test, empty) {
	auto a = Sphere();
	EXPECT_TRUE( a.empty() );

	a.set( one3, 1 );
	EXPECT_TRUE( ! a.empty() );
}

TEST(math_sphere_test, containsPoint) {
	auto a = Sphere( one3.clone(), 1 );

	EXPECT_TRUE( ! a.containsPoint( zero3 ) );
	EXPECT_TRUE( a.containsPoint( one3 ) );
}

TEST(math_sphere_test, distanceToPoint) {
	auto a = Sphere( one3.clone(), 1 );

	EXPECT_TRUE( ( a.distanceToPoint( zero3 ) - 0.7320 ) < 0.001 );
	EXPECT_TRUE( a.distanceToPoint( one3 ) == -1 );
}

TEST(math_sphere_test, intersectsSphere) {
	auto a = Sphere( one3.clone(), 1 );
	auto b = Sphere( zero3.clone(), 1 );
	auto c = Sphere( zero3.clone(), 0.25 );

	EXPECT_TRUE( a.intersectsSphere( b )  );
	EXPECT_TRUE( ! a.intersectsSphere( c )  );
}

TEST(math_sphere_test, clampPoint) {
	auto a = Sphere( one3.clone(), 1 );

	EXPECT_TRUE( a.clampPoint( Vector3( 1, 1, 3 ) ).equals( Vector3( 1, 1, 2 ) ) );
	EXPECT_TRUE( a.clampPoint( Vector3( 1, 1, -3 ) ).equals( Vector3( 1, 1, 0 ) ) );
}

TEST(math_sphere_test, getBoundingBox) {
	auto a = Sphere( one3.clone(), 1 );

	EXPECT_TRUE( a.getBoundingBox().equals( Box3( zero3, two3 ) ) );

	a.set( zero3, 0 );
	EXPECT_TRUE( a.getBoundingBox().equals( Box3( zero3, zero3 ) ) );
}

TEST(math_sphere_test, applyMatrix4) {
	auto a = Sphere( one3.clone(), 1 );

	auto m = Matrix4().makeTranslation( 1, -2, 1 );

	EXPECT_TRUE( a.clone().applyMatrix4( m ).getBoundingBox().equals( a.getBoundingBox().applyMatrix4( m ) ) );
}

TEST(math_sphere_test, translate) {
	auto a = Sphere( one3.clone(), 1 );

	a.translate( one3.clone().negate() );
	EXPECT_TRUE( a.center.equals( zero3 ) );
}