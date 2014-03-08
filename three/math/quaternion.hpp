#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

namespace three {
  class Euler;

  class Quaternion {
  private:
    union {
      struct { float _x, _y, _z, _w; };
      float xyzw[4];
    };

  // todo lazy init?
    Euler* _euler;

    void _updateEuler();

  public:

    Quaternion() : _x( 0.f ), _y( 0.f ), _z( 0.f ), _w( 1.f ) { }
    Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f ) : _x( xIn ), _y( yIn ), _z( zIn ), _w( wIn ) { }
    //Quaternion( Quaternion& v ) : _x( v.x() ), _y( v.y() ), _z( v.z() ), _w( v.w() ) { }
    Quaternion& operator= ( const Quaternion& q ) { return copy( q ); }

    inline float x() const {
     return _x;
    }

    inline Quaternion& x(const float& value) {
      _x = value;
      _updateEuler();
	  return *this;
    }

     inline float y() const {
     return _y;
    }

    inline Quaternion& y(const float& value) {
      _y = value;
      _updateEuler();
	  return *this;
    }

     inline float z() const {
     return _z;
    }

    inline Quaternion& z(const float& value) {
      _z = value;
      _updateEuler();
	  return *this;
    }

     inline float w() const {
     return _w;
    }

    inline Quaternion& w(const float& value) {
      _w = value;
      _updateEuler();
	  return *this;
    }

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

 };

} // namespace three
#if defined(THREE_HEADER_ONLY)
# include <three/math/impl/quaternion.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_QUATERNION_HPP