#ifndef THREE_PLANE_IPP
#define THREE_PLANE_IPP

#include <three/math/plane.hpp>
#include <three/math/line3.hpp>

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
  		constant = - point.dot( normal );	// must be normal, not normal, as normal is normalized

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
    auto divBy = normal.length();
    auto inverseNormalLength = 1.0 / divBy != 0.f ? divBy : NEAR_ZERO_FLOAT_32;
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

  Vector3 Plane::projectPoint( const Vector3& point, const Vector3& target ) const {
    return orthoPoint( point, target ).sub( point ).negate();
  }

  Vector3 Plane::orthoPoint( const Vector3& point) const {
    auto perpendicularMagnitude = distanceToPoint( point );
    auto result = Vector3();
    return result.copy( normal ).multiplyScalar( perpendicularMagnitude );
  }

  Vector3 Plane::orthoPoint( const Vector3& point, const Vector3& target ) const {
    auto perpendicularMagnitude = distanceToPoint( point );
    auto result = target;
    return result.copy( normal ).multiplyScalar( perpendicularMagnitude );
  }

  bool Plane::isIntersectionLine( const Line3& line ) const {
  		// Note: this tests if a line intersects the plane, not whether it (or its end-points) are coplanar with it.
    auto startSign = distanceToPoint( line.start );
    auto endSign = distanceToPoint( line.end );
    return ( startSign < 0 && endSign > 0 ) || ( endSign < 0 && startSign > 0 );
  }

  Vector3* Plane::intersectLine( const Line3& line) const {
    return intersectLine(line, Vector3());
  }

    // @todo refactor nullptr
  Vector3* Plane::intersectLine( const Line3& line, const Vector3& target) const {

    auto v1 = Vector3();
		auto result = target;// || new THREE.Vector3();
		auto direction = line.delta( v1 );
		auto denominator = normal.dot( direction );

		if ( denominator == 0 ) {
			// line is coplanar, return origin
			if( distanceToPoint( line.start ) == 0 ) {
				return &result.copy( line.start );
			}
			// Unsure if this is the correct method to handle this case.
			return nullptr;
		}

		auto t = - ( line.start.dot( normal ) + constant ) / denominator;

		if( t < 0 || t > 1 ) {
			return nullptr;
		}
		return &result.copy( direction ).multiplyScalar( t ).add( line.start );
 }

 Vector3 Plane::coplanarPoint() const {
  return coplanarPoint(Vector3());
}

Vector3 Plane::coplanarPoint( const Vector3& target ) const {
  auto result = target;
  return result.copy( normal ).multiplyScalar( - constant );
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

Plane Plane::clone() {
  return *this;
}



}

#endif //THREE_PLANE_IPP