#ifndef THREE_EXTRA_ICOSAHEDRON_GEOMETRY_CPP
#define THREE_EXTRA_ICOSAHEDRON_GEOMETRY_CPP

#include <three/core/geometry.h>

#include <three/extras/geometries/icosahedron_geometry.h>
#include <three/extras/geometries/polyhedron_geometry.h>

#include <three/utils/conversion.h>

namespace three {

IcosahedronGeometry::Ptr IcosahedronGeometry::create( float radius, int detail ) {
  return make_shared<IcosahedronGeometry>( radius, detail );
}

IcosahedronGeometry::IcosahedronGeometry( float radius, int detail )
  : PolyhedronGeometry( radius, detail ) {

  float t = ( 1.f + Math::sqrt( 5.f ) ) / 2.f;

  // wtb vs2012 initializer list initialization :-(
  std::vector<std::array<float, 3>> vertices;
  vertices.push_back( toArray<float>( -1, t, 0 ) );
  vertices.push_back( toArray<float>( 1, t, 0 ) );
  vertices.push_back( toArray<float>( -1, -t, 0 ) );
  vertices.push_back( toArray<float>( 1, -t, 0 ) );
  vertices.push_back( toArray<float>( 0, -1, t ) );
  vertices.push_back( toArray<float>( 0, 1, t ) );
  vertices.push_back( toArray<float>( 0, -1, -t ) );
  vertices.push_back( toArray<float>( 0, 1, -t ) );
  vertices.push_back( toArray<float>( t, 0, -1 ) );
  vertices.push_back( toArray<float>( t, 0, 1 ) );
  vertices.push_back( toArray<float>( -t, 0, -1 ) );
  vertices.push_back( toArray<float>( -t, 0, 1 ) );

  std::vector<std::array<float, 3>> faces;
  faces.push_back( toArray<float>( 0, 11, 5 ) );
  faces.push_back( toArray<float>( 0, 5, 1 ) );
  faces.push_back( toArray<float>( 0, 1, 7 ) );
  faces.push_back( toArray<float>( 0, 7, 10 ) );
  faces.push_back( toArray<float>( 0, 10, 11 ) );
  faces.push_back( toArray<float>( 1, 5, 9 ) );
  faces.push_back( toArray<float>( 5, 11, 4 ) );
  faces.push_back( toArray<float>( 11, 10, 2 ) );
  faces.push_back( toArray<float>( 10, 7, 6 ) );
  faces.push_back( toArray<float>( 7, 1, 8 ) );

  faces.push_back( toArray<float>( 3, 9, 4 ) );
  faces.push_back( toArray<float>( 3, 4, 2 ) );
  faces.push_back( toArray<float>( 3, 2, 6 ) );
  faces.push_back( toArray<float>( 3, 6, 8 ) );
  faces.push_back( toArray<float>( 3, 8, 9 ) );

  faces.push_back( toArray<float>( 4, 9, 5 ) );
  faces.push_back( toArray<float>( 2, 4, 11 ) );
  faces.push_back( toArray<float>( 6, 2, 10 ) );
  faces.push_back( toArray<float>( 8, 6, 7 ) );
  faces.push_back( toArray<float>( 9, 8, 1 ) );

  this->initialize( std::move( vertices ), std::move( faces ), radius, detail );

}


} // end namespace three

#endif // THREE_EXTRA_ICOSAHEDRON_GEOMETRY_CPP