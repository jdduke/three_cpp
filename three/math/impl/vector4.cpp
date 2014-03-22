#ifndef THREE_VECTOR4_CPP
#define THREE_VECTOR4_CPP

#include <three/math/vector3.h>
#include <three/math/vector4.h>
#include <three/math/matrix4.h>
#include <three/math/quaternion.h>

namespace three {



Vector4& Vector4::applyMatrix4( const Matrix4& m ) {

  auto e = m.elements;

  x = e[0] * x + e[4] * y + e[8] * z + e[12] * w;
  y = e[1] * x + e[5] * y + e[9] * z + e[13] * w;
  z = e[2] * x + e[6] * y + e[10] * z + e[14] * w;
  w = e[3] * x + e[7] * y + e[11] * z + e[15] * w;

  return *this;
}



Vector4& Vector4::setAxisAngleFromQuaternion( const Quaternion& q ) {
  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
  // q is assumed to be normalized
  w = 2 * Math::acos( q.w() );

  auto s = Math::sqrt( 1.f - q.w() * q.w() );

  if ( s < 0.0001f ) {
    x = 1.f;
    y = 0.f;
    z = 0.f;
  } else {
    x = q.x() / s;
    y = q.y() / s;
    z = q.z() / s;
  }
  return *this;
}

Vector4& Vector4::setAxisAngleFromRotationMatrix( const Matrix4& m ) {

  // @todo r65 mem optimization

  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm

  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  float x, y, z;   // variables for result
  float epsilon = 0.01f;   // margin to allow for rounding errors
  float epsilon2 = 0.1f;   // margin to distinguish between 0 and 180 degrees

  auto te = m.elements;

  auto m11 = te[0], m12 = te[4], m13 = te[8];
  auto m21 = te[1], m22 = te[5], m23 = te[9];
  auto m31 = te[2], m32 = te[6], m33 = te[10];

  if ( ( Math::abs( m12 - m21 ) < epsilon )
       && ( Math::abs( m13 - m31 ) < epsilon )
       && ( Math::abs( m23 - m32 ) < epsilon ) ) {

    // singularity found
    // first check for identity matrix which must have +1 for all terms
    // in leading diagonal and zero in other terms

    if ( ( Math::abs( m12 + m21 ) < epsilon2 )
         && ( Math::abs( m13 + m31 ) < epsilon2 )
         && ( Math::abs( m23 + m32 ) < epsilon2 )
         && ( Math::abs( m11 + m22 + m33 - 3.f ) < epsilon2 ) ) {

      // this singularity is identity matrix so angle = 0

      set( 1.f, 0.f, 0.f, 0.f );

      return *this; // zero angle, arbitrary axis

    }

    // otherwise this singularity is angle = 180

    float angle = Math::PI();

    auto xx = ( m11 + 1 ) / 2.f;
    auto yy = ( m22 + 1 ) / 2.f;
    auto zz = ( m33 + 1 ) / 2.f;
    auto xy = ( m12 + m21 ) / 4.f;
    auto xz = ( m13 + m31 ) / 4.f;
    auto yz = ( m23 + m32 ) / 4.f;

    if ( ( xx > yy ) && ( xx > zz ) ) { // m11 is the largest diagonal term

      if ( xx < epsilon ) {

        x = 0.f;
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
        y = 0.f;
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
        z = 0.f;

      } else {

        z = Math::sqrt( zz );
        x = xz / z;
        y = yz / z;

      }

    }

    set( x, y, z, angle );

    return *this; // return 180 deg rotation

  }

  // as we have reached here there are no singularities so we can handle normally

  float s = Math::sqrt( ( m32 - m23 ) * ( m32 - m23 )
                        + ( m13 - m31 ) * ( m13 - m31 )
                        + ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize

  if ( Math::abs( s ) < 0.001 ) s = 1.f;

  // prevent divide by zero, should not happen if matrix is orthogonal and should be
  // caught by singularity test above, but I've left it in just in case

  x = ( m32 - m23 ) / s;
  y = ( m13 - m31 ) / s;
  z = ( m21 - m12 ) / s;
  w = Math::acos( ( m11 + m22 + m33 - 1 ) / 2.f );

  return *this;
}

} // namespace three

#endif // THREE_VECTOR4_CPP