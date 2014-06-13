#ifndef THREE_EXTRA_POLYHEDRON_GEOMETRY_H
#define THREE_EXTRA_POLYHEDRON_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face3.h>

namespace three {

class THREE_DECL PolyhedronGeometry : public Geometry {
private:

  struct IndexedVertex {
    size_t index;
    Vector3 vector;
    Vector2 uv;
  };

public:

  typedef std::shared_ptr<PolyhedronGeometry> Ptr;

  static Ptr create( std::vector<std::array<float,3>> vertices,
                     std::vector<std::array<float,3>> faces,
                     float radius = 1,
                     int detail = 0 );

  float radius;
  int detail;

protected:

  PolyhedronGeometry( float radiusIn, int detailIn );

  void initialize( std::vector<std::array<float,3>> verticesIn,
                   std::vector<std::array<float,3>> facesIn,
                   float radius,
                   int detail );

  IndexedVertex prepare( Vector3& vector );

  void make( const IndexedVertex& v1, const IndexedVertex& v2, const IndexedVertex& v3 );

  void subdivide( const Face3& face, int detail );

  float azimuth( const Vector3& vector ) const;

  float inclination( const Vector3& vector ) const;

  Vector2 correctUV( Vector2 uv, const Vector3& vector, float azimuth ) const;

private:

  std::vector<IndexedVertex> indexedVertices;

};

} // end namespace three

#endif // THREE_EXTRA_POLYHEDRON_GEOMETRY_H