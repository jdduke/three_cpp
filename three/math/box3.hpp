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

  inline Box3& set( const Vector3& minIn, const Vector3& maxIn );

  inline Box3& addPoint ( const Vector3& point );

  inline Box3& setFromPoints(const std::vector<Vector3>& points );

  inline Box3& setFromCenterAndSize( const Vector3& center, const Vector3& size );

  Box3& setFromObject ( Object3D& object);

  inline Box3& copy( const Box3& b );

  inline Box3& makeEmpty();

  inline bool empty() const;

  inline Vector3 center() const;

  inline Vector3& center( Vector3& target );

  inline Vector3 size() const;

  inline Vector3& size( Vector3& target );

  inline Box3& expandByPoint( const Vector3& point );

  inline Box3& expandByVector( const Vector3& vector );

  inline Box3& expandByScalar( float scalar );

  inline bool containsPoint ( const Vector3& point ) const;

  inline bool containsBox( const Box3& box ) const;

  inline Vector3 getParameter( const Vector3& point ) const;

  inline Vector3& getParameter( const Vector3& point, Vector3& target );

  inline bool isIntersectionBox( const Box3& box ) const;

  inline Vector3 clampPoint( const Vector3& point ) const;

  inline Vector3& clampPoint( const Vector3& point, Vector3& target );

  inline float distanceToPoint( const Vector3& point ) const;

  inline Sphere getBoundingSphere() const;

  inline Sphere& getBoundingSphere( Sphere& target);

  inline Box3& intersect( const Box3& box );

  inline Box3& unionBox( const Box3& box );

  Box3& applyMatrix4(const Matrix4& matrix);

  inline Box3& translate( float& offset );

  inline bool equals ( const Box3& box ) const;

  inline Box3 clone() const;

}; // end namespace

static_assert( sizeof( Box3 ) == sizeof( Vector3 ) * 2, "Invalid Box3 storage size" );

}

#endif // THREE_BOX3_HPP