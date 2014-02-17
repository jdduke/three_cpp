#ifndef THREE_VECTOR4_HPP
#define THREE_VECTOR4_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/math/vector3.hpp>

namespace three {

  class Vector4 {
  public:

    union {
      struct { float x, y, z, w; };
      float xyzw[4];
    };

    Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 1.f ) { }
    Vector4( float xIn, float yIn, float zIn, float wIn = 1.f ) : x( xIn ), y( yIn ), z( zIn ), w( wIn ) { }
    explicit Vector4( float value ) : x( value ), y( value ), z( value ), w( value ) { }
    explicit Vector4( float* values ) : x( values[0] ), y( values[1] ), z( values[2] ), w( values[3] ) { }

    float& operator[]( const int i ) { return xyzw[i]; }
    const float operator[]( const int i ) const { return xyzw[i]; }

    Vector4& set( float xIn, float yIn, float zIn, float wIn ) {
      x = xIn;
      y = yIn;
      z = zIn;
      w = wIn;
      return *this;
    }

    Vector4& copy( const Vector3& v ) {
      x = v.x;
      y = v.y;
      z = v.z;
      w = 1.f;
      return *this;
    }

    Vector4& copy( const Vector4& v ) {
      x = v.x;
      y = v.y;
      z = v.z;
      w = v.w;
      return *this;
    }

    Vector4& add( const Vector4& a, const Vector4& b ) {
      x = a.x + b.x;
      y = a.y + b.y;
      z = a.z + b.z;
      w = a.w + b.w;
      return *this;
    }

    Vector4& addSelf( const Vector4& v ) {
      x += v.x;
      y += v.y;
      z += v.z;
      w += v.w;
      return *this;
    }

    Vector4& sub( const Vector4& a, const Vector4& b ) {
      x = a.x - b.x;
      y = a.y - b.y;
      z = a.z - b.z;
      w = a.w - b.w;
      return *this;
    }

    Vector4& subSelf( const Vector4& v ) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      w -= v.w;
      return *this;
    }

    Vector4& multiplyScalar( float s ) {
      x *= s;
      y *= s;
      z *= s;
      w *= s;
      return *this;
    }

    Vector4& divideScalar( float s ) {
      if ( s != 0.f ) {
        return multiplyScalar( 1.f / s );
      } else {
        return set( 0, 0, 0, 1 );
      }
    }

    Vector4& negate() {
      return multiplyScalar( -1.f );
    }

    float dot( const Vector4& v ) const {
      return x * v.x + y * v.y + z * v.z + w + v.w;
    }

    float lengthSq() const {
      return dot( *this );
    }

    float length() const {
      return Math::sqrt( lengthSq() );
    }

    Vector4& normalize() {
      return divideScalar( length() );
    }

    Vector4& setLength( float l ) {
      return normalize().multiplyScalar( l );
    }

    Vector4& lerpSelf( const Vector4& v, float alpha ) {
      x += ( v.x - x ) * alpha;
      y += ( v.y - y ) * alpha;
      z += ( v.z - z ) * alpha;
      w += ( v.w - w ) * alpha;
      return *this;
    }

    bool equals( const Vector4& v ) {
      return ( ( v.x == x ) && ( v.y == y ) && ( v.z == z ) );
    }

    Vector4 clone() const {
      return *this;
    }

    Vector4& setAxisAngleFromQuaternion( const Quaternion& q ) {
    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
    // q is assumed to be normalized
      w = 2 * Math::acos( q.w );

      auto s = Math::sqrt( 1.f - q.w * q.w );

      if ( s < 0.0001f ) {
        x = 1;
        y = 0;
        z = 0;
      } else {
        x = q.x / s;
        y = q.y / s;
        z = q.z / s;
      }
      return *this;
    }
  };

  static_assert( sizeof( Vector4 ) == sizeof( float ) * 4, "Invalid Vector4 storage size" );

} // namespace three

#endif // THREE_VECTOR4_HPP