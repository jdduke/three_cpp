#ifndef THREE_SPLINE_H
#define THREE_SPLINE_H

#include <three/common.h>

#include <three/math/math.h>

#include <vector>
#include <tuple>

namespace three {

class Spline {
  
public:

  Spline( std::vector<Vector3> points )
    : points( std::move( points ) ) {}

  std::vector<Vector3> points;

  inline Spline& initFromArray( const float* a, size_t n ) {

    points.resize( n );

    for ( size_t i = 0; i < n; i += 3 ) {
      points [ i ].set( a[ i ], a[ i + 1 ], a[ i + 2 ] );
    }

    return *this;

  }

  Vector3 getPoint( float k ) const;

  inline const std::vector<Vector3>& getControlPointsArray() const {

    return points;

  }

  std::tuple<std::vector<float>, float> getLength( size_t nSubDivisions = 0 );

  void reparametrizeByArcLength( float samplingCoef );

  inline static float interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 ) {

    auto v0 = ( p2 - p0 ) * 0.5f,
         v1 = ( p3 - p1 ) * 0.5f;

    return ( 2.f * ( p1 - p2 ) + v0 + v1 ) * t3 + ( - 3.f * ( p1 - p2 ) - 2.f * v0 - v1 ) * t2 + v0 * t + p1;

  }

};

} // namespace three

#endif // THREE_SPLINE_H