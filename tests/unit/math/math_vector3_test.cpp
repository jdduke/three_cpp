#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/vector3.h>
#include <three/math/impl/vector3.cpp>

using namespace three;

TEST(math_vector3_test, constructor) {
	auto a = Vector3();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );
	EXPECT_EQ( a.z, 0 );

	a = Vector3( x, y, z );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
	EXPECT_EQ( a.z, z );
}

TEST(math_vector3_test, copy) {
	auto a = Vector3( x, y, z );
	auto b = Vector3().copy( a );
	EXPECT_EQ( b.x, x );
	EXPECT_EQ( b.y, y );
	EXPECT_EQ( b.z, z );

	// ensure that it is a true copy
	a.x = 0;
	a.y = -1;
	a.z = -2;
	EXPECT_EQ( b.x, x );
	EXPECT_EQ( b.y, y );
	EXPECT_EQ( b.z, z );
}

TEST(math_vector3_test, set) {
	auto a = Vector3();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );
	EXPECT_EQ( a.z, 0 );

	a.set( x, y, z );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
	EXPECT_EQ( a.z, z );
}

TEST(math_vector3_test, setX_setY_setZ) {
	auto a = Vector3();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );
	EXPECT_EQ( a.z, 0 );

	a.setX( x );
	a.setY( y );
	a.setZ( z );

	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
	EXPECT_EQ( a.z, z );
}

TEST(math_vector3_test, setComponent_getComponent) {
	auto a = Vector3();
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );
	EXPECT_EQ( a.z, 0 );

	a.setComponent( 0, 1 );
	a.setComponent( 1, 2 );
	a.setComponent( 2, 3 );
	EXPECT_EQ( a.getComponent( 0 ), 1 );
	EXPECT_EQ( a.getComponent( 1 ), 2 );
	EXPECT_EQ( a.getComponent( 2 ), 3 );
}

TEST(math_vector3_test, add) {
	auto a = Vector3( x, y, z );
	auto b = Vector3( -x, -y, -z );

	a.add( b );
	EXPECT_EQ( a.x, 0 );
	EXPECT_EQ( a.y, 0 );
	EXPECT_EQ( a.z, 0 );

	auto c = Vector3().addVectors( b, b );
	EXPECT_EQ( c.x, -2*x );
	EXPECT_EQ( c.y, -2*y );
	EXPECT_EQ( c.z, -2*z );
}

TEST(math_vector3_test, sub) {
	auto a = Vector3( x, y, z );
	auto b = Vector3( -x, -y, -z );

	a.sub( b );
	EXPECT_EQ( a.x, 2*x );
	EXPECT_EQ( a.y, 2*y );
	EXPECT_EQ( a.z, 2*z );

	auto c = Vector3().subVectors( a, a );
	EXPECT_EQ( c.x, 0 );
	EXPECT_EQ( c.y, 0 );
	EXPECT_EQ( c.z, 0 );
}

TEST(math_vector3_test, multiply_divide) {
	auto a = Vector3( x, y, z );
	auto b = Vector3( -x, -y, -z );

	a.multiplyScalar( -2 );
	EXPECT_EQ( a.x, x*-2 );
	EXPECT_EQ( a.y, y*-2 );
	EXPECT_EQ( a.z, z*-2 );

	b.multiplyScalar( -2 );
	EXPECT_EQ( b.x, 2*x );
	EXPECT_EQ( b.y, 2*y );
	EXPECT_EQ( b.z, 2*z );

	a.divideScalar( -2 );
	EXPECT_EQ( a.x, x );
	EXPECT_EQ( a.y, y );
	EXPECT_EQ( a.z, z );

	b.divideScalar( -2 );
	EXPECT_EQ( b.x, -x );
	EXPECT_EQ( b.y, -y );
	EXPECT_EQ( b.z, -z );
}

TEST(math_vector3_test, min_max_clamp) {
	auto a = Vector3( x, y, z );
	auto b = Vector3( -x, -y, -z );
	auto c = Vector3();

	c.copy( a ).min( b );
	EXPECT_EQ( c.x, -x );
	EXPECT_EQ( c.y, -y );
	EXPECT_EQ( c.z, -z );

	c.copy( a ).max( b );
	EXPECT_EQ( c.x, x );
	EXPECT_EQ( c.y, y );
	EXPECT_EQ( c.z, z );

	c.set( -2*x, 2*y, -2*z );
	c.clamp( b, a );
	EXPECT_EQ( c.x, -x );
	EXPECT_EQ( c.y, y );
	EXPECT_EQ( c.z, -z );
}

TEST(math_vector3_test, negate) {
	auto a = Vector3( x, y, z );

	a.negate();
	EXPECT_EQ( a.x, -x );
	EXPECT_EQ( a.y, -y );
	EXPECT_EQ( a.z, -z );
}

TEST(math_vector3_test, dot) {
	auto a = Vector3( x, y, z );
	auto b = Vector3( -x, -y, -z );
	auto c = Vector3();

	auto result = a.dot( b );
	EXPECT_EQ( result, (-x*x-y*y-z*z) );

	result = a.dot( c );
	EXPECT_EQ( result, 0 );
}

TEST(math_vector3_test, length_lengthSq) {
	auto a = Vector3( x, 0, 0 );
	auto b = Vector3( 0, -y, 0 );
	auto c = Vector3( 0, 0, z );
	auto d = Vector3();

	EXPECT_EQ( a.length(), x );
	EXPECT_EQ( a.lengthSq(), x*x );
	EXPECT_EQ( b.length(), y );
	EXPECT_EQ( b.lengthSq(), y*y );
	EXPECT_EQ( c.length(), z );
	EXPECT_EQ( c.lengthSq(), z*z );
	EXPECT_EQ( d.length(), 0 );
	EXPECT_EQ( d.lengthSq(), 0 );

	a.set( x, y, z );
	EXPECT_EQ( a.length(), Math::sqrt( x*x + y*y + z*z ) );
	EXPECT_EQ( a.lengthSq(), ( x*x + y*y + z*z ) );
}

TEST(math_vector3_test, normalize) {
	auto a = Vector3( x, 0, 0 );
	auto b = Vector3( 0, -y, 0 );
	auto c = Vector3( 0, 0, z );

	a.normalize();
	EXPECT_EQ( a.length(), 1 );
	EXPECT_EQ( a.x, 1 );

	b.normalize();
	EXPECT_EQ( b.length(), 1 );
	EXPECT_EQ( b.y, -1 );

	c.normalize();
	EXPECT_EQ( c.length(), 1 );
	EXPECT_EQ( c.z, 1 );
}

TEST(math_vector3_test, distanceTo_distanceToSquared) {
	auto a = Vector3( x, 0, 0 );
	auto b = Vector3( 0, -y, 0 );
	auto c = Vector3( 0, 0, z );
	auto d = Vector3();

	EXPECT_EQ( a.distanceTo( d ), x );
	EXPECT_EQ( a.distanceToSquared( d ), x*x );

	EXPECT_EQ( b.distanceTo( d ), y );
	EXPECT_EQ( b.distanceToSquared( d ), y*y );

	EXPECT_EQ( c.distanceTo( d ), z );
	EXPECT_EQ( c.distanceToSquared( d ), z*z );
}

TEST(math_vector3_test, setLength) {
	auto a = Vector3( x, 0, 0 );

	EXPECT_EQ( a.length(), x );
	a.setLength( y );
	EXPECT_EQ( a.length(), y );

	a = Vector3( 0, 0, 0 );
	EXPECT_EQ( a.length(), 0 );
	a.setLength( y );
	EXPECT_EQ( a.length(), 0 );

}

TEST(math_vector3_test, projectOnVector) {
	auto a = Vector3( 1, 0, 0 );
	auto b = Vector3();
	auto normal = Vector3( 10, 0, 0 );

	EXPECT_TRUE( b.copy( a ).projectOnVector( normal ).equals( Vector3( 1, 0, 0 ) ) );

	a.set( 0, 1, 0 );
	EXPECT_TRUE( b.copy( a ).projectOnVector( normal ).equals( Vector3( 0, 0, 0 ) ) );

	a.set( 0, 0, -1 );
	EXPECT_TRUE( b.copy( a ).projectOnVector( normal ).equals( Vector3( 0, 0, 0 ) ) );

	a.set( -1, 0, 0 );
	EXPECT_TRUE( b.copy( a ).projectOnVector( normal ).equals( Vector3( -1, 0, 0 ) ) );

}

TEST(math_vector3_test, projectOnPlane) {
	auto a = Vector3( 1, 0, 0 );
	auto b = Vector3();
	auto normal = Vector3( 1, 0, 0 );

	EXPECT_TRUE( b.copy( a ).projectOnPlane( normal ).equals( Vector3( 0, 0, 0 ) ) );

	a.set( 0, 1, 0 );
	EXPECT_TRUE( b.copy( a ).projectOnPlane( normal ).equals( Vector3( 0, 1, 0 ) ) );

	a.set( 0, 0, -1 );
	EXPECT_TRUE( b.copy( a ).projectOnPlane( normal ).equals( Vector3( 0, 0, -1 ) ) );

	a.set( -1, 0, 0 );
	EXPECT_TRUE( b.copy( a ).projectOnPlane( normal ).equals( Vector3( 0, 0, 0 ) ) );

}

TEST(math_vector3_test, reflect) {
	auto a = Vector3( 1, 0, 0 );
	auto normal = Vector3( 1, 0, 0 );
	auto b = Vector3( 0, 0, 0 );

	EXPECT_TRUE( b.copy( a ).reflect( normal ).equals( Vector3( 1, 0, 0 ) ) );

	a.set( 1, -1, 0 );
	EXPECT_TRUE( b.copy( a ).reflect( normal ).equals( Vector3( 1, 1, 0 ) ) );

	a.set( 1, -1, 0 );
	normal.set( 0, -1, 0 );
	EXPECT_TRUE( b.copy( a ).reflect(  normal ).equals( Vector3( -1, -1, 0 ) ) );
}

TEST(math_vector3_test, angleTo) {
	auto a = Vector3( 0, -0.18851655680720186, 0.9820700116639124 );
	auto b = Vector3( 0, 0.18851655680720186, -0.9820700116639124 );

	EXPECT_EQ( a.angleTo( a ), 0 );
	EXPECT_LT( a.angleTo( b ) - Math::PI(), 0.00000000000001 );

	auto x = Vector3( 1, 0, 0 );
	auto y = Vector3( 0, 1, 0 );
	auto z = Vector3( 0, 0, 1 );

	EXPECT_EQ( x.angleTo( y ), Math::PI() / 2 );
	EXPECT_EQ( x.angleTo( z ), Math::PI() / 2 );
	EXPECT_EQ( z.angleTo( x ), Math::PI() / 2 );

	EXPECT_LT( Math::abs( x.angleTo( Vector3( 1, 1, 0 ) ) - ( Math::PI() / 4 ) ), 0.0000001 );
}

TEST(math_vector3_test, lerp_clone) {
	auto a = Vector3( x, 0, z );
	auto b = Vector3( 0, -y, 0 );

	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 0.5 ) ) );
	EXPECT_TRUE( a.lerp( a, 0 ).equals( a.lerp( a, 1 ) ) );

	EXPECT_TRUE( a.clone().lerp( b, 0 ).equals( a ) );

	EXPECT_EQ( a.clone().lerp( b, 0.5 ).x, x*0.5 );
	EXPECT_EQ( a.clone().lerp( b, 0.5 ).y, -y*0.5 );
	EXPECT_EQ( a.clone().lerp( b, 0.5 ).z, z*0.5 );

	EXPECT_TRUE( a.clone().lerp( b, 1 ).equals( b ) );
}

TEST(math_vector3_test, equals) {
	auto a = Vector3( x, 0, z );
	auto b = Vector3( 0, -y, 0 );

	EXPECT_NE( a.x, b.x );
	EXPECT_NE( a.y, b.y );
	EXPECT_NE( a.z, b.z );

	EXPECT_FALSE( a.equals( b ) );
	EXPECT_FALSE( b.equals( a ) );

	a.copy( b );
	EXPECT_EQ( a.x, b.x );
	EXPECT_EQ( a.y, b.y );
	EXPECT_EQ( a.z, b.z );

	EXPECT_TRUE( a.equals( b ) );
	EXPECT_TRUE( b.equals( a ) );
}
