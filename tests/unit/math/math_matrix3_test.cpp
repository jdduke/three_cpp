#include "gtest/gtest.h"
#include <tests/test_constants.h>

#include <three/math/matrix3.h>
#include <three/math/impl/matrix3.cpp>
#include <three/math/matrix4.h>

using namespace three;

template <typename TMatrixA, typename TMatrixB>
bool matrixEquals3 ( const TMatrixA& a, const TMatrixB& b, float tolerance ) {
  for( auto i = 0, il = 9; i < il; i ++ ) {
    auto delta = a.elements[i] - b.elements[i];
    if( delta > tolerance ) {
      return false;
    }
  }
  return true;
};


Matrix4 toMatrix4( const Matrix3& m3 ) {
  auto result = Matrix4();
  auto& re = result.elements;
  const auto& me = m3.elements;
  re[0] = me[0];
  re[1] = me[1];
  re[2] = me[2];
  re[4] = me[3];
  re[5] = me[4];
  re[6] = me[5];
  re[8] = me[6];
  re[9] = me[7];
  re[10] = me[8];

  return result;
};

TEST(math_matrix3_test, constructor) {
  auto a = Matrix3();
  EXPECT_EQ( a.determinant(), 1 );

  auto b = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  EXPECT_EQ( b.elements[0], 0 );
  EXPECT_EQ( b.elements[1], 3 );
  EXPECT_EQ( b.elements[2], 6 );
  EXPECT_EQ( b.elements[3], 1 );
  EXPECT_EQ( b.elements[4], 4 );
  EXPECT_EQ( b.elements[5], 7 );
  EXPECT_EQ( b.elements[6], 2 );
  EXPECT_EQ( b.elements[7], 5 );
  EXPECT_EQ( b.elements[8], 8 );

  EXPECT_FALSE( matrixEquals3( a, b, 0.00001f ) );
}

TEST(math_matrix3_test, copy) {
  auto a = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  auto b = Matrix3().copy( a );

  EXPECT_TRUE( matrixEquals3( a, b, 0.00001f ) );

  // ensure that it is a true copy
  a.elements[0] = 2;
  EXPECT_FALSE( matrixEquals3( a, b, 0.00001f ) );
}

TEST(math_matrix3_test, set) {
  auto b = Matrix3();
  EXPECT_EQ( b.determinant(), 1 );

  b.set( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  EXPECT_EQ( b.elements[0], 0 );
  EXPECT_EQ( b.elements[1], 3 );
  EXPECT_EQ( b.elements[2], 6 );
  EXPECT_EQ( b.elements[3], 1 );
  EXPECT_EQ( b.elements[4], 4 );
  EXPECT_EQ( b.elements[5], 7 );
  EXPECT_EQ( b.elements[6], 2 );
  EXPECT_EQ( b.elements[7], 5 );
  EXPECT_EQ( b.elements[8], 8 );
}

TEST(math_matrix3_test, identity) {
  auto b = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  EXPECT_EQ( b.elements[0], 0 );
  EXPECT_EQ( b.elements[1], 3 );
  EXPECT_EQ( b.elements[2], 6 );
  EXPECT_EQ( b.elements[3], 1 );
  EXPECT_EQ( b.elements[4], 4 );
  EXPECT_EQ( b.elements[5], 7 );
  EXPECT_EQ( b.elements[6], 2 );
  EXPECT_EQ( b.elements[7], 5 );
  EXPECT_EQ( b.elements[8], 8 );

  auto a = Matrix3();
  EXPECT_FALSE( matrixEquals3( a, b, 0.00001f ) );

  b.identity();
  EXPECT_TRUE( matrixEquals3( a, b, 0.00001f ) );
}

TEST(math_matrix3_test, multiplyScalar) {
  auto b = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  EXPECT_EQ( b.elements[0], 0 );
  EXPECT_EQ( b.elements[1], 3 );
  EXPECT_EQ( b.elements[2], 6 );
  EXPECT_EQ( b.elements[3], 1 );
  EXPECT_EQ( b.elements[4], 4 );
  EXPECT_EQ( b.elements[5], 7 );
  EXPECT_EQ( b.elements[6], 2 );
  EXPECT_EQ( b.elements[7], 5 );
  EXPECT_EQ( b.elements[8], 8 );

  b.multiplyScalar( 2 );
  EXPECT_EQ( b.elements[0], 0*2 );
  EXPECT_EQ( b.elements[1], 3*2 );
  EXPECT_EQ( b.elements[2], 6*2 );
  EXPECT_EQ( b.elements[3], 1*2 );
  EXPECT_EQ( b.elements[4], 4*2 );
  EXPECT_EQ( b.elements[5], 7*2 );
  EXPECT_EQ( b.elements[6], 2*2 );
  EXPECT_EQ( b.elements[7], 5*2 );
  EXPECT_EQ( b.elements[8], 8*2 );
}

TEST(math_matrix3_test, determinant) {
  auto a = Matrix3();
  EXPECT_EQ( a.determinant(), 1 );

  a.elements[0] = 2;
  EXPECT_EQ( a.determinant(), 2 );

  a.elements[0] = 0;
  EXPECT_EQ( a.determinant(), 0 );

  // calculated via http://www.euclideanspace.com/maths/algebra/matrix/functions/determinant/threeD/index.htm
  a.set( 2, 3, 4, 5, 13, 7, 8, 9, 11 );
  EXPECT_EQ( a.determinant(), -73 );
}


TEST(math_matrix3_test, getInverse) {
  auto identity = Matrix4();
  auto a = Matrix4();
  auto b = Matrix3( 0, 0, 0, 0, 0, 0, 0, 0, 0 );
  auto c = Matrix4( 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  EXPECT_FALSE( matrixEquals3( a, b, 0.00001f ) );
  b.getInverse( a, false );
  EXPECT_TRUE( matrixEquals3( b, Matrix3(), 0.00001f ) );

    EXPECT_THROW(b.getInverse( c, true ), int);

  Matrix4 testMatrices[] = {
    Matrix4().makeRotationX( 0.3 ),
    Matrix4().makeRotationX( -0.3 ),
    Matrix4().makeRotationY( 0.3 ),
    Matrix4().makeRotationY( -0.3 ),
    Matrix4().makeRotationZ( 0.3 ),
    Matrix4().makeRotationZ( -0.3 ),
    Matrix4().makeScale( 1, 2, 3 ),
    Matrix4().makeScale( 1.f/8, 1.f/2, 1.f/3 )
  };

  for( const auto& m : testMatrices ) {
    auto mInverse3 = Matrix3().getInverse( m );

    auto mInverse = toMatrix4( mInverse3 );

    // the determinant of the inverse should be the reciprocal
    EXPECT_LT( Math::abs( m.determinant() * mInverse3.determinant() - 1 ), 0.0001f );
    EXPECT_LT( Math::abs( m.determinant() * mInverse.determinant() - 1 ), 0.0001f );

    auto mProduct = Matrix4().multiplyMatrices( m, mInverse );
    EXPECT_LT( Math::abs( mProduct.determinant() - 1 ), 0.0001f );
    EXPECT_TRUE( matrixEquals3( mProduct, identity, 0.0001f ) );
  }
}

TEST(math_matrix3_test, transpose) {
  auto a = Matrix3();
  auto b = a.clone().transpose();
  EXPECT_TRUE( matrixEquals3( a, b, 0.0001f ) );

  b = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  auto c = b.clone().transpose();
  EXPECT_FALSE( matrixEquals3( b, c, 0.0001f ) );
  c.transpose();
  EXPECT_TRUE( matrixEquals3( b, c, 0.0001f ) );
}

TEST(math_matrix3_test, clone) {
  auto a = Matrix3( 0, 1, 2, 3, 4, 5, 6, 7, 8 );
  auto b = a.clone();

  EXPECT_TRUE( matrixEquals3( a, b, 0.0001f ) );

  // ensure that it is a true copy
  a.elements[0] = 2;
  EXPECT_FALSE( matrixEquals3( a, b, 0.0001f ) );
}