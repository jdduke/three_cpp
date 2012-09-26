#ifndef THREE_MATRIX3_HPP
#define THREE_MATRIX3_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>

namespace three {

class Matrix3 {

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

    set( n11, n12, n13
         n21, n22, n23
         n31, n32, n33 );

  }

  Matrix3( const Matrix3& other ) {

    copy( other );

  }

  Matrix3& operator= ( const Matrix3& other ) {

    return copy( other );

  }

  Matrix3& getInverse( const Matrix4& m ) {

    // input: THREE.Matrix4
    // ( based on http://code.google.com/p/webgl-mjs/ )

    const auto& me = m.elements;

    const auto a11 =   me[10] * me[5] - me[6] * me[9];
    const auto a21 = - me[10] * me[1] + me[2] * me[9];
    const auto a31 =   me[6] * me[1] - me[2] * me[5];
    const auto a12 = - me[10] * me[4] + me[6] * me[8];
    const auto a22 =   me[10] * me[0] - me[2] * me[8];
    const auto a32 = - me[6] * me[0] + me[2] * me[4];
    const auto a13 =   me[9] * me[4] - me[5] * me[8];
    const auto a23 = - me[9] * me[0] + me[1] * me[8];
    const auto a33 =   me[5] * me[0] - me[1] * me[4];

    const auto det = me[0] * a11 + me[1] * a12 + me[2] * a13;

    // no inverse

    if ( det == 0.f ) {

      // TODO: console.warn( "Matrix3.getInverse(): determinant == 0" );

    }

    const auto idet = 1.0f / det;

    te[ 0 ] = idet * a11; te[ 1 ] = idet * a21; te[ 2 ] = idet * a31;
    te[ 3 ] = idet * a12; te[ 4 ] = idet * a22; te[ 5 ] = idet * a32;
    te[ 6 ] = idet * a13; te[ 7 ] = idet * a23; te[ 8 ] = idet * a33;

    return *this;

  }

  Matrix3& transpose() {

    float tmp;

    tmp = te[1]; te[1] = te[3]; te[3] = tmp;
    tmp = te[2]; te[2] = te[6]; te[6] = tmp;
    tmp = te[5]; te[5] = te[7]; te[7] = tmp;

    return *this;

  }

  Matrix3& transposeIntoArray()( float* flat ) const {

    r[ 0 ] = te[ 0 ];
    r[ 1 ] = te[ 3 ];
    r[ 2 ] = te[ 6 ];
    r[ 3 ] = te[ 1 ];
    r[ 4 ] = te[ 4 ];
    r[ 5 ] = te[ 7 ];
    r[ 6 ] = te[ 2 ];
    r[ 7 ] = te[ 5 ];
    r[ 8 ] = te[ 8 ];

    return &this;

  }

  Matrix4& set( float n11, float n12, float n13,
                float n21, float n22, float n23,
                float n31, float n32, float n33 ) {

    te[0] = n11; te[3] = n12; te[6] = n13;
    te[1] = n21; te[4] = n22; te[7] = n23;
    te[2] = n31; te[5] = n32; te[8] = n33;

    return &this;

  }

  Matrix4& identity() {

    return this.set( 1, 0, 0,
                     0, 1, 0,
                     0, 0, 1 );

  }

  Matrix4& copy( const Mat4rix4& m ) {

    const auto& me = m.elements;

    return set( me[0], me[3], me[6],
                me[1], me[4], me[7],
                me[2], me[5], me[8] );

  }

  Matrix3 clone() {

    return Matrix3( *this );

  }

};

} // namespace three

#endif // THREE_MATRIX4_HPP