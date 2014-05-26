#include <three/math/triangle.h>

#include <three/math/math.h>

namespace three {

namespace {

class TriangleHelper {

public:

  static Vector3 normal( const Vector3& a, const Vector3& b, const Vector3& c) {

    auto target = Vector3();

    TriangleHelper::normal( a, b, c, target );

    return target;

  }

  static Vector3 barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c) {

    auto target = Vector3();

    TriangleHelper::barycoordFromPoint( point, a, b, c, target );

    return target;

  }

  static bool containsPoint ( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c ) {

    auto v1 = Vector3();
    auto result = TriangleHelper::barycoordFromPoint( point, a, b, c, v1 );

    return ( result.x >= 0 ) && ( result.y >= 0 ) && ( ( result.x + result.y ) <= 1 );

  }

  static Vector3& normal( const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target ) {
    auto v0 = Vector3();

    target.subVectors( c, b );
    v0.subVectors( a, b );
    target.cross( v0 );

    float resultLengthSq = target.lengthSq();
    if( resultLengthSq > 0 ) {
      return target.multiplyScalar( 1 / Math::sqrt( resultLengthSq ) );
    }

    return target.set( 0, 0, 0 );
  }

  static Vector3& barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target ) {
    auto v0 = Vector3();
    auto v1 = Vector3();
    auto v2 = Vector3();


    v0.subVectors( c, a );
    v1.subVectors( b, a );
    v2.subVectors( point, a );

    auto dot00 = v0.dot( v0 );
    auto dot01 = v0.dot( v1 );
    auto dot02 = v0.dot( v2 );
    auto dot11 = v1.dot( v1 );
    auto dot12 = v1.dot( v2 );

    auto denom = ( dot00 * dot11 - dot01 * dot01 );

    // colinear or singular triangle
    if( denom == 0 ) {
      // arbitrary location outside of triangle?
      // not sure if this is the best idea, maybe should be returning undefined
      return target.set( -2, -1, -1 );
    }

    auto invDenom = 1.f / denom;
    auto u = ( dot11 * dot02 - dot01 * dot12 ) * invDenom;
    auto v = ( dot00 * dot12 - dot01 * dot02 ) * invDenom;

    // barycoordinates must always sum to 1
    return target.set( 1 - u - v, v, u );
  }

};

} // namespace

Triangle& Triangle::set( const Vector3& aIn, const Vector3& bIn, const Vector3& cIn ) {

  a.copy( aIn );
  b.copy( bIn );
  c.copy( cIn );

  return *this;

}

Triangle& Triangle::setFromPointsAndIndices( const Vector3 points[], size_t i0, size_t i1, size_t i2 ) {

  a.copy( points[i0] );
  b.copy( points[i1] );
  c.copy( points[i2] );

  return *this;

}

Triangle& Triangle::copy( const Triangle& triangle ) {

  a.copy( triangle.a );
  b.copy( triangle.b );
  c.copy( triangle.c );

  return *this;

}

float Triangle::area() const {

  auto v0 = Vector3();
  auto v1 = Vector3();

  v0.subVectors( c, b );
  v1.subVectors( a, b );

  return v0.cross( v1 ).length() * 0.5f;
}

Vector3 Triangle::midpoint() const {

  auto target = Vector3();

  midpoint( target );

  return target;

}

Vector3& Triangle::midpoint( Vector3& target ) const {

  return target.addVectors( a, b ).add( c ).multiplyScalar( 1.f / 3.f );

}

Vector3 Triangle::normal() const {

  return TriangleHelper::normal( a, b, c );

}

Vector3& Triangle::normal( Vector3& target ) const {

  return TriangleHelper::normal( a, b, c, target );

}

Plane Triangle::plane() const {

  auto target = Plane();

  plane( target );

  return target;

}

Plane& Triangle::plane( Plane& target ) const {

  return target.setFromCoplanarPoints( a, b, c );

}

Vector3 Triangle::barycoordFromPoint( const Vector3& point ) const {

  return TriangleHelper::barycoordFromPoint( point, a, b, c );

}

Vector3& Triangle::barycoordFromPoint( const Vector3& point, Vector3& target ) const {

  return TriangleHelper::barycoordFromPoint( point, a, b, c, target );

}

bool Triangle::containsPoint( const Vector3& point ) const {

  return TriangleHelper::containsPoint( point, a, b, c );

}

bool Triangle::equals( const Triangle& triangle ) const {

  return triangle.a.equals( a ) && triangle.b.equals( b ) && triangle.c.equals( c );

}

Triangle Triangle::clone() const {

  return *this;

}

} // namespace three
