#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/common.h>
#include <three/utils/memory.h>
#include <three/math/math.h>
#include <memory>

namespace three {

class Quaternion {

public:

  Quaternion() 
  : _x( 0.f ), _y( 0.f ), _z( 0.f ), _w( 1.f ) {}

  Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f ) 
  : _x( xIn ), _y( yIn ), _z( zIn ), _w( wIn ) {}

  inline float x() const {

    return _x;

  }

  inline Quaternion& x( float value ) {

    _x = value;

    _updateEuler();

    return *this;

  }

  inline float y() const {

    return _y;

  }

  inline Quaternion& y( float value ) {

    _y = value;

    _updateEuler();

    return *this;

  }

  inline float z() const {

    return _z;

  }

  inline Quaternion& z(float value) {

    _z = value;

    _updateEuler();

    return *this;

  }

  inline float w() const {

    return _w;

  }

  inline Quaternion& w(float value) {

    _w = value;

    _updateEuler();

    return *this;

  }

  inline Quaternion& set( float x, float y, float z, float w ) {

    _x = x;
    _y = y;
    _z = z;
    _w = w;

    _updateEuler();

    return *this;

  }

  inline Quaternion& copy( const Quaternion& quaternion ) {

    _x = quaternion._x;
    _y = quaternion._y;
    _z = quaternion._z;
    _w = quaternion._w;

    _updateEuler();

    return *this;

  }

  Quaternion& setFromEuler( const Euler& euler, bool update = false );

  inline Quaternion& setFromAxisAngle( const Vector3& axis, float angle ) {

  // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
  // axis have to be normalized

    auto halfAngle = angle / 2.f, s = Math::sin( halfAngle );

    _x = axis.x * s;
    _y = axis.y * s;
    _z = axis.z * s;
    _w = Math::cos( halfAngle );

    _updateEuler();

    return *this;

  }

  Quaternion& setFromRotationMatrix( const Matrix4& m );

  Quaternion& inverse() {

    conjugate().normalize();

    return *this;

  }

  inline Quaternion& conjugate() {

    _x *= -1;
    _y *= -1;
    _z *= -1;

    _updateEuler();

    return *this;

  }

  inline float lengthSq() const {

    return _x * _x + _y * _y + _z * _z + _w * _w;

  }

  inline float length() const {

    return Math::sqrt( _x * _x + _y * _y + _z * _z + _w * _w );

  }

  Quaternion& normalize();

  Quaternion& multiply( const Quaternion& q ) {
    return multiplyQuaternions( *this, q );
  }

  Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

  inline Vector3& multiplyVector3( Vector3& vector ) const {
    //console.warn( 'DEPRECATED: Quaternion\'s .multiplyVector3() has been removed. Use is now vector.applyQuaternion( quaternion ) instead.' );
    return vector.applyQuaternion( *this );
  }

  Quaternion& slerp( const Quaternion& qb, float t );

  inline bool equals( const Quaternion& quaternion ) const {
    return ( quaternion.x() == _x ) && ( quaternion.y() == _y ) && ( quaternion.z() == _z ) && ( quaternion.w() == _w );
  }

  inline Quaternion clone() {
    return *this;
  }

private:

  union {
    struct { float _x, _y, _z, _w; };
    float _xyzw[4];
  };

  std::shared_ptr<Euler> _euler;

  void _updateEuler();

};

} // namespace three

#endif // THREE_QUATERNION_HPP