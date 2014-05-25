#include <three/extras/geometries/box_geometry.h>

#include <three/core/face3.h>
#include <three/utils/conversion.h>

namespace three {

BoxGeometry::Ptr BoxGeometry::create( float width,
                                      float height,
                                      float depth,
                                      int segmentsWidth,
                                      int segmentsHeight,
                                      int segmentsDepth ) {

  auto geometry = make_shared<BoxGeometry>();
  geometry->initialize( width,
                       height,
                       depth,
                       segmentsWidth,
                       segmentsHeight,
                       segmentsDepth );

  return geometry;
}

BoxGeometry::BoxGeometry() : Geometry() { }

void BoxGeometry::initialize( float width,
                              float height,
                              float depth,
                              int segmentsWidth,
                              int segmentsHeight,
                              int segmentsDepth ) {

  auto width_half  = width  / 2;
  auto height_half = height / 2;
  auto depth_half  = depth  / 2;

  buildPlane( Z, Y, - 1, - 1, depth, height,  width_half,  0, segmentsWidth, segmentsHeight, segmentsDepth ); // px
  buildPlane( Z, Y,   1, - 1, depth, height, -width_half,  1, segmentsWidth, segmentsHeight, segmentsDepth ); // nx
  buildPlane( X, Z,   1,   1, width, depth,   height_half, 2, segmentsWidth, segmentsHeight, segmentsDepth ); // py
  buildPlane( X, Z,   1, - 1, width, depth,  -height_half, 3, segmentsWidth, segmentsHeight, segmentsDepth ); // ny
  buildPlane( X, Y,   1, - 1, width, height,  depth_half,  4, segmentsWidth, segmentsHeight, segmentsDepth ); // pz
  buildPlane( X, Y, - 1, - 1, width, height, -depth_half,  5, segmentsWidth, segmentsHeight, segmentsDepth ); // nz

  computeCentroids();
  mergeVertices();
}

void BoxGeometry::buildPlane( Side u, Side v,
                              float udir, float vdir,
                              float width, float height, float depth,
                              int materialIndex,
                              int segmentsWidth, int segmentsHeight, int segmentsDepth ) {

  int gridX = segmentsWidth;
  int gridY = segmentsHeight;

  const float width_half = width / 2;
  const float height_half = height / 2;

  int offset = (int)vertices.size();

  Side w = X;

  if ( ( u == X && v == Y ) || ( u == Y && v == X ) ) {
    w = Z;
  } else if ( ( u == X && v == Z ) || ( u == Z && v == X ) ) {
    w = Y;
    gridY = segmentsDepth;
  } else if ( ( u == Z && v == Y ) || ( u == Y && v == Z ) ) {
    w = X;
    gridX = segmentsDepth;
  }

  const auto gridX1 = gridX + 1;
  const auto gridY1 = gridY + 1;
  const auto segment_width = width / gridX;
  const auto segment_height = height / gridY;

  Vector3 normal;
  normal[ w ] = depth > 0 ? 1.f : -1.f;

  for ( int iy = 0; iy < gridY1; iy ++ ) {
    for ( int ix = 0; ix < gridX1; ix ++ ) {

      Vertex vertex;
      vertex[ u ] = ( ix * segment_width - width_half ) * udir;
      vertex[ v ] = ( iy * segment_height - height_half ) * vdir;
      vertex[ w ] = depth;
      vertices.push_back( vertex );

    }
  }

  for ( int iy = 0; iy < gridY; iy++ ) {
    for ( int ix = 0; ix < gridX; ix++ ) {

      const auto a = ix + gridX1 * iy;
      const auto b = ix + gridX1 * ( iy + 1 );
      const auto c = ( ix + 1 ) + gridX1 * ( iy + 1 );
      const auto d = ( ix + 1 ) + gridX1 * iy;

      const auto uva = Vector2( (float)ix / gridX, 1.f - iy / gridY );
      const auto uvb = Vector2( (float)ix / gridX, 1.f - ( iy + 1 ) / gridY );
      const auto uvc = Vector2( ( (float)ix + 1 ) / gridX, 1.f - ( iy + 1 ) / gridY );
      const auto uvd = Vector2( ( (float)ix + 1 ) / gridX, 1.f - iy / gridY );

      Face3 face( a + offset, b + offset, d + offset );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );
      face.materialIndex = materialIndex;
      faces.push_back( face );

      faceVertexUvs[ 0 ].push_back( toArray( uva, uvb, uvd ) );

      face = Face3( b + offset, c + offset, d + offset );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );
      face.materialIndex = materialIndex;
      faces.push_back( face );

      faceVertexUvs[ 0 ].push_back( toArray( uvb, uvc, uvd ) );

    }

  }

}

} // namespace three
