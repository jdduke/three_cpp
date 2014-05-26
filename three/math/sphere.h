#ifndef THREE_SPHERE_H
#define THREE_SPHERE_H

#include <three/common.h>

#include <three/math/vector3.h>

namespace three {

class THREE_DECL Sphere {

public:

  THREE_TYPE(Sphere)

  Sphere()
    : center( Vector3() ), radius( 0.f ) {}

  Sphere( Vector3 centerIn )
    : center( centerIn), radius( 0.f ) {}

  Sphere( Vector3 centerIn, float radiusIn)
    : center(centerIn), radius(radiusIn) {}

  Vector3 center;

  float radius;

  Sphere& set( const Vector3& center, float radius );
  Sphere& setFromPoints( const std::vector<Vector3>& points);
  Sphere& setFromPoints( const std::vector<Vector3>& points, const Vector3& center  );

  Sphere& copy( const Sphere& sphere );

  bool empty() const;

  bool containsPoint( const Vector3& point ) const;

  float distanceToPoint( const Vector3& point ) const;

  bool intersectsSphere( const Sphere& sphere ) const;

  Vector3 clampPoint( const Vector3& point ) const;
  Vector3& clampPoint( const Vector3& point, Vector3& target ) const;

  Box3 getBoundingBox( ) const;
  Box3& getBoundingBox( Box3& target ) const;

  Sphere& applyMatrix4( const Matrix4& matrix );

  Sphere& translate( const Vector3& offset );

  bool equals( const Sphere& sphere ) const;

  Sphere clone() const;

};

} // namespace three

#endif //THREE_SPHERE_H