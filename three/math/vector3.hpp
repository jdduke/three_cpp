#ifndef THREE_VECTOR3_HPP
#define THREE_VECTOR3_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/math/matrix3.hpp>

namespace three {
    
  class Vector3 {
  public:

    union {
      struct { float x, y, z; };
      float xyz[3];
    };

    Vector3() : x( 0 ), y( 0 ), z( 0 ) { }
    Vector3( float xIn, float yIn, float zIn ) : x( xIn ), y( yIn ), z( zIn ) { }
    explicit Vector3( float value ) : x( value ), y( value ), z( value ) { }
    explicit Vector3( float* values ) : x( values[0] ), y( values[1] ), z( values[2] ) { }

    float& operator[]( const int i ) { return xyz[i]; }
    const float operator[]( const int i ) const { return xyz[i]; }

    Vector3& set( float xIn, float yIn, float zIn ) {
      x = xIn;
      y = yIn;
      z = zIn;
      return *this;
    }

    Vector3& setX( float xIn ) {
      x = xIn;
      return *this;
    }

    Vector3& setY( float yIn ) {
      y = yIn;
      return *this;
    }

    Vector3& setZ( float zIn ) {
      z = zIn;
      return *this;
    }

    Vector3& copy( const Vector3& v ) {
      x = v.x;
      y = v.y;
      z = v.z;
      return *this;
    }

    Vector3& add( const Vector3& v ) {
      x += v.x;
      y += v.y;
      z += v.z;
      return *this;
    }

    Vector3& addScalar( float s ) {
      x += s;
      y += s;
      z += s;
      return *this;
    }

    Vector3& addVectors( const Vector3& a, const Vector3& b ) {
      x = a.x + b.x;
      y = a.y + b.y;
      z = a.z + b.z;
      return *this;
    }

    Vector3& sub( const Vector3& v ) {
      x -= v.x;
      y -= v.y;
      z -= v.z;
      return *this;
    }

    Vector3& subVectors( const Vector3& a, const Vector3& b ) {
      x = a.x - b.x;
      y = a.y - b.y;
      z = a.z - b.z;
      return *this;
    }

    Vector3& multiply( const Vector3& v ) {
      x *= v.x;
      y *= v.y;
      z *= v.z;
      return *this;
    }

    Vector3& multiplyScalar( float s ) {
      x *= s;
      y *= s;
      z *= s;
      return *this;
    }

    Vector3& multiplyVectors( const Vector3& a, const Vector3& b ) {
      x = a.x * b.x;
      y = a.y * b.y;
      z = a.z * b.z;
      return *this;
    }

    Vector3& applyMatrix3( const Matrix3& m ) {
      auto e = m.elements;

      x = e[0] * x + e[3] * y + e[6] * z;
      y = e[1] * x + e[4] * y + e[7] * z;
      z = e[2] * x + e[5] * y + e[8] * z;

      return *this;
    }

    Vector3& applyMatrix4( const Matrix4& m ) {
      auto e = m.elements;

      x = e[0] * x + e[4] * y + e[8]  * z + e[12];
      y = e[1] * x + e[5] * y + e[9]  * z + e[13];
      z = e[2] * x + e[6] * y + e[10] * z + e[14];

      return *this;
    }

    Vector3& applyProjection( const Matrix4& m ) {
      auto e = m.elements;
      auto d = 1.0f / ( e[3] * x + e[7] * y + e[11] * z + e[15] ); // perspective divide

      x = ( e[0] * x + e[4] * y + e[8]  * z + e[12] ) * d;
      y = ( e[1] * x + e[5] * y + e[9]  * z + e[13] ) * d;
      z = ( e[2] * x + e[6] * y + e[10] * z + e[14] ) * d;

      return *this;
    }

  Vector3& applyQuaternion( const Quaternion& q ) {

    auto qx = q.x;
    auto qy = q.y;
    auto qz = q.z;
    auto qw = q.w;

    // calculate quat * vector

    auto ix =  qw * x + qy * z - qz * y;
    auto iy =  qw * y + qz * x - qx * z;
    auto iz =  qw * z + qx * y - qy * x;
    auto iw = -qx * x - qy * y - qz * z;

    // calculate result * inverse quat

    x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

    return *this;

  }

  Vector3& transformDirection( const Matrix4& m ) {
    auto e = m.elements;

    x = e[0] * x + e[4] * y + e[8]  * z;
    y = e[1] * x + e[5] * y + e[9]  * z;
    z = e[2] * x + e[6] * y + e[10] * z;

    normalize();

    return *this;
  }

  Vector3& divide( const Vector3& v ) {
    // @todo what's the desired/correct behavior?
    if(v.x != 0.f && v.y != 0.f && v.z != 0.f) {
      x /= v.x;
      y /= v.y;
      z /= v.z;
    } else {
      set( 0.f, 0.f, 0.f);
    }
    return *this;
  }

  Vector3& divideScalar( float s ) {
    if ( s != 0.f ) {
      auto invScalar = 1.f / s;

      x *= invScalar;
      y *= invScalar;
      z *= invScalar;
    } else {
      set( 0.f, 0.f, 0.f );
    }
    return *this;
  }

  Vector3& min( const Vector3& v ) {
    if ( x > v.x ) {
      x = v.x;
    }

    if ( y > v.y ) {
      y = v.y;
    }

    if ( z > v.z ) {
      z = v.z;
    }
    return *this;
  }

  Vector3& max( const Vector3& v ) {

    if ( x < v.x ) {
      x = v.x;
    }

    if ( y < v.y ) {
      y = v.y;
    }

    if ( z < v.z ) {
      z = v.z;
    }

    return *this;
  }

  Vector3& clamp( const Vector3& min, const Vector3&  max ) {

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

    return *this;
  }

  Vector3& negate() {
    return multiplyScalar( -1.f );
  }

  float dot( const Vector3& v ) const {
    return x * v.x + y * v.y + z * v.z;
  }

  float lengthSq() const {
    return x * x + y * y + z * z;
  }

  float length() const {
    return Math::sqrt( lengthSq() );
  }

  float lengthManhattan() const {
    return Math::abs( x ) + Math::abs( y ) + Math::abs( z );
  }

  Vector3& normalize() {
    return divideScalar( length() );
  }

  Vector3& setLength( float l ) {
    return normalize().multiplyScalar( l );
  }

  Vector3& lerp( const Vector3& v, float alpha ) {
    x += ( v.x - x ) * alpha;
    y += ( v.y - y ) * alpha;
    z += ( v.z - z ) * alpha;
    return *this;
  }

  Vector3& cross( const Vector3& v) {
    x = y * v.z - z * v.y;
    y = z * v.x - x * v.z;
    z = x * v.y - y * v.x;
    return *this;
  }

  Vector3& crossVectors( const Vector3& a, const Vector3& b ) {
    x = a.y * b.z - a.z * b.y;
    y = a.z * b.x - a.x * b.z;
    z = a.x * b.y - a.y * b.x;
    return *this;
  }


  float angleTo( const Vector3& v ) {
    //@todo near zero correct behavior?
    auto l = length() * v.length();
    auto theta = dot( v ) / l == 0.f ? NEAR_ZERO_FLOAT : l;

    // clamp, to handle numerical problems
    return Math::acos( Math::clamp( theta, -1.f, 1.f ) );
  }

  float distanceTo( const Vector3& v ) const {
    return Math::sqrt( distanceToSquared( v ) );
  }

  float distanceToSquared( const Vector3& v ) const {
    return Vector3().subVectors( *this, v ).lengthSq();
  }

  Vector3& setFromMatrixPosition( const Matrix4& m ) {
    x = m.elements[ 12 ];
    y = m.elements[ 13 ];
    z = m.elements[ 14 ];
    return *this;
  }

  Vector3& setFromMatrixScale( const Matrix4& m ) {
    // @todo test
    x = set( m.elements[ 0 ], m.elements[ 1 ], m.elements[  2 ] ).length();
    y = set( m.elements[ 4 ], m.elements[ 5 ], m.elements[  6 ] ).length();
    z = set( m.elements[ 8 ], m.elements[ 9 ], m.elements[ 10 ] ).length();

    return *this;
  }

  Vector3& setFromMatrixColumn( const size_t index, const Matrix4& matrix ) {

    auto offset = index * 4;

    x = matrix.elements[ offset ];
    y = matrix.elements[ offset + 1 ];
    z = matrix.elements[ offset + 2 ];

    return *this;
  }

  bool equals( const Vector3& v ) const {
    return ( ( v.x == x ) && ( v.y == y ) && ( v.z == z ) );
  }

  Vector3 clone() const {
    return *this;
  }

  bool isZero() const {
    return ( lengthSq() < 0.0001f /* almostZero */ );
  }

  Vector3& applyEuler( const Euler& euler ) {
    applyQuaternion( Quaternion().setFromEuler( euler ) );
    return *this;
  }

  Vector3& applyAxisAngle( const Vector3& axis, float angle ) {
    applyQuaternion( Quaternion().setFromAxisAngle( axis, angle ) );
    return *this;
  }

  Vector3& projectOnVector( const Vector3& vector ) {
    auto v1 = Vector3();
    v1.copy( vector ).normalize();
    auto d = dot( v1 );
    return copy( v1 ).multiplyScalar( d );
  }

  Vector3& projectOnPlane( const Vector3& planeNormal ) {
    auto v1 = Vector3();
    v1.copy( *this ).projectOnVector( planeNormal );
    return sub( v1 );
  }

  Vector3& reflect( const Vector3& vector ) {
    auto v1 = Vector3();
    v1.copy( *this ).projectOnVector( vector ).multiplyScalar( 2 );
    return subVectors( v1, *this );
  }

};

  static_assert( sizeof( Vector3 ) == sizeof( float ) * 3, "Invalid Vector3 storage size" );

  inline Vector3 add( const Vector3& a, const Vector3& b ) {
    return Vector3().addVectors( a, b );
  }

  inline Vector3 sub( const Vector3& a, const Vector3& b ) {
    return Vector3().subVectors( a, b );
  }

  inline float dot( const Vector3& a, const Vector3& b ) {
    return a.dot( b );
  }

  inline Vector3 cross( const Vector3& a, const Vector3& b ) {
    return Vector3().crossVectors( a, b );
  }

} // namespace three

#endif // THREE_VECTOR3_HPP