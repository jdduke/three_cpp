#ifndef THREE_SPHERE_HPP
#define THREE_SPHERE_HPP

#include <three/common.hpp>
#include <three/math/matrix4.hpp>

namespace three {

class Sphere {

public:

  Sphere() 
    : center( Vector3() ), radius( 0.f ) {}

  Sphere( Vector3 centerIn ) 
    : center( centerIn), radius( 0.f ) {}

  Sphere( Vector3 centerIn, float radiusIn) 
    : center(centerIn), radius(radiusIn) {}

  Vector3 center;
  
  float radius;

  inline Sphere& set( const Vector3& center, float radius ) {

    this->center.copy( center );
    this->radius = radius;

    return *this;

  }

  Sphere& setFromPoints( const std::vector<Vector3>& points);
  
  Sphere& setFromPoints( const std::vector<Vector3>& points, const Vector3& center  );

  inline Sphere& copy( const Sphere& sphere ) {

    center.copy( sphere.center );
    radius = sphere.radius;

    return *this;

  }

  inline bool empty() const {

    return ( radius <= 0.f );

  }

  inline bool containsPoint( const Vector3& point ) const {

    return ( point.distanceToSquared( center ) <= ( radius * radius ) );

  }

  inline float distanceToPoint( const Vector3& point ) const {

    return ( point.distanceTo( center ) - radius );

  }

  inline bool intersectsSphere( const Sphere& sphere ) const {

    auto radiusSum = radius + sphere.radius;

    return sphere.center.distanceToSquared( center ) <= ( radiusSum * radiusSum );

  }

  inline Vector3 clampPoint( const Vector3& point ) const {

    auto result = Vector3();

    clampPoint(point, result );

    return result;

  }

  inline Vector3& clampPoint( const Vector3& point, Vector3& target ) const {

    auto deltaLengthSq = center.distanceToSquared( point );

    target.copy( point );

    if ( deltaLengthSq > ( radius * radius ) ) {

      target.sub( center ).normalize();
      target.multiplyScalar( radius ).add( center );

    }

    return target;

  }

  Box3 getBoundingBox( ) const;

  Box3& getBoundingBox( Box3& target ) const;

  inline Sphere& applyMatrix4( const Matrix4& matrix ) {

    center.applyMatrix4( matrix );
    radius = radius * matrix.getMaxScaleOnAxis();

    return *this;

  }

  inline Sphere& translate( const Vector3& offset ) {

    center.add( offset );

    return *this;

  }

  inline bool equals( const Sphere& sphere ) const {

    return sphere.center.equals( center ) && ( sphere.radius == radius );

  }

  inline Sphere clone() {

    return *this;

  }

};

} // end namespace

#endif //THREE_SPHERE_HPP