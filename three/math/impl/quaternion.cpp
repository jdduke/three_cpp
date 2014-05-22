#ifndef THREE_QUATERNION_CPP
#define THREE_QUATERNION_CPP

#include <three/common.h>

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/euler.h>
#include <three/math/matrix4.h>
#include <three/math/quaternion.h>

namespace three {

Quaternion& Quaternion::setFromEuler( const Euler& euler, bool update ) {

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

    x.value = s1 * c2 * c3 + c1 * s2 * s3;
    y.value = c1 * s2 * c3 - s1 * c2 * s3;
    z.value = c1 * c2 * s3 + s1 * s2 * c3;
    w.value = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::YXZ ) {

    x.value = s1 * c2 * c3 + c1 * s2 * s3;
    y.value = c1 * s2 * c3 - s1 * c2 * s3;
    z.value = c1 * c2 * s3 - s1 * s2 * c3;
    w.value = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::ZXY ) {

    x.value = s1 * c2 * c3 - c1 * s2 * s3;
    y.value = c1 * s2 * c3 + s1 * c2 * s3;
    z.value = c1 * c2 * s3 + s1 * s2 * c3;
    w.value = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::ZYX ) {

    x.value = s1 * c2 * c3 - c1 * s2 * s3;
    y.value = c1 * s2 * c3 + s1 * c2 * s3;
    z.value = c1 * c2 * s3 - s1 * s2 * c3;
    w.value = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::YZX ) {

    x.value = s1 * c2 * c3 + c1 * s2 * s3;
    y.value = c1 * s2 * c3 + s1 * c2 * s3;
    z.value = c1 * c2 * s3 - s1 * s2 * c3;
    w.value = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( euler.order() == enums::EulerRotationOrder::XZY ) {

    x.value = s1 * c2 * c3 - c1 * s2 * s3;
    y.value = c1 * s2 * c3 - s1 * c2 * s3;
    z.value = c1 * c2 * s3 + s1 * s2 * c3;
    w.value = c1 * c2 * c3 + s1 * s2 * s3;

  }

  if ( update ) _updateEuler();

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

    w.value = 0.25f / s;
    x.value = ( m32 - m23 ) * s;
    y.value = ( m13 - m31 ) * s;
    z.value = ( m21 - m12 ) * s;

  } else if ( m11 > m22 && m11 > m33 ) {

    s = 2.0f * Math::sqrt( 1.0f + m11 - m22 - m33 );

    w.value = (m32 - m23 ) / s;
    x.value = 0.25f * s;
    y.value = (m12 + m21 ) / s;
    z.value = (m13 + m31 ) / s;

  } else if ( m22 > m33 ) {

    s = 2.0f * Math::sqrt( 1.0f + m22 - m11 - m33 );

    w.value = (m13 - m31 ) / s;
    x.value = (m12 + m21 ) / s;
    y.value = 0.25f * s;
    z.value = (m23 + m32 ) / s;

  } else {

    s = 2.0f * Math::sqrt( 1.0f + m33 - m11 - m22 );

    w.value = ( m21 - m12 ) / s;
    x.value = ( m13 + m31 ) / s;
    y.value = ( m23 + m32 ) / s;
    z.value = 0.25f * s;

  }

  _updateEuler();

  return *this;

}

Quaternion& Quaternion::normalize() {

  auto l = length();

  if ( l == 0.f ) {

    x.value = 0;
    y.value = 0;
    z.value = 0;
    w.value = 1;

  } else {

    l = 1.f / l;

    x.value = x.value * l;
    y.value = y.value * l;
    z.value = z.value * l;
    w.value = w.value * l;

  }

  return *this;
}

Quaternion& Quaternion::multiplyQuaternions( const Quaternion& a, const Quaternion& b ) {

  // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm

  auto qax = a.x.value, qay = a.y.value, qaz = a.z.value, qaw = a.w.value;
  auto qbx = b.x.value, qby = b.y.value, qbz = b.z.value, qbw = b.w.value;

  x.value = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
  y.value = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
  z.value = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
  w.value = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

  _updateEuler();

  return *this;

}

Quaternion& Quaternion::slerp( const Quaternion& qb, float t ) {

  auto x = this->x.value, y = this->y.value, z = this->z.value, w = this->w.value;

  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

  auto cosHalfTheta = w * qb.w.value + x * qb.x.value + y * qb.y.value + z * qb.z.value;

  if ( cosHalfTheta < 0.f ) {

    this->w.value = -qb.w.value;
    this->x.value = -qb.x.value;
    this->y.value = -qb.y.value;
    this->z.value = -qb.z.value;

    cosHalfTheta = -cosHalfTheta;

  } else {

    copy( qb );

  }

  if ( cosHalfTheta >= 1.0f ) {

    this->w.value = w;
    this->x.value = x;
    this->y.value = y;
    this->z.value = z;

    return *this;

  }

  auto halfTheta = Math::acos( cosHalfTheta );
  auto sinHalfTheta = Math::sqrt( 1.0f - cosHalfTheta * cosHalfTheta );

  if ( Math::abs( sinHalfTheta ) < 0.001f ) {

    this->w.value = 0.5f * ( w + this->w.value );
    this->x.value = 0.5f * ( x + this->x.value );
    this->y.value = 0.5f * ( y + this->y.value );
    this->z.value = 0.5f * ( z + this->z.value );

    return *this;

  }

  auto ratioA = Math::sin( ( 1.f - t ) * halfTheta ) / sinHalfTheta,
       ratioB = Math::sin( t * halfTheta ) / sinHalfTheta;

  this->w.value = ( w * ratioA + this->w.value * ratioB );
  this->x.value = ( x * ratioA + this->x.value * ratioB );
  this->y.value = ( y * ratioA + this->y.value * ratioB );
  this->z.value = ( z * ratioA + this->z.value * ratioB );

  _updateEuler();

  return *this;

}

void Quaternion::_updateEuler() {
  if(_euler) {
    _euler->setFromQuaternion( *this, false );
  }
}

} // namespace three

#endif // THREE_QUATERNION_CPP