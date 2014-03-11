#ifndef THREE_TRIANGLE_IPP
#define THREE_TRIANGLE_IPP

#include <three/math/triangle.hpp>

namespace three {

  Vector3& TriangleHelper::normal( const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target ) {

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

  Vector3& TriangleHelper::barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target ) {

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

}

#endif // THREE_TRIANGLE_IPP