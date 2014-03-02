#ifndef THREE_RAY_HPP
#define THREE_RAY_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/objects/mesh.hpp>

namespace three {
    
class Ray {
public:

  Vector3 origin;
  Vector3 direction;

  Ray( const Vector3& originIn = Vector3(), const Vector3& directionIn = Vector3() ) 
  : origin(originIn), direction(directionIn) {
  }

  THREE_DECL Ray& set( const Vector3& origin, const Vector3& direction );

  THREE_DECL Ray& copy( const Ray& ray );

  THREE_DECL Vector3& at( float t ) const;

  THREE_DECL Vector3& at( float t, const Vector3& optionalTarget ) const;

  THREE_DECL Ray& recast( float t );

  THREE_DECL Vector3& closestPointToPoint( const Vector3& point );

  THREE_DECL Vector3& closestPointToPoint( const Vector3& point, const Vector3& target );

  THREE_DECL float distanceToPoint( const Vector3& point ) const;

  THREE_DECL float distanceSqToSegment( const Vector3& v0, const Vector3& v1, Vector3* optionalPointOnRay = nullptr, Vector3* optionalPointOnSegment = nullptr ) const;

  THREE_DECL bool isIntersectionSphere( const Sphere& sphere ) const;

  THREE_DECL bool isIntersectionPlane( const Plane& plane ) const;

  THREE_DECL float distanceToPlane( const Plane& plane ) const;

  THREE_DECL Vector3::Ptr intersectPlane( const Vector3& plane );

  THREE_DECL Vector3::Ptr intersectPlane( const Plane& plane, const Vector3& target );

  THREE_DECL bool isIntersectionBox( const Box& box ) const;

  THREE_DECL Box3::Ptr intersectBox( const Box3& box );

  THREE_DECL Box3::Ptr intersectBox( const Box3& box , const Vector3& target );

  THREE_DECL Vector3::Ptr intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c, bool backfaceCulling, const Vector3& optionalTarget );

  THREE_DECL Ray& applyMatrix4( const Matrxi4& matrix4 );

  THREE_DECL bool equals( const Ray& ray ) const;

  THREE_DECL Ray clone();
  
};

} // namespace three

#endif // THREE
