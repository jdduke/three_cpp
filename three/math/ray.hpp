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

  Ray()
    : origin(Vector3()), direction(Vector3()) { };

  Ray( const Vector3& originIn ) 
    : origin(originIn), direction(Vector3()) { }

  Ray( const Vector3& originIn, const Vector3& directionIn ) 
    : origin(originIn), direction(directionIn) { }

  inline Ray& set( const Vector3& origin, const Vector3& direction );

  inline Ray& copy( const Ray& ray );

  inline Vector3 at( float t ) const;

  inline Vector3 at( float t, const Vector3& optionalTarget ) const;

  inline Ray& recast( float t );

  inline Vector3 closestPointToPoint( const Vector3& point );

  inline Vector3 closestPointToPoint( const Vector3& point, const Vector3& target );

  inline float distanceToPoint( const Vector3& point ) const;

  float distanceSqToSegment( const Vector3& v0, const Vector3& v1, Vector3* optionalPointOnRay = nullptr, Vector3* optionalPointOnSegment = nullptr ) const;

  inline bool isIntersectionSphere( const Sphere& sphere ) const;

  inline bool isIntersectionPlane( const Plane& plane ) const;

  inline float distanceToPlane( const Plane& plane ) const;

  inline Vector3 intersectPlane( const Vector3& plane );

  inline Vector3 intersectPlane( const Plane& plane, Vector3& target );

  bool isIntersectionBox( const Box& box ) const;

  Box3 intersectBox( const Box3& box );

  Box3 intersectBox( const Box3& box , Vector3& target );

  Vector3 intersectTriangle( const Vector3& a, const Vector3& b, const Vector3& c, bool backfaceCulling, const Vector3& optionalTarget );

  Ray& applyMatrix4( const Matrix4& matrix4 );

  inline bool equals( const Ray& ray ) const;

  inline Ray clone();

};

} // namespace three

#endif // THREE
