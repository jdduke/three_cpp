#include <three/math/quaternion.h>

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/euler.h>
#include <three/math/matrix4.h>

namespace three {

Quaternion& Quaternion::setFromEuler( const Euler& euler ) {

  // http://www.mathworks.com/matlabcentral/fileexchange/
  //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
  //  content/SpinCalc.m

  auto c1 = Math::cos( euler.x / 2 );
  auto c2 = Math::cos( euler.y / 2 );
  auto c3 = Math::cos( euler.z / 2 );
  auto s1 = Math::sin( euler.x / 2 );
  auto s2 = Math::sin( euler.y / 2 );
  auto s3 = Math::sin( euler.z / 2 );

  if ( euler.order() == enums::EulerRotationOrder::XYZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::YXZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::ZXY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::ZYX ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::YZX ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::XZY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  }

  return *this;

}

Quaternion& Quaternion::setFromRotationMatrix( const Matrix4& m ) {

  // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

  // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  const auto& te = m.elements;

  auto m11 = te[0], m12 = te[4], m13 = te[8],
       m21 = te[1], m22 = te[5], m23 = te[9],
       m31 = te[2], m32 = te[6], m33 = te[10];

  auto trace = m11 + m22 + m33;
  float s;

  if ( trace > 0.f ) {

    s = 0.5f / Math::sqrt( trace + 1.0f );

    w = 0.25f / s;
    x = ( m32 - m23 ) * s;
    y = ( m13 - m31 ) * s;
    z = ( m21 - m12 ) * s;

  } else if ( m11 > m22 && m11 > m33 ) {

    s = 2.0f * Math::sqrt( 1.0f + m11 - m22 - m33 );

    w = (m32 - m23 ) / s;
    x = 0.25f * s;
    y = (m12 + m21 ) / s;
    z = (m13 + m31 ) / s;

  } else if ( m22 > m33 ) {

    s = 2.0f * Math::sqrt( 1.0f + m22 - m11 - m33 );

    w = (m13 - m31 ) / s;
    x = (m12 + m21 ) / s;
    y = 0.25f * s;
    z = (m23 + m32 ) / s;

  } else {

    s = 2.0f * Math::sqrt( 1.0f + m33 - m11 - m22 );

    w = ( m21 - m12 ) / s;
    x = ( m13 + m31 ) / s;
    y = ( m23 + m32 ) / s;
    z = 0.25f * s;

  }

  return *this;

}

Quaternion& Quaternion::normalize() {

  auto l = length();

  if ( l == 0.f ) {

    x = 0;
    y = 0;
    z = 0;
    w = 1;

  } else {

    l = 1.f / l;

    x = x * l;
    y = y * l;
    z = z * l;
    w = w * l;

  }

  return *this;
}

Quaternion& Quaternion::multiplyQuaternions( const Quaternion& a, const Quaternion& b ) {

  // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm

  auto qax = a.x, qay = a.y, qaz = a.z, qaw = a.w;
  auto qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

  x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
  y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
  z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
  w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

  return *this;

}

Quaternion& Quaternion::slerp( const Quaternion& qb, float t ) {

  auto x = this->x, y = this->y, z = this->z, w = this->w;

  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

  auto cosHalfTheta = w * qb.w + x * qb.x + y * qb.y + z * qb.z;

  if ( cosHalfTheta < 0.f ) {

    this->w = -qb.w;
    this->x = -qb.x;
    this->y = -qb.y;
    this->z = -qb.z;

    cosHalfTheta = -cosHalfTheta;

  } else {

    copy( qb );

  }

  if ( cosHalfTheta >= 1.0f ) {

    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;

    return *this;

  }

  auto halfTheta = Math::acos( cosHalfTheta );
  auto sinHalfTheta = Math::sqrt( 1.0f - cosHalfTheta * cosHalfTheta );

  if ( Math::abs( sinHalfTheta ) < 0.001f ) {

    this->w = 0.5f * ( w + this->w );
    this->x = 0.5f * ( x + this->x );
    this->y = 0.5f * ( y + this->y );
    this->z = 0.5f * ( z + this->z );

    return *this;

  }

  auto ratioA = Math::sin( ( 1.f - t ) * halfTheta ) / sinHalfTheta,
       ratioB = Math::sin( t * halfTheta ) / sinHalfTheta;

  this->w = ( w * ratioA + this->w * ratioB );
  this->x = ( x * ratioA + this->x * ratioB );
  this->y = ( y * ratioA + this->y * ratioB );
  this->z = ( z * ratioA + this->z * ratioB );

  return *this;

}

} // namespace three
