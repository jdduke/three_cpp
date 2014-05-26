#include <three/math/line3.h>

namespace three {

Line3& Line3::set( const Vector3& startIn, const Vector3& endIn ) {

  start.copy( startIn );
  end.copy( endIn );

  return *this;

}

Line3& Line3::copy( const Line3& line ) {

  start.copy( line.start );
  end.copy( line.end );

  return *this;

}

Vector3 Line3::center() const {

  return Vector3().addVectors( start, end ).multiplyScalar( 0.5f );

}

Vector3& Line3::center( Vector3& target ) {

  return target.addVectors( start, end ).multiplyScalar( 0.5f );

}

Vector3 Line3::delta() const {

  return Vector3().subVectors( end, start );

}

Vector3& Line3::delta( Vector3& target ) {

  return target.subVectors( end, start );

}

float Line3::distanceSq() {

  return start.distanceToSquared( end );

}

float Line3::distance() {

  return start.distanceTo( end );

}

Vector3 Line3::at( float t ) {

  return delta().multiplyScalar( t ).add( start );

}

Vector3& Line3::at( float t, Vector3& target ) {

  return delta( target ).multiplyScalar( t ).add( start );

}

float Line3::closestPointToPointParameter( const Vector3& point, bool clampToLine ) const {

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

Vector3 Line3::closestPointToPoint( const Vector3& point, bool clampToLine ) {

  auto t = closestPointToPointParameter( point, clampToLine );

  return delta().multiplyScalar( t ).add( start );

}

Vector3& Line3::closestPointToPoint( const Vector3& point, bool clampToLine, Vector3& target ) {

  auto t = closestPointToPointParameter( point, clampToLine );

  return delta( target ).multiplyScalar( t ).add( start );

}

Line3& Line3::applyMatrix4( const Matrix4& matrix ) {

  start.applyMatrix4( matrix );
  end.applyMatrix4( matrix );

  return *this;

}

bool Line3::equals( const Line3& line ) const {

  return line.start.equals( start ) && line.end.equals( end );

}

Line3 Line3::clone() const {

  return *this;

}

} // namespace three
