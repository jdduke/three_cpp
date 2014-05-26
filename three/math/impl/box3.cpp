#include <three/math/box3.h>

#include <three/core/object3d.h>

#include <three/math/vector3.h>

namespace three {

Box3& Box3::set( const Vector3& minIn, const Vector3& maxIn ) {

  min.copy(minIn);
  max.copy(maxIn);

  return *this;

}

Box3& Box3::addPoint ( const Vector3& point ) {

  if ( point.x < min.x ) {

    min.x = point.x;

  } else if ( point.x > max.x ) {

    max.x = point.x;

  }

  if ( point.y < min.y ) {

    min.y = point.y;

  } else if ( point.y > max.y ) {

    max.y = point.y;

  }

  if ( point.z < min.z ) {

    min.z = point.z;

  } else if ( point.z > max.z ) {

    max.z = point.z;

  }

  return *this;

}

Box3& Box3::addPoint ( const Vector4& point ) {

  if ( point.x < min.x ) {

    min.x = point.x;

  } else if ( point.x > max.x ) {

    max.x = point.x;

  }

  if ( point.y < min.y ) {

    min.y = point.y;

  } else if ( point.y > max.y ) {

    max.y = point.y;

  }

  if ( point.z < min.z ) {

    min.z = point.z;

  } else if ( point.z > max.z ) {

    max.z = point.z;

  }

  return *this;

}

Box3& Box3::setFromCenterAndSize( const Vector3& center, const Vector3& size ) {

  auto v1 = Vector3();
  auto halfSize = v1.copy( size ).multiplyScalar( 0.5 );

  min.copy( center ).sub( halfSize );
  max.copy( center ).add( halfSize );

  return *this;

}

Box3& Box3::copy( const Box3& b ) {

  min.copy(b.min);
  max.copy(b.max);

  return *this;

}

Box3& Box3::makeEmpty() {

  min.x = min.y = min.z = Math::INF();
  max.x = max.y = max.z = -Math::INF();

  return *this;

}

bool Box3::empty() const {

  return ( max.x < min.x ) || ( max.y < min.y ) || ( max.z < min.z );

}

Vector3 Box3::center() const {

  return Vector3().addVectors( min, max ).multiplyScalar( 0.5 );

}

Vector3& Box3::center( Vector3& target ) {

  return target.addVectors( min, max ).multiplyScalar( 0.5 );

}

Vector3 Box3::size() const {

  return Vector3().subVectors( max, min );

}

Vector3& Box3::size( Vector3& target ) {

  return target.subVectors( min, max );

}

Box3& Box3::expandByPoint( const Vector3& point ) {

  min.min( point );
  max.max( point );

  return *this;

}

Box3& Box3::expandByVector( const Vector3& vector ) {

  min.sub( vector );
  max.add( vector );

  return *this;

}

Box3& Box3::expandByScalar( float scalar ) {

  min.addScalar( -scalar );
  max.addScalar( scalar );

  return *this;

}

bool Box3::containsPoint ( const Vector3& point ) const {

  if ( point.x < min.x || point.x > max.x ||
       point.y < min.y || point.y > max.y ||
       point.z < min.z || point.z > max.z ) {

    return false;

  }

  return true;

}

bool Box3::containsBox( const Box3& box ) const {

  if ( ( min.x <= box.min.x ) && ( box.max.x <= max.x ) &&
       ( min.y <= box.min.y ) && ( box.max.y <= max.y ) &&
       ( min.z <= box.min.z ) && ( box.max.z <= max.z ) ) {

    return true;

  }

  return false;

}

optional<Vector3> Box3::getParameter( const Vector3& point ) const {

    auto divX = max.x - min.x;
    auto divY = max.y - min.y;
    auto divZ = max.z - min.z;

    if(divX == 0.f || divY == 0.f || divZ == 0.f) {
        return nullptr;
    }

    return Vector3().set(
                      ( point.x - min.x ) / divX,
                      ( point.y - min.y ) / divY,
                      ( point.z - min.z ) / divZ
                      );

}

optional<Vector3> Box3::getParameter( const Vector3& point, Vector3& target ) const {

    auto divX = max.x - min.x;
    auto divY = max.y - min.y;
    auto divZ = max.z - min.z;

    if(divX == 0.f || divY == 0.f || divZ == 0.f) {
        return nullptr;
    }

    return target.set(
                         ( point.x - min.x ) / divX,
                         ( point.y - min.y ) / divY,
                         ( point.z - min.z ) / divZ
                         );

  };


bool Box3::isIntersectionBox( const Box3& box ) const {

  // using 6 splitting planes to rule out intersections.

  if ( box.max.x < min.x || box.min.x > max.x ||
       box.max.y < min.y || box.min.y > max.y ||
       box.max.z < min.z || box.min.z > max.z ) {

    return false;

  }

  return true;

}

Vector3 Box3::clampPoint( const Vector3& point ) const {

  return Vector3().copy( point ).clamp( min, max );

}

Vector3& Box3::clampPoint( const Vector3& point, Vector3& target ) {

  return target.copy( point ).clamp( min, max );

}

float Box3::distanceToPoint( const Vector3& point ) const {

  auto v1 = point;
  auto clampedPoint = v1.copy( point ).clamp( min, max );

  return clampedPoint.sub( point ).length();

}

Sphere Box3::getBoundingSphere() const {

  auto s = Sphere();

  s.center = center();
  s.radius = size().length() * 0.5f;

  return s;

}

Sphere& Box3::getBoundingSphere( Sphere& target) {

  target.center = center();
  target.radius = size().length() * 0.5f;

  return target;

}

Box3& Box3::intersect( const Box3& box ) {

  min.max( box.min );
  max.min( box.max );

  return *this;

}

Box3& Box3::unionBox( const Box3& box ) {

  min.min( box.min );
  max.max( box.max );

  return *this;

}

Box3& Box3::applyMatrix4( const Matrix4& matrix) {

  std::vector<Vector3> points;
  points.resize(8);

  // NOTE: I am using a binary pattern to specify all 2^3 combinations below
  points[0].set( min.x, min.y, min.z ).applyMatrix4( matrix ); // 000
  points[1].set( min.x, min.y, max.z ).applyMatrix4( matrix ); // 001
  points[2].set( min.x, max.y, min.z ).applyMatrix4( matrix ); // 010
  points[3].set( min.x, max.y, max.z ).applyMatrix4( matrix ); // 011
  points[4].set( max.x, min.y, min.z ).applyMatrix4( matrix ); // 100
  points[5].set( max.x, min.y, max.z ).applyMatrix4( matrix ); // 101
  points[6].set( max.x, max.y, min.z ).applyMatrix4( matrix ); // 110
  points[7].set( max.x, max.y, max.z ).applyMatrix4( matrix );  // 111

  makeEmpty();

  setFromPoints( points );

  return *this;

}

Box3& Box3::translate( const Vector3& offset ) {

  min.add( offset );
  max.add( offset );

  return *this;

}

bool Box3::equals( const Box3& box ) const {

  return box.min.equals( min ) && box.max.equals( max );

}

Box3 Box3::clone() const {

  return Box3(*this);

}

} // namespace three
