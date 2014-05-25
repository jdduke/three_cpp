#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/vector2.h>

using namespace three;

TEST(math_vector2_test, constructor) {
	auto a = Vector2();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );

	a = Vector2( x, y );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
}

TEST(math_vector2_test, copy) {
	auto a = Vector2( x, y );
	auto b = Vector2().copy( a );
	EXPECT_EQ( b.x, x );
	EXPECT_EQ( b.y, y );

	// ensure that it is a true copy
	a.x = 0;
	a.y = -1;
	EXPECT_EQ( b.x, x );
	EXPECT_EQ( b.y, y );
}

TEST(math_vector2_test, set) {
	auto a = Vector2();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );

	a.set( x, y );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
}

TEST(math_vector2_test, setX_setY) {
	auto a = Vector2();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );

	a.setX( x );
	a.setY( y );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
}

TEST(math_vector2_test, setComponent_getComponent) {
	auto a = Vector2();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );

	a.setComponent( 0, 1 );
	a.setComponent( 1, 2 );
	EXPECT_EQ( a.getComponent( 0 ), 1 );
	EXPECT_EQ( a.getComponent( 1 ), 2 );
}

TEST(math_vector2_test, add) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.add( b );
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );

	auto c = Vector2().addVectors( b, b );
	EXPECT_EQ( c.x, -2*x );
	EXPECT_EQ( c.y, -2*y );
}

TEST(math_vector2_test, sub) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.sub( b );
	EXPECT_EQ( a.x, 2*x );
	EXPECT_EQ( a.y, 2*y );

	auto c = Vector2().subVectors( a, a );
	EXPECT_EQ( c.x, 0 );
	EXPECT_EQ( c.y, 0 );
}

TEST(math_vector2_test, multiply_divide) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );

	a.multiplyScalar( -2 );
	EXPECT_EQ( a.x, x*-2 );
	EXPECT_EQ( a.y, y*-2 );

	b.multiplyScalar( -2 );
	EXPECT_EQ( b.x, 2*x );
	EXPECT_EQ( b.y, 2*y );

	a.divideScalar( -2 );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );

	b.divideScalar( -2 );
	EXPECT_EQ( b.x, -x );
	EXPECT_EQ( b.y, -y );
}


TEST(math_vector2_test, min_max_clamp) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );
	auto c = Vector2();

	c.copy( a ).min( b );
	EXPECT_EQ( c.x, -x );
	EXPECT_EQ( c.y, -y );

	c.copy( a ).max( b );
	EXPECT_EQ( c.x, x );
	EXPECT_EQ( c.y, y );

	c.set( -2*x, 2*y );
	c.clamp( b, a );
	EXPECT_EQ( c.x, -x );
	EXPECT_EQ( c.y, y );
}

TEST(math_vector2_test, negate) {
	auto a = Vector2( x, y );

	a.negate();
	EXPECT_EQ( a.x, -x );
	EXPECT_EQ( a.y, -y );
}

TEST(math_vector2_test, dot) {
	auto a = Vector2( x, y );
	auto b = Vector2( -x, -y );
	auto c = Vector2();

	auto result = a.dot( b );
	EXPECT_EQ( result, (-x*x-y*y) );

	result = a.dot( c );
	EXPECT_EQ( result, 0 );
}

TEST(math_vector2_test, length_lengthSq) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );
	auto c = Vector2();

	EXPECT_EQ( a.length(), x );
	EXPECT_EQ( a.lengthSq(), x*x );
	EXPECT_EQ( b.length(), y );
	EXPECT_EQ( b.lengthSq(), y*y );
	EXPECT_EQ( c.length(), 0 );
	EXPECT_EQ( c.lengthSq(), 0 );

	a.set( x, y );
	EXPECT_EQ( a.length(), Math::sqrt( x*x + y*y ) );
	EXPECT_EQ( a.lengthSq(), ( x*x + y*y ) );
}

TEST(math_vector2_test, normalize) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	a.normalize();
	EXPECT_EQ( a.length(), 1 );
	EXPECT_EQ( a.x, 1 );

	b.normalize();
	EXPECT_EQ( b.length(), 1 );
	EXPECT_EQ( b.y, -1 );
}

TEST(math_vector2_test, distanceTo_distanceToSquared) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );
	auto c = Vector2();

	EXPECT_EQ( a.distanceTo( c ), x );
	EXPECT_EQ( a.distanceToSquared( c ), x*x );

	EXPECT_EQ( b.distanceTo( c ), y );
	EXPECT_EQ( b.distanceToSquared( c ), y*y );
}

TEST(math_vector2_test, setLength) {
	auto a = Vector2( x, 0 );

	EXPECT_EQ( a.length(), x );
	a.setLength( y );
	EXPECT_EQ( a.length(), y );

	a = Vector2( 0, 0 );
	EXPECT_EQ( a.length(), 0 );
	a.setLength( y );
	EXPECT_EQ( a.length(), 0 );
}

TEST(math_vector2_test, lerp_clone) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 0.5 ) ) );
	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 1 ) ) );

	EXPECT_TRUE( a.clone().lerp( b, 0 ).equals( a ) );

	EXPECT_EQ( a.clone().lerp( b, 0.5 ).x, x*0.5 );
	EXPECT_EQ( a.clone().lerp( b, 0.5 ).y, -y*0.5 );

	EXPECT_TRUE( a.clone().lerp( b, 1 ).equals( b ) );
}

TEST(math_vector2_test, equals) {
	auto a = Vector2( x, 0 );
	auto b = Vector2( 0, -y );

	EXPECT_NE( a.x, b.x );
	EXPECT_NE( a.y, b.y );

	EXPECT_FALSE( a.equals( b ) );
	EXPECT_FALSE( b.equals( a ) );

	a.copy( b );
	EXPECT_EQ( a.x, b.x );
	EXPECT_EQ( a.y, b.y );

	EXPECT_TRUE( a.equals( b ) );
	EXPECT_TRUE( b.equals( a ) );
}
