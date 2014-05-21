#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/vector4.h>
#include <three/math/impl/vector4.cpp>

using namespace three;

TEST(math_vector4_test, constructor) {
	auto a = Vector4();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );
	EXPECT_TRUE( a.z == 0 );
	EXPECT_TRUE( a.w == 1 );

	a = Vector4( x, y, z, w );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
	EXPECT_TRUE( a.z == z );
	EXPECT_TRUE( a.w == w );
}

TEST(math_vector4_test, copy) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4().copy( a );
	EXPECT_TRUE( b.x == x );
	EXPECT_TRUE( b.y == y );
	EXPECT_TRUE( b.z == z );
	EXPECT_TRUE( b.w == w );

	// ensure that it is a true copy
	a.x = 0;
	a.y = -1;
	a.z = -2;
	a.w = -3;
	EXPECT_TRUE( b.x == x );
	EXPECT_TRUE( b.y == y );
	EXPECT_TRUE( b.z == z );
	EXPECT_TRUE( b.w == w );
}

TEST(math_vector4_test, set) {
	auto a = Vector4();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );
	EXPECT_TRUE( a.z == 0 );
	EXPECT_TRUE( a.w == 1 );

	a.set( x, y, z, w );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
	EXPECT_TRUE( a.z == z );
	EXPECT_TRUE( a.w == w );
}

TEST(math_vector4_test, setX_setY_setZ_setW) {
	auto a = Vector4();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );
	EXPECT_TRUE( a.z == 0 );
	EXPECT_TRUE( a.w == 1 );

	a.setX( x );
	a.setY( y );
	a.setZ( z );
	a.setW( w );

	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
	EXPECT_TRUE( a.z == z );
	EXPECT_TRUE( a.w == w );
}

TEST(math_vector4_test, setComponent_getComponent) {
	auto a = Vector4();
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );
	EXPECT_TRUE( a.z == 0 );
	EXPECT_TRUE( a.w == 1 );

	a.setComponent( 0, 1 );
	a.setComponent( 1, 2 );
	a.setComponent( 2, 3 );
	a.setComponent( 3, 4 );
	EXPECT_TRUE( a.getComponent( 0 ) == 1 );
	EXPECT_TRUE( a.getComponent( 1 ) == 2 );
	EXPECT_TRUE( a.getComponent( 2 ) == 3 );
	EXPECT_TRUE( a.getComponent( 3 ) == 4 );
}

TEST(math_vector4_test, add) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4( -x, -y, -z, -w );

	a.add( b );
	EXPECT_TRUE( a.x == 0 );
	EXPECT_TRUE( a.y == 0 );
	EXPECT_TRUE( a.z == 0 );
	EXPECT_TRUE( a.w == 0 );

	auto c = Vector4().addVectors( b, b );
	EXPECT_TRUE( c.x == -2*x );
	EXPECT_TRUE( c.y == -2*y );
	EXPECT_TRUE( c.z == -2*z );
	EXPECT_TRUE( c.w == -2*w );
}

TEST(math_vector4_test, sub) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4( -x, -y, -z, -w );

	a.sub( b );
	EXPECT_TRUE( a.x == 2*x );
	EXPECT_TRUE( a.y == 2*y );
	EXPECT_TRUE( a.z == 2*z );
	EXPECT_TRUE( a.w == 2*w );

	auto c = Vector4().subVectors( a, a );
	EXPECT_TRUE( c.x == 0 );
	EXPECT_TRUE( c.y == 0 );
	EXPECT_TRUE( c.z == 0 );
	EXPECT_TRUE( c.w == 0 );
}

TEST(math_vector4_test, multiply_divide) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4( -x, -y, -z, -w );

	a.multiplyScalar( -2 );
	EXPECT_TRUE( a.x == x*-2 );
	EXPECT_TRUE( a.y == y*-2 );
	EXPECT_TRUE( a.z == z*-2 );
	EXPECT_TRUE( a.w == w*-2 );

	b.multiplyScalar( -2 );
	EXPECT_TRUE( b.x == 2*x );
	EXPECT_TRUE( b.y == 2*y );	
	EXPECT_TRUE( b.z == 2*z );	
	EXPECT_TRUE( b.w == 2*w );	

	a.divideScalar( -2 );
	EXPECT_TRUE( a.x == x );
	EXPECT_TRUE( a.y == y );
	EXPECT_TRUE( a.z == z );
	EXPECT_TRUE( a.w == w );

	b.divideScalar( -2 );
	EXPECT_TRUE( b.x == -x );
	EXPECT_TRUE( b.y == -y );
	EXPECT_TRUE( b.z == -z );
	EXPECT_TRUE( b.w == -w );
}

TEST(math_vector4_test, min_max_clamp) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4( -x, -y, -z, -w );
	auto c = Vector4();

	c.copy( a ).min( b );
	EXPECT_TRUE( c.x == -x );
	EXPECT_TRUE( c.y == -y );
	EXPECT_TRUE( c.z == -z );
	EXPECT_TRUE( c.w == -w );

	c.copy( a ).max( b );
	EXPECT_TRUE( c.x == x );
	EXPECT_TRUE( c.y == y );
	EXPECT_TRUE( c.z == z );
	EXPECT_TRUE( c.w == w );

	c.set( -2*x, 2*y, -2*z, 2*w );
	c.clamp( b, a );
	EXPECT_TRUE( c.x == -x );
	EXPECT_TRUE( c.y == y );
	EXPECT_TRUE( c.z == -z );
	EXPECT_TRUE( c.w == w );
}

TEST(math_vector4_test, negate) {
	auto a = Vector4( x, y, z, w );

	a.negate();
	EXPECT_TRUE( a.x == -x );
	EXPECT_TRUE( a.y == -y );
	EXPECT_TRUE( a.z == -z );
	EXPECT_TRUE( a.w == -w );
}

TEST(math_vector4_test, dot) {
	auto a = Vector4( x, y, z, w );
	auto b = Vector4( -x, -y, -z, -w );
	auto c = Vector4( 0, 0, 0, 0 );

	auto result = a.dot( b );
	EXPECT_TRUE( result == (-x*x-y*y-z*z-w*w) );

	result = a.dot( c );
	EXPECT_TRUE( result == 0 );
}

TEST(math_vector4_test, length_lengthSq) {
	auto a = Vector4( x, 0, 0, 0 );
	auto b = Vector4( 0, -y, 0, 0 );
	auto c = Vector4( 0, 0, z, 0 );
	auto d = Vector4( 0, 0, 0, w );
	auto e = Vector4( 0, 0, 0, 0 );
	
	EXPECT_TRUE( a.length() == x );
	EXPECT_TRUE( a.lengthSq() == x*x );
	EXPECT_TRUE( b.length() == y );
	EXPECT_TRUE( b.lengthSq() == y*y );
	EXPECT_TRUE( c.length() == z );
	EXPECT_TRUE( c.lengthSq() == z*z );
	EXPECT_TRUE( d.length() == w );
	EXPECT_TRUE( d.lengthSq() == w*w );
	EXPECT_TRUE( e.length() == 0 );
	EXPECT_TRUE( e.lengthSq() == 0 );

	a.set( x, y, z, w );
	EXPECT_TRUE( a.length() == Math::sqrt( x*x + y*y + z*z + w*w ) );
	EXPECT_TRUE( a.lengthSq() == ( x*x + y*y + z*z + w*w ) );
}

TEST(math_vector4_test, normalize) {
	auto a = Vector4( x, 0, 0, 0 );
	auto b = Vector4( 0, -y, 0, 0 );
	auto c = Vector4( 0, 0, z, 0 );
	auto d = Vector4( 0, 0, 0, -w );
	
	a.normalize();
	EXPECT_TRUE( a.length() == 1 );
	EXPECT_TRUE( a.x == 1 );

	b.normalize();
	EXPECT_TRUE( b.length() == 1 );
	EXPECT_TRUE( b.y == -1 );

	c.normalize();
	EXPECT_TRUE( c.length() == 1 );
	EXPECT_TRUE( c.z == 1 );

	d.normalize();
	EXPECT_TRUE( d.length() == 1 );
	EXPECT_TRUE( d.w == -1 );
}

TEST(math_vector4_test, setLength) {
	auto a = Vector4( x, 0, 0, 0 );

	EXPECT_TRUE( a.length() == x );
	a.setLength( y );
	EXPECT_TRUE( a.length() == y );

	a = Vector4( 0, 0, 0, 0 );
	EXPECT_TRUE( a.length() == 0 );
	a.setLength( y );
	EXPECT_TRUE( a.length() == 0 );
}

TEST(math_vector4_test, lerp_clone) {
	auto a = Vector4( x, 0, z, 0 );
	auto b = Vector4( 0, -y, 0, -w );

	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 0.5 ) ) );
	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 1 ) ) );

	EXPECT_TRUE( a.clone().lerp( b, 0 ).equals( a ) );

	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).x == x*0.5 );
	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).y == -y*0.5 );
	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).z == z*0.5 );
	EXPECT_TRUE( a.clone().lerp( b, 0.5 ).w == -w*0.5 );

	EXPECT_TRUE( a.clone().lerp( b, 1 ).equals( b ) );
}

TEST(math_vector4_test, equals) {
	auto a = Vector4( x, 0, z, 0 );
	auto b = Vector4( 0, -y, 0, -w );

	EXPECT_TRUE( a.x != b.x );
	EXPECT_TRUE( a.y != b.y );
	EXPECT_TRUE( a.z != b.z );
	EXPECT_TRUE( a.w != b.w );

	EXPECT_TRUE( ! a.equals( b ) );
	EXPECT_TRUE( ! b.equals( a ) );

	a.copy( b );
	EXPECT_TRUE( a.x == b.x );
	EXPECT_TRUE( a.y == b.y );
	EXPECT_TRUE( a.z == b.z );
	EXPECT_TRUE( a.w == b.w );

	EXPECT_TRUE( a.equals( b ) );
	EXPECT_TRUE( b.equals( a ) );
}