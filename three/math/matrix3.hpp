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

  THREE_DECL Matrix3();
  THREE_DECL Matrix3( float n11, float n12, float n13,
                      float n21, float n22, float n23,
                      float n31, float n32, float n33 );
  THREE_DECL Matrix3( const Matrix3& other );
  THREE_DECL Matrix3& operator= ( const Matrix3& other );

  THREE_DECL Matrix3& getInverse( const Matrix4& m );
  THREE_DECL Matrix3& transpose();

  THREE_DECL const Matrix3& transposeIntoArray( float* r ) const;

  THREE_DECL Matrix3& set( float n11, float n12, float n13,
                           float n21, float n22, float n23,
                           float n31, float n32, float n33 );
  THREE_DECL Matrix3& identity();

  THREE_DECL Matrix3& copy( const Matrix3& m );
  THREE_DECL Matrix3& copy( const Matrix4& m );
  THREE_DECL Matrix3 clone();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/matrix3.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATRIX3_HPP