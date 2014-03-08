#ifndef THREE_MATRIX3_HPP
#define THREE_MATRIX3_HPP

#include <three/common.hpp>
#include <three/math/matrix4.hpp>

namespace three {

class Matrix3 {

public:

  union {
    float elements[9];
    float te[9];
  };

  Matrix3();
  Matrix3( float n11, float n12, float n13,
                      float n21, float n22, float n23,
                      float n31, float n32, float n33 );
  Matrix3( const Matrix3& other );
  Matrix3& operator= ( const Matrix3& other );

  Matrix3& set( float n11, float n12, float n13,
                           float n21, float n22, float n23,
                           float n31, float n32, float n33 );
  Matrix3& identity();

  Matrix3& copy( const Matrix3& m );
  Matrix3& copy( const Matrix4& m );

  std::vector<float>& multiplyVector3Array( std::vector<float>& a);

  Matrix3& multiplyScalar( float s );

  float determinant() const;

  Matrix3& getInverse( const Matrix4& m );
  Matrix3& transpose();

  Matrix3& getNormalMatrix( const Matrix4& m );

  const Matrix3& transposeIntoArray( float* r ) const;

  Matrix3 clone();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/matrix3.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATRIX3_HPP