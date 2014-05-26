#ifndef THREE_RAY_H
#define THREE_RAY_H

#include <three/common.h>
#include <three/math/vector3.h>
#include <three/utils/optional.h>

namespace three {

class THREE_DECL Ray {

public:

  Ray()
    : origin( Vector3() ), direction( Vector3() ) { };

  Ray( const Vector3& originIn )
    : origin( originIn ), direction( Vector3() ) { }

  Ray( const Vector3& originIn, const Vector3& directionIn )
    : origin( originIn ), direction( directionIn ) { }

  Vector3 origin;
  Vector3 direction;

  Ray& set( const Vector3& origin, const Vector3& direction );

  Ray& copy( const Ray& ray );

  Vector3 at( float t ) const;
  Vector3& at( float t, Vector3& target ) const;
  Vector3 at( float t, optional<Vector3>& target ) const;

  Ray& recast( float t );

  Vector3 closestPointToPoint( const Vector3& point );
  Vector3 closestPointToPoint( const Vector3& point, Vector3& target );

  float distanceToPoint( const Vector3& point ) const;
  float distanceSqToSegment( const Vector3& v0, const Vector3& v1, Vector3* optionalPointOnRay = nullptr, Vector3* optionalPointOnSegment = nullptr);

  bool isIntersectionSphere( const Sphere& sphere ) const;
  bool isIntersectionPlane( const Plane& plane ) const;

  float distanceToPlane( const Plane& plane ) const;

  optional<Vector3> intersectPlane( const Plane& plane );
  optional<Vector3> intersectPlane( const Plane& plane, Vector3& target );

  bool isIntersectionBox( const Box3& box );
  optional<Vector3> intersectBox( const Box3& box );
  optional<Vector3> intersectBox( const Box3& box, Vector3& target );
  optional<Vector3> intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c, bool backfaceCulling, optional<Vector3> optionalTarget = nullptr );

  Ray& applyMatrix4( const Matrix4& matrix4 );

  bool equals( const Ray& ray ) const;

  Ray clone() const;

};

} // namespace three

#endif // THREE
