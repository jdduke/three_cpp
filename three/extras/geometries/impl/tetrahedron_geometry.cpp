#ifndef THREE_EXTRA_TETRAHEDRON_GEOMETRY_CPP
#define THREE_EXTRA_TETRAHEDRON_GEOMETRY_CPP

#include <three/core/geometry.h>

#include <three/extras/geometries/tetrahedron_geometry.h>
#include <three/extras/geometries/polyhedron_geometry.h>

#include <three/utils/conversion.h>

namespace three {

TetrahedronGeometry::Ptr TetrahedronGeometry::create( float radius, int detail ) {
  return make_shared<TetrahedronGeometry>( radius, detail );
}

TetrahedronGeometry::TetrahedronGeometry( float radius, int detail )
  : PolyhedronGeometry( radius, detail ) {

  // wtb vs2012 initializer list initialization :-(
  std::vector<std::array<float, 3>> vertices;
  vertices.push_back( toArray<float>( 1,  1,  1 ) );
  vertices.push_back( toArray<float>( -1, -1, 1 ) );
  vertices.push_back( toArray<float>( -1, 1, -1 ) );
  vertices.push_back( toArray<float>( 1, -1, -1 ) );

  std::vector<std::array<float, 3>> faces;
  faces.push_back( toArray<float>( 2, 1, 0 ) );
  faces.push_back( toArray<float>( 0, 3, 2 ) );
  faces.push_back( toArray<float>( 1, 3, 0 ) );
  faces.push_back( toArray<float>( 2, 3, 1 ) );

  this->initialize( std::move( vertices ), std::move( faces ), radius, detail );

}

} // end namespace three

#endif // THREE_EXTRA_TETRAHEDRON_GEOMETRY_CPP