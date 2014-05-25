#ifndef THREE_QUATERNION_H
#define THREE_QUATERNION_H

#include <three/common.h>
#include <three/math/math.h>
#include <three/math/vector3.h>
#include <three/utils/memory.h>
#include <memory>

namespace three {

class Quaternion {

public:

  Quaternion()
    : x( 0.f ),
      y( 0.f ),
      z( 0.f ),
      w( 1.f ) {}

  Quaternion( const float xIn, const float yIn, const float zIn, const float wIn = 1.f )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      w( wIn ) {}

  float x;
  float y;
  float z;
  float w;

  inline Quaternion& set( const float xIn, const float yIn, const float zIn, const float wIn ) {

    x = xIn;
    y = yIn;
    z = zIn;
    w = wIn;

    return *this;

  }

  inline Quaternion& copy( const Quaternion& quaternion ) {

    x = quaternion.x;
    y = quaternion.y;
    z = quaternion.z;
    w = quaternion.w;

    return *this;

  }

  Quaternion& setFromEuler( const Euler& euler );

  inline Quaternion& setFromAxisAngle( const Vector3& axis, const float angle ) {

    // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
    // axis have to be normalized

    auto halfAngle = angle / 2.f, s = Math::sin( halfAngle );

    x = axis.x * s;
    y = axis.y * s;
    z = axis.z * s;
    w = Math::cos( halfAngle );

    return *this;

  }

  Quaternion& setFromRotationMatrix( const Matrix4& m );

  Quaternion& inverse() {

    conjugate().normalize();

    return *this;

  }

  inline Quaternion& conjugate() {

    x *= -1;
    y *= -1;
    z *= -1;

    return *this;

  }

  inline float lengthSq() const {

    return x * x + y * y + z * z + w * w;

  }

  inline float length() const {

    return Math::sqrt( x * x + y * y + z * z + w * w );

  }

  Quaternion& normalize();

  Quaternion& multiply( const Quaternion& q ) {
    return multiplyQuaternions( *this, q );
  }

  Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

  Quaternion& slerp( const Quaternion& qb, float t );

  inline bool equals( const Quaternion& quaternion ) const {
    return ( quaternion.x == x ) && ( quaternion.y == y ) && ( quaternion.z == z ) && ( quaternion.w == w );
  }

  inline Quaternion clone() {
    return *this;
  }

};

} // namespace three

#endif // THREE_QUATERNION_H
