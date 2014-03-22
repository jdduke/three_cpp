#ifndef THREE_RAY_HPP
#define THREE_RAY_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/objects/mesh.hpp>

namespace three {
    
class Ray {

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

  Vector3::Ptr at( float t, Vector3::Ptr target ) const;

  Ray& recast( float t );

  Vector3 closestPointToPoint( const Vector3& point );

  Vector3& closestPointToPoint( const Vector3& point, Vector3& target );

  float distanceToPoint( const Vector3& point ) const;

  float distanceSqToSegment( const Vector3& v0, const Vector3& v1, Vector3::Ptr optionalPointOnRay = nullptr, Vector3::Ptr optionalPointOnSegment = nullptr );

  bool isIntersectionSphere( const Sphere& sphere ) const;

  bool isIntersectionPlane( const Plane& plane ) const;

  float distanceToPlane( const Plane& plane ) const;

  Vector3 intersectPlane( const Plane& plane );

  Vector3::Ptr intersectPlane( const Plane& plane, Vector3& target );

  bool isIntersectionBox( const Box3& box );

  Vector3::Ptr intersectBox( const Box3& box );

  Vector3::Ptr intersectBox( const Box3& box, Vector3& target );

  Vector3::Ptr intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c, bool backfaceCulling, Vector3::Ptr optionalTarget );

  Ray& applyMatrix4( const Matrix4& matrix4 );

  bool equals( const Ray& ray ) const;

  Ray clone();

};

} // namespace three

#endif // THREE
