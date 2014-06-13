#ifndef THREE_EXTRA_POLYHEDRON_GEOMETRY_CPP
#define THREE_EXTRA_POLYHEDRON_GEOMETRY_CPP

#include <three/extras/geometries/polyhedron_geometry.h>

namespace three {

struct PolyhedronGeometry::IndexedVertex {
  size_t index;
  Vector3 vector;
  Vector2 uv;
};

PolyhedronGeometry::Ptr PolyhedronGeometry::create( std::vector<std::array<float,3>> vertices,
                   std::vector<std::array<float,3>> faces,
                   float radius,
                   int detail ) {

  auto polyhedronGeometry = make_shared<PolyhedronGeometry>();

  polyhedronGeometry->initialize( vertices, faces, radius, detail );

  return polyhedronGeometry;
}

PolyhedronGeometry::PolyhedronGeometry() {}

void PolyhedronGeometry::initialize( std::vector<std::array<float,3>> verticesIn,
                   std::vector<std::array<float,3>> facesIn,
                   float radius,
                   int detail ) {


  for ( size_t i = 0; i < verticesIn.size(); i ++ ) {
    auto v = Vector3( verticesIn[ i ][ 0 ], verticesIn[ i ][ 1 ], verticesIn[ i ][ 2 ] );
    prepare( v );
  }

  std::vector<Face3> f;

  for ( size_t i = 0; i < facesIn.size(); i ++ ) {

    auto v1 = indexedVertices[ facesIn[ i ][ 0 ] ];
    auto v2 = indexedVertices[ facesIn[ i ][ 1 ] ];
    auto v3 = indexedVertices[ facesIn[ i ][ 2 ] ];

    f[ i ] = Face3( v1.index, v2.index, v3.index, v1.vector.clone(), v2.vector.clone(), v3.vector.clone() );

  }

  for ( size_t i = 0; i < f.size(); i ++ ) {

    subdivide( f[ i ], detail );

  }


  // Handle case when face straddles the seam

  for ( size_t i = 0; i < this->faceVertexUvs[ 0 ].size(); i ++ ) {

    auto& uvs = this->faceVertexUvs[ 0 ][ i ];

    auto x0 = uvs[ 0 ].x;
    auto x1 = uvs[ 1 ].x;
    auto x2 = uvs[ 2 ].x;

    auto max = Math::max( x0, Math::max( x1, x2 ) );
    auto min = Math::min( x0, Math::min( x1, x2 ) );

    if ( max > 0.9 && min < 0.1 ) { // 0.9 is somewhat arbitrary

      if ( x0 < 0.2 ) uvs[ 0 ].x += 1;
      if ( x1 < 0.2 ) uvs[ 1 ].x += 1;
      if ( x2 < 0.2 ) uvs[ 2 ].x += 1;

    }

  }


  // Apply radius

  for ( size_t i = 0; i < this->vertices.size(); i ++ ) {

    this->vertices[ i ].multiplyScalar( radius );

  }


  // Merge vertices

  this->mergeVertices();

  this->computeCentroids();

  this->computeFaceNormals();

  this->boundingSphere = Sphere( Vector3(), radius );

  indexedVertices.clear();
  indexedVertices.shrink_to_fit();

}

// Project vector onto sphere's surface
PolyhedronGeometry::IndexedVertex PolyhedronGeometry::prepare( Vector3& vector ) {

  IndexedVertex vertex;

  vertex.vector = vector.normalize().clone();
  vertex.index = this->vertices.size();

  this->vertices.push_back( vertex.vector );

  // Texture coords are equivalent to map coords, calculate angle and convert to fraction of a circle.

  auto u = azimuth( vector ) / 2.f / Math::PI() + 0.5f;
  auto v = inclination( vector ) / Math::PI() + 0.5f;
  vertex.uv = Vector2( u, 1 - v );

  indexedVertices.push_back( vertex );

  return vertex;

}

// Approximate a curved face with recursively sub-divided triangles.
void PolyhedronGeometry::make( const IndexedVertex& v1, const IndexedVertex& v2, const IndexedVertex& v3 ) {

  auto face = Face3( v1.index, v2.index, v3.index, v1.vector.clone(), v2.vector.clone(), v3.vector.clone() );
  face.centroid.add( v1.vector ).add( v2.vector ).add( v3.vector ).divideScalar( 3 );
  this->faces.push_back( face );

  auto azi = azimuth( face.centroid );

  this->faceVertexUvs[ 0 ].push_back( {
    correctUV( v1.uv, v1.vector, azi ),
    correctUV( v2.uv, v2.vector, azi ),
    correctUV( v3.uv, v3.vector, azi )
  } );

}

// Analytically subdivide a face to the required detail level.
void PolyhedronGeometry::subdivide( const Face3& face, int detail ) {

  auto cols = Math::pow( 2, (int)detail );
  //auto cells = Math::pow( 4, (int)detail );
  auto a = prepare( this->vertices[ face.a ] );
  auto b = prepare( this->vertices[ face.b ] );
  auto c = prepare( this->vertices[ face.c ] );

  std::vector<std::vector<IndexedVertex>> v;

  // Construct all of the vertices for this subdivision.

  for ( int i = 0 ; i <= cols; i ++ ) {

    auto aj = prepare( a.vector.clone().lerp( c.vector, (float)i / cols ) );
    auto bj = prepare( b.vector.clone().lerp( c.vector, (float)i / cols ) );
    auto rows = cols - i;

    for ( int j = 0; j <= rows; j ++) {

      if ( j == 0 && i == cols ) {

        v[ i ][ j ] = aj;

      } else {

        v[ i ][ j ] = prepare( aj.vector.clone().lerp( bj.vector, (float)j / rows ) );

      }

    }

  }

  // Construct all of the faces.

  for ( int i = 0; i < cols ; i ++ ) {

    for ( int j = 0; j < 2 * (cols - i) - 1; j ++ ) {

      int k = Math::floor( j / 2.f );

      if ( j % 2 == 0 ) {

        make(
          v[ i ][ k + 1],
          v[ i + 1 ][ k ],
          v[ i ][ k ]
        );

      } else {

        make(
          v[ i ][ k + 1 ],
          v[ i + 1][ k + 1],
          v[ i + 1 ][ k ]
        );

      }

    }

  }

}

// Angle around the Y axis, counter-clockwise when looking from above.
float PolyhedronGeometry::azimuth( const Vector3& vector ) const {

  return Math::atan2( vector.z, -vector.x );

}

// Angle above the XZ plane.
float PolyhedronGeometry::inclination( const Vector3& vector ) const {

  return Math::atan2( -vector.y, Math::sqrt( ( vector.x * vector.x ) + ( vector.z * vector.z ) ) );

}

// Texture fixing helper. Spheres have some odd behaviours.
Vector2 PolyhedronGeometry::correctUV( Vector2 uv, const Vector3& vector, float azimuth ) const {

  if ( ( azimuth < 0 ) && ( uv.x == 1 ) ) uv = Vector2( uv.x - 1, uv.y );
  if ( ( vector.x == 0 ) && ( vector.z == 0 ) ) uv = Vector2( azimuth / 2.f / Math::PI() + 0.5f, uv.y );

  return uv;

}

} // end namespace three

#endif // THREE_EXTRA_POLYHEDRON_GEOMETRY_CPP