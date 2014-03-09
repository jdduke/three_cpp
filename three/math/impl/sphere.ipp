#ifndef THREE_SPHERE_IPP
#define THREE_SPHERE_IPP

#include <three/math/sphere.hpp>
#include <three/math/box3.hpp>
#include <three/math/vector3.hpp>

namespace three {

  inline Sphere& Sphere::set( const Vector3& center, float radius ) {

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

  inline Sphere& Sphere::copy( const Sphere& sphere ) {

    center.copy( sphere.center );
    radius = sphere.radius;

    return *this;

  }

  inline bool Sphere::empty() const {

    return ( radius <= 0.f );

  }

  inline bool Sphere::containsPoint( const Vector3& point ) const {

    return ( point.distanceToSquared( center ) <= ( radius * radius ) );

  }

  inline float Sphere::distanceToPoint( const Vector3& point ) const {

    return ( point.distanceTo( center ) - radius );

  }

  inline bool Sphere::intersectsSphere( const Sphere& sphere ) const {

    auto radiusSum = radius + sphere.radius;

    return sphere.center.distanceToSquared( center ) <= ( radiusSum * radiusSum );

  }

  inline Vector3 Sphere::clampPoint( const Vector3& point ) const {

    auto result = Vector3();

    clampPoint(point, result );

    return result;

  }

  inline Vector3& Sphere::clampPoint( const Vector3& point, Vector3& target ) const {

    auto deltaLengthSq = center.distanceToSquared( point );

    target.copy( point );

    if ( deltaLengthSq > ( radius * radius ) ) {

      target.sub( center ).normalize();
      target.multiplyScalar( radius ).add( center );

    }

    return target;

  }

  inline Box3 Sphere::getBoundingBox( ) const {

    auto result = Box3();

    getBoundingBox( result );

    return result;

  }

  inline Box3& Sphere::getBoundingBox( Box3& target ) const {

    target.set( center, center );
    target.expandByScalar( radius );

    return target;

  }

  inline Sphere& Sphere::applyMatrix4( const Matrix4& matrix ) {

    center.applyMatrix4( matrix );
    radius = radius * matrix.getMaxScaleOnAxis();

    return *this;

  }

  inline Sphere& Sphere::translate( const Vector3& offset ) {

    center.add( offset );

    return *this;

  }

  inline bool Sphere::equals( const Sphere& sphere ) const {

    return sphere.center.equals( center ) && ( sphere.radius == radius );

  }

  inline Sphere Sphere::clone() {

    return *this;

  }

};

}
#endif //THREE_SPHERE_IPP