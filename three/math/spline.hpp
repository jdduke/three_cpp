#ifndef THREE_SPLINE_HPP
#define THREE_SPLINE_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

#include <vector>
#include <tuple>

namespace three {

class Spline {
  
public:

  Spline( std::vector<Vector3> points )
    : points( std::move( points ) ) {}

  Spline& initFromArray( const float* a, size_t n );

  Vector3 getPoint( float k ) const;

  const std::vector<Vector3>& getControlPointsArray() const;

  std::tuple<std::vector<float>, float> getLength( size_t nSubDivisions = 0 );

  void reparametrizeByArcLength( float samplingCoef );

  static float interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 );

  std::vector<Vector3> points;

};

} // namespace three

#endif // THREE_SPLINE_HPP