#include <three/math/plane.h>

#include <three/math/math.h>
#include <three/math/line3.h>
#include <three/math/matrix3.h>
#include <three/math/sphere.h>
#include <three/utils/optional.h>

namespace three {

Plane& Plane::set( const Vector3& normalIn, float constantIn ) {

  normal.copy( normalIn );
  constant = constantIn;

  return *this;

}

Plane& Plane::setComponents( float x, float y, float z, float w ) {

  normal.set( x, y, z );
  constant = w;

  return *this;

}

// @todo point should be vec3?
Plane& Plane::setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point ) {

  this->normal.copy( normal );
  constant = - point.dot( normal ); // must be normal, not normal, as normal is normalized

  return *this;

}

Plane& Plane::setFromCoplanarPoints( const Vector3& a, const Vector3& b, const Vector3& c) {

  auto v1 = Vector3();
  auto v2 = Vector3();

  auto normal = v1.subVectors( c, b ).cross( v2.subVectors( a, b ) ).normalize();

  // Q: should an error be thrown if normal is zero (e.g. degenerate plane)?
  setFromNormalAndCoplanarPoint( normal, a );

  return *this;

}

Plane& Plane::copy( const Plane& plane ) {

  normal.copy( plane.normal );
  constant = plane.constant;

  return *this;

}

Plane& Plane::normalize() {

  auto inverseNormalLength = 1.0f / normal.length();

  normal.multiplyScalar( inverseNormalLength );
  constant *= inverseNormalLength;

  return *this;

}

Plane& Plane::negate() {

  constant *= -1.f;
  normal.negate();

  return *this;

}

float Plane::distanceToPoint( const Vector3& point ) const {

  return normal.dot( point ) + constant;

}

float Plane::distanceToSphere( const Sphere& sphere ) const {

  return distanceToPoint( sphere.center ) - sphere.radius;

}

Vector3 Plane::projectPoint( const Vector3& point ) const {

  return orthoPoint( point ).sub( point ).negate();

}

Vector3& Plane::projectPoint( const Vector3& point, Vector3& target ) {

  return orthoPoint( point, target ).sub( point ).negate();

}

Vector3 Plane::orthoPoint( const Vector3& point) const {

  auto perpendicularMagnitude = distanceToPoint( point );

  return Vector3().copy( normal ).multiplyScalar( perpendicularMagnitude );

}

Vector3& Plane::orthoPoint( const Vector3& point, Vector3& target ) {

  auto perpendicularMagnitude = distanceToPoint( point );

  return target.copy( normal ).multiplyScalar( perpendicularMagnitude );

}

bool Plane::isIntersectionLine( const Line3& line ) const {

  // Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.
  auto startSign = distanceToPoint( line.start );
  auto endSign = distanceToPoint( line.end );

  return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );

}

optional<Vector3> Plane::intersectLine( const Line3& line) {

  auto target = Vector3();

  return intersectLine(line, target);

}

optional<Vector3> Plane::intersectLine( const Line3& line, Vector3& target ) {

  auto direction = line.delta();
  auto denominator = normal.dot( direction );

  if ( denominator == 0 ) {
    // line is coplanar, return origin
    if( distanceToPoint( line.start ) == 0 ) {

      target.copy( line.start );
      return target;

    }

    // Unsure if this is the correct method to handle this case.
    return nullptr;

  }

  auto t = - ( line.start.dot( normal ) + constant ) / denominator;

  if( t < 0 || t > 1 ) {

    return nullptr;

  }

  return target.copy( direction ).multiplyScalar( t ).add( line.start );
}

Vector3 Plane::coplanarPoint() const {

  Vector3 temp;
  return coplanarPoint( temp );

}

Vector3& Plane::coplanarPoint( Vector3& target ) const {

  return target.copy( normal ).multiplyScalar( - constant );

}

Plane& Plane::applyMatrix4( const Matrix4& matrix ) {

  auto normalMatrix = Matrix3().getNormalMatrix( matrix );

  return this->applyMatrix4(matrix, normalMatrix );

}

Plane& Plane::applyMatrix4( const Matrix4& matrix, const Matrix3& normalMatrix) {

  auto v1 = Vector3();
  auto v2 = Vector3();
  //auto m1 = Matrix3();

  // compute new normal based on theory here:
  // http://www.songho.ca/opengl/gl_normaltransform.html
  auto newNormal = v1.copy( normal ).applyMatrix3( normalMatrix );

  auto newCoplanarPoint = coplanarPoint( v2 );
  newCoplanarPoint.applyMatrix4( matrix );

  setFromNormalAndCoplanarPoint( newNormal, newCoplanarPoint );

  return *this;

}

Plane& Plane::translate( const Vector3& offset ) {

  constant = constant - offset.dot( normal );

  return *this;

}

bool Plane::equals( const Plane& plane ) const {

  return plane.normal.equals( normal ) && ( plane.constant == constant );

}

Plane Plane::clone() const {

  return *this;

}

} // namespace three
