#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/common.hpp>

#include <three/constants.hpp>

namespace three {    
class Quaternion {
public:

  union {
    struct { float x, y, z, w; };
    float xyzw[4];
  };

  Quaternion() : x( 0 ), y( 0 ), z( 0 ), w( 1.f ) { }
  Quaternion( float xIn, float yIn, float zIn, float wIn = 1.f ) : x( xIn ), y( yIn ), z( zIn ), w( wIn ) { }
  Quaternion( const Quaternion& v ) : x( v.x ), y( v.y ), z( v.z ), w( v.w ) { }
  Quaternion& operator= ( const Quaternion& v ) { return copy( v ); }

  Quaternion& set( float xIn, float yIn, float zIn, float wIn ) {
    x = xIn;
    y = yIn;
    z = zIn;
    w = wIn;
    return *this;
  }

  Quaternion& copy( const Quaternion& v ) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
  }

  THREE_DECL Vector3 getEuler( THREE::EulerRotationOrder order = THREE::XYZ ) const;
  THREE_DECL Quaternion& setFromEuler( const Vector3& v, THREE::EulerRotationOrder order = THREE::XYZ );
  THREE_DECL Quaternion& setFromEuler( const Euler& v, THREE::EulerRotationOrder order = THREE::XYZ );
  THREE_DECL Quaternion& setFromAxisAngle( const Vector3& axis, float angle );
  THREE_DECL Quaternion& calculateW();
  THREE_DECL Quaternion& inverse();
  THREE_DECL float length() const;
  THREE_DECL Quaternion& normalize();
  THREE_DECL Quaternion& multiply( const Quaternion& a, const Quaternion& b );
  THREE_DECL Quaternion& multiplySelf( const Quaternion& b );
  THREE_DECL Vector3 multiplyVector3( const Vector3& v );
  THREE_DECL Quaternion& slerpSelf( const Quaternion& qb, float t );

  Quaternion clone() {
    return *this;
  }

private:

  THREE_DECL float lengthSq() const;
  THREE_DECL Quaternion& multiplyScalar( float s );
  THREE_DECL Quaternion& divideScalar( float s );

};

} // namespace three
#if defined(THREE_HEADER_ONLY)
# include <three/math/impl/quaternion.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_QUATERNION_HPP