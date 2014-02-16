#ifndef THREE_PLANE_GEOMETRY_HPP
#define THREE_PLANE_GEOMETRY_HPP

#include <three/core/geometry.hpp>
#include <three/math/face4.hpp>

namespace three {

class PlaneGeometry : public Geometry {
public:

   typedef std::shared_ptr<PlaneGeometry> Ptr;

  static Ptr create( float width,
                     float height,
                     int segmentsWidth = 1,
                     int segmentsHeight = 1 ) {

    auto geometry = make_shared<PlaneGeometry>();
    geometry->initialize( width,
                          height,
                          segmentsWidth,
                          segmentsHeight );

    return geometry;
  }

protected:

  PlaneGeometry() : Geometry() { }

  void initialize( float width,
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

        Face4 face( a, b, c, d );
        face.normal.copy( normal );
        face.vertexNormals.fill( normal );

        faces.push_back( face );

        std::array<UV, 4> uvs = {
         UV( (float)ix / gridX, 1.f - iz / gridZ ),
         UV( (float)ix / gridX, 1.f - ( iz + 1 ) / gridZ ),
         UV( ( (float)ix + 1 ) / gridX, 1.f - ( iz + 1 ) / gridZ ),
         UV( ( (float)ix + 1 ) / gridX, 1.f - iz / gridZ )
        };
        faceVertexUvs[ 0 ].push_back( std::move( uvs ) );

      }

    }

    computeCentroids();

  }

};

} // namespace three

#endif // THREE_PLANE_GEOMETRY_HPP