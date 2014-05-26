#ifndef THREE_SPLINE_H
#define THREE_SPLINE_H

#include <three/common.h>

#include <three/math/vector3.h>

#include <vector>
#include <tuple>

namespace three {

class THREE_DECL Spline {

public:

  explicit Spline( std::vector<Vector3> points )
    : points( std::move( points ) ) {}

  std::vector<Vector3> points;

  Spline& initFromArray( const float* a, size_t n );

  Vector3 getPoint( float k ) const;

  std::tuple<std::vector<float>, float> getLength( size_t nSubDivisions = 0 );

  void reparametrizeByArcLength( float samplingCoef );

  inline const std::vector<Vector3>& getControlPointsArray() const {

    return points;

  }

};

} // namespace three

#endif // THREE_SPLINE_H