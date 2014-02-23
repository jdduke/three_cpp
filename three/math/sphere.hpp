#ifndef THREE_SPHERE_HPP
#define THREE_SPHERE_HPP

#include <three/math/box3.hpp>

namespace three {

class Sphere {
public:

  Vector3 center;
  float radius;

  Sphere() {
    center = Vector3();
    radius = 0.f;
  }

  Sphere( Vector3 centerIn ) : center(centerIn) {
    radius = 0.f;
  }

  Sphere( Vector3 centerIn, float radiusIn) : center(centerIn), radius(radiusIn) { }

  Sphere& set( const Vector3& center, float radius ) {
    this->center.copy( center );
    this->radius = radius;
    return *this;
  }

  Sphere& setFromPoints( const std::vector<Vector3>& points) {
    auto box = Box3();
    auto center = Vector3();
    box.setFromPoints( points ).center( center ); 
    return setFromPoints(points, center);
  }

  
  Sphere& setFromPoints( const std::vector<Vector3>& points, const Vector3& center  ) {
    this->center.copy( center );

    auto maxRadiusSq = 0.f;

    // @todo efficiency? sort possible?
    for ( auto it = points.begin(); it < points.end(); it++ ) {
      maxRadiusSq = Math::max( maxRadiusSq, center.distanceToSquared( *it ) );
    }

    radius = Math::sqrt( maxRadiusSq );
    return *this;     
  }

  Sphere& copy( const Sphere& sphere ) {
    center.copy( sphere.center );
    radius = sphere.radius;
    return *this;
  }

  bool empty() const {
    return ( radius <= 0.f );
  }

  bool containsPoint( const Vector3& point ) const {
    return ( point.distanceToSquared( center ) <= ( radius * radius ) );
  }

  bool distanceToPoint( const Vector3& point ) const {
    return ( point.distanceTo( center ) - radius );
  }

  bool intersectsSphere( const Sphere& sphere ) const {
    auto radiusSum = radius + sphere.radius;
    return sphere.center.distanceToSquared( center ) <= ( radiusSum * radiusSum );
  }

  Vector3 clampPoint( const Vector3& point ) const {
    return clampPoint(point, Vector3());
  }

  Vector3 clampPoint( const Vector3& point, const Vector3& target ) const {

    auto deltaLengthSq = center.distanceToSquared( point );

    auto result = target;
    result.copy( point );

    if ( deltaLengthSq > ( radius * radius ) ) {
       result.sub( center ).normalize();
       result.multiplyScalar( radius ).add( center );
     }
     return result;
   }

   Box3 getBoundingBox( ) const {
      return getBoundingBox(Box3());
    }

    Box3 getBoundingBox( const Box3& target ) const {
      auto box = target;
      box.set( center, center );
      box.expandByScalar( radius );
      return box;
    }

    Sphere& applyMatrix4( const Matrix4& matrix ) {

      center.applyMatrix4( matrix );
      radius = radius * matrix.getMaxScaleOnAxis();
      return *this;
    }

    Sphere& translate( const Vector3& offset ) {
      center.add( offset );
      return *this;
    }

    bool equals( const Sphere& sphere ) const {
      return sphere.center.equals( center ) && ( sphere.radius == radius );
    }

    Sphere clone() {
      return *this;
    }
};

}
#endif //THREE_SPHERE_HPP