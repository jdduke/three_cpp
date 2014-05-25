#include <three/math/sphere.h>

#include <three/math/box3.h>
#include <three/math/vector3.h>

namespace three {

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

} // end namespace
