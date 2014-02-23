#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/utils/macros.hpp>

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

    Quaternion() : _x( 0 ), _y( 0 ), _z( 0 ), _w( 1.f ) { }
    Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f ) : _x( xIn ), _y( yIn ), _z( zIn ), _w( wIn ) { }
    Quaternion( const Quaternion& v ) : _x( v.x ), _y( v.y ), _z( v.z ), _w( v.w ) { }
    Quaternion& operator= ( const Quaternion& q ) { return copy( q ); }

    PROPERTY(float, x);
    GET(x) {
     return _x;
    }

    inline Quaternion& SET(x) {
      _x = value;
      _updateEuler();
      return *this;
    }

    PROPERTY(float, y);
    GET(y) {
     return _y;
    }

    inline Quaternion& SET(y) {
      _y = value;
      _updateEuler();
      return *this;
    }

    PROPERTY(float, z);
    GET(z) {
     return _z;
    }

    inline Quaternion& SET(z) {
      _z = value;
      _updateEuler();
      return *this;
    }

    PROPERTY(float, w);
    GET(w) {
     return _w;
    }

    inline Quaternion& SET(w) {
      _w = value;
      _updateEuler();
      return *this;
    }

   /*THREE_DECL inline float getX() const;
   THREE_DECL inline Quaternion& setX(const float& value);
   THREE_DECL inline float getY() const;
   THREE_DECL inline Quaternion& setY(const float& value);
   THREE_DECL inline float getZ() const;
   THREE_DECL inline Quaternion& setZ(const float& value);
   THREE_DECL inline float getW() const;*/

   THREE_DECL Quaternion& setW(const float& value);

   THREE_DECL Quaternion& set( float x, float y, float z, float w );

   THREE_DECL Quaternion& copy( const Quaternion& quaternion );

   THREE_DECL Quaternion& setFromEuler( const Euler& euler, bool update = false );

   THREE_DECL Quaternion& setFromAxisAngle( const Vector3& axis, float angle );

   THREE_DECL Quaternion& setFromRotationMatrix( const Matrix4& m );

   THREE_DECL Quaternion& inverse();

   THREE_DECL Quaternion& conjugate();

   THREE_DECL Quaternion& lengthSq();

   THREE_DECL Quaternion& length();

   THREE_DECL Quaternion& normalize();

   THREE_DECL Quaternion& multiply( const Quaternion& q );

   THREE_DECL Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

   THREE_DECL Quaternion& multiplyVector3( Vector3& vector ) const;

   THREE_DECL Quaternion& slerp( const Quaternion& qb, float t );

   THREE_DECL Quaternion& equals( const Quaternion& quaternion ) const;

   THREE_DECL Quaternion clone();

 };

} // namespace three
#if defined(THREE_HEADER_ONLY)
# include <three/math/impl/quaternion.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_QUATERNION_HPP