#ifndef THREE_QUATERNION_H
#define THREE_QUATERNION_H

#include <three/common.h>
#include <three/math/math.h>
#include <three/math/vector3.h>
#include <three/utils/memory.h>
#include <three/utils/floating_point_hook.h>
#include <memory>

namespace three {

class Quaternion {

public:

  Quaternion()
    : x( FLOAT_HOOK( Quaternion, _updateEuler, 0.f ) ),
      y( FLOAT_HOOK( Quaternion, _updateEuler, 0.f ) ),
      z( FLOAT_HOOK( Quaternion, _updateEuler, 0.f ) ),
      w( FLOAT_HOOK( Quaternion, _updateEuler, 1.f ) ),
      _euler( nullptr ) {}

  Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f )
    : x( FLOAT_HOOK( Quaternion, _updateEuler, xIn ) ),
      y( FLOAT_HOOK( Quaternion, _updateEuler, yIn ) ),
      z( FLOAT_HOOK( Quaternion, _updateEuler, zIn ) ),
      w( FLOAT_HOOK( Quaternion, _updateEuler, wIn ) ),
      _euler( nullptr ) {}

  FloatingPointHook<float, Quaternion> x;
  FloatingPointHook<float, Quaternion> y;
  FloatingPointHook<float, Quaternion> z;
  FloatingPointHook<float, Quaternion> w;

  inline Quaternion& set( float xIn, float yIn, float zIn, float wIn ) {

    x.value = xIn;
    y.value = yIn;
    z.value = zIn;
    w.value = wIn;

    _updateEuler();

    return *this;

  }

  inline Quaternion& copy( const Quaternion& quaternion ) {

    x.value = quaternion.x.value;
    y.value = quaternion.y.value;
    z.value = quaternion.z.value;
    w.value = quaternion.w.value;

    _updateEuler();

    return *this;

  }

  Quaternion& setFromEuler( const Euler& euler, bool update = false );

  inline Quaternion& setFromAxisAngle( const Vector3& axis, float angle ) {

    // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
    // axis have to be normalized

    auto halfAngle = angle / 2.f, s = Math::sin( halfAngle );

    x.value = axis.x * s;
    y.value = axis.y * s;
    z.value = axis.z * s;
    w.value = Math::cos( halfAngle );

    _updateEuler();

    return *this;

  }

  Quaternion& setFromRotationMatrix( const Matrix4& m );

  Quaternion& inverse() {

    conjugate().normalize();

    return *this;

  }

  inline Quaternion& conjugate() {

    x.value *= -1;
    y.value *= -1;
    z.value *= -1;

    _updateEuler();

    return *this;

  }

  inline float lengthSq() const {

    return x.value * x.value + y.value * y.value + z.value * z.value + w.value * w.value;

  }

  inline float length() const {

    return Math::sqrt( x.value * x.value + y.value * y.value + z.value * z.value + w.value * w.value );

  }

  Quaternion& normalize();

  Quaternion& multiply( const Quaternion& q ) {
    return multiplyQuaternions( *this, q );
  }

  Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

  Quaternion& slerp( const Quaternion& qb, float t );

  inline bool equals( const Quaternion& quaternion ) const {
    return ( quaternion.x.value == x.value ) && ( quaternion.y.value == y.value ) && ( quaternion.z.value == z.value ) && ( quaternion.w.value == w.value );
  }

  inline Quaternion clone() {
    return *this;
  }

private:

  friend class Object3D;
  friend class Euler;

  Euler* _euler;

  void _updateEuler();

};

} // namespace three

#endif // THREE_QUATERNION_H