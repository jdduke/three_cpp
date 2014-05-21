#ifndef THREE_VECTOR4_H
#define THREE_VECTOR4_H

#include <three/common.h>

#include <three/math/math.h>
#include <three/math/vector3.h>

namespace three {

class Vector4 {

public:

  union {
    struct {
      float x, y, z, w;
    };
    float xyzw[4];
  };

  Vector4()
    : x( 0 ), y( 0 ), z( 0 ), w( 1.f ) {}

  Vector4( float xIn, float yIn, float zIn, float wIn = 1.f )
    : x( xIn ), y( yIn ), z( zIn ), w( wIn ) {}

  explicit Vector4( float value )
    : x( value ), y( value ), z( value ), w( value ) { }

  explicit Vector4( float* values ) : x( values[0] ), y( values[1] ), z( values[2] ), w( values[3] ) { }

  float& operator[]( const int i ) {
    return xyzw[i];
  }

  const float operator[]( const int i ) const {
    return xyzw[i];
  }

  inline Vector4& set( float xIn, float yIn, float zIn, float wIn ) {

    x = xIn;
    y = yIn;
    z = zIn;
    w = wIn;

    return *this;
  }

  inline Vector4& setX( float xIn ) {

    x = xIn;

    return *this;

  }

  inline Vector4& setY( float yIn ) {

    y = yIn;

    return *this;

  }

  inline Vector4& setZ( float zIn ) {

    z = zIn;

    return *this;

  }

  inline Vector4& setW( float wIn ) {

    w = wIn;

    return *this;

  }

  inline Vector4& setComponent( int index, float value ) {

    switch ( index ) {

      case 0: x = value; break;
      case 1: y = value; break;
      case 2: z = value; break;
      case 3: w = value; break;

    }

    return *this;
  }

  inline float getComponent( int index ) {

    switch ( index ) {

      case 0: return x;
      case 1: return y;
      case 2: return z;
      case 3: return w;
      default: throw;

    }

  }

  inline Vector4& copy( const Vector3& v ) {

    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.f;

    return *this;

  }

  inline Vector4& copy( const Vector4& v ) {

    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;

  }

  inline Vector4& add( const Vector4& v ) {

    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;

    return *this;

  }

  inline Vector4& addVectors( const Vector4& a, const Vector4& b ) {

    x = a.x + b.x;
    y = a.y + b.y;
    z = a.z + b.z;
    w = a.w + b.w;

    return *this;

  }

  inline Vector4& addScalar( float s ) {

    x += s;
    y += s;
    z += s;
    w += s;

    return *this;

  }

  inline Vector4& sub( const Vector4& v ) {

    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;

    return *this;

  }

  inline Vector4& subVectors( const Vector4& a, const Vector4& b ) {

    x = a.x - b.x;
    y = a.y - b.y;
    z = a.z - b.z;
    w = a.w - b.w;

    return *this;
  }

  inline Vector4& multiplyScalar( float s ) {

    x *= s;
    y *= s;
    z *= s;
    w *= s;

    return *this;

  }

  Vector4& applyMatrix4( const Matrix4& m );

  inline Vector4& divideScalar( float s ) {

    if ( s != 0.f ) {

      return multiplyScalar( 1.f / s );

    } else {

      return set( 0, 0, 0, 1 );

    }

  }

  Vector4& setAxisAngleFromQuaternion( const Quaternion& q );

  Vector4& setAxisAngleFromRotationMatrix( const Matrix4& m );

  inline Vector4& min( const Vector4& v ) {

    if ( x > v.x ) {

      x = v.x;

    }

    if ( y > v.y ) {

      y = v.y;

    }

    if ( z > v.z ) {

      z = v.z;

    }

    if ( w > v.w ) {

      w = v.w;

    }

    return *this;

  }

  inline Vector4& max( const Vector4& v ) {

    if ( x < v.x ) {

      x = v.x;

    }

    if ( y < v.y ) {

      y = v.y;

    }

    if ( z < v.z ) {

      z = v.z;

    }

    if ( w < v.w ) {

      w = v.w;

    }

    return *this;

  }

  inline Vector4& clamp( const Vector4& min, const Vector4& max ) {
    // This function assumes min < max, if this assumption isn't true it will not operate correctly

    if ( x < min.x ) {

      x = min.x;

    } else if ( x > max.x ) {

      x = max.x;

    }

    if ( y < min.y ) {

      y = min.y;

    } else if ( y > max.y ) {

      y = max.y;

    }

    if ( z < min.z ) {

      z = min.z;

    } else if ( z > max.z ) {

      z = max.z;

    }

    if ( w < min.w ) {

      w = min.w;

    } else if ( w > max.w ) {

      w = max.w;

    }

    return *this;

  }

  inline Vector4& negate() {

    return multiplyScalar( -1.f );

  }

  inline float dot( const Vector4& v ) const {

    return x * v.x + y * v.y + z * v.z + w * v.w;

  }

  inline float lengthSq() const {

    return dot( *this );

  }

  inline float length() const {

    return Math::sqrt( lengthSq() );

  }

  inline float lengthManhattan() const {

    return Math::abs( x ) + Math::abs( y ) + Math::abs( z ) + Math::abs( w );

  }

  inline Vector4& normalize() {

    return divideScalar( length() );

  }

  inline Vector4& setLength( float l ) {

    auto oldLength = length();
      
    if ( oldLength != 0.f && l != oldLength ) {
          
      multiplyScalar( l / oldLength );
          
    }
      
    return *this;

  }

  inline Vector4& lerp( const Vector4& v, float alpha ) {

    x += ( v.x - x ) * alpha;
    y += ( v.y - y ) * alpha;
    z += ( v.z - z ) * alpha;
    w += ( v.w - w ) * alpha;

    return *this;

  }

  inline bool equals( const Vector4& v ) {

    return ( ( v.x == x ) && ( v.y == y ) && ( v.z == z ) );

  }

  inline Vector4 clone() const {

    return *this;

  }

};

static_assert( sizeof( Vector4 ) == sizeof( float ) * 4, "Invalid Vector4 storage size" );

} // namespace three

#endif // THREE_VECTOR4_H