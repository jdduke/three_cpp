#ifndef THREE_LINE3_H
#define THREE_LINE3_H

#include <three/common.h>
#include <three/math/vector3.h>

namespace three {

class THREE_DECL Line3 {

public:

  Line3() : start( Vector3() ), end( Vector3() ) {}

  Line3( Vector3 startIn, Vector3 endIn) : start( startIn ), end( endIn ) {}

  Vector3 start, end;

  Line3& set( const Vector3& startIn, const Vector3& endIn );
  Line3& copy( const Line3& line );

  Vector3 center() const;
  Vector3& center( Vector3& target );

  Vector3 delta() const;
  Vector3& delta( Vector3& target );

  float distanceSq();
  float distance();

  Vector3 at( float t );
  Vector3& at( float t, Vector3& target );

  float closestPointToPointParameter( const Vector3& point, bool clampToLine ) const;
  Vector3 closestPointToPoint( const Vector3& point, bool clampToLine );
  Vector3& closestPointToPoint( const Vector3& point, bool clampToLine, Vector3& target );

  Line3& applyMatrix4( const Matrix4& matrix );

  bool equals( const Line3& line ) const;

  Line3 clone() const;

};

} // namespace three

#endif //THREE_LINE3_H