#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

namespace three {

  class Quaternion {

  public:

   union {
      struct { float _x, _y, _z, _w; };
      float _xyzw[4];
   };

    Quaternion() 
      : _x( 0.f ), _y( 0.f ), _z( 0.f ), _w( 1.f ) {}

    Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f ) 
      : _x( xIn ), _y( yIn ), _z( zIn ), _w( wIn ) {}

   float x() const;

   Quaternion& x( float value );

   float y() const;

   Quaternion& y( float value );

   float z() const;

   Quaternion& z(float value);

   float w() const;

   Quaternion& w(float value);

   Quaternion& set( float x, float y, float z, float w );

   Quaternion& copy( const Quaternion& quaternion );

   Quaternion& setFromEuler( const Euler& euler, bool update = false );

   Quaternion& setFromAxisAngle( const Vector3& axis, float angle );

   Quaternion& setFromRotationMatrix( const Matrix4& m );

   Quaternion& inverse();

   Quaternion& conjugate();

   float lengthSq() const;

   float length() const;

   Quaternion& normalize();

   Quaternion& multiply( const Quaternion& q );

   Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

   Vector3& multiplyVector3( Vector3& vector ) const;

   Quaternion& slerp( const Quaternion& qb, float t );

   bool equals( const Quaternion& quaternion ) const;

   Quaternion clone();

  private:
    
    void _updateEuler();

    std::shared_ptr<Euler> _euler;

 };

} // namespace three

#endif // THREE_QUATERNION_HPP