
#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/quaternion.h>
#include <three/math/impl/quaternion.cpp>
#include <three/math/euler.h>
#include <three/math/matrix4.h>
#include <three/constants.h>
#include <array>

using namespace three;

std::vector<enums::EulerRotationOrder> orders = {
    enums::EulerRotationOrder::XYZ,
    enums::EulerRotationOrder::YXZ,
    enums::EulerRotationOrder::ZXY,
    enums::EulerRotationOrder::ZYX,
    enums::EulerRotationOrder::YZX,
    enums::EulerRotationOrder::XZY
};

auto eulerAngles = Euler( 0.1, -0.3, 0.25 );

Quaternion qSub( const Quaternion& a, const Quaternion& b ) {
	auto result = Quaternion();
	result.copy( a );

	result.x( result.x() - b.x() );
	result.y( result.y() - b.y() );
	result.z( result.z() - b.z() );
	result.w( result.w() - b.w() );

	return result;

}

TEST(math_quaternion_test, constructor) {
	auto a = Quaternion();
	EXPECT_TRUE( a.x() == 0 );
	EXPECT_TRUE( a.y() == 0 );
	EXPECT_TRUE( a.z() == 0 );
	EXPECT_TRUE( a.w() == 1 );

	a = Quaternion( x, y, z, w );
	EXPECT_TRUE( a.x() == x );
	EXPECT_TRUE( a.y() == y );
	EXPECT_TRUE( a.z() == z );
	EXPECT_TRUE( a.w() == w );
}

TEST(math_quaternion_test, copy) {
	auto a = Quaternion( x, y, z, w );
	auto b = Quaternion().copy( a );
	EXPECT_TRUE( b.x() == x );
	EXPECT_TRUE( b.y() == y );
	EXPECT_TRUE( b.z() == z );
	EXPECT_TRUE( b.w() == w );

	// ensure that it is a true copy
	a.x(0);
	a.y(-1);
	a.z(0);
	a.w(-1);
	EXPECT_TRUE( b.x() == x );
	EXPECT_TRUE( b.y() == y );
}

TEST(math_quaternion_test, set) {
	auto a = Quaternion();
	EXPECT_TRUE( a.x() == 0 );
	EXPECT_TRUE( a.y() == 0 );
	EXPECT_TRUE( a.z() == 0 );
	EXPECT_TRUE( a.w() == 1 );

	a.set( x, y, z, w );
	EXPECT_TRUE( a.x() == x );
	EXPECT_TRUE( a.y() == y );
	EXPECT_TRUE( a.z() == z );
	EXPECT_TRUE( a.w() == w );
}

TEST(math_quaternion_test, setFromAxisAngle) {

	// TODO: find cases to validate.
	EXPECT_TRUE( true );

	auto zero = Quaternion();

	auto a = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), 0 );
	EXPECT_TRUE( a.equals( zero ) );
	a = Quaternion().setFromAxisAngle( Vector3( 0, 1, 0 ), 0 );
	EXPECT_TRUE( a.equals( zero ) );
	a = Quaternion().setFromAxisAngle( Vector3( 0, 0, 1 ), 0 );
	EXPECT_TRUE( a.equals( zero ) );

	auto b1 = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), Math::PI() );
	EXPECT_TRUE( ! a.equals( b1 ) );
	auto b2 = Quaternion().setFromAxisAngle( Vector3( 1, 0, 0 ), -Math::PI() );
	EXPECT_TRUE( ! a.equals( b2 ) );

	b1.multiply( b2 );
	EXPECT_TRUE( a.equals( b1 ) );
}


TEST(math_quaternion_test, setFromEuler_setFromQuaternion) {

    std::vector<Vector3> angles = { Vector3( 1, 0, 0 ), Vector3( 0, 1, 0 ), Vector3( 0, 0, 1 ) };

	// ensure euler conversion to/from Quaternion matches.
	for( auto i = 0; i < orders.size(); i ++ ) {
		for( auto j = 0; j < angles.size(); j ++ ) {
			auto eulers2 = Euler().setFromQuaternion( Quaternion().setFromEuler( Euler( angles[j].x, angles[j].y, angles[j].z, orders[i] ) ), orders[i] );
			auto newAngle = Vector3( eulers2.x, eulers2.y, eulers2.z );
			EXPECT_TRUE( newAngle.distanceTo( angles[j] ) < 0.001 );
		}
	}

}

TEST(math_quaternion_test, setFromEuler_setFromRotationMatrix) {

	// ensure euler conversion for Quaternion matches that of Matrix4
	for( auto i = 0; i < orders.size(); i ++ ) {
		auto q = Quaternion().setFromEuler( eulerAngles ); // EA: 2nd param removed
		auto m = Matrix4().makeRotationFromEuler( eulerAngles ); // EA: 2nd param removed
		auto q2 = Quaternion().setFromRotationMatrix( m );

		EXPECT_TRUE( qSub( q, q2 ).length() < 0.001 );
	}

}

TEST(math_quaternion_test, normalize_length_lengthSq) {
	auto a = Quaternion( x, y, z, w );
	//auto b = Quaternion( -x, -y, -z, -w );

	EXPECT_TRUE( a.length() != 1);
	EXPECT_TRUE( a.lengthSq() != 1);
	a.normalize();
	EXPECT_TRUE( a.length() == 1);
	EXPECT_TRUE( a.lengthSq() == 1);

	a.set( 0, 0, 0, 0 );
	EXPECT_TRUE( a.lengthSq() == 0);
	EXPECT_TRUE( a.length() == 0);
	a.normalize();
	EXPECT_TRUE( a.lengthSq() == 1);
	EXPECT_TRUE( a.length() == 1);
}

TEST(math_quaternion_test, inverse_conjugate) {
	auto a = Quaternion( x, y, z, w );

	// TODO: add better validation here.

	auto b = a.clone().conjugate();

	EXPECT_TRUE( a.x() == -b.x() );
	EXPECT_TRUE( a.y() == -b.y() );
	EXPECT_TRUE( a.z() == -b.z() );
	EXPECT_TRUE( a.w() == b.w() );
}


TEST(math_quaternion_test, multiplyQuaternions_multiply) {

    std::vector<Euler> angles = { Euler( 1, 0, 0 ), Euler( 0, 1, 0 ), Euler( 0, 0, 1 ) };

	auto q1 = Quaternion().setFromEuler( angles[0], true );
	auto q2 = Quaternion().setFromEuler( angles[1], true );
	auto q3 = Quaternion().setFromEuler( angles[2], true );

	auto q = Quaternion().multiplyQuaternions( q1, q2 ).multiply( q3 );

	auto m1 = Matrix4().makeRotationFromEuler( angles[0] );
	auto m2 = Matrix4().makeRotationFromEuler( angles[1] );
	auto m3 = Matrix4().makeRotationFromEuler( angles[2] );

	auto m = Matrix4().multiplyMatrices( m1, m2 ).multiply( m3 );

	auto qFromM = Quaternion().setFromRotationMatrix( m );

	EXPECT_TRUE( qSub( q, qFromM ).length() < 0.001 );
}

TEST(math_quaternion_test, multiplyVector3) {
	
    std::vector<Euler> angles = { Euler( 1, 0, 0 ), Euler( 0, 1, 0 ), Euler( 0, 0, 1 ) };

	// ensure euler conversion for Quaternion matches that of Matrix4
	for( auto i = 0; i < orders.size(); i ++ ) {
		for( auto j = 0; j < angles.size(); j ++ ) {
			auto q = Quaternion().setFromEuler( angles[j] );
			auto m = Matrix4().makeRotationFromEuler( angles[j] );

			auto v0 = Vector3(1, 0, 0);
			auto qv = v0.clone().applyQuaternion( q );
			auto mv = v0.clone().applyMatrix4( m );
		
			EXPECT_TRUE( qv.distanceTo( mv ) < 0.001 );
		}
	}

}

TEST(math_quaternion_test, equals) {
	auto a = Quaternion( x, y, z, w );
	auto b = Quaternion( -x, -y, -z, -w );
	
	EXPECT_TRUE( a.x() != b.x() );
	EXPECT_TRUE( a.y() != b.y() );

	EXPECT_TRUE( ! a.equals( b ) );
	EXPECT_TRUE( ! b.equals( a ) );

	a.copy( b );
	EXPECT_TRUE( a.x() == b.x() );
	EXPECT_TRUE( a.y() == b.y() );

	EXPECT_TRUE( a.equals( b ) );
	EXPECT_TRUE( b.equals( a ) );
}

