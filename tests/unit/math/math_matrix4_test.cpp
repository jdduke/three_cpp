#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/console.h>
#include <three/impl/console.cpp>

#include <three/math/matrix4.h>
#include <three/math/impl/matrix4.cpp>
#include <three/math/vector3.h>
#include <three/math/quaternion.h>
#include <three/math/euler.h>

using namespace three;

TEST(math_matrix4_test, constructor) {
	auto a = Matrix4();
	EXPECT_TRUE( a.determinant() == 1 );

	auto b = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	EXPECT_TRUE( b.elements[0] == 0 );
	EXPECT_TRUE( b.elements[1] == 4 );
	EXPECT_TRUE( b.elements[2] == 8 );
	EXPECT_TRUE( b.elements[3] == 12 );
	EXPECT_TRUE( b.elements[4] == 1 );
	EXPECT_TRUE( b.elements[5] == 5 );
	EXPECT_TRUE( b.elements[6] == 9 );
	EXPECT_TRUE( b.elements[7] == 13 );
	EXPECT_TRUE( b.elements[8] == 2 );
	EXPECT_TRUE( b.elements[9] == 6 );
	EXPECT_TRUE( b.elements[10] == 10 );
	EXPECT_TRUE( b.elements[11] == 14 );
	EXPECT_TRUE( b.elements[12] == 3 );
	EXPECT_TRUE( b.elements[13] == 7 );
	EXPECT_TRUE( b.elements[14] == 11 );
	EXPECT_TRUE( b.elements[15] == 15 );

	EXPECT_TRUE( ! matrixEquals4( a, b ) );
}

TEST(math_matrix4_test, copy) {
	auto a = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	auto b = Matrix4().copy( a );

	EXPECT_TRUE( matrixEquals4( a, b ) );

	// ensure that it is a true copy
	a.elements[0] = 2;
	EXPECT_TRUE( ! matrixEquals4( a, b ) );
}

TEST(math_matrix4_test, set) {
	auto b = Matrix4();
	EXPECT_TRUE( b.determinant() == 1 );

	b.set( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	EXPECT_TRUE( b.elements[0] == 0 );
	EXPECT_TRUE( b.elements[1] == 4 );
	EXPECT_TRUE( b.elements[2] == 8 );
	EXPECT_TRUE( b.elements[3] == 12 );
	EXPECT_TRUE( b.elements[4] == 1 );
	EXPECT_TRUE( b.elements[5] == 5 );
	EXPECT_TRUE( b.elements[6] == 9 );
	EXPECT_TRUE( b.elements[7] == 13 );
	EXPECT_TRUE( b.elements[8] == 2 );
	EXPECT_TRUE( b.elements[9] == 6 );
	EXPECT_TRUE( b.elements[10] == 10 );
	EXPECT_TRUE( b.elements[11] == 14 );
	EXPECT_TRUE( b.elements[12] == 3 );
	EXPECT_TRUE( b.elements[13] == 7 );
	EXPECT_TRUE( b.elements[14] == 11 );
	EXPECT_TRUE( b.elements[15] == 15 );
}

TEST(math_matrix4_test, identity) {
	auto b = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	EXPECT_TRUE( b.elements[0] == 0 );
	EXPECT_TRUE( b.elements[1] == 4 );
	EXPECT_TRUE( b.elements[2] == 8 );
	EXPECT_TRUE( b.elements[3] == 12 );
	EXPECT_TRUE( b.elements[4] == 1 );
	EXPECT_TRUE( b.elements[5] == 5 );
	EXPECT_TRUE( b.elements[6] == 9 );
	EXPECT_TRUE( b.elements[7] == 13 );
	EXPECT_TRUE( b.elements[8] == 2 );
	EXPECT_TRUE( b.elements[9] == 6 );
	EXPECT_TRUE( b.elements[10] == 10 );
	EXPECT_TRUE( b.elements[11] == 14 );
	EXPECT_TRUE( b.elements[12] == 3 );
	EXPECT_TRUE( b.elements[13] == 7 );
	EXPECT_TRUE( b.elements[14] == 11 );
	EXPECT_TRUE( b.elements[15] == 15 );

	auto a = Matrix4();
	EXPECT_TRUE( ! matrixEquals4( a, b ) );

	b.identity();
	EXPECT_TRUE( matrixEquals4( a, b ) );
}

TEST(math_matrix4_test, multiplyScalar) {
	auto b = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	EXPECT_TRUE( b.elements[0] == 0 );
	EXPECT_TRUE( b.elements[1] == 4 );
	EXPECT_TRUE( b.elements[2] == 8 );
	EXPECT_TRUE( b.elements[3] == 12 );
	EXPECT_TRUE( b.elements[4] == 1 );
	EXPECT_TRUE( b.elements[5] == 5 );
	EXPECT_TRUE( b.elements[6] == 9 );
	EXPECT_TRUE( b.elements[7] == 13 );
	EXPECT_TRUE( b.elements[8] == 2 );
	EXPECT_TRUE( b.elements[9] == 6 );
	EXPECT_TRUE( b.elements[10] == 10 );
	EXPECT_TRUE( b.elements[11] == 14 );
	EXPECT_TRUE( b.elements[12] == 3 );
	EXPECT_TRUE( b.elements[13] == 7 );
	EXPECT_TRUE( b.elements[14] == 11 );
	EXPECT_TRUE( b.elements[15] == 15 );

	b.multiplyScalar( 2 );
	EXPECT_TRUE( b.elements[0] == 0*2 );
	EXPECT_TRUE( b.elements[1] == 4*2 );
	EXPECT_TRUE( b.elements[2] == 8*2 );
	EXPECT_TRUE( b.elements[3] == 12*2 );
	EXPECT_TRUE( b.elements[4] == 1*2 );
	EXPECT_TRUE( b.elements[5] == 5*2 );
	EXPECT_TRUE( b.elements[6] == 9*2 );
	EXPECT_TRUE( b.elements[7] == 13*2 );
	EXPECT_TRUE( b.elements[8] == 2*2 );
	EXPECT_TRUE( b.elements[9] == 6*2 );
	EXPECT_TRUE( b.elements[10] == 10*2 );
	EXPECT_TRUE( b.elements[11] == 14*2 );
	EXPECT_TRUE( b.elements[12] == 3*2 );
	EXPECT_TRUE( b.elements[13] == 7*2 );
	EXPECT_TRUE( b.elements[14] == 11*2 );
	EXPECT_TRUE( b.elements[15] == 15*2 );
}

TEST(math_matrix4_test, determinant) {
	auto a = Matrix4();
	EXPECT_TRUE( a.determinant() == 1 );

	a.elements[0] = 2;
	EXPECT_TRUE( a.determinant() == 2 );

	a.elements[0] = 0;
	EXPECT_TRUE( a.determinant() == 0 );

	// calculated via http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/fourD/index.htm
	a.set( 2, 3, 4, 5, -1, -21, -3, -4, 6, 7, 8, 10, -8, -9, -10, -12 );
	EXPECT_TRUE( a.determinant() == 76 );
}

TEST(math_matrix4_test, getInverse) {
	auto identity = Matrix4();

	auto a = Matrix4();
	auto b = Matrix4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	auto c = Matrix4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

	EXPECT_TRUE( ! matrixEquals4( a, b ) );
	b.getInverse( a, false );
	EXPECT_TRUE( matrixEquals4( b, Matrix4() ) );

    EXPECT_THROW(b.getInverse( c, true ), int);

    std::vector<Matrix4> testMatrices = {
		Matrix4().makeRotationX( 0.3 ),
		Matrix4().makeRotationX( -0.3 ),
		Matrix4().makeRotationY( 0.3 ),
		Matrix4().makeRotationY( -0.3 ),
		Matrix4().makeRotationZ( 0.3 ),
		Matrix4().makeRotationZ( -0.3 ),
		Matrix4().makeScale( 1, 2, 3 ),
		Matrix4().makeScale( 1.f/8, 1.f/2, 1.f/3 ),
		Matrix4().makeFrustum( -1, 1, -1, 1, 1, 1000 ),
		Matrix4().makeFrustum( -16, 16, -9, 9, 0.1, 10000 ),
		Matrix4().makeTranslation( 1, 2, 3 )
    };

	for( size_t i = 0; i < testMatrices.size(); i ++ ) {
		auto m = testMatrices[i];

		auto mInverse = Matrix4().getInverse( m );
		auto mSelfInverse = m.clone();
		mSelfInverse.getInverse( mSelfInverse );


		// self-inverse should the same as inverse
		EXPECT_TRUE( matrixEquals4( mSelfInverse, mInverse ) );

		// the determinant of the inverse should be the reciprocal
		EXPECT_TRUE( Math::abs( m.determinant() * mInverse.determinant() - 1 ) < 0.0001 );

		auto mProduct = Matrix4().multiplyMatrices( m, mInverse );

		// the determinant of the identity matrix is 1
		EXPECT_TRUE( Math::abs( mProduct.determinant() - 1 ) < 0.0001 );
		EXPECT_TRUE( matrixEquals4( mProduct, identity ) );
	}
}

TEST(math_matrix4_test, transpose) {
	auto a = Matrix4();
	auto b = a.clone().transpose();
	EXPECT_TRUE( matrixEquals4( a, b ) );

	b = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	auto c = b.clone().transpose();
	EXPECT_TRUE( ! matrixEquals4( b, c ) ); 
	c.transpose();
	EXPECT_TRUE( matrixEquals4( b, c ) ); 
}

TEST(math_matrix4_test, clone) {
	auto a = Matrix4( 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 );
	auto b = a.clone();

	EXPECT_TRUE( matrixEquals4( a, b ) );

	// ensure that it is a true copy
	a.elements[0] = 2;
	EXPECT_TRUE( ! matrixEquals4( a, b ) );
}


TEST(math_matrix4_test, compose_decompose) {
    std::vector<Vector3> tValues = {
		Vector3(),
		Vector3( 3, 0, 0 ),
		Vector3( 0, 4, 0 ),
		Vector3( 0, 0, 5 ),
		Vector3( -6, 0, 0 ),
		Vector3( 0, -7, 0 ),
		Vector3( 0, 0, -8 ),
		Vector3( -2, 5, -9 ),
		Vector3( -2, -5, -9 )
	};

	std::vector<Vector3> sValues = {
		Vector3( 1, 1, 1 ),
		Vector3( 2, 2, 2 ),
		Vector3( 1, -1, 1 ),
		Vector3( -1, 1, 1 ),
		Vector3( 1, 1, -1 ),
		Vector3( 2, -2, 1 ),
		Vector3( -1, 2, -2 ),
		Vector3( -1, -1, -1 ),
		Vector3( -2, -2, -2 )
	};

    std::vector<Quaternion> rValues = {
		Quaternion(),
		Quaternion().setFromEuler( Euler( 1, 1, 0 ) ),
		Quaternion().setFromEuler( Euler( 1, -1, 1 ) ),
		Quaternion( 0, 0.9238795292366128, 0, 0.38268342717215614 )
	};


	for( auto ti = 0; ti < tValues.size(); ti ++ ) {
		for( auto si = 0; si < sValues.size(); si ++ ) {
			for( auto ri = 0; ri < rValues.size(); ri ++ ) {
				auto t = tValues[ti];
				auto s = sValues[si];
				auto r = rValues[ri];

				auto m = Matrix4().compose( t, r, s );
				auto t2 = Vector3();
				auto r2 = Quaternion();
				auto s2 = Vector3();

				m.decompose( t2, r2, s2 );

				auto m2 = Matrix4().compose( t2, r2, s2 );
			
				EXPECT_TRUE( matrixEquals4( m, m2 ) );

			}
		}
	}
}