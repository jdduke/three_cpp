#ifndef THREE_BOX2_IPP
#define THREE_BOX2_IPP

#include <three/common.hpp>
#include <three/math/math.hpp>
#include <three/math/box2.hpp>

namespace three {

inline Box2& Box2::set( const Vector2& minIn, const Vector2& maxIn ) {

  min.copy(minIn);
  max.copy(maxIn);

  return *this;
}

inline Box2& Box2::copy( const Box2& b ) {

  min.copy(b.min);
  max.copy(b.max);

  return *this;
}

inline float Box2::distanceToPoint( const Vector2& point ) const {

  auto v1 = point;
  auto clampedPoint = v1.copy( point ).clamp( min, max );

  return clampedPoint.sub( point ).length();
}

Box2& Box2::setFromPoints(const std::vector<Vector2>& points ) {

  if ( points.size() == 0 ) {
    makeEmpty();

    return *this;
  }

  min.copy( *points.begin() );
  max.copy( *points.begin() );

  for ( auto it = ++points.begin(); it != points.end(); it++ ) {

    auto point = *it;
    if ( point.x < min.x ) {
      min.x = point.x;
    } else if ( point.x > max.x ) {
      max.x = point.x;
    }

    if ( point.y < min.y ) {
      min.y = point.y;
    } else if ( point.y > max.y ) {
      max.y = point.y;
    }
  }

  return *this;
}

inline Box2& Box2::setFromCenterAndSize( const Vector2& center, const Vector2& size ) {

  auto v1 = Vector2();
  auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );
  min.copy( center ).sub( halfSize );
  max.copy( center ).add( halfSize );

  return *this;
}

inline Box2& Box2::makeEmpty() {

  min.x = min.y = Math::INF();
  max.x = max.y = -Math::INF();

  return *this;
}

inline bool Box2::empty() const {
  return ( max.x < min.x ) || ( max.y < min.y );
}

inline Box2& Box2::expandByPoint( const Vector2& point ) {

  min.min( point );
  max.max( point );

  return *this;
}

inline Box2& Box2::expandByVector( const Vector2& vector ) {

  min.sub( vector );
  max.add( vector );

  return *this;
}

inline Box2& Box2::expandByScalar( float scalar ) {

  min.addScalar( -scalar );
  max.addScalar( scalar );

  return *this;
}

inline Vector2 Box2::center() const {
  return Vector2().addVectors( min, max ).multiplyScalar( 0.5 );
}

inline Vector2& Box2::center( Vector2& target ) {
  return target.addVectors( min, max ).multiplyScalar( 0.5 );
}

inline bool Box2::containsBox( const Box2& box ) const {

  if ( ( min.x <= box.min.x ) && ( box.max.x <= max.x ) && ( min.y <= box.min.y ) && ( box.max.y <= max.y ) ) {
    return true;
  }

  return false;
}

inline bool Box2::containsPoint ( const Vector2& point ) const {
  if ( point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y ) {
    return false;
  }

  return true;
}

inline Vector2 Box2::clampPoint( const Vector2& point ) const {
  return Vector2().copy( point ).clamp( min, max );
}

inline Vector2 Box2::clampPoint( const Vector2& point, Vector2& target ) {
  return target.copy( point ).clamp( min, max );
}

inline Box2 Box2::clone() const {
  return *this;
}

inline Vector2 Box2::size() const {
  return Vector2().subVectors( max, min );
}

inline Vector2& Box2::size( Vector2& target ) {
  return target.subVectors( min, max );
}

inline Box2& Box2::intersect( const Box2& box ) {

  min.max( box.min );
  max.min( box.max );

  return *this;
}

inline bool Box2::isIntersectionBox( const Box2& box ) const {

  if ( box.max.x < min.x || box.min.x > max.x || box.max.y < min.y || box.min.y > max.y ) {
    return false;
  }

  return true;
}

inline Box2& Box2::unionBox( const Box2& box ) {

  min.min( box.min );
  max.max( box.max );

  return *this;
}

inline Box2& Box2::translate( float& offset ) {

  min.addScalar( offset );
  max.addScalar( offset );

  return *this;
}

inline bool Box2::equals ( const Box2& box ) const {
  return box.min.equals( min ) && box.max.equals( max );
}

inline Vector2 Box2::getParameter( const Vector2& point ) const {

  auto divX = ( max.x - min.x );
  auto divY = ( max.y - min.y );

  return Vector2().set(
           ( point.x - min.x ) / divX == 0 ? NEAR_ZERO_FLOAT_32 : divX,
           ( point.y - min.y ) / divX == 0 ? NEAR_ZERO_FLOAT_32 : divY
         );

}

inline Vector2& Box2::getParameter( const Vector2& point, Vector2& target ) {
  auto divX = ( max.x - min.x );
  auto divY = ( max.y - min.y );
  return target.set(
           ( point.x - min.x ) / divX == 0 ? NEAR_ZERO_FLOAT_32 : divX,
           ( point.y - min.y ) / divX == 0 ? NEAR_ZERO_FLOAT_32 : divY
         );
}

} // namespace three

#endif // THREE_BOX2_IPP