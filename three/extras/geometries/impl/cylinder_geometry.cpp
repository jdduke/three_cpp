#ifndef THREE_EXTRA_CYLINDER_GEOMETRY_CPP
#define THREE_EXTRA_CYLINDER_GEOMETRY_CPP

#include <three/extras/geometries/cylinder_geometry.h>

#include <three/core/face3.h>

namespace three {

CylinderGeometry::Ptr CylinderGeometry::create( float radiusTop,
                              float radiusBottom,
                              float height,
                              size_t radialSegments,
                              size_t heightSegments,
                              bool openEnded ) {

  auto cylinderGeometry = make_shared<CylinderGeometry>();

  cylinderGeometry->initialize( radiusTop, radiusBottom, height, radialSegments, heightSegments, openEnded );

  return cylinderGeometry;
}

CylinderGeometry::CylinderGeometry() {}

void CylinderGeometry::initialize( float radiusTop,
                 float radiusBottom,
                 float height,
                 size_t radialSegments,
                 size_t heightSegments,
                 bool openEnded ) {

  auto heightHalf = height / 2.f;

  unsigned int x, y;

  std::vector<std::vector<int>> verticesRows;
  std::vector<std::vector<Vector2>> uvsRows;

  for ( y = 0; y <= heightSegments; y ++ ) {

    std::vector<int> verticesRow;
    std::vector<Vector2> uvsRow;

    auto v = (float) y / heightSegments;
    auto radius = v * ( radiusBottom - radiusTop ) + radiusTop;

      for ( unsigned int x = 0; x <= radialSegments; x ++ ) {

        float u = (float)x / (float)radialSegments;

        auto vertex = Vector3();
        vertex.x = radius * Math::sin( u * Math::PI() * 2 );
        vertex.y = - v * height + heightHalf;
        vertex.z = radius * Math::cos( u * Math::PI() * 2 );

        this->vertices.push_back( vertex );

        verticesRow.push_back( this->vertices.size() - 1 );
        uvsRow.push_back( Vector2( u, 1 - v ) );

      }

    verticesRows.push_back( verticesRow );
    uvsRows.push_back( uvsRow );

  }

  auto tanTheta = ( radiusBottom - radiusTop ) / height;
  Vector3 na, nb;

  for ( x = 0; x < radialSegments; x ++ ) {

    if ( radiusTop != 0 ) {

      na = this->vertices[ verticesRows[ 0 ][ x ] ].clone();
      nb = this->vertices[ verticesRows[ 0 ][ x + 1 ] ].clone();

    } else {

      na = this->vertices[ verticesRows[ 1 ][ x ] ].clone();
      nb = this->vertices[ verticesRows[ 1 ][ x + 1 ] ].clone();

    }

    na.setY( Math::sqrt( na.x * na.x + na.z * na.z ) * tanTheta ).normalize();
    nb.setY( Math::sqrt( nb.x * nb.x + nb.z * nb.z ) * tanTheta ).normalize();

    for ( y = 0; y < heightSegments; y ++ ) {

      auto v1 = verticesRows[ y ][ x ];
      auto v2 = verticesRows[ y + 1 ][ x ];
      auto v3 = verticesRows[ y + 1 ][ x + 1 ];
      auto v4 = verticesRows[ y ][ x + 1 ];

      auto n1 = na.clone();
      auto n2 = na.clone();
      auto n3 = nb.clone();
      auto n4 = nb.clone();

      auto uv1 = uvsRows[ y ][ x ].clone();
      auto uv2 = uvsRows[ y + 1 ][ x ].clone();
      auto uv3 = uvsRows[ y + 1 ][ x + 1 ].clone();
      auto uv4 = uvsRows[ y ][ x + 1 ].clone();

      this->faces.push_back( Face3( v1, v2, v4, n1, n2, n4 ) );
      this->faceVertexUvs[ 0 ].push_back( { uv1, uv2, uv4 } );

      this->faces.push_back( Face3( v2, v3, v4, n2.clone(), n3, n4.clone() ) );
      this->faceVertexUvs[ 0 ].push_back( { uv2.clone(), uv3, uv4.clone() } );

    }

  }

  // top cap

  if ( ! openEnded && radiusTop > 0 ) {

    this->vertices.push_back( Vector3( 0, heightHalf, 0 ) );

    for ( x = 0; x < radialSegments; x ++ ) {

      auto v1 = verticesRows[ 0 ][ x ];
      auto v2 = verticesRows[ 0 ][ x + 1 ];
      auto v3 = this->vertices.size() - 1;

      auto n1 = Vector3( 0, 1, 0 );
      auto n2 = Vector3( 0, 1, 0 );
      auto n3 = Vector3( 0, 1, 0 );

      auto uv1 = uvsRows[ 0 ][ x ].clone();
      auto uv2 = uvsRows[ 0 ][ x + 1 ].clone();
      auto uv3 = Vector2( uv2.x, 0 );

      this->faces.push_back( Face3( v1, v2, v3, n1, n2, n3 ) );
      this->faceVertexUvs[ 0 ].push_back( { uv1, uv2, uv3 } );

    }

  }

  // bottom cap

  if ( ! openEnded && radiusBottom > 0 ) {

    this->vertices.push_back( Vector3( 0, - heightHalf, 0 ) );

    for ( x = 0; x < radialSegments; x ++ ) {

      auto v1 = verticesRows[ y ][ x + 1 ];
      auto v2 = verticesRows[ y ][ x ];
      auto v3 = this->vertices.size() - 1;

      auto n1 = Vector3( 0, - 1, 0 );
      auto n2 = Vector3( 0, - 1, 0 );
      auto n3 = Vector3( 0, - 1, 0 );

      auto uv1 = uvsRows[ y ][ x + 1 ].clone();
      auto uv2 = uvsRows[ y ][ x ].clone();
      auto uv3 = Vector2( uv2.x, 1 );

      this->faces.push_back( Face3( v1, v2, v3, n1, n2, n3 ) );
      this->faceVertexUvs[ 0 ].push_back( { uv1, uv2, uv3 } );

    }

  }

  computeCentroids();
  computeFaceNormals();

}

} // end namespace three

#endif // THREE_EXTRA_CYLINDER_GEOMETRY_CPP