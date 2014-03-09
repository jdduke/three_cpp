#ifndef THREE_VECTOR4_HPP
#define THREE_VECTOR4_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/math/vector3.hpp>
#include <three/math/quaternion.hpp>

namespace three {

  class Vector4 {
  public:

    union {
      struct { float x, y, z, w; };
      float xyzw[4];
    };

    Vector4() 
        : x( 0 ), y( 0 ), z( 0 ), w( 1.f ) {}
    Vector4( float xIn, float yIn, float zIn, float wIn = 1.f ) 
        : x( xIn ), y( yIn ), z( zIn ), w( wIn ) {}

    explicit Vector4( float value ) 
     : x( value ), y( value ), z( value ), w( value ) { }

    explicit Vector4( float* values ) : x( values[0] ), y( values[1] ), z( values[2] ), w( values[3] ) { }

    float& operator[]( const int i ) { return xyzw[i]; }

    const float operator[]( const int i ) const { return xyzw[i]; }

    Vector4& set( float xIn, float yIn, float zIn, float wIn ); 

    Vector4& setX( float xIn);

    Vector4& setY( float yIn );

    Vector4& setZ( float zIn );

    Vector4& setW( float wIn );

    Vector4& copy( const Vector3& v );

    Vector4& copy( const Vector4& v );

    Vector4& add( const Vector4& v );

    Vector4& addVectors( const Vector4& a, const Vector4& b );

    Vector4& addScalar( float s );

    Vector4& sub( const Vector4& v );

    Vector4& subVectors( const Vector4& a, const Vector4& b );

    Vector4& multiplyScalar( float s );

    Vector4& applyMatrix4( const Matrix4& m );

    Vector4& divideScalar( float s );

    Vector4& setAxisAngleFromQuaternion( const Quaternion& q );

    Vector4& setAxisAngleFromRotationMatrix( const Matrix4& m );

    Vector4& min( const Vector4& v );

    Vector4& max( const Vector4& v );

    Vector4& clamp( const Vector4& min, const Vector4& max );

    Vector4& negate();

    float dot( const Vector4& v ) const;

    float lengthSq() const;

    float length() const;

    float lengthManhattan() const;

    Vector4& normalize();

    Vector4& setLength( float l );

    Vector4& lerp( const Vector4& v, float alpha );

    bool equals( const Vector4& v );

    Vector4 clone() const;

  };

  static_assert( sizeof( Vector4 ) == sizeof( float ) * 4, "Invalid Vector4 storage size" );

} // namespace three

#endif // THREE_VECTOR4_HPP