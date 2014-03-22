#ifndef THREE_LINE3_HPP
#define THREE_LINE3_HPP

#include <three/common.h>

namespace three {

class Line3 {

public:

	Line3() : start( Vector3() ), end( Vector3() ) {}

	Line3( Vector3 startIn, Vector3 endIn) : start( startIn ), end( endIn ) {}

  Vector3 start, end;

	inline Line3& set( const Vector3& startIn, const Vector3& endIn ) {

		start.copy( startIn );
		end.copy( endIn );

		return *this;

	}

	inline Line3& copy( const Line3& line ) {

		start.copy( line.start );
		end.copy( line.end );

		return *this;

	}

	inline Vector3 center() const {

		return Vector3().addVectors( start, end ).multiplyScalar( 0.5f );

	}

	inline Vector3& center( Vector3& target ) {

		return target.addVectors( start, end ).multiplyScalar( 0.5f );

	}

	inline Vector3 delta() const {

		return Vector3().subVectors( end, start );

	}

	inline Vector3& delta( Vector3& target ) {

		return target.subVectors( end, start );

	}

	inline float distanceSq() {

		return start.distanceToSquared( end );

	}

	inline float distance() {

		return start.distanceTo( end );

	}

	inline Vector3 at( float t ) {

		return delta().multiplyScalar( t ).add( start );

	}

	inline Vector3& at( float t, Vector3& target ) {

		return delta( target ).multiplyScalar( t ).add( start );

	}

	inline float closestPointToPointParameter( const Vector3& point, bool clampToLine ) const {

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

	inline Vector3 closestPointToPoint( const Vector3& point, bool clampToLine ) {

		auto t = closestPointToPointParameter( point, clampToLine );

		return delta().multiplyScalar( t ).add( start );

	}

	inline Vector3& closestPointToPoint( const Vector3& point, bool clampToLine, Vector3& target ) {

		auto t = closestPointToPointParameter( point, clampToLine );

		return delta( target ).multiplyScalar( t ).add( start );

	}

	inline Line3& applyMatrix4( const Matrix4& matrix ) {

		start.applyMatrix4( matrix );
		end.applyMatrix4( matrix );

		return *this;

	}

	inline bool equals( const Line3& line ) const {

		return line.start.equals( start ) && line.end.equals( end );

	}

	inline Line3 clone() {

		return *this;

	}
  
};
    
} // namespace three

#endif //THREE_LINE3_HPP