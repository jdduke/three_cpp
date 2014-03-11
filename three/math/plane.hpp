#ifndef THREE_PLANE_HPP
#define THREE_PLANE_HPP

#include <three/common.hpp>
#include <three/math/line3.hpp>

namespace three {

class Plane {

public: 

  Plane() 
    : normal( Vector3( 1.f, 0.f, 0.f) ), constant( 0.f ) {}

  Plane( Vector3 v, float constant) 
    : normal( v ), constant( constant ) {}

  Vector3 normal;

  float constant;

  Plane& set( const Vector3& normalIn, float constantIn ) {

    normal.copy( normalIn );
    constant = constantIn;

    return *this;

  }

  Plane& setComponents( float x, float y, float z, float w ) {

    normal.set( x, y, z );
    constant = w;

    return *this;

  }

  // @todo point should be vec3?
  Plane& setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point ) {

    this->normal.copy( normal );
  constant = - point.dot( normal ); // must be normal, not normal, as normal is normalized

  return *this;

}

Plane& setFromCoplanarPoints( const Vector3& a, const Vector3& b, const Vector3& c) {

  auto v1 = Vector3();
  auto v2 = Vector3();

  auto normal = v1.subVectors( c, b ).cross( v2.subVectors( a, b ) ).normalize();

    // Q: should an error be thrown if normal is zero (e.g. degenerate plane)?
  setFromNormalAndCoplanarPoint( normal, a );

  return *this;

}


Plane& copy( const Plane& plane ) {

  normal.copy( plane.normal );
  constant = plane.constant;

  return *this;

}

Plane& normalize() {

  auto divBy = normal.length();
  auto inverseNormalLength = 1.0 / divBy != 0.f ? divBy : NEAR_ZERO_FLOAT_32;

  normal.multiplyScalar( inverseNormalLength );
  constant *= inverseNormalLength;

  return *this;

}

Plane& negate() {

  constant *= -1.f;
  normal.negate();

  return *this;

}

float distanceToPoint( const Vector3& point ) const {

  return normal.dot( point ) + constant;

}

float distanceToSphere( const Sphere& sphere ) const {

  return distanceToPoint( sphere.center ) - sphere.radius;

}

Vector3 projectPoint( const Vector3& point ) const {

  return orthoPoint( point ).sub( point ).negate();

}

Vector3& projectPoint( const Vector3& point, Vector3& target ) {

  return orthoPoint( point, target ).sub( point ).negate();

}

Vector3 orthoPoint( const Vector3& point) const {

  auto perpendicularMagnitude = distanceToPoint( point );

  return Vector3().copy( normal ).multiplyScalar( perpendicularMagnitude );

}

Vector3& orthoPoint( const Vector3& point, Vector3& target ) {

  auto perpendicularMagnitude = distanceToPoint( point );

  return target.copy( normal ).multiplyScalar( perpendicularMagnitude );

}

bool isIntersectionLine( const Line3& line ) const {

  // Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.
  auto startSign = distanceToPoint( line.start );
  auto endSign = distanceToPoint( line.end );

  return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );

}

Vector3 intersectLine( const Line3& line) const;

Vector3& intersectLine( const Line3& line, Vector3& target);

inline Vector3 coplanarPoint() const {

  return coplanarPoint(Vector3());

}

inline Vector3 coplanarPoint( const Vector3& target ) const {

  auto result = target;

  return result.copy( normal ).multiplyScalar( - constant );

}

Plane& applyMatrix4( const Matrix4& matrix );

Plane& applyMatrix4( const Matrix4& matrix, const Matrix3& normalMatrix);

inline Plane& translate( const Vector3& offset ) {

  constant = constant - offset.dot( normal );

  return *this;

}

inline bool equals( const Plane& plane ) const {

  return plane.normal.equals( normal ) && ( plane.constant == constant );

}

inline Plane clone() {

  return *this;

}

};

}

#endif //THREE_PLANE_HPP