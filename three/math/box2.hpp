#ifndef THREE_BOX2_HPP
#define THREE_BOX2_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

namespace three {

  class Box2 {
  public:

    union {
      struct { Vector2 min, max; };
      Vector2 minmax[2];
    };

    Box2() {
      min = Vector2( Math::INF(), Math::INF() );
      max = Vector2( -Math::INF(), -Math::INF() );
    }
    Box2( Vector2 minIn, Vector2 maxIn ) : min( minIn ), max( maxIn ) { }
    
    explicit Box2( Vector2* values ) : min( values[0] ), max( values[1] ) { }

    Box2& set( const Vector2& minIn, const Vector2& maxIn ) {
      min.copy(minIn);
      max.copy(maxIn);
      return *this;
    }

    Box2& copy( const Box2& b ) {
      min.copy(b.min);
      max.copy(b.max);
      return *this;
    }

    float distanceToPoint( const Vector2& point ) const {
      auto v1 = point;
      auto clampedPoint = v1.copy( point ).clamp( min, max );
      return clampedPoint.sub( point ).length();
    }

    Box2& setFromPoints(const std::vector<Vector2>& points ) {
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

    Box2& setFromCenterAndSize( const Vector2& center, const Vector2& size ) {
      auto v1 = Vector2();
      auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );
      min.copy( center ).sub( halfSize );
      max.copy( center ).add( halfSize );
      return *this;
    }

    Box2& makeEmpty() {
      min.x = min.y = Math::INF();
      max.x = max.y = -Math::INF();
      return *this;
    }

    bool empty() const {
      return ( max.x < min.x ) || ( max.y < min.y );
    }

    Box2& expandByPoint( const Vector2& point ) {
      min.min( point );
      max.max( point );
      return *this;
    }

    Box2& expandByVector( const Vector2& vector ) {
      min.min( vector );
      max.max( vector );
      return *this;
    }

    Box2& expandByScalar( float scalar ) {
      min.addScalar( -scalar );
      max.addScalar( scalar );
      return *this;
    }

    Vector2 center() const {
      return Vector2().addVectors( min, max ).multiplyScalar( 0.5 );
    }

    Vector2 center( const Vector2& target ) const {
      auto result = target;
      return result.addVectors( min, max ).multiplyScalar( 0.5 );
    }

    bool containsBox( const Box2& box ) const {
      if ( ( min.x <= box.min.x ) && ( box.max.x <= max.x ) && ( min.y <= box.min.y ) && ( box.max.y <= max.y ) ) {
        return true;
      }
      return false;
    }

    bool containsPoint ( const Vector2& point ) const {
      if ( point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y ) {
        return false;
      }
      return true;
    }

    Vector2 clampPoint( const Vector2& point ) const {
    // @todo mem check
      return clampPoint(point, Vector2());
    }

    Vector2 clampPoint( const Vector2& point, const Vector2& target ) const {
      auto result = target;
      return result.copy( point ).clamp( min, max );
    }

    Box2 clone() const {
      return *this;
    }

    Vector2 size() const {
      return Vector2().subVectors( max, min );
    }

    Vector2 size( const Vector2& target ) const {
      auto result = target;
      return result.subVectors( min, max );
    }

    Box2& intersect( const Box2& box ) {
      min.max( box.min );
      max.min( box.max );
      return *this;
    }

    bool isIntersectionBox( const Box2& box ) const {
      if ( box.max.x < min.x || box.min.x > max.x || box.max.y < min.y || box.min.y > max.y ) {
        return false;
      }
      return true;
    }

    Box2& unionBox( const Box2& box ) {
      min.min( box.min );
      max.max( box.max );
      return *this;
    }

    Box2& translate( float& offset ) {
      min.addScalar( offset );
      max.addScalar( offset );
      return *this;
    }

    bool equals ( const Box2& box ) const {
      return box.min.equals( min ) && box.max.equals( max );
    }

    Vector2 getParameter( const Vector2& point, const Vector2& target ) const {
      auto result = target;
      auto divX = ( max.x - min.x );
      auto divY = ( max.y - min.y );
      return result.set(
        ( point.x - min.x ) / divX == 0 ? 1 : divX,
        ( point.y - min.y ) / divX == 0 ? 1 : divY
      );
    }

    Vector2 getParameter( const Vector2& point ) const {
      auto result = Vector2();
      auto divX = ( max.x - min.x );
      auto divY = ( max.y - min.y );
      return result.set(
        ( point.x - min.x ) / divX == 0 ? 1 : divX,
        ( point.y - min.y ) / divX == 0 ? 1 : divY
      );
    }

  };

  static_assert( sizeof( Box2 ) == sizeof( Vector2 ) * 2, "Invalid Box2 storage size" );

} // namespace three

#endif // THREE_BOX2_HPP