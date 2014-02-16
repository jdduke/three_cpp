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
    : points( std::move( points ) ) { }

  //////////////////////////////////////////////////////////////////////////

  std::vector<Vector3> points;

  //////////////////////////////////////////////////////////////////////////

  Spline& initFromArray( const float* a, size_t n ) {

    points.resize( n );

    for ( size_t i = 0; i < n; i += 3 ) {
      points [ i ].set( a[ i ], a[ i + 1 ], a[ i + 2 ] );
    }

    return *this;

  }

  Vector3 getPoint( float k ) const {

    float point = k * ( ( int )points.size() - 1 );
    int intPoint = ( int )std::floor( point );
    float weight = point - intPoint;
    int c[4];

    c[ 0 ] = intPoint == 0 ? intPoint : intPoint - 1;
    c[ 1 ] = intPoint;
    c[ 2 ] = intPoint  > ( int )points.size() - 2 ? ( int )points.size() - 1 : intPoint + 1;
    c[ 3 ] = intPoint  > ( int )points.size() - 3 ? ( int )points.size() - 1 : intPoint + 2;

    const auto& pa = points[ c[ 0 ] ];
    const auto& pb = points[ c[ 1 ] ];
    const auto& pc = points[ c[ 2 ] ];
    const auto& pd = points[ c[ 3 ] ];

    auto w2 = weight * weight;
    auto w3 = weight * w2;

    Vector3 v3;

    v3.x = interpolate( pa.x, pb.x, pc.x, pd.x, weight, w2, w3 );
    v3.y = interpolate( pa.y, pb.y, pc.y, pd.y, weight, w2, w3 );
    v3.z = interpolate( pa.z, pb.z, pc.z, pd.z, weight, w2, w3 );

    return v3;

  }

  const std::vector<Vector3>& getControlPointsArray() const {

    return points;

  }

  // approximate length by summing linear segments

  std::tuple<std::vector<float>, float> getLength( size_t nSubDivisions = 0 ) {

    // first point has 0 length

    std::vector<float> chunkLengths( 1, 0 );

    float totalLength = 0;

    if ( points.size() == 0 )
      return std::make_tuple( chunkLengths, totalLength );

    if ( nSubDivisions == 0 ) nSubDivisions = 100;

    auto nSamples = points.size() * nSubDivisions;

    Vector3 oldPosition( points[ 0 ] );
    int oldIntPoint = 0;

    for ( size_t i = 1; i < nSamples; i ++ ) {

      auto index = ( float )i / nSamples;

      auto position = getPoint( index );

      totalLength += position.distanceTo( oldPosition );

      oldPosition.copy( position );

      auto point = ( ( float )( points.size() - 1 ) ) * index;
      auto intPoint = ( int )Math::floor( point );

      if ( intPoint != oldIntPoint ) {

        chunkLengths.push_back( totalLength );
        oldIntPoint = intPoint;

      }

    }

    // last point ends with total length

    chunkLengths.push_back( totalLength );

    return std::make_tuple( chunkLengths, totalLength );

  }

  void reparametrizeByArcLength( float samplingCoef ) {

    if ( points.size() == 0 ) return;

    auto sl = getLength();
    const auto& chunks = std::get<0>( sl );
    const auto& total  = std::get<1>( sl );

    std::vector<Vector3> newpoints( 1, points[ 0 ] );

    const auto length = points.size();

    for ( size_t i = 1; i < length; ++i ) {

      auto realDistance = chunks[ i ] - chunks[ i - 1 ];

      auto sampling = Math::ceil( samplingCoef * realDistance / total );

      auto indexCurrent = ( float )( i - 1 ) / ( length - 1 );
      auto indexNext = ( float )i / ( length - 1 );

      for ( size_t j = 1; j < sampling - 1; j++ ) {

        auto index = indexCurrent + ( float )j * ( 1.f / sampling ) * ( indexNext - indexCurrent );

        newpoints.push_back( getPoint( index ) );

      }

      newpoints.push_back( points[ i ] );

    }

    points = std::move( newpoints );

  }


  // Catmull-Rom

  static float interpolate( float p0, float p1, float p2, float p3, float t, float t2, float t3 ) {

    auto v0 = ( p2 - p0 ) * 0.5f,
         v1 = ( p3 - p1 ) * 0.5f;

    return ( 2.f * ( p1 - p2 ) + v0 + v1 ) * t3 + ( - 3.f * ( p1 - p2 ) - 2.f * v0 - v1 ) * t2 + v0 * t + p1;

  }


};

} // namespace three

#endif // THREE_SPLINE_HPP