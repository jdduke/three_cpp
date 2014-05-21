#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/vector2.h>

using namespace three;

TEST(math_vector2_test, constructor) {
	auto a = Vector2();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );

	a = Vector2( x, y );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
}

TEST(math_vector2_test, copy) {
	auto a = Vector2( x, y );
	auto b = Vector2().copy( a );
	EXPECT_TRUE( b.x == x );
	EXPECT_TRUE( b.y == y );

	// ensure that it is a true copy
	a.x = 0;
	a.y = -1;
	EXPECT_TRUE( b.x == x );
	EXPECT_TRUE( b.y == y );
}

TEST(math_vector2_test, set) {
	auto a = Vector2();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );

	a.set( x, y );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
}

TEST(math_vector2_test, setX_setY) {
	auto a = Vector2();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );

	a.setX( x );
	a.setY( y );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
}

TEST(math_vector2_test, setComponent_getComponent) {
	auto a = Vector2();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );

	a.setComponent( 0, 1 );
	a.setComponent( 1, 2 );
	EXPECT_TRUE( a.getComponent( 0 ) == 1 );
	EXPECT_TRUE( a.getComponent( 1 ) == 2 );
}

TEST(math_vector2_test, add) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.add( b );
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );

	auto c = Vector2().addVectors( b, b );
	EXPECT_TRUE( c.x == -2*x );
	EXPECT_TRUE( c.y == -2*y );
}

TEST(math_vector2_test, sub) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.sub( b );
	EXPECT_TRUE( a.x == 2*x );
	EXPECT_TRUE( a.y == 2*y );

	auto c = Vector2().subVectors( a, a );
	EXPECT_TRUE( c.x == 0 );
	EXPECT_TRUE( c.y == 0 );
}

TEST(math_vector2_test, multiply_divide) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.multiplyScalar( -2 );
	EXPECT_TRUE( a.x == x*-2 );
	EXPECT_TRUE( a.y == y*-2 );

	b.multiplyScalar( -2 );
	EXPECT_TRUE( b.x == 2*x );
	EXPECT_TRUE( b.y == 2*y );

	a.divideScalar( -2 );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );

	b.divideScalar( -2 );
	EXPECT_TRUE( b.x == -x );
	EXPECT_TRUE( b.y == -y );
}


TEST(math_vector2_test, min_max_clamp) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );
	auto c = Vector2();

	c.copy( a ).min( b );
	EXPECT_TRUE( c.x == -x );
	EXPECT_TRUE( c.y == -y );

	c.copy( a ).max( b );
	EXPECT_TRUE( c.x == x );
	EXPECT_TRUE( c.y == y );

	c.set( -2*x, 2*y );
	c.clamp( b, a );
	EXPECT_TRUE( c.x == -x );
	EXPECT_TRUE( c.y == y );
}

TEST(math_vector2_test, negate) {
	auto a = Vector2( x, y );

	a.negate();
	EXPECT_TRUE( a.x == -x );
	EXPECT_TRUE( a.y == -y );
}

TEST(math_vector2_test, dot) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );
	auto c = Vector2();

	auto result = a.dot( b );
	EXPECT_TRUE( result == (-x*x-y*y) );

	result = a.dot( c );
	EXPECT_TRUE( result == 0 );
}

TEST(math_vector2_test, length_lengthSq) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );
	auto c = Vector2();

	EXPECT_TRUE( a.length() == x );
	EXPECT_TRUE( a.lengthSq() == x*x );
	EXPECT_TRUE( b.length() == y );
	EXPECT_TRUE( b.lengthSq() == y*y );
	EXPECT_TRUE( c.length() == 0 );
	EXPECT_TRUE( c.lengthSq() == 0 );

	a.set( x, y );
	EXPECT_TRUE( a.length() == Math::sqrt( x*x + y*y ) );
	EXPECT_TRUE( a.lengthSq() == ( x*x + y*y ) );
}

TEST(math_vector2_test, normalize) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	a.normalize();
	EXPECT_TRUE( a.length() == 1 );
	EXPECT_TRUE( a.x == 1 );

	b.normalize();
	EXPECT_TRUE( b.length() == 1 );
	EXPECT_TRUE( b.y == -1 );
}

TEST(math_vector2_test, distanceTo_distanceToSquared) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );
	auto c = Vector2();

	EXPECT_TRUE( a.distanceTo( c ) == x );
	EXPECT_TRUE( a.distanceToSquared( c ) == x*x );

	EXPECT_TRUE( b.distanceTo( c ) == y );
	EXPECT_TRUE( b.distanceToSquared( c ) == y*y );
}

TEST(math_vector2_test, setLength) {
	auto a = Vector2( x, 0 );

	EXPECT_TRUE( a.length() == x );
	a.setLength( y );
	EXPECT_TRUE( a.length() == y );

	a = Vector2( 0, 0 );
	EXPECT_TRUE( a.length() == 0 );
	a.setLength( y );
	EXPECT_TRUE( a.length() == 0 );
}

TEST(math_vector2_test, lerp_clone) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 0.5 ) ) );
	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 1 ) ) );

	EXPECT_TRUE( a.clone().lerp( b, 0 ).equals( a ) );

	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).x == x*0.5 );
	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).y == -y*0.5 );

	EXPECT_TRUE( a.clone().lerp( b, 1 ).equals( b ) );
}

TEST(math_vector2_test, equals) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	EXPECT_TRUE( a.x != b.x );
	EXPECT_TRUE( a.y != b.y );

	EXPECT_TRUE( ! a.equals( b ) );
	EXPECT_TRUE( ! b.equals( a ) );

	a.copy( b );
	EXPECT_TRUE( a.x == b.x );
	EXPECT_TRUE( a.y == b.y );

	EXPECT_TRUE( a.equals( b ) );
	EXPECT_TRUE( b.equals( a ) );
}