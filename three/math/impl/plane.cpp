#ifndef THREE_PLANE_IPP
#define THREE_PLANE_IPP

#include <three/math/plane.h>
#include <three/math/line3.h>

namespace three {

Vector3 Plane::intersectLine( const Line3& line) const {

  auto direction = line.delta();
  auto denominator = normal.dot( direction );

  auto result = Vector3();

  if ( denominator == 0 ) {

      // line is coplanar, return origin
      if( distanceToPoint( line.start ) == 0 ) {

          return result.copy( line.start );

      }

      // Unsure if this is the correct method to handle this case.
      return result;

  }

  auto t = - ( line.start.dot( normal ) + constant ) / denominator;

  if( t < 0 || t > 1 ) {

      return result;

  }

  return result.copy( direction ).multiplyScalar( t ).add( line.start );

}

Vector3& Plane::intersectLine( const Line3& line, Vector3& target) {

	auto direction = line.delta();
	auto denominator = normal.dot( direction );

	if ( denominator == 0 ) {
		// line is coplanar, return origin
		if( distanceToPoint( line.start ) == 0 ) {

			return target.copy( line.start );

		}

		// Unsure if this is the correct method to handle this case.
		return target;

	}

	auto t = - ( line.start.dot( normal ) + constant ) / denominator;

	if( t < 0 || t > 1 ) {

		return target;

	}
	return target.copy( direction ).multiplyScalar( t ).add( line.start );
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


} // namespace three

#endif //THREE_PLANE_IPP