#include <three/extras/geometries/plane_geometry.h>

#include <three/core/face3.h>
#include <three/utils/conversion.h>

namespace three {

PlaneGeometry::Ptr PlaneGeometry::create( float width,
                                          float height,
                                          int segmentsWidth,
                                          int segmentsHeight ) {

  auto geometry = make_shared<PlaneGeometry>();
  geometry->initialize( width,
                        height,
                        segmentsWidth,
                        segmentsHeight );

  return geometry;
}

PlaneGeometry::PlaneGeometry() { }

void PlaneGeometry::initialize( float width,
                                float height,
                                int segmentsWidth,
                                int segmentsHeight ) {

  const auto width_half = width / 2,
             height_half = height / 2;
  const auto gridX = segmentsWidth,
             gridZ = segmentsHeight;
  const auto gridX1 = gridX + 1,
             gridZ1 = gridZ + 1;
  const auto segment_width = width / gridX,
             segment_height = height / gridZ;

  Vector3 normal( 0, 0, 1 );

  for ( int iz = 0; iz < gridZ1; iz ++ ) {

    for ( int ix = 0; ix < gridX1; ix ++ ) {

      const auto x = (float)ix * segment_width - width_half;
      const auto y = (float)iz * segment_height - height_half;

      vertices.push_back( Vector3( x, -y, 0 ) );

    }

  }

  for ( int iz = 0; iz < gridZ; iz ++ ) {

    for ( int ix = 0; ix < gridX; ix ++ ) {

      const auto a = ix + gridX1 * iz;
      const auto b = ix + gridX1 * ( iz + 1 );
      const auto c = ( ix + 1 ) + gridX1 * ( iz + 1 );
      const auto d = ( ix + 1 ) + gridX1 * iz;

      const Vector2 uva( (float)ix / gridX, 1.f - iz / gridZ );
      const Vector2 uvb( (float)ix / gridX, 1.f - ( iz + 1 ) / gridZ );
      const Vector2 uvc( ( (float)ix + 1 ) / gridX, 1.f - ( iz + 1 ) / gridZ );
      const Vector2 uvd( ( (float)ix + 1 ) / gridX, 1.f - iz / gridZ );

      Face3 face( a, b, d );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );

      faces.push_back( face );
      faceVertexUvs[ 0 ].push_back( toArray( uva, uvb, uvd ) );

      face = Face3( b, c, d );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );

      faces.push_back( face );
      faceVertexUvs[ 0 ].push_back( toArray( uvb, uvc, uvd ) );

    }

  }

}

} // namespace three
