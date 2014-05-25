#ifndef THREE_BOX3_H
#define THREE_BOX3_H

#include <three/common.h>
#include <three/utils/optional.h>
#include <three/math/vector4.h>
#include <three/math/sphere.h>

namespace three {

class THREE_DECL Box3 {

public:

  THREE_TYPE(Box3)

  Box3()
    : min( Vector3( Math::INF(), Math::INF(), Math::INF() ) ), max( -Math::INF(), -Math::INF(), -Math::INF() ) {}

  Box3( Vector3 minIn, Vector3 maxIn )
    : min( minIn ), max( maxIn ) {}

  Vector3 min, max;

  inline Box3& set( const Vector3& minIn, const Vector3& maxIn ) {

    min.copy(minIn);
    max.copy(maxIn);

    return *this;

  }

  inline bool equals ( const Box3& box ) const {

    return box.min.equals( min ) && box.max.equals( max );

  }

  inline Box3 clone() const {

    return Box3(*this);

  }

  Box3& addPoint ( const Vector3& point );
  Box3& addPoint ( const Vector4& point );

  Box3& setFromCenterAndSize( const Vector3& center, const Vector3& size );
  Box3& setFromObject ( Object3D& object );

  Box3& copy( const Box3& b );

  Box3& makeEmpty();
  bool empty() const;

  Vector3 center() const;
  Vector3& center( Vector3& target );

  Vector3 size() const;
  Vector3& size( Vector3& target );

  Box3& expandByPoint( const Vector3& point );
  Box3& expandByVector( const Vector3& vector );
  Box3& expandByScalar( float scalar );

  bool containsPoint ( const Vector3& point ) const;
  bool containsBox( const Box3& box ) const;

  optional<Vector3> getParameter( const Vector3& point ) const;
  optional<Vector3> getParameter( const Vector3& point, Vector3& target ) const;

  bool isIntersectionBox( const Box3& box ) const;

  Vector3 clampPoint( const Vector3& point ) const;
  Vector3& clampPoint( const Vector3& point, Vector3& target );

  float distanceToPoint( const Vector3& point ) const;

  Sphere getBoundingSphere() const;
  Sphere& getBoundingSphere( Sphere& target);

  Box3& intersect( const Box3& box );

  Box3& unionBox( const Box3& box );

  Box3& applyMatrix4( const Matrix4& matrix);

  Box3& translate( const Vector3& offset );

  template <typename C>
  Box3& setFromPoints(const C& points ) {

    auto it = points.begin();

    if ( it == points.end() ) {

      makeEmpty();

      return *this;

    }

    min.copy( *it );
    max.copy( *it );

    for ( ++it; it != points.end(); ++it ) {

      addPoint(*it);

    }

    return *this;

  }

}; // namespace three

}

#endif // THREE_BOX3_H