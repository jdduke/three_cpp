#include <three/math/quaternion.h>

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/euler.h>
#include <three/math/matrix4.h>
#include <three/math/vector3.h>

namespace three {

Quaternion& Quaternion::set( const float xIn, const float yIn, const float zIn, const float wIn ) {

  x = xIn;
  y = yIn;
  z = zIn;
  w = wIn;

  return *this;

}

Quaternion& Quaternion::copy( const Quaternion& quaternion ) {

  x = quaternion.x;
  y = quaternion.y;
  z = quaternion.z;
  w = quaternion.w;

  return *this;

}

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

  if ( euler.order() == THREE::EulerRotationOrder::XYZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == THREE::EulerRotationOrder::YXZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == THREE::EulerRotationOrder::ZXY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == THREE::EulerRotationOrder::ZYX ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == THREE::EulerRotationOrder::YZX ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == THREE::EulerRotationOrder::XZY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  }

  return *this;

}

Quaternion& Quaternion::setFromAxisAngle( const Vector3& axis, const float angle ) {

  // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
  // axis have to be normalized

  auto halfAngle = angle / 2.f, s = Math::sin( halfAngle );

  x = axis.x * s;
  y = axis.y * s;
  z = axis.z * s;
  w = Math::cos( halfAngle );

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

Quaternion& Quaternion::inverse() {

  conjugate().normalize();

  return *this;

}

Quaternion& Quaternion::conjugate() {

  x *= -1;
  y *= -1;
  z *= -1;

  return *this;

}

float Quaternion::lengthSq() const {

  return x * x + y * y + z * z + w * w;

}

float Quaternion::length() const {

  return Math::sqrt( lengthSq() );

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

Quaternion& Quaternion::multiply( const Quaternion& q ) {

  return multiplyQuaternions( *this, q );

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

bool Quaternion::equals( const Quaternion& quaternion ) const {
  return ( quaternion.x == x ) && ( quaternion.y == y ) && ( quaternion.z == z ) && ( quaternion.w == w );
}

Quaternion Quaternion::clone() const {
  return *this;
}

} // namespace three
