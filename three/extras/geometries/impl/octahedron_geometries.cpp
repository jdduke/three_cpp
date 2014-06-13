#ifndef THREE_EXTRA_OCTAHEDRON_GEOMETRY_CPP
#define THREE_EXTRA_OCTAHEDRON_GEOMETRY_CPP

#include <three/core/geometry.h>

#include <three/extras/geometries/octahedron_geometry.h>
#include <three/extras/geometries/polyhedron_geometry.h>

#include <three/utils/conversion.h>

namespace three {

OctahedronGeometry::Ptr OctahedronGeometry::create( float radius, int detail ) {
  return make_shared<OctahedronGeometry>( radius, detail );
}

OctahedronGeometry::OctahedronGeometry( float radius, int detail )
  : PolyhedronGeometry( radius, detail ) {

  // wtb vs2012 initializer list initialization :-(
  std::vector<std::array<float, 3>> vertices;
  vertices.push_back( toArray<float>(  1, 0, 0 ) );
  vertices.push_back( toArray<float>(  -1, 0, 0 ) );
  vertices.push_back( toArray<float>(  0, 1, 0 ) );
  vertices.push_back( toArray<float>(  0, -1, 0 ) );
  vertices.push_back( toArray<float>(  0, 0, 1 ) );
  vertices.push_back( toArray<float>(  0, 0, -1 ) );

  std::vector<std::array<float, 3>> faces;
  faces.push_back( toArray<float>(  0, 2, 4 ) );
  faces.push_back( toArray<float>(  0, 4, 3 ) );
  faces.push_back( toArray<float>(  0, 3, 5 ) );
  faces.push_back( toArray<float>(  0, 5, 2 ) );
  faces.push_back( toArray<float>(  1, 2, 5 ) );
  faces.push_back( toArray<float>(  1, 5, 3 ) );
  faces.push_back( toArray<float>(  1, 3, 4 ) );
  faces.push_back( toArray<float>(  1, 4, 2 ) );

  this->initialize( std::move( vertices ), std::move( faces ), radius, detail );

}

} // end namespace three

#endif // THREE_EXTRA_OCTAHEDRON_GEOMETRY_CPP