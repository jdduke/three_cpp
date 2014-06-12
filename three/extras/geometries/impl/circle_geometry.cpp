#ifndef THREE_CUBE_GEOMETRY_CPP
#define THREE_CUBE_GEOMETRY_CPP

#include <three/core/face3.h>
#include <three/extras/geometries/circle_geometry.h>

namespace three {

CircleGeometry::Ptr CircleGeometry::create(float radius,
                  size_t segments, 
                  float thetaStart, 
                  float thetaLength) {

  auto circleGeometry = make_shared<CircleGeometry>();

  circleGeometry->initialize( radius, segments, thetaStart, thetaLength );

  return circleGeometry;
}

CircleGeometry::CircleGeometry() { };

void CircleGeometry::initialize( float radius, size_t segments, float thetaStart, float thetaLength ) {

  std::vector<Vector2> uvs;

  auto center = Vector3();
  auto centerUV = Vector2( 0.5f, 0.5f );

  vertices.push_back(center);
  uvs.push_back( centerUV );

  for ( int i = 0; i <= segments; i ++ ) {

    auto vertex = Vector3();
    float segment = thetaStart + (float)i / (float)segments * thetaLength;

    vertex.x = radius * Math::cos( segment );
    vertex.y = radius * Math::sin( segment );

    vertices.push_back( vertex );
    uvs.push_back( Vector2( ( vertex.x / radius + 1 ) / 2.f, ( vertex.y / radius + 1 ) / 2.f ) );

  }

  auto n = Vector3( 0, 0, 1 );

  for ( int i = 1; i <= segments; i ++ ) {

    auto v1 = (float)i;
    auto v2 = (float)i + 1.f ;
    auto v3 = (float)0;

    faces.push_back( Face3( v1, v2, v3, n.clone(), n.clone(), n.clone()  ) );
    faceVertexUvs[ 0 ].push_back( { uvs[ i ].clone(), uvs[ i + 1 ].clone(), centerUV.clone() } );

  }

  computeCentroids();
  computeFaceNormals();

  boundingSphere = Sphere( Vector3(), radius );

}

} // namespace three

#endif // THREE_CIRCLE_GEOMETRY_CPP