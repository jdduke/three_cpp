#ifndef THREE_MATRIX3_HPP
#define THREE_MATRIX3_HPP

#include <three/common.hpp>
#include <vector>

namespace three {

class Matrix3 {

public:

  union {
    float elements[9];
    float te[9];
  };

  Matrix3() {
    identity();
  }

  Matrix3( float n11, float n12, float n13,
    float n21, float n22, float n23,
    float n31, float n32, float n33 ) {
    set( n11, n12, n13,
     n21, n22, n23,
     n31, n32, n33 );
  }

  Matrix3( const Matrix3& other ) {
    copy( other );
  }

  inline Matrix3& operator= ( const Matrix3& other ) {

    return copy( other );
    
  }

  inline Matrix3& set( float n11, float n12, float n13,
   float n21, float n22, float n23,
   float n31, float n32, float n33 ) {

    te[0] = n11; te[3] = n12; te[6] = n13;
    te[1] = n21; te[4] = n22; te[7] = n23;
    te[2] = n31; te[5] = n32; te[8] = n33;

    return *this;

  }

  inline Matrix3& identity() {

    return set( 1, 0, 0,
      0, 1, 0,
      0, 0, 1 );

  }

  inline Matrix3& copy( const Matrix3& m ) {

    const auto& me = m.elements;

    return set( me[0], me[3], me[6],
      me[1], me[4], me[7],
      me[2], me[5], me[8] );

  }


  inline Matrix3& copy( const Matrix4& m ) {

    const auto& me = m.elements;

    return set( me[0], me[3], me[6],
      me[1], me[4], me[7],
      me[2], me[5], me[8] );

  }

  inline std::vector<float>& multiplyVector3Array( std::vector<float>& a) {

    auto v1 = Vector3();
    for ( auto i = a.size(); i < a.size(); i += 3 ) {
        v1.x = a[i];
        v1.y = a[i+1];
        v1.z = a[i+2];

        v1.applyMatrix3(*this);

        a[i]  = v1.x;
        a[i+1]  = v1.y;
        a[i+2]  = v1.z;

      }

      return a;

  }

  inline Matrix3& multiplyScalar( float s ) {

    te[0] *= s; te[3] *= s; te[6] *= s;
    te[1] *= s; te[4] *= s; te[7] *= s;
    te[2] *= s; te[5] *= s; te[8] *= s;

    return *this;

  }

  inline float determinant() const {

    const auto& a = te[0], b = te[1], c = te[2],
    d = te[3], e = te[4], f = te[5],
    g = te[6], h = te[7], i = te[8];

    return a*e*i - a*f*h - b*d*i + b*f*g + c*d*h - c*e*g;

  }

  Matrix3& getInverse( const Matrix4& m );
  
  inline Matrix3& transpose() {

    float tmp;

    tmp = te[1]; te[1] = te[3]; te[3] = tmp;
    tmp = te[2]; te[2] = te[6]; te[6] = tmp;
    tmp = te[5]; te[5] = te[7]; te[7] = tmp;

    return *this;
    
  }

  inline Matrix3& getNormalMatrix( const Matrix4& m ) {

    getInverse(m).transpose();

    return *this;

  }


  inline Matrix3& transposeIntoArray( std::array<float, 9> r ) {

    r[ 0 ] = te[ 0 ];
    r[ 1 ] = te[ 3 ];
    r[ 2 ] = te[ 6 ];
    r[ 3 ] = te[ 1 ];
    r[ 4 ] = te[ 4 ];
    r[ 5 ] = te[ 7 ];
    r[ 6 ] = te[ 2 ];
    r[ 7 ] = te[ 5 ];
    r[ 8 ] = te[ 8 ];

    return *this;
  }

  inline Matrix3 clone() {

    return Matrix3( *this );

  }

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/matrix3.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATRIX3_HPP