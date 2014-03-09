#ifndef THREE_SPHERE_GEOMETRY_HPP
#define THREE_SPHERE_GEOMETRY_HPP

#include <three/core/geometry.hpp>
#include <three/core/face3.hpp>
#include <three/core/face4.hpp>
#include <three/math/math.hpp>

namespace three {

class SphereGeometry : public Geometry {
public:

  typedef std::shared_ptr<SphereGeometry> Ptr;

  static Ptr create( float radius = 50,
                     float segmentsWidth = 8,
                     float segmentsHeight = 6,
                     float phiStart = 0,
                     float phiLength = Math::PI() * 2,
                     float thetaStart = 0,
                     float thetaLength = Math::PI() ) {

    auto geometry = make_shared<SphereGeometry>();
    geometry->initialize( radius,
                          segmentsWidth,
                          segmentsHeight,
                          phiStart,
                          phiLength,
                          thetaStart,
                          thetaLength );
    return geometry;
  }

protected:

  SphereGeometry() : Geometry() { }

  void initialize( float radius,
                   float segmentsWidth,
                   float segmentsHeight,
                   float phiStart,
                   float phiLength,
                   float thetaStart,
                   float thetaLength ) {

    const auto segmentsX = Math::max( 3, ( int )Math::floor( segmentsWidth ) );
    const auto segmentsY = Math::max( 2, ( int )Math::floor( segmentsHeight ) );

    std::vector<std::vector<int>> indices;
    std::vector<std::vector<UV>> uvs;

    for ( int y = 0; y <= segmentsY; y ++ ) {

      std::vector<int> indicesRow;
      std::vector<UV> uvsRow;

      for ( int x = 0; x <= segmentsX; x ++ ) {

        const auto u = ( float )x / segmentsX;
        const auto v = ( float )y / segmentsY;

        Vertex vertex;
        vertex.x = - radius * Math::cos( phiStart + u * phiLength ) * Math::sin( thetaStart + v * thetaLength );
        vertex.y = radius * Math::cos( thetaStart + v * thetaLength );
        vertex.z = radius * Math::sin( phiStart + u * phiLength ) * Math::sin( thetaStart + v * thetaLength );

        vertices.push_back( vertex );

        indicesRow.push_back( ( int )vertices.size() - 1 );
        uvsRow.push_back( UV( u, 1 - v ) );

      }

      indices.push_back( indicesRow );
      uvs.push_back( uvsRow );

    }

    for ( int y = 0; y < segmentsY; y ++ ) {

      for ( int x = 0; x < segmentsX; x ++ ) {

        const auto v1 = indices[ y ][ x + 1 ];
        const auto v2 = indices[ y ][ x ];
        const auto v3 = indices[ y + 1 ][ x ];
        const auto v4 = indices[ y + 1 ][ x + 1 ];

        const auto n1 = vertices[ v1 ].clone().normalize();
        const auto n2 = vertices[ v2 ].clone().normalize();
        const auto n3 = vertices[ v3 ].clone().normalize();
        const auto n4 = vertices[ v4 ].clone().normalize();

        const auto& uv1 = uvs[ y ][ x + 1 ];
        const auto& uv2 = uvs[ y ][ x ];
        const auto& uv3 = uvs[ y + 1 ][ x ];
        const auto& uv4 = uvs[ y + 1 ][ x + 1 ];

        if ( Math::abs( vertices[ v1 ].y ) == radius ) {

          faces.push_back( Face3( v1, v3, v4, n1, n3, n4 ) );
          faceVertexUvs[ 0 ].push_back( toArray( uv1, uv3, uv4 ) );

        } else if ( Math::abs( vertices[ v3 ].y ) ==  radius ) {

          faces.push_back( Face3( v1, v2, v3, n1, n2, n3 ) );
          faceVertexUvs[ 0 ].push_back( toArray( uv1, uv2, uv3 ) );

        } else {

          faces.push_back( Face4( v1, v2, v3, v4, n1, n2, n3, n4 ) );
          faceVertexUvs[ 0 ].push_back( toArray( uv1, uv2, uv3, uv4 ) );

        }

      }

    }

    computeCentroids();
    computeFaceNormals();

    boundingSphere.radius = radius;

  }

};

} // namespace three

#endif // THREE_SPHERE_GEOMETRY_HPP