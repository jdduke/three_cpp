#include <three/extras/geometries/torus_geometry.h>

#include <three/core/face3.h>

#include <three/utils/conversion.h>

namespace three {

TorusGeometry::Ptr TorusGeometry::create( float radius,
                                          float tube,
                                          int segmentsR,
                                          int segmentsT,
                                          float arc) {

  auto geometry = make_shared<TorusGeometry>();
  geometry->initialize( radius,
                        tube,
                        segmentsR,
                        segmentsT,
                        arc );
  return geometry;
}

TorusGeometry::TorusGeometry() { }

void TorusGeometry::initialize( float radius,
                                float tube,
                                int segmentsR,
                                int segmentsT,
                                float arc ) {

  Vector3 center;

  std::vector<Vector2> tmpUvs;
  std::vector<Vector3> tmpNormals;

  for ( int j = 0; j <= segmentsR; j++ ) {

    for ( int i = 0; i <= segmentsT; i++ ) {

      const auto u = (float)i / segmentsT * arc;
      const auto v = (float)j / segmentsR * Math::PI() * 2;

      center.x = radius * Math::cos( u );
      center.y = radius * Math::sin( u );

      Vector3 vertex;
      vertex.x = ( radius + tube * Math::cos( v ) ) * Math::cos( u );
      vertex.y = ( radius + tube * Math::cos( v ) ) * Math::sin( u );
      vertex.z = tube * Math::sin( v );

      vertices.push_back( vertex );

      tmpUvs.push_back( Vector2( (float)i / segmentsT, (float)j / segmentsR ) );
      tmpNormals.push_back( vertex.clone().sub( center ).normalize() );

    }
  }


  for ( int j = 1; j <= segmentsR; j ++ ) {

    for ( int i = 1; i <= segmentsT; i ++ ) {

      const auto a = ( segmentsT + 1 ) * j + i - 1;
      const auto b = ( segmentsT + 1 ) * ( j - 1 ) + i - 1;
      const auto c = ( segmentsT + 1 ) * ( j - 1 ) + i;
      const auto d = ( segmentsT + 1 ) * j + i;

      Face face( a, b, d, tmpNormals[ a ], tmpNormals[ b ], tmpNormals[ d ] );
      faces.push_back( face );
      faceVertexUvs[ 0 ].push_back( toArray( tmpUvs[ a ], tmpUvs[ b ], tmpUvs[ d ] ) );

      face = Face( b, c, d, tmpNormals[ b ], tmpNormals[ c ], tmpNormals[ d ] );
      faces.push_back( face );
      faceVertexUvs[ 0 ].push_back( toArray( tmpUvs[ b ], tmpUvs[ c ], tmpUvs[ d ] ) );

    }

  }

  computeCentroids();
  computeFaceNormals();

}

} // namespace three
