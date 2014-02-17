#ifndef THREE_QUATERNION_IPP
#define THREE_QUATERNION_IPP

#include <three/math/quaternion.hpp>

#include <three/constants.hpp>
#include <three/math/math.hpp>

namespace three {

Vector3 Quaternion::getEuler( THREE::EulerRotationOrder order /*= THREE::XYZ*/ ) const {
  // assumed to be normalized

  // clamp, to handle numerical problems
  auto clamp = []( float x ) {
    return Math::min( Math::max( x, -1.f ), 1.f );
  };

  auto sqx = x * x;
  auto sqy = y * y;
  auto sqz = z * z;
  auto sqw = w * w;

  Vector3 euler;

  if ( order == THREE::XYZ ) {
    euler.x = Math::atan2( 2.f * ( x * w - y * z ), ( sqw - sqx - sqy + sqz ) );
    euler.y = Math::asin( clamp( 2.f * ( x * z + y * w ) ) );
    euler.z = Math::atan2( 2.f * ( z * w - x * y ), ( sqw + sqx - sqy - sqz ) );
  } else if ( order == THREE::YXZ ) {
    euler.x = Math::asin( clamp( 2.f * ( x * w - y * z ) ) );
    euler.y = Math::atan2( 2.f * ( x * z + y * w ), ( sqw - sqx - sqy + sqz ) );
    euler.z = Math::atan2( 2.f * ( x * y + z * w ), ( sqw - sqx + sqy - sqz ) );
  } else if ( order == THREE::ZXY ) {
    euler.x = Math::asin( clamp( 2.f * ( x * w + y * z ) ) );
    euler.y = Math::atan2( 2.f * ( y * w - z * x ), ( sqw - sqx - sqy + sqz ) );
    euler.z = Math::atan2( 2.f * ( z * w - x * y ), ( sqw - sqx + sqy - sqz ) );
  } else if ( order == THREE::ZYX ) {
    euler.x = Math::atan2( 2.f * ( x * w + z * y ), ( sqw - sqx - sqy + sqz ) );
    euler.y = Math::asin( clamp( 2.f * ( y * w - x * z ) ) );
    euler.z = Math::atan2( 2.f * ( x * y + z * w ), ( sqw + sqx - sqy - sqz ) );
  } else if ( order == THREE::YZX ) {
    euler.x = Math::atan2( 2.f * ( x * w - z * y ), ( sqw - sqx + sqy - sqz ) );
    euler.y = Math::atan2( 2.f * ( y * w - x * z ), ( sqw + sqx - sqy - sqz ) );
    euler.z = Math::asin( clamp( 2.f * ( x * y + z * w ) ) );
  } else if ( order == THREE::XZY ) {
    euler.x = Math::atan2( 2.f * ( x * w + y * z ), ( sqw - sqx + sqy - sqz ) );
    euler.y = Math::atan2( 2.f * ( x * z + y * w ), ( sqw + sqx - sqy - sqz ) );
    euler.z = Math::asin( clamp( 2.f * ( z * w - x * y ) ) );
  }

  return euler;
}

Quaternion& Quaternion::setFromEuler( const Vector3& v, THREE::EulerRotationOrder order /*= THREE::XYZ*/ ) {

  // http://www.mathworks.com/matlabcentral/fileexchange/
  //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
  //  content/SpinCalc.m

  auto c1 = Math::cos( v.x / 2 );
  auto c2 = Math::cos( v.y / 2 );
  auto c3 = Math::cos( v.z / 2 );
  auto s1 = Math::sin( v.x / 2 );
  auto s2 = Math::sin( v.y / 2 );
  auto s3 = Math::sin( v.z / 2 );

  if ( order == THREE::XYZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::YXZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( order == THREE::ZXY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::ZYX ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( order == THREE::YZX ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::XZY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  }

  return *this;
}

Quaternion& Quaternion::setFromEuler( const Euler& v, THREE::EulerRotationOrder order /*= THREE::XYZ*/ ) {

  // http://www.mathworks.com/matlabcentral/fileexchange/
  //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
  //  content/SpinCalc.m

  auto c1 = Math::cos( v.x / 2 );
  auto c2 = Math::cos( v.y / 2 );
  auto c3 = Math::cos( v.z / 2 );
  auto s1 = Math::sin( v.x / 2 );
  auto s2 = Math::sin( v.y / 2 );
  auto s3 = Math::sin( v.z / 2 );

  if ( order == THREE::XYZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::YXZ ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( order == THREE::ZXY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::ZYX ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  } else if ( order == THREE::YZX ) {

    x = s1 * c2 * c3 + c1 * s2 * s3;
    y = c1 * s2 * c3 + s1 * c2 * s3;
    z = c1 * c2 * s3 - s1 * s2 * c3;
    w = c1 * c2 * c3 - s1 * s2 * s3;

  } else if ( order == THREE::XZY ) {

    x = s1 * c2 * c3 - c1 * s2 * s3;
    y = c1 * s2 * c3 - s1 * c2 * s3;
    z = c1 * c2 * s3 + s1 * s2 * c3;
    w = c1 * c2 * c3 + s1 * s2 * s3;

  }

  return *this;
}

Quaternion& Quaternion::setFromAxisAngle( const Vector3& axis, float angle ) {
  // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
  // axis have to be normalized

  float halfAngle = angle / 2.f;
  float s = Math::sin( halfAngle );

  x = axis.x * s;
  y = axis.y * s;
  z = axis.z * s;
  w = Math::cos( halfAngle );

  return *this;
}

Quaternion& Quaternion::calculateW() {
  w = - Math::sqrt( Math::abs( 1.f - x * x - y * y - z * z ) );
  return *this;
}

Quaternion& Quaternion::inverse() {
  x *= -1.f;
  y *= -1.f;
  z *= -1.f;
  return *this;
}

float Quaternion::length() const {
  return Math::sqrt( lengthSq() );
}

Quaternion& Quaternion::normalize() {
  return divideScalar( length() );
}

Quaternion& Quaternion::multiply( const Quaternion& a, const Quaternion& b ) {
  // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
  float qax = a.x, qay = a.y, qaz = a.z, qaw = a.w,
        qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

  x =  qax * qbw + qay * qbz - qaz * qby + qaw * qbx;
  y = -qax * qbz + qay * qbw + qaz * qbx + qaw * qby;
  z =  qax * qby - qay * qbx + qaz * qbw + qaw * qbz;
  w = -qax * qbx - qay * qby - qaz * qbz + qaw * qbw;

  return *this;
}

Quaternion& Quaternion::multiplySelf( const Quaternion& b ) {
  float qax = x,   qay = y,   qaz = z,   qaw = w,
        qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

  x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
  y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
  z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
  w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

  return *this;
}

Vector3 Quaternion::multiplyVector3( const Vector3& v ) {
  float qx = x, qy = y, qz = z, qw = w;

  // calculate quat * vector
  float ix =  qw * v.x + qy * v.z - qz * v.y,
        iy =  qw * v.y + qz * v.x - qx * v.z,
        iz =  qw * v.z + qx * v.y - qy * v.x,
        iw = -qx * v.x - qy * v.y - qz * v.z;

  // calculate result * inverse quat
  Vector3 dest;
  dest.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
  dest.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
  dest.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

  return dest;
}

Quaternion& Quaternion::slerpSelf( const Quaternion& qb, float t ) {

  // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
  auto cosHalfTheta = w * qb.w + x * qb.x + y * qb.y + z * qb.z;

  if ( cosHalfTheta < 0 ) {
    w = -qb.w;
    x = -qb.x;
    y = -qb.y;
    z = -qb.z;

    cosHalfTheta = -cosHalfTheta;
  } else {
    copy( qb );
  }

  if ( cosHalfTheta >= 1.0f ) {
    w = w;
    x = x;
    y = y;
    z = z;
    return *this;
  }

  auto halfTheta    = Math::acos( cosHalfTheta );
  auto sinHalfTheta = Math::sqrt( 1.0f - cosHalfTheta * cosHalfTheta );

  if ( Math::abs( sinHalfTheta ) < 0.001f ) {
    w = 0.5f * ( w + w );
    x = 0.5f * ( x + x );
    y = 0.5f * ( y + y );
    z = 0.5f * ( z + z );
    return *this;
  }

  auto ratioA = Math::sin( ( 1.f - t ) * halfTheta ) / sinHalfTheta;
  auto ratioB = Math::sin( t * halfTheta ) / sinHalfTheta;

  w = ( w * ratioA + w * ratioB );
  x = ( x * ratioA + x * ratioB );
  y = ( y * ratioA + y * ratioB );
  z = ( z * ratioA + z * ratioB );

  return *this;
}

float Quaternion::lengthSq() const {
  return x * x + y * y + z * z + w * w;
}

Quaternion& Quaternion::multiplyScalar( float s ) {
  x *= s;
  y *= s;
  z *= s;
  w *= s;
  return *this;
}

Quaternion& Quaternion::divideScalar( float s ) {
  if ( s != 0.f ) {
    return multiplyScalar( 1.f / s );
  } else {
    return set( 0, 0, 0, 0 );
  }
}

} // namespace three

#endif // THREE_QUATERNION_IPP