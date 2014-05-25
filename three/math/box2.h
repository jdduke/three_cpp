#ifndef THREE_BOX2_H
#define THREE_BOX2_H

#include <three/common.h>
#include <three/math/math.h>
#include <three/math/vector2.h>

namespace three {

class THREE_DECL Box2 {

public:

  Box2()
    : min( Vector2( Math::INF(), Math::INF() ) ), max( Vector2( -Math::INF(), -Math::INF() ) ) { }

  Box2( const Vector2& minIn, const Vector2& maxIn )
    : min( minIn ), max( maxIn ) { }

  Vector2 min, max;

  inline Box2& set( const Vector2& minIn, const Vector2& maxIn ) {

    min.copy(minIn);
    max.copy(maxIn);

    return *this;

  }

  inline Box2& copy( const Box2& b ) {

    min.copy(b.min);
    max.copy(b.max);

    return *this;

  }

  inline bool equals ( const Box2& box ) const {

    return box.min.equals( min ) && box.max.equals( max );

  }

  inline Box2 clone() const {

    return Box2(*this);

  }

  float distanceToPoint( const Vector2& point ) const;

  Box2& setFromPoints(const std::vector<Vector2>& points );
  Box2& setFromCenterAndSize( const Vector2& center, const Vector2& size );

  Box2& makeEmpty();
  bool empty() const;

  Box2& expandByPoint( const Vector2& point );
  Box2& expandByVector( const Vector2& vector );
  Box2& expandByScalar( float scalar );

  Vector2 center() const;
  Vector2& center( Vector2& target );

  bool containsBox( const Box2& box ) const;
  bool containsPoint ( const Vector2& point ) const;

  Vector2 clampPoint( const Vector2& point ) const;
  Vector2& clampPoint( const Vector2& point, Vector2& target );

  Vector2 size() const ;
  Vector2& size( Vector2& target );

  Box2& intersect( const Box2& box );
  bool isIntersectionBox( const Box2& box ) const;

  Box2& unionBox( const Box2& box ) ;
  Box2& translate( const Vector2& offset );

  Vector2 getParameter( const Vector2& point ) const;
  Vector2& getParameter( const Vector2& point, Vector2& target );

};

static_assert( sizeof( Box2 ) == sizeof( Vector2 ) * 2, "Invalid Box2 storage size" );

} // namespace three

#endif // THREE_BOX2_H