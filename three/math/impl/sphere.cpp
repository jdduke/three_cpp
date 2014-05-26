#include <three/math/sphere.h>

#include <three/math/box3.h>
#include <three/math/matrix4.h>

namespace three {

Sphere& Sphere::set( const Vector3& center, float radius ) {

  this->center.copy( center );
  this->radius = radius;

  return *this;

}

Sphere& Sphere::setFromPoints( const std::vector<Vector3>& points) {

  auto box = Box3();
  auto center = Vector3();

  box.setFromPoints( points ).center( center );

  return setFromPoints(points, center);

}

Sphere& Sphere::setFromPoints( const std::vector<Vector3>& points, const Vector3& center  ) {

  this->center.copy( center );

  auto maxRadiusSq = 0.f;

  for ( auto it = points.begin(); it < points.end(); it++ ) {
    maxRadiusSq = Math::max( maxRadiusSq, center.distanceToSquared( *it ) );
  }

  radius = Math::sqrt( maxRadiusSq );

  return *this;

}


Sphere& Sphere::copy( const Sphere& sphere ) {

  center.copy( sphere.center );
  radius = sphere.radius;

  return *this;

}

bool Sphere::empty() const {

  return ( radius <= 0.f );

}

bool Sphere::containsPoint( const Vector3& point ) const {

  return ( point.distanceToSquared( center ) <= ( radius * radius ) );

}

float Sphere::distanceToPoint( const Vector3& point ) const {

  return ( point.distanceTo( center ) - radius );

}

bool Sphere::intersectsSphere( const Sphere& sphere ) const {

  auto radiusSum = radius + sphere.radius;

  return sphere.center.distanceToSquared( center ) <= ( radiusSum * radiusSum );

}

Vector3 Sphere::clampPoint( const Vector3& point ) const {

  auto result = Vector3();

  clampPoint(point, result );

  return result;

}

Vector3& Sphere::clampPoint( const Vector3& point, Vector3& target ) const {

  auto deltaLengthSq = center.distanceToSquared( point );

  target.copy( point );

  if ( deltaLengthSq > ( radius * radius ) ) {

    target.sub( center ).normalize();
    target.multiplyScalar( radius ).add( center );

  }

  return target;

}

Box3 Sphere::getBoundingBox( ) const {

  auto result = Box3();

  getBoundingBox( result );

  return result;

}

Box3& Sphere::getBoundingBox( Box3& target ) const {

  target.set( center, center );
  target.expandByScalar( radius );

  return target;

}

Sphere& Sphere::applyMatrix4( const Matrix4& matrix ) {

  center.applyMatrix4( matrix );
  radius = radius * matrix.getMaxScaleOnAxis();

  return *this;

}

Sphere& Sphere::translate( const Vector3& offset ) {

  center.add( offset );

  return *this;

}

bool Sphere::equals( const Sphere& sphere ) const {

  return sphere.center.equals( center ) && ( sphere.radius == radius );

}

Sphere Sphere::clone() const {

  return *this;

}

} // end namespace
