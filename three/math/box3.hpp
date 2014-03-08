#ifndef THREE_BOX3_HPP
#define THREE_BOX3_HPP

#include <three/math/vector3.hpp>

namespace three {

class Box3 {

public:

  Vector3 min, max;

  Box3()
    : min(Vector3( Math::INF(), Math::INF(), Math::INF() )), max(-Math::INF(), -Math::INF(), -Math::INF()) {}

  Box3( const Vector3& minIn, const Vector3& maxIn )
    : min( minIn ), max( maxIn ) {}

  THREE_DECL inline Box3& set( const Vector3& minIn, const Vector3& maxIn );

  THREE_DECL inline Box3& addPoint ( const Vector3& point );

  THREE_DECL inline Box3& setFromPoints(const std::vector<Vector3>& points );

  THREE_DECL inline Box3& setFromCenterAndSize( const Vector3& center, const Vector3& size );

  THREE_DECL Box3& setFromObject ( Object3D& object);

  THREE_DECL inline Box3& copy( const Box3& b );

  THREE_DECL inline Box3& makeEmpty();

  THREE_DECL inline bool empty() const;

  THREE_DECL inline Vector3 center() const;

  THREE_DECL inline Vector3& center( Vector3& target );

  THREE_DECL inline Vector3 size() const;

  THREE_DECL inline Vector3& size( Vector3& target );

  THREE_DECL inline Box3& expandByPoint( const Vector3& point );

  THREE_DECL inline Box3& expandByVector( const Vector3& vector );

  THREE_DECL inline Box3& expandByScalar( float scalar );

  THREE_DECL inline bool containsPoint ( const Vector3& point ) const;

  THREE_DECL inline bool containsBox( const Box3& box ) const;

  THREE_DECL inline Vector3 getParameter( const Vector3& point ) const;

  THREE_DECL inline Vector3& getParameter( const Vector3& point, Vector3& target );

  THREE_DECL inline bool isIntersectionBox( const Box3& box ) const;

  THREE_DECL inline Vector3 clampPoint( const Vector3& point ) const;

  THREE_DECL inline Vector3& clampPoint( const Vector3& point, Vector3& target );

  THREE_DECL inline float distanceToPoint( const Vector3& point ) const;

  THREE_DECL inline Sphere getBoundingSphere() const;

  THREE_DECL inline Sphere& getBoundingSphere( Sphere& target);

  THREE_DECL inline Box3& intersect( const Box3& box );

  THREE_DECL inline Box3& unionBox( const Box3& box );

  THREE_DECL Box3& applyMatrix4(const Matrix4& matrix);

  THREE_DECL inline Box3& translate( float& offset );

  THREE_DECL inline bool equals ( const Box3& box ) const;

  THREE_DECL inline Box3 clone() const;

}; // end namespace

static_assert( sizeof( Box3 ) == sizeof( Vector3 ) * 2, "Invalid Box3 storage size" );

}

#endif // THREE_BOX3_HPP