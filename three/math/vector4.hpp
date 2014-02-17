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

    Vector4() : x( 0 ), y( 0 ), z( 0 ), w( 1.f ) { }
    Vector4( float xIn, float yIn, float zIn, float wIn = 1.f ) : x( xIn ), y( yIn ), z( zIn ), w( wIn ) { }
    explicit Vector4( float value ) : x( value ), y( value ), z( value ), w( value ) { }
    explicit Vector4( float* values ) : x( values[0] ), y( values[1] ), z( values[2] ), w( values[3] ) { }

    float& operator[]( const int i ) { return xyzw[i]; }
    const float operator[]( const int i ) const { return xyzw[i]; }

    Vector4& set( float xIn, float yIn, float zIn, float wIn ); // r50
    Vector4& setX( float xIn); // r65
    Vector4& setY( float yIn ); // r65
    Vector4& setZ( float zIn ); // r65
    Vector4& setW( float wIn ); // r65
    Vector4& copy( const Vector3& v ); // r50
    Vector4& copy( const Vector4& v ); // r50
    Vector4& add( const Vector4& v ); // r65
    Vector4& addVectors( const Vector4& a, const Vector4& b ); //r 65
    Vector4& addScalar( float s ); // r65
    Vector4& sub( const Vector4& v ); // r65
    Vector4& subVectors( const Vector4& a, const Vector4& b ); // r65
    Vector4& multiplyScalar( float s );
    Vector4& applyMatrix4( const Matrix4& m ); //r65
    Vector4& divideScalar( float s );
    Vector4& setAxisAngleFromQuaternion( const Quaternion& q ); //r65
    Vector4& setAxisAngleFromRotationMatrix( const Matrix4& m ); // r65
    Vector4& min( const Vector4& v ); // r65
    Vector4& max( const Vector4& v ); // r65
    Vector4& clamp( const Vector4& min, const Vector4& max ); // r65
    Vector4& negate(); // r50
    float dot( const Vector4& v ) const; //r50
    float lengthSq() const; //r50
    float length() const; //r50
    float lengthManhattan() const; //r65
    Vector4& normalize(); //r50
    Vector4& setLength( float l ); //r50
    Vector4& lerp( const Vector4& v, float alpha );  //r65
    bool equals( const Vector4& v ); //r50
    Vector4 clone() const;

  };

  static_assert( sizeof( Vector4 ) == sizeof( float ) * 4, "Invalid Vector4 storage size" );

} // namespace three

#endif // THREE_VECTOR4_HPP