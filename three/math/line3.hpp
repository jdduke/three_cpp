#ifndef THREE_LINE3_HPP
#define THREE_LINE3_HPP

#include <three/math/vector3.hpp>

namespace three {

class Line3 {
public:
    
    Vector3 start, end; 

	Line3() {
		start = Vector3();
		end = Vector3();
	}

	Line3( const Vector3& startIn, const Vector3& endIn) : start(startIn), end(endIn) {}

	THREE_DECL Line3& set( const Vector3& startIn, const Vector3& endIn ) {
		start.copy( startIn );
		end.copy( endIn );
		return *this;
	}

	THREE_DECL Line3& copy( const Line3& line ) {
		start.copy( line.start );
		end.copy( line.end );
		return *this;
	}

	THREE_DECL Vector3 center() const {
		return Vector3().addVectors( start, end ).multiplyScalar( 0.5f );
	}

	THREE_DECL Vector3& center( Vector3& target ) {
		return target.addVectors( start, end ).multiplyScalar( 0.5f );
	}

	THREE_DECL Vector3 delta() const {
		return Vector3().subVectors( end, start );
	}

	THREE_DECL Vector3& delta( Vector3& target ) {
		return target.subVectors( end, start );
	}

	THREE_DECL float distanceSq() {
		return start.distanceToSquared( end );
	}

	THREE_DECL float distance() {
		return start.distanceTo( end );
	}

	THREE_DECL Vector3 at( float t ) {
		return delta().multiplyScalar( t ).add( start );
	}

	THREE_DECL Vector3& at( float t, Vector3& target ) {
		return delta( target ).multiplyScalar( t ).add( start );
	}

	THREE_DECL float closestPointToPointParameter( const Vector3& point, bool clampToLine ) const {

		auto startP = Vector3();
		auto startEnd = Vector3();

		startP.subVectors( point, start );
		startEnd.subVectors( end, start );

		auto startEnd2 = startEnd.dot( startEnd );
		auto startEnd_startP = startEnd.dot( startP );

		auto t = startEnd_startP / startEnd2;

		if ( clampToLine ) {
			t = Math::clamp( t, 0.f, 1.f );
		}
		return t;
	}

	THREE_DECL Vector3 closestPointToPoint( const Vector3& point, bool clampToLine ) {
		auto t = closestPointToPointParameter( point, clampToLine );
		return delta().multiplyScalar( t ).add( start );
	}

	THREE_DECL Vector3& closestPointToPoint( const Vector3& point, bool clampToLine, Vector3& target ) {
		auto t = closestPointToPointParameter( point, clampToLine );
		return delta( target ).multiplyScalar( t ).add( start );
	}

	THREE_DECL Line3& applyMatrix4( const Matrix4& matrix ) {
		start.applyMatrix4( matrix );
		end.applyMatrix4( matrix );
		return *this;
	}

	THREE_DECL bool equals( const Line3& line ) const {
		return line.start.equals( start ) && line.end.equals( end );
	}

	THREE_DECL Line3 clone() {
		return *this;
	}

};
    
}
#endif //THREE_LINE3_HPP