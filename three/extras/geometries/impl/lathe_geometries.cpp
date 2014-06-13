#ifndef THREE_EXTRA_LATHE_GEOMETRY_CPP
#define THREE_EXTRA_LATHE_GEOMETRY_CPP

#include <three/extras/geometries/lathe_geometry.h>

#include <three/core/geometry.h>
#include <three/core/face3.h>

#include <three/utils/conversion.h>

namespace three {

LatheGeometry::Ptr LatheGeometry::create( const std::vector<Vector3>& points,
                                          size_t segments,
                                          float phiStart,
                                          float phiLength ) {

  auto latheGeometry = make_shared<LatheGeometry>();

  latheGeometry->initialize( points, segments, phiStart, phiLength );

  return latheGeometry;
}

LatheGeometry::LatheGeometry() {}

void LatheGeometry::initialize( const std::vector<Vector3>& points,
                                size_t segments,
                                float phiStart,
                                float phiLength ) {

  auto inversePointLength = 1.f / ( (float)points.size() - 1.f );
  auto inverseSegments = 1.f / (float)segments;

  for ( size_t i = 0; i <= segments; i ++ ) {

    auto phi = phiStart + (float)i * inverseSegments * phiLength;

    auto c = Math::cos( phi ),
         s = Math::sin( phi );

    for ( size_t j = 0; j < points.size(); j ++ ) {

      const auto& pt = points[ j ];

      auto vertex = Vector3();

      vertex.x = c * pt.x - s * pt.y;
      vertex.y = s * pt.x + c * pt.y;
      vertex.z = pt.z;

      this->vertices.push_back( vertex );

    }

  }

  float np = (float)points.size();

  for ( size_t i = 0; i < segments; i ++ ) {

    for ( size_t j = 0; j < points.size() - 1; j ++ ) {

      float base = (float)j + np * (float)i;
      auto a = base;
      auto b = base + np;
      auto c = base + 1.f + np;
      auto d = base + 1.f;

      auto u0 = (float)i * inverseSegments;
      auto v0 = (float)j * inversePointLength;
      auto u1 = u0 + inverseSegments;
      auto v1 = v0 + inversePointLength;

      this->faces.push_back( Face3( a, b, d ) );

      this->faceVertexUvs[ 0 ].push_back( toArray(

        Vector2( u0, v0 ),
        Vector2( u1, v0 ),
        Vector2( u0, v1 )

      ) );

      this->faces.push_back( Face3( b, c, d ) );

      this->faceVertexUvs[ 0 ].push_back( toArray(

        Vector2( u1, v0 ),
        Vector2( u1, v1 ),
        Vector2( u0, v1 )

      ) );

    }

  }

  this->mergeVertices();
  this->computeCentroids();
  this->computeFaceNormals();
  this->computeVertexNormals();

}

} // end namespace three

#endif // THREE_EXTRA_LATHE_GEOMETRY_CPP