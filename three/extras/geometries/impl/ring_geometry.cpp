#ifndef THREE_EXTRA_RING_GEOMETRY_CPP
#define THREE_EXTRA_RING_GEOMETRY_CPP

#include <three/extras/geometries/ring_geometry.h>

#include <three/core/geometry.h>
#include <three/core/face3.h>
#include <three/math/sphere.h>
#include <three/math/vector2.h>
#include <three/math/vector3.h>

namespace three {

RingGeometry::Ptr RingGeometry::create( float innerRadius,
                                        float outerRadius,
                                        size_t thetaSegments,
                                        size_t phiSegments,
                                        float thetaStart,
                                        float thetaLength) {

  auto ringGeometry = make_shared<RingGeometry>();

  ringGeometry->initialize( innerRadius, outerRadius, thetaSegments,
  phiSegments, thetaStart, thetaLength );

  return ringGeometry;
}

RingGeometry::RingGeometry(){}

void RingGeometry::initialize( float innerRadius,
                 float outerRadius,
                 size_t thetaSegments,
                 size_t phiSegments,
                 float thetaStart,
                 float thetaLength) {


  size_t i, o;

  std::vector<Vector2> uvs;

  auto radius = innerRadius;
  auto radiusStep = ( ( outerRadius - innerRadius ) / (float)phiSegments );

  for ( i = 0; i <= phiSegments; i ++ ) { // concentric circles inside ring

    for ( o = 0; o <= thetaSegments; o ++ ) { // number of segments per circle

      auto vertex = Vector3();
      auto segment = thetaStart + (float)o / (float)thetaSegments * thetaLength;

      vertex.x = radius * Math::cos( segment );
      vertex.y = radius * Math::sin( segment );

      this->vertices.push_back( vertex );
      uvs.push_back( Vector2( ( vertex.x / outerRadius + 1 ) / 2, ( vertex.y / outerRadius + 1 ) / 2 ) );
    }

    radius += radiusStep;

  }

  auto n = Vector3( 0, 0, 1 );

  for ( i = 0; i < phiSegments; i ++ ) { // concentric circles inside ring

  auto thetaSegment = i * thetaSegments;

    for ( o = 0; o <= thetaSegments; o ++ ) { // number of segments per circle

      auto segment = o + thetaSegment;

      auto v1 = segment + i;
      auto v2 = segment + thetaSegments + i;
      auto v3 = segment + thetaSegments + 1 + i;

      this->faces.push_back( Face3( v1, v2, v3, n.clone(), n.clone(), n.clone() ) );
      this->faceVertexUvs[ 0 ].push_back( { uvs[ v1 ].clone(), uvs[ v2 ].clone(), uvs[ v3 ].clone() } );

      v1 = segment + i;
      v2 = segment + thetaSegments + 1 + i;
      v3 = segment + 1 + i;

      this->faces.push_back( Face3( v1, v2, v3, n.clone(), n.clone(), n.clone() ) );
      this->faceVertexUvs[ 0 ].push_back( { uvs[ v1 ].clone(), uvs[ v2 ].clone(), uvs[ v3 ].clone() } );

    }

  }

  this->computeCentroids();
  this->computeFaceNormals();

  this->boundingSphere = Sphere( Vector3(), radius );
}

} // end namespace three

#endif // THREE_EXTRA_RING_GEOMETRY_CPP