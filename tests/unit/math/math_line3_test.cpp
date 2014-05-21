#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/line3.h>

using namespace three;

TEST(math_line3_test, constructor_equals) {
	auto a = Line3();
	EXPECT_TRUE( a.start.equals( zero3 ) );
	EXPECT_TRUE( a.end.equals( zero3 ) );

	a = Line3( two3.clone(), one3.clone() );
	EXPECT_TRUE( a.start.equals( two3 ) );
	EXPECT_TRUE( a.end.equals( one3 ) );
}

TEST(math_line3_test, copy_equals) {
	auto a = Line3( zero3.clone(), one3.clone() );
	auto b = Line3().copy( a );
	EXPECT_TRUE( b.start.equals( zero3 ) );
	EXPECT_TRUE( b.end.equals( one3 ) );

	// ensure that it is a true copy
	a.start = zero3;
	a.end = one3;
	EXPECT_TRUE( b.start.equals( zero3 ) );
	EXPECT_TRUE( b.end.equals( one3 ) );
}

TEST(math_line3_test, set) {
	auto a = Line3();

	a.set( one3, one3 );
	EXPECT_TRUE( a.start.equals( one3 ) );
	EXPECT_TRUE( a.end.equals( one3 ) );
}

TEST(math_line3_test, at) {
	auto a = Line3( one3.clone(), Vector3( 1, 1, 2 ) );

	EXPECT_TRUE( a.at( -1 ).distanceTo( Vector3( 1, 1, 0 ) ) < 0.0001 );
	EXPECT_TRUE( a.at( 0 ).distanceTo( one3.clone() ) < 0.0001 );
	EXPECT_TRUE( a.at( 1 ).distanceTo( Vector3( 1, 1, 2 ) ) < 0.0001 );
	EXPECT_TRUE( a.at( 2 ).distanceTo( Vector3( 1, 1, 3 ) ) < 0.0001 );
}

TEST(math_line3_test, closestPointToPoint_closestPointToPointParameter) {
	auto a = Line3( one3.clone(), Vector3( 1, 1, 2 ) );

	// nearby the ray
	EXPECT_TRUE( a.closestPointToPointParameter( zero3.clone(), true ) == 0 );
	auto b1 = a.closestPointToPoint( zero3.clone(), true );
	EXPECT_TRUE( b1.distanceTo( Vector3( 1, 1, 1 ) ) < 0.0001 );

	// nearby the ray
	EXPECT_TRUE( a.closestPointToPointParameter( zero3.clone(), false ) == -1 );
	auto b2 = a.closestPointToPoint( zero3.clone(), false );
	EXPECT_TRUE( b2.distanceTo( Vector3( 1, 1, 0 ) ) < 0.0001 );

	// nearby the ray
	EXPECT_TRUE( a.closestPointToPointParameter( Vector3( 1, 1, 5 ), true ) == 1 );
	auto b = a.closestPointToPoint( Vector3( 1, 1, 5 ), true );
	EXPECT_TRUE( b.distanceTo( Vector3( 1, 1, 2 ) ) < 0.0001 );

	// exactly on the ray
	EXPECT_TRUE( a.closestPointToPointParameter( one3.clone(), true ) == 0 );
	auto c = a.closestPointToPoint( one3.clone(), true );
	EXPECT_TRUE( c.distanceTo( one3.clone() ) < 0.0001 );
}