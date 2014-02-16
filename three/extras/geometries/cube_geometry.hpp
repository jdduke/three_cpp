#ifndef THREE_CUBE_GEOMETRY_HPP
#define THREE_CUBE_GEOMETRY_HPP

#include <three/core/geometry.hpp>
#include <three/math/face4.hpp>

namespace three {

class CubeGeometry : public Geometry {
public:

  struct Sides {
    Sides()
      : px( true ), nx( true ),
        py( true ), ny( true ),
        pz( true ), nz( true ) { }
    bool px, nx, py, ny, pz, nz;
  };

  typedef std::shared_ptr<CubeGeometry> Ptr;

  static Ptr create( float width,
                     float height,
                     float depth,
                     int segmentsWidth = 1,
                     int segmentsHeight = 1,
                     int segmentsDepth = 1,
                     std::vector<Material::Ptr> materials = std::vector<Material::Ptr>(),
                     Sides sides = Sides() ) {

    auto geometry = make_shared<CubeGeometry>();
    geometry->initialize( width,
                          height,
                          depth,
                          segmentsWidth,
                          segmentsHeight,
                          segmentsDepth,
                          materials,
                          sides );

    return geometry;
  }

protected:

  enum Side { X = 0, Y = 1, Z = 2 };

  CubeGeometry() : Geometry() { }

  void initialize( float width,
                   float height,
                   float depth,
                   int segmentsWidth,
                   int segmentsHeight,
                   int segmentsDepth,
                   std::vector<Material::Ptr> materials,
                   Sides sides ) {

    auto width_half  = width  / 2;
    auto height_half = height / 2;
    auto depth_half  = depth  / 2;

    int mpx = -1, mpy = -1, mpz = -1, mnx = -1, mny = -1, mnz = -1;

    if ( !materials.empty() ) {

      if ( materials.size() > 1 ) {
        this->materials = materials;
      } else {
        this->materials.clear();
        for ( int i = 0; i < 6; i ++ ) {
          this->materials.push_back( materials[0] );
        }
      }
      mpx = 0; mnx = 1; mpy = 2; mny = 3; mpz = 4; mnz = 5;

    } else {
      this->materials.clear();
    }

    this->sides = sides;

    if ( sides.px ) buildPlane( Z, Y, - 1, - 1, depth, height,  width_half,  mpx, segmentsWidth, segmentsHeight, segmentsDepth ); // px
    if ( sides.nx ) buildPlane( Z, Y,   1, - 1, depth, height, -width_half,  mnx, segmentsWidth, segmentsHeight, segmentsDepth ); // nx
    if ( sides.py ) buildPlane( X, Z,   1,   1, width, depth,   height_half, mpy, segmentsWidth, segmentsHeight, segmentsDepth ); // py
    if ( sides.ny ) buildPlane( X, Z,   1, - 1, width, depth,  -height_half, mny, segmentsWidth, segmentsHeight, segmentsDepth ); // ny
    if ( sides.pz ) buildPlane( X, Y,   1, - 1, width, height,  depth_half,  mpz, segmentsWidth, segmentsHeight, segmentsDepth ); // pz
    if ( sides.nz ) buildPlane( X, Y, - 1, - 1, width, height, -depth_half,  mnz, segmentsWidth, segmentsHeight, segmentsDepth ); // nz

    computeCentroids();
    mergeVertices();
  }

  void buildPlane( Side u, Side v,
                   float udir, float vdir,
                   float width, float height, float depth,
                   int material,
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

        Face4 face( a + offset, b + offset, c + offset, d + offset );
        face.normal.copy( normal );
        face.vertexNormals.fill( normal );
        face.materialIndex = material;
        faces.push_back( face );

        std::array<UV, 4> uvs = {
          UV( (float)ix / gridX, 1.f - iy / gridY ),
          UV( (float)ix / gridX, 1.f - ( iy + 1 ) / gridY ),
          UV( ( (float)ix + 1 ) / gridX, 1.f - ( iy + 1 ) / gridY ),
          UV( ( (float)ix + 1 ) / gridX, 1.f - iy / gridY )
        };
        faceVertexUvs[ 0 ].push_back( std::move( uvs ) );

      }

    }

  }

  Sides sides;

};

} // namespace three

#endif // THREE_CUBE_GEOMETRY_HPP