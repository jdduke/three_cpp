#ifndef THREE_LINE3_HPP
#define THREE_LINE3_HPP

#include <three/math/vector3.hpp>

namespace three {

class Line3 {
public:
    
    union {
        struct { Vector3 start, end; };
        Vector3 vectors[2];
    };

	Line3() {
		start = Vector3();
		end = Vector3();
	}

	Line3(const Vector3& startIn, const Vector3& endIn) : start(startIn), end(endIn) {}

	Line3& set( const Vector3& startIn, const Vector3& endIn ) {
		start.copy( startIn );
		end.copy( endIn );
		return *this;
	}

	Line3& copy( const Line3& line ) {
		start.copy( line.start );
		end.copy( line.end );
		return *this;
	}

	Vector3& center() const {
		return center(Vector3());
	}

	Vector3& center( const Vector3& target ) const {
		auto result = target;
		return result.addVectors( start, end ).multiplyScalar( 0.5f );
	}

	Vector3& delta() const {
		return delta(Vector3());
	}

	Vector3& delta( const Vector3& target ) const {
		auto result = target;
		return result.subVectors( end, start );
	}

	float distanceSq() {
		return start.distanceToSquared( end );
	}

	float distance() {
		return start.distanceTo( end );
	}

	Vector3& at( float t ) {
		return at(t, Vector3());
	}

	Vector3& at( float t, const Vector3& target ) {
		auto result = target;
		return delta( result ).multiplyScalar( t ).add( start );
	}

	float closestPointToPointParameter( const Vector3& point, bool clampToLine ) const {

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

	Vector3& closestPointToPoint( const Vector3& point, bool clampToLine ) {
		return closestPointToPoint(point, clampToLine, Vector3());
	}

	Vector3& closestPointToPoint( const Vector3& point, bool clampToLine, const Vector3& target ) {
		auto t = closestPointToPointParameter( point, clampToLine );
		auto result = target;
		return delta( result ).multiplyScalar( t ).add( start );
	}

	Line3& applyMatrix4( const Matrix4& matrix ) {
		start.applyMatrix4( matrix );
		end.applyMatrix4( matrix );
		return *this;
	}

	bool equals( const Line3& line ) const {
		return line.start.equals( start ) && line.end.equals( end );
	}

	Line3 clone() {
		return *this;
	}

};
    
}
#endif //THREE_LINE3_HPP