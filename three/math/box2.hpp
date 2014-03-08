#ifndef THREE_BOX2_HPP
#define THREE_BOX2_HPP

#include <three/common.hpp>
#include <three/math/math.hpp>

namespace three {

class Box2 {

public:

  Vector2 min, max;

  Box2()
    : min( Vector2() ), max( Vector2() ) { }

  Box2( const Vector2& minIn, const Vector2& maxIn )
    : min( minIn ), max( maxIn ) { }

  inline Box2& set( const Vector2& minIn, const Vector2& maxIn );

  inline Box2& copy( const Box2& b );

  inline float distanceToPoint( const Vector2& point ) const;

  inline Box2& setFromPoints(const std::vector<Vector2>& points );

  inline Box2& setFromCenterAndSize( const Vector2& center, const Vector2& size );

  inline Box2& makeEmpty();

  inline bool empty() const;

  inline Box2& expandByPoint( const Vector2& point );

  inline Box2& expandByVector( const Vector2& vector );

  inline Box2& expandByScalar( float scalar );

  inline Vector2 center() const;

  inline Vector2& center( Vector2& target );

  inline bool containsBox( const Box2& box ) const;

  inline bool containsPoint ( const Vector2& point ) const;

  inline Vector2 clampPoint( const Vector2& point ) const;

  inline Vector2 clampPoint( const Vector2& point, Vector2& target );

  inline Box2 clone() const;

  inline Vector2 size() const;

  inline Vector2& size( Vector2& target );

  inline Box2& intersect( const Box2& box );

  inline bool isIntersectionBox( const Box2& box ) const;

  inline Box2& unionBox( const Box2& box );

  inline Box2& translate( float& offset );

  inline bool equals ( const Box2& box ) const;

  inline Vector2 getParameter( const Vector2& point ) const;

  inline Vector2& getParameter( const Vector2& point, Vector2& target );

};

static_assert( sizeof( Box2 ) == sizeof( Vector2 ) * 2, "Invalid Box2 storage size" );

} // namespace three

#endif // THREE_BOX2_HPP