#ifndef THREE_VECTOR2_H
#define THREE_VECTOR2_H

#include <three/common.h>

#include <three/math/math.h>

namespace three {

class THREE_DECL Vector2 {

public:

  union {
    struct {
      float x, y;
    };
    float xy[2];
  };

  Vector2()
    : x( 0 ), y( 0 ) {}

  Vector2( float xIn, float yIn )
    : x( xIn ), y( yIn ) {}

  explicit Vector2( float value )
    : x( value ), y( value ) {}

  explicit Vector2( float* values )
    : x( values[0] ), y( values[1] ) {}

  inline float& operator[]( const int i ) {

    return xy[i];

  }

  inline const float operator[]( const int i ) const {

    return xy[i];

  }

  inline Vector2& set( float xIn, float yIn ) {

    x = xIn;
    y = yIn;

    return *this;
  }

  inline Vector2& setX( float xIn ) {

    x = xIn;

    return *this;
  }

  inline Vector2& setY( float yIn ) {

    y = yIn;

    return *this;
  }

  inline Vector2& setComponent (size_t index, float value ) {

    if (index == 0) {

      x = value;

    } else if (index == 1) {

      y = value;

    }

    return *this;

  }

  inline float getComponent ( size_t index ) {

    if(index <= 0) {

      return x;

    } else {

      return y;

    }

  }

  inline Vector2& copy( const Vector2& v ) {

    x = v.x;
    y = v.y;

    return *this;

  }

  inline Vector2& add( const Vector2& v) {

    x += v.x;
    y += v.y;

    return *this;

  }

  inline Vector2& addVectors( const Vector2& a, const Vector2& b ) {

    x = a.x + b.x;
    y = a.y + b.y;

    return *this;

  }

  inline Vector2& addScalar( float value ) {

    x += value;
    y += value;

    return *this;

  }

  inline Vector2& sub( const Vector2& v ) {

    x -= v.x;
    y -= v.y;

    return *this;

  }

  inline Vector2& subVectors( const Vector2& a, const Vector2& b ) {

    x = a.x - b.x;
    y = a.y - b.y;

    return *this;

  }

  inline Vector2& multiplyScalar( float s ) {

    x *= s;
    y *= s;

    return *this;

  }

  inline Vector2& divideScalar( float s ) {

    if ( s ) {

      return multiplyScalar( 1.f / s );

    } else {

      return set( 0, 0 );

    }

  }

  inline Vector2& min ( const Vector2& v ) {

    if ( x > v.x ) {
      x = v.x;
    }

    if ( y > v.y ) {
      y = v.y;
    }

    return *this;

  }

  inline Vector2& max ( const Vector2& v ) {

    if ( x < v.x ) {
      x = v.x;
    }

    if ( y < v.y ) {
      y = v.y;
    }

    return *this;

  }


  inline Vector2& clamp ( const Vector2& min, const Vector2& max ) {

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

    return *this;


  }

  inline Vector2& clampScalar( float minVal, float maxVal ) {

    auto min = Vector2();
    auto max = Vector2();

    min.set( minVal, minVal );
    max.set( maxVal, maxVal );

    return clamp( min, max );

  }

  inline Vector2& floor() {

    x = Math::floor( x );
    y = Math::floor( y );

    return *this;

  }

  inline Vector2& ceil () {

    x = Math::ceil( x );
    y = Math::ceil( y );

    return *this;

  }

  inline Vector2& round() {

    x = Math::round( x );
    y = Math::round( y );

    return *this;

  }

  inline Vector2& roundToZero() {

    x = ( x < 0.0f ) ? Math::ceil( x ) : Math::floor( x );
    y = ( y < 0.0f ) ? Math::ceil( y ) : Math::floor( y );

    return *this;

  }

  inline Vector2& negate() {

    return multiplyScalar( -1.f );

  }

  inline float dot( const Vector2& v ) const {

    return x * v.x + y * v.y;

  }

  inline float lengthSq() const {

    return x * x + y * y;

  }

  inline float length() const {

    return Math::sqrt( lengthSq() );

  }

  inline Vector2& normalize() {

    return divideScalar( length() );

  }

  inline float distanceTo( const Vector2& v ) const {

    return Math::sqrt( distanceToSquared( v ) );

  }

  inline float distanceToSquared( const Vector2& v ) const {

    auto dx = x - v.x, dy = y - v.y;

    return dx * dx + dy * dy;

  }

  inline Vector2& setLength( float l ) {

    auto oldLength = length();

    if ( oldLength != 0.f && l != oldLength ) {

      return multiplyScalar( l / oldLength);

    }

    return *this;

  }

  inline Vector2& lerp( const Vector2& v, float alpha ) {

    x += ( v.x - x ) * alpha;
    y += ( v.y - y ) * alpha;

    return *this;

  }

  inline bool equals( const Vector2& v ) const {

    return ( ( v.x == x ) && ( v.y == y ) );

  }

  inline Vector2 clone() const {

    return *this;

  }

};

static_assert( sizeof( Vector2 ) == sizeof( float ) * 2, "Invalid Vector2 storage size" );

} // namespace three

#endif // THREE_VECTOR2_H