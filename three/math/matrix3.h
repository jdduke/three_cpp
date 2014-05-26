#ifndef THREE_MATRIX3_H
#define THREE_MATRIX3_H

#include <three/common.h>

#include <vector>
#include <array>

namespace three {

class THREE_DECL Matrix3 {

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

  Matrix3& operator=( const Matrix3& other );

  Matrix3& set( float n11, float n12, float n13,
                float n21, float n22, float n23,
                float n31, float n32, float n33 );

  Matrix3& identity();

  Matrix3& copy( const Matrix3& m );
  Matrix3& copy( const Matrix4& m );

  std::vector<float>& multiplyVector3Array( std::vector<float>& a);

  Matrix3& multiplyScalar( float s );

  float determinant() const;

  Matrix3& getInverse(  const Matrix4& m, bool throwOnInvertable = false );

  Matrix3& transpose();

  Matrix3& transposeIntoArray( std::array<float, 9> r );

  Matrix3 clone() const;

  template<typename TMatrix>
  inline Matrix3& getNormalMatrix( const TMatrix& m ) {

    getInverse(m).transpose();

    return *this;

  }

};

} // namespace three

#endif // THREE_MATRIX3_H