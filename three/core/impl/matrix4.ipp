#ifndef THREE_MATRIX4_IPP
#define THREE_MATRIX4_IPP

#include <three/core/matrix4.hpp>

namespace three {

Matrix4::Matrix4() {
  identity();
}

Matrix4::Matrix4( float n11, float n12, float n13, float n14,
                  float n21, float n22, float n23, float n24,
                  float n31, float n32, float n33, float n34,
                  float n41, float n42, float n43, float n44 ) {

  set( n11, n12, n13, n14,
       n21, n22, n23, n24,
       n31, n32, n33, n34,
       n41, n42, n43, n44 );

}

Matrix4::Matrix4( const Matrix4& other ) {
  copy( other );
}

Matrix4& Matrix4::operator= ( const Matrix4& other ) {
  return copy( other );
}

Matrix4& Matrix4::set( float n11, float n12, float n13, float n14,
                       float n21, float n22, float n23, float n24,
                       float n31, float n32, float n33, float n34,
                       float n41, float n42, float n43, float n44 ) {

  te[0] = n11; te[4] = n12; te[8]  = n13; te[12] = n14;
  te[1] = n21; te[5] = n22; te[9]  = n23; te[13] = n24;
  te[2] = n31; te[6] = n32; te[10] = n33; te[14] = n34;
  te[3] = n41; te[7] = n42; te[11] = n43; te[15] = n44;

  return *this;

}

Matrix4& Matrix4::identity() {

  return set( 1, 0, 0, 0,
              0, 1, 0, 0,
              0, 0, 1, 0,
              0, 0, 0, 1 );

}

Matrix4& Matrix4::copy( const Matrix4& m ) {

  const auto& me = m.elements;

  return set( me[0], me[4], me[8], me[12],
              me[1], me[5], me[9], me[13],
              me[2], me[6], me[10], me[14],
              me[3], me[7], me[11], me[15] );

}

Matrix4& Matrix4::lookAt( const Vector3& eye, const Vector3& target, const Vector3& up ) {

  auto z = sub( eye, target ).normalize();
  //auto z = sub( target, eye ).normalize();
  if ( z.isZero() ) {
    z.z = 1;
  }

  auto x = cross( up, z ).normalize();
  if ( x.isZero() ) {
    z.x += 0.0001f;
    x = cross( up, z ).normalize();
  }

  auto y = cross( z, x );

  te[0] = x.x; te[4] = y.x; te[8]  = z.x;
  te[1] = x.y; te[5] = y.y; te[9]  = z.y;
  te[2] = x.z; te[6] = y.z; te[10] = z.z;

  return *this;
}

/*Matrix4& Matrix4::multiply( const Matrix4& a, const Matrix4& b ) {

  const auto& ae = a.elements;
  const auto& be = b.elements;

  float a11 = ae[0], a12 = ae[4], a13 = ae[8], a14 = ae[12];
  float a21 = ae[1], a22 = ae[5], a23 = ae[9], a24 = ae[13];
  float a31 = ae[2], a32 = ae[6], a33 = ae[10], a34 = ae[14];
  float a41 = ae[3], a42 = ae[7], a43 = ae[11], a44 = ae[15];

  float b11 = be[0], b12 = be[4], b13 = be[8], b14 = be[12];
  float b21 = be[1], b22 = be[5], b23 = be[9], b24 = be[13];
  float b31 = be[2], b32 = be[6], b33 = be[10], b34 = be[14];
  float b41 = be[3], b42 = be[7], b43 = be[11], b44 = be[15];

  te[0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
  te[4] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
  te[8] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
  te[12] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

  te[1] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
  te[5] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
  te[9] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
  te[13] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

  te[2] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
  te[6] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
  te[10] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
  te[14] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

  te[3] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
  te[7] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
  te[11] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
  te[15] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

  return *this;
}*/

Matrix4& Matrix4::multiply( const Matrix4& am, const Matrix4& bm ) {

  const auto& a = am.elements;
  const auto& b = bm.elements;

  te[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8] * b[2]  + a[12] * b[3];
  te[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8] * b[6]  + a[12] * b[7];
  te[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8] * b[10] + a[12] * b[11];
  te[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];

  te[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9] * b[2]  + a[13] * b[3];
  te[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9] * b[6]  + a[13] * b[7];
  te[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9] * b[10] + a[13] * b[11];
  te[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];

  te[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
  te[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
  te[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
  te[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];

  te[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];
  te[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];
  te[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];
  te[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

  return *this;
}

Matrix4& Matrix4::multiplySelf( const Matrix4& m ) {

  return multiply( *this, m );

}

Matrix4& Matrix4::multiplyToArray( const Matrix4& a, const Matrix4& b, float* r ) {

  multiply( a, b );

  r[ 0 ] = te[0]; r[ 1 ] = te[1]; r[ 2 ] = te[2]; r[ 3 ] = te[3];
  r[ 4 ] = te[4]; r[ 5 ] = te[5]; r[ 6 ] = te[6]; r[ 7 ] = te[7];
  r[ 8 ]  = te[8]; r[ 9 ]  = te[9]; r[ 10 ] = te[10]; r[ 11 ] = te[11];
  r[ 12 ] = te[12]; r[ 13 ] = te[13]; r[ 14 ] = te[14]; r[ 15 ] = te[15];

  return *this;

}

Matrix4& Matrix4::multiplyScalar( float s ) {

  te[0] *= s; te[4] *= s; te[8] *= s; te[12] *= s;
  te[1] *= s; te[5] *= s; te[9] *= s; te[13] *= s;
  te[2] *= s; te[6] *= s; te[10] *= s; te[14] *= s;
  te[3] *= s; te[7] *= s; te[11] *= s; te[15] *= s;

  return *this;

}

Vector3 Matrix4::multiplyVector3( Vector3 v ) const {

  auto vx = v.x, vy = v.y, vz = v.z;
  auto d = 1.f / ( te[3] * vx + te[7] * vy + te[11] * vz + te[15] );

  v.x = ( te[0] * vx + te[4] * vy + te[8] * vz + te[12] ) * d;
  v.y = ( te[1] * vx + te[5] * vy + te[9] * vz + te[13] ) * d;
  v.z = ( te[2] * vx + te[6] * vy + te[10] * vz + te[14] ) * d;

  return v;

}

Vector4 Matrix4::multiplyVector4( Vector4 v ) const {

  float vx = v.x, vy = v.y, vz = v.z, vw = v.w;

  v.x = te[0] * vx + te[4] * vy + te[8] * vz + te[12] * vw;
  v.y = te[1] * vx + te[5] * vy + te[9] * vz + te[13] * vw;
  v.z = te[2] * vx + te[6] * vy + te[10] * vz + te[14] * vw;
  v.w = te[3] * vx + te[7] * vy + te[11] * vz + te[15] * vw;

  return v;

}

std::vector<float>& Matrix4::multiplyVector3Array( std::vector<float>& a ) const {

  Vector3 tmp;

  for ( size_t i = 0, il = a.size(); i < il; i += 3 ) {

    tmp.x = a[ i ];
    tmp.y = a[ i + 1 ];
    tmp.z = a[ i + 2 ];

    multiplyVector3( tmp );

    a[ i ]     = tmp.x;
    a[ i + 1 ] = tmp.y;
    a[ i + 2 ] = tmp.z;

  }

  return a;
}

Vector3 Matrix4::rotateAxis( Vector3 v ) const {

  auto vx = v.x, vy = v.y, vz = v.z;

  v.x = vx * te[0] + vy * te[4] + vz * te[8];
  v.y = vx * te[1] + vy * te[5] + vz * te[9];
  v.z = vx * te[2] + vy * te[6] + vz * te[10];

  v.normalize();

  return v;
}

Vector4 Matrix4::crossVector( Vector4 a ) const {

  Vector4 v;

  v.x = te[0] * a.x + te[4] * a.y + te[8] * a.z + te[12] * a.w;
  v.y = te[1] * a.x + te[5] * a.y + te[9] * a.z + te[13] * a.w;
  v.z = te[2] * a.x + te[6] * a.y + te[10] * a.z + te[14] * a.w;

  v.w = ( a.w ) ? te[3] * a.x + te[7] * a.y + te[11] * a.z + te[15] * a.w : 1;

  return v;
}

float Matrix4::determinant() const {

  auto n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  auto n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  auto n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  auto n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  //TODO: make this more efficient
  //( based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm )

  return (
           n14 * n23 * n32 * n41 -
           n13 * n24 * n32 * n41 -
           n14 * n22 * n33 * n41 +
           n12 * n24 * n33 * n41 +

           n13 * n22 * n34 * n41 -
           n12 * n23 * n34 * n41 -
           n14 * n23 * n31 * n42 +
           n13 * n24 * n31 * n42 +

           n14 * n21 * n33 * n42 -
           n11 * n24 * n33 * n42 -
           n13 * n21 * n34 * n42 +
           n11 * n23 * n34 * n42 +

           n14 * n22 * n31 * n43 -
           n12 * n24 * n31 * n43 -
           n14 * n21 * n32 * n43 +
           n11 * n24 * n32 * n43 +

           n12 * n21 * n34 * n43 -
           n11 * n22 * n34 * n43 -
           n13 * n22 * n31 * n44 +
           n12 * n23 * n31 * n44 +

           n13 * n21 * n32 * n44 -
           n11 * n23 * n32 * n44 -
           n12 * n21 * n33 * n44 +
           n11 * n22 * n33 * n44
         );

}

Matrix4& Matrix4::transpose() {

  auto tmp = te[1]; te[1] = te[4]; te[4] = tmp;
  tmp = te[2]; te[2] = te[8]; te[8] = tmp;
  tmp = te[6]; te[6] = te[9]; te[9] = tmp;

  tmp = te[3]; te[3] = te[12]; te[12] = tmp;
  tmp = te[7]; te[7] = te[13]; te[13] = tmp;
  tmp = te[11]; te[11] = te[14]; te[14] = tmp;

  return *this;

}

Vector3 Matrix4::getScale() const {
  auto sx = Vector3( te[0], te[1], te[2] ).length();
  auto sy = Vector3( te[4], te[5], te[6] ).length();
  auto sz = Vector3( te[8], te[9], te[10] ).length();
  return Vector3( sx, sy, sz );
}

Vector3 Matrix4::getPosition() const {
  return Vector3( te[12], te[13], te[14] );
}

Quaternion Matrix4::getRotation() const {

  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  float x, y, z, w;

  auto m11 = te[0];
  auto m12 = te[4];
  auto m13 = te[8];
  auto m21 = te[1];
  auto m22 = te[5];
  auto m23 = te[9];
  auto m31 = te[2];
  auto m32 = te[6];
  auto m33 = te[10];

  auto trace = m11 + m22 + m33;

  if ( trace > 0 ) {

    float s = 0.5f / Math::sqrt( trace + 1.0f );

    w = 0.25f / s;
    x = ( m32 - m23 ) * s;
    y = ( m13 - m31 ) * s;
    z = ( m21 - m12 ) * s;

  } else if ( m11 > m22 && m11 > m33 ) {

    float s = 2.0f * Math::sqrt( 1.0f + m11 - m22 - m33 );

    w = ( m32 - m23 ) / s;
    x = 0.25f * s;
    y = ( m12 + m21 ) / s;
    z = ( m13 + m31 ) / s;

  } else if ( m22 > m33 ) {

    float s = 2.0f * Math::sqrt( 1.0f + m22 - m11 - m33 );

    w = ( m13 - m31 ) / s;
    x = ( m12 + m21 ) / s;
    y = 0.25f * s;
    z = ( m23 + m32 ) / s;

  } else {

    float s = 2.0f * Math::sqrt( 1.0f + m33 - m11 - m22 );

    w = ( m21 - m12 ) / s;
    x = ( m13 + m31 ) / s;
    y = ( m23 + m32 ) / s;
    z = 0.25f * s;

  }

  return Quaternion( x, y, z, w );
}


Vector3 Matrix4::getEulerRotation( THREE::Order order /*= THREE::XYZ*/ ) const {
  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  // clamp, to handle numerical problems
  auto clamp = []( float x ) { return Math::clamp( x, -1.f, 1.f ); };

  auto m11 = te[0], m12 = te[4], m13 = te[8];
  auto m21 = te[1], m22 = te[5], m23 = te[9];
  auto m31 = te[2], m32 = te[6], m33 = te[10];

  auto x = 0.f, y = 0.f, z = 0.f;

  if ( order == THREE::XYZ ) {

    y = Math::asin( clamp( m13 ) );

    if ( Math::abs( m13 ) < 0.99999f ) {

      x = Math::atan2( - m23, m33 );
      z = Math::atan2( - m12, m11 );

    } else {

      x = Math::atan2( m21, m22 );
      z = 0;

    }

  } else if ( order == THREE::YXZ ) {

    x = Math::asin( - clamp( m23 ) );

    if ( Math::abs( m23 ) < 0.99999f ) {

      y = Math::atan2( m13, m33 );
      z = Math::atan2( m21, m22 );

    } else {

      y = Math::atan2( - m31, m11 );
      z = 0;

    }

  } else if ( order == THREE::ZXY ) {

    x = Math::asin( clamp( m32 ) );

    if ( Math::abs( m32 ) < 0.99999f ) {

      y = Math::atan2( - m31, m33 );
      z = Math::atan2( - m12, m22 );

    } else {

      y = 0;
      z = Math::atan2( m13, m11 );

    }

  } else if ( order == THREE::ZYX ) {

    y = Math::asin( - clamp( m31 ) );

    if ( Math::abs( m31 ) < 0.99999f ) {

      x = Math::atan2( m32, m33 );
      z = Math::atan2( m21, m11 );

    } else {

      x = 0;
      z = Math::atan2( - m12, m22 );

    }

  } else if ( order == THREE::YZX ) {

    z = Math::asin( clamp( m21 ) );

    if ( Math::abs( m21 ) < 0.99999f ) {

      x = Math::atan2( - m23, m22 );
      y = Math::atan2( - m31, m11 );

    } else {

      x = 0;
      y = Math::atan2( m31, m33 );

    }

  } else if ( order == THREE::XZY ) {

    z = Math::asin( - clamp( m12 ) );

    if ( Math::abs( m12 ) < 0.99999f ) {

      x = Math::atan2( m32, m22 );
      y = Math::atan2( m13, m11 );

    } else {

      x = Math::atan2( - m13, m33 );
      y = 0;

    }

  }

  return Vector3( x, y, z );

}



Vector4 Matrix4::getAxisAngle( ) const {
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  float angle, x, y, z, w; // variables for result
  float epsilon = 0.01f;   // margin to allow for rounding errors
  float epsilon2 = 0.1f;   // margin to distinguish between 0 and 180 degrees
  float m11 = te[0], m12 = te[4], m13 = te[8],
        m21 = te[1], m22 = te[5], m23 = te[9],
        m31 = te[2], m32 = te[6], m33 = te[10];

  if ( ( Math::abs( m12 - m21 ) < epsilon )
       && ( Math::abs( m13 - m31 ) < epsilon )
       && ( Math::abs( m23 - m32 ) < epsilon ) ) {

    // singularity found
    // first check for identity matrix which must have +1 for all terms
    // in leading diagonal and zero in other terms

    if ( ( Math::abs( m12 + m21 ) < epsilon2 )
         && ( Math::abs( m13 + m31 ) < epsilon2 )
         && ( Math::abs( m23 + m32 ) < epsilon2 )
         && ( Math::abs( m11 + m22 + m33 - 3 ) < epsilon2 ) ) {

      // this singularity is identity matrix so angle = 0
      return Vector4( 1, 0, 0, 0 );  // zero angle, arbitrary axis
    }

    // otherwise this singularity is angle = 180
    angle = Math::PI();

    auto xx = ( m11 + 1.f ) / 2.f;
    auto yy = ( m22 + 1.f ) / 2.f;
    auto zz = ( m33 + 1.f ) / 2.f;
    auto xy = ( m12 + m21 ) / 4.f;
    auto xz = ( m13 + m31 ) / 4.f;
    auto yz = ( m23 + m32 ) / 4.f;

    if ( ( xx > yy ) && ( xx > zz ) ) { // m11 is the largest diagonal term
      if ( xx < epsilon ) {
        x = 0;
        y = 0.707106781f;
        z = 0.707106781f;
      } else {
        x = Math::sqrt( xx );
        y = xy / x;
        z = xz / x;
      }
    } else if ( yy > zz ) { // m22 is the largest diagonal term
      if ( yy < epsilon ) {
        x = 0.707106781f;
        y = 0;
        z = 0.707106781f;
      } else {
        y = Math::sqrt( yy );
        x = xy / y;
        z = yz / y;
      }
    } else { // m33 is the largest diagonal term so base result on this
      if ( zz < epsilon ) {
        x = 0.707106781f;
        y = 0.707106781f;
        z = 0;
      } else {
        z = Math::sqrt( zz );
        x = xz / z;
        y = yz / z;
      }
    }

    return Vector4( x, y, z, angle );  // return 180 deg rotation
  }

  // as we have reached here there are no singularities so we can handle normally
  auto s = Math::sqrt( ( m32 - m23 ) * ( m32 - m23 )
                       + ( m13 - m31 ) * ( m13 - m31 )
                       + ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize

  if ( Math::abs( s ) < 0.001f ) s = 1;

  // prevent divide by zero, should not happen if matrix is orthogonal and should be
  // caught by singularity test above, but I've left it in just in case

  x = ( m32 - m23 ) / s;
  y = ( m13 - m31 ) / s;
  z = ( m21 - m12 ) / s;
  w = Math::acos( ( m11 + m22 + m33 - 1 ) / 2 );

  return Vector4( x, y, z, w );
}

Matrix4& Matrix4::setPosition( const Vector3& v ) {

  te[12] = v.x;
  te[13] = v.y;
  te[14] = v.z;

  return *this;

}

Vector3 Matrix4::getColumnX() const {
  return Vector3( te[0], te[1], te[2] );
}

Vector3 Matrix4::getColumnY() const {
  return Vector3( te[4], te[5], te[6] );
}

Vector3 Matrix4::getColumnZ() const {
  return Vector3( te[8], te[9], te[10] );
}

Matrix4& Matrix4::getInverse( const Matrix4& m ) {

  // based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
  const auto& me = m.elements;

  auto n11 = me[0], n12 = me[4], n13 = me[8], n14 = me[12];
  auto n21 = me[1], n22 = me[5], n23 = me[9], n24 = me[13];
  auto n31 = me[2], n32 = me[6], n33 = me[10], n34 = me[14];
  auto n41 = me[3], n42 = me[7], n43 = me[11], n44 = me[15];

  te[0] = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
  te[4] = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
  te[8] = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
  te[12] = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;
  te[1] = n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44;
  te[5] = n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44;
  te[9] = n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44;
  te[13] = n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34;
  te[2] = n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44;
  te[6] = n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44;
  te[10] = n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44;
  te[14] = n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34;
  te[3] = n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43;
  te[7] = n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43;
  te[11] = n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43;
  te[15] = n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33;
  multiplyScalar( 1 / m.determinant() );

  return *this;

}

Matrix4& Matrix4::setRotationFromEuler( const Vector3& v, THREE::Order order /*= THREE::XYZ*/ ) {

  auto x = v.x, y = v.y, z = v.z;
  auto a = Math::cos( x ), b = Math::sin( x );
  auto c = Math::cos( y ), d = Math::sin( y );
  auto e = Math::cos( z ), f = Math::sin( z );

  if ( order == THREE::XYZ ) {

    float ae = a * e, af = a * f, be = b * e, bf = b * f;

    te[0] = c * e;
    te[4] = - c * f;
    te[8] = d;

    te[1] = af + be * d;
    te[5] = ae - bf * d;
    te[9] = - b * c;

    te[2] = bf - ae * d;
    te[6] = be + af * d;
    te[10] = a * c;

  } else if ( order == THREE::YXZ ) {

    float ce = c * e, cf = c * f, de = d * e, df = d * f;

    te[0] = ce + df * b;
    te[4] = de * b - cf;
    te[8] = a * d;

    te[1] = a * f;
    te[5] = a * e;
    te[9] = - b;

    te[2] = cf * b - de;
    te[6] = df + ce * b;
    te[10] = a * c;

  } else if ( order == THREE::ZXY ) {

    auto ce = c * e, cf = c * f, de = d * e, df = d * f;

    te[0] = ce - df * b;
    te[4] = - a * f;
    te[8] = de + cf * b;

    te[1] = cf + de * b;
    te[5] = a * e;
    te[9] = df - ce * b;

    te[2] = - a * d;
    te[6] = b;
    te[10] = a * c;

  } else if ( order == THREE::ZYX ) {

    auto ae = a * e, af = a * f, be = b * e, bf = b * f;

    te[0] = c * e;
    te[4] = be * d - af;
    te[8] = ae * d + bf;

    te[1] = c * f;
    te[5] = bf * d + ae;
    te[9] = af * d - be;

    te[2] = - d;
    te[6] = b * c;
    te[10] = a * c;

  } else if ( order == THREE::YZX ) {

    auto ac = a * c, ad = a * d, bc = b * c, bd = b * d;

    te[0] = c * e;
    te[4] = bd - ac * f;
    te[8] = bc * f + ad;

    te[1] = f;
    te[5] = a * e;
    te[9] = - b * e;

    te[2] = - d * e;
    te[6] = ad * f + bc;
    te[10] = ac - bd * f;

  } else if ( order == THREE::XZY ) {

    auto ac = a * c, ad = a * d, bc = b * c, bd = b * d;

    te[0] = c * e;
    te[4] = - f;
    te[8] = d * e;

    te[1] = ac * f + bd;
    te[5] = a * e;
    te[9] = ad * f - bc;

    te[2] = bc * f - ad;
    te[6] = b * e;
    te[10] = bd * f + ac;

  }

  return *this;

}

Matrix4& Matrix4::setRotationFromQuaternion( const Quaternion& q ) {

  auto x = q.x, y = q.y, z = q.z, w = q.w;
  auto x2 = x + x, y2 = y + y, z2 = z + z;
  auto xx = x * x2, xy = x * y2, xz = x * z2;
  auto yy = y * y2, yz = y * z2, zz = z * z2;
  auto wx = w * x2, wy = w * y2, wz = w * z2;

  te[0] = 1.f - ( yy + zz );
  te[4] = xy - wz;
  te[8] = xz + wy;

  te[1] = xy + wz;
  te[5] = 1.f - ( xx + zz );
  te[9] = yz - wx;

  te[2] = xz - wy;
  te[6] = yz + wx;
  te[10] = 1.f - ( xx + yy );

  return *this;

}

Matrix4& Matrix4::compose( const Vector3& translation, const Quaternion& rotation, const Vector3& scale ) {

  Matrix4 tmpRotation;
  Matrix4 tmpScale;

  tmpRotation.identity();
  tmpRotation.setRotationFromQuaternion( rotation );

  tmpScale.makeScale( scale.x, scale.y, scale.z );

  multiply( tmpRotation, tmpScale );

  te[12] = translation.x;
  te[13] = translation.y;
  te[14] = translation.z;

  return *this;

}

void Matrix4::decompose( Vector3& translation, Quaternion& rotation, Vector3& scale ) {

  Vector3 x( te[0], te[1], te[2] );
  Vector3 y( te[4], te[5], te[6] );
  Vector3 z( te[8], te[9], te[10] );

  scale.x = x.length();
  scale.y = y.length();
  scale.z = z.length();

  translation.x = te[12];
  translation.y = te[13];
  translation.z = te[14];

  // scale the rotation part

  Matrix4 matrix( *this );

  matrix.elements[0] /= scale.x;
  matrix.elements[1] /= scale.x;
  matrix.elements[2] /= scale.x;

  matrix.elements[4] /= scale.y;
  matrix.elements[5] /= scale.y;
  matrix.elements[6] /= scale.y;

  matrix.elements[8] /= scale.z;
  matrix.elements[9] /= scale.z;
  matrix.elements[10] /= scale.z;

  rotation = matrix.getRotation();

  //return [ translation, rotation, scale ];

}

Matrix4& Matrix4::extractPosition( const Matrix4& m ) {

  auto me = m.elements;

  te[12] = me[12];
  te[13] = me[13];
  te[14] = me[14];

  return *this;

}

Matrix4& Matrix4::extractRotation( const Matrix4& m ) {

  auto me = m.elements;

  Vector3 vector;

  auto scaleX = 1.f / vector.set( me[0], me[1], me[2] ).length();
  auto scaleY = 1.f / vector.set( me[4], me[5], me[6] ).length();
  auto scaleZ = 1.f / vector.set( me[8], me[9], me[10] ).length();

  te[0] = me[0] * scaleX;
  te[1] = me[1] * scaleX;
  te[2] = me[2] * scaleX;

  te[4] = me[4] * scaleY;
  te[5] = me[5] * scaleY;
  te[6] = me[6] * scaleY;

  te[8] = me[8] * scaleZ;
  te[9] = me[9] * scaleZ;
  te[10] = me[10] * scaleZ;

  return *this;

}

//

Matrix4& Matrix4::translate( const Vector3& v ) {

  auto x = v.x, y = v.y, z = v.z;

  te[12] = te[0] * x + te[4] * y + te[8] * z + te[12];
  te[13] = te[1] * x + te[5] * y + te[9] * z + te[13];
  te[14] = te[2] * x + te[6] * y + te[10] * z + te[14];
  te[15] = te[3] * x + te[7] * y + te[11] * z + te[15];

  return *this;

}

Matrix4& Matrix4::rotateX( float angle ) {

  auto m12 = te[4];
  auto m22 = te[5];
  auto m32 = te[6];
  auto m42 = te[7];
  auto m13 = te[8];
  auto m23 = te[9];
  auto m33 = te[10];
  auto m43 = te[11];
  auto c = Math::cos( angle );
  auto s = Math::sin( angle );

  te[4] = c * m12 + s * m13;
  te[5] = c * m22 + s * m23;
  te[6] = c * m32 + s * m33;
  te[7] = c * m42 + s * m43;

  te[8] = c * m13 - s * m12;
  te[9] = c * m23 - s * m22;
  te[10] = c * m33 - s * m32;
  te[11] = c * m43 - s * m42;

  return *this;

}

Matrix4& Matrix4::rotateY( float angle ) {

  auto m11 = te[0];
  auto m21 = te[1];
  auto m31 = te[2];
  auto m41 = te[3];
  auto m13 = te[8];
  auto m23 = te[9];
  auto m33 = te[10];
  auto m43 = te[11];
  auto c = Math::cos( angle );
  auto s = Math::sin( angle );

  te[0] = c * m11 - s * m13;
  te[1] = c * m21 - s * m23;
  te[2] = c * m31 - s * m33;
  te[3] = c * m41 - s * m43;

  te[8] = c * m13 + s * m11;
  te[9] = c * m23 + s * m21;
  te[10] = c * m33 + s * m31;
  te[11] = c * m43 + s * m41;

  return *this;

}

Matrix4& Matrix4::rotateZ( float angle ) {

  auto m11 = te[0];
  auto m21 = te[1];
  auto m31 = te[2];
  auto m41 = te[3];
  auto m12 = te[4];
  auto m22 = te[5];
  auto m32 = te[6];
  auto m42 = te[7];
  auto c = Math::cos( angle );
  auto s = Math::sin( angle );

  te[0] = c * m11 + s * m12;
  te[1] = c * m21 + s * m22;
  te[2] = c * m31 + s * m32;
  te[3] = c * m41 + s * m42;

  te[4] = c * m12 - s * m11;
  te[5] = c * m22 - s * m21;
  te[6] = c * m32 - s * m31;
  te[7] = c * m42 - s * m41;

  return *this;

}

Matrix4& Matrix4::rotateByAxis( const Vector3& axis, float angle ) {

  if ( axis.x == 1 && axis.y == 0 && axis.z == 0 ) {
    return rotateX( angle );
  } else if ( axis.x == 0 && axis.y == 1 && axis.z == 0 ) {
    return rotateY( angle );
  } else if ( axis.x == 0 && axis.y == 0 && axis.z == 1 ) {
    return rotateZ( angle );
  }

  auto x = axis.x, y = axis.y, z = axis.z;
  auto n = Math::sqrt( x * x + y * y + z * z );

  x /= n;
  y /= n;
  z /= n;

  auto xx = x * x, yy = y * y, zz = z * z;
  auto c = Math::cos( angle );
  auto s = Math::sin( angle );
  auto oneMinusCosine = 1.f - c;
  auto xy = x * y * oneMinusCosine;
  auto xz = x * z * oneMinusCosine;
  auto yz = y * z * oneMinusCosine;
  auto xs = x * s;
  auto ys = y * s;
  auto zs = z * s;

  auto r11 = xx + ( 1.f - xx ) * c;
  auto r21 = xy + zs;
  auto r31 = xz - ys;
  auto r12 = xy - zs;
  auto r22 = yy + ( 1.f - yy ) * c;
  auto r32 = yz + xs;
  auto r13 = xz + ys;
  auto r23 = yz - xs;
  auto r33 = zz + ( 1.f - zz ) * c;

  auto m11 = te[0], m21 = te[1], m31 = te[2], m41 = te[3];
  auto m12 = te[4], m22 = te[5], m32 = te[6], m42 = te[7];
  auto m13 = te[8], m23 = te[9], m33 = te[10], m43 = te[11];
  //auto m14 = te[12], m24 = te[13], m34 = te[14], m44 = te[15];

  te[0] = r11 * m11 + r21 * m12 + r31 * m13;
  te[1] = r11 * m21 + r21 * m22 + r31 * m23;
  te[2] = r11 * m31 + r21 * m32 + r31 * m33;
  te[3] = r11 * m41 + r21 * m42 + r31 * m43;

  te[4] = r12 * m11 + r22 * m12 + r32 * m13;
  te[5] = r12 * m21 + r22 * m22 + r32 * m23;
  te[6] = r12 * m31 + r22 * m32 + r32 * m33;
  te[7] = r12 * m41 + r22 * m42 + r32 * m43;

  te[8] = r13 * m11 + r23 * m12 + r33 * m13;
  te[9] = r13 * m21 + r23 * m22 + r33 * m23;
  te[10] = r13 * m31 + r23 * m32 + r33 * m33;
  te[11] = r13 * m41 + r23 * m42 + r33 * m43;

  return *this;

}

Matrix4& Matrix4::scale( const Vector3& v ) {

  auto x = v.x, y = v.y, z = v.z;

  te[0] *= x; te[4] *= y; te[8] *= z;
  te[1] *= x; te[5] *= y; te[9] *= z;
  te[2] *= x; te[6] *= y; te[10] *= z;
  te[3] *= x; te[7] *= y; te[11] *= z;

  return *this;

}

THREE_DECL float Matrix4::getMaxScaleOnAxis() const {

  auto scaleXSq =  te[0] * te[0] + te[1] * te[1] + te[2] * te[2];
  auto scaleYSq =  te[4] * te[4] + te[5] * te[5] + te[6] * te[6];
  auto scaleZSq =  te[8] * te[8] + te[9] * te[9] + te[10] * te[10];

  return Math::sqrt( Math::max( scaleXSq, Math::max( scaleYSq, scaleZSq ) ) );

}

//

Matrix4& Matrix4::makeTranslation( float x, float y, float z ) {

  set( 1, 0, 0, x,
       0, 1, 0, y,
       0, 0, 1, z,
       0, 0, 0, 1 );

  return *this;

}

Matrix4& Matrix4::makeRotationX( float theta ) {

  auto c = Math::cos( theta ), s = Math::sin( theta );

  set( 1, 0,  0, 0,
       0, c, -s, 0,
       0, s,  c, 0,
       0, 0,  0, 1 );

  return *this;

}

Matrix4& Matrix4::makeRotationY( float theta ) {

  auto c = Math::cos( theta ), s = Math::sin( theta );

  set( c, 0, s, 0,
       0, 1, 0, 0,
       -s, 0, c, 0,
       0, 0, 0, 1 );

  return *this;

}

Matrix4& Matrix4::makeRotationZ( float theta ) {

  float c = Math::cos( theta ), s = Math::sin( theta );

  set( c, -s, 0, 0,
       s,  c, 0, 0,
       0,  0, 1, 0,
       0,  0, 0, 1 );

  return *this;

}

Matrix4& Matrix4::makeRotationAxis( const Vector3& axis, float angle ) {

  // Based on http://www.gamedev.net/reference/articles/article1199.asp

  auto c = Math::cos( angle );
  auto s = Math::sin( angle );
  auto t = 1.f - c;
  auto x = axis.x, y = axis.y, z = axis.z;
  auto tx = t * x, ty = t * y;

  set( tx * x + c, tx * y - s * z, tx * z + s * y, 0,
       tx * y + s * z, ty * y + c, ty * z - s * x, 0,
       tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
       0, 0, 0, 1 );

  return *this;

}

Matrix4& Matrix4::makeScale( float x, float y, float z ) {

  set( x, 0, 0, 0,
       0, y, 0, 0,
       0, 0, z, 0,
       0, 0, 0, 1 );

  return *this;

}

Matrix4& Matrix4::makeFrustum( float left, float right, float bottom, float top, float near, float far ) {

  auto x = 2.f * near / ( right - left );
  auto y = 2.f * near / ( top - bottom );

  auto a = ( right + left ) / ( right - left );
  auto b = ( top + bottom ) / ( top - bottom );
  auto c = - ( far + near ) / ( far - near );
  auto d = - 2.f * far * near / ( far - near );

  te[0] = x;  te[4] = 0;  te[8] = a;   te[12] = 0;
  te[1] = 0;  te[5] = y;  te[9] = b;   te[13] = 0;
  te[2] = 0;  te[6] = 0;  te[10] = c;   te[14] = d;
  te[3] = 0;  te[7] = 0;  te[11] = - 1; te[15] = 0;

  return *this;

}

Matrix4& Matrix4::makePerspective( float fov, float aspect, float near, float far ) {

  auto ymax = near * Math::tan( fov * Math::PI() / 360 );
  auto ymin = - ymax;
  auto xmin = ymin * aspect;
  auto xmax = ymax * aspect;

  return makeFrustum( xmin, xmax, ymin, ymax, near, far );

}

Matrix4& Matrix4::makeOrthographic( float left, float right, float top, float bottom, float near, float far ) {

  auto w = right - left;
  auto h = top - bottom;
  auto p = far - near;

  auto x = ( right + left ) / w;
  auto y = ( top + bottom ) / h;
  auto z = ( far + near ) / p;

  te[0] = 2 / w; te[4] = 0;     te[8] = 0;      te[12] = -x;
  te[1] = 0;     te[5] = 2 / h; te[9] = 0;      te[13] = -y;
  te[2] = 0;     te[6] = 0;     te[10] = -2 / p; te[14] = -z;
  te[3] = 0;     te[7] = 0;     te[11] = 0;      te[15] = 1;

  return *this;

}

} // namespace three

#endif // THREE_MATRIX4_IPP