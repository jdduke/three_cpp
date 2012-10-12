#ifndef THREE_GEOMETRY_HPP
#define THREE_GEOMETRY_HPP

#include <three/common.hpp>

#include <three/core/geometry_buffer.hpp>
#include <three/core/geometry_group.hpp>

#include <three/core/math.hpp>
#include <three/core/color.hpp>
#include <three/core/face.hpp>
#include <three/core/vector3.hpp>
#include <three/core/vertex.hpp>
#include <three/core/uv.hpp>
#include <three/core/interfaces.hpp>

#include <three/materials/material.hpp>

#include <three/utils/memory.hpp>

#include <array>
#include <unordered_map>
#include <tuple>

namespace three {

struct MorphTarget {
  std::string name;
  std::vector<Vertex> vertices;
};

struct Box {
  Vector3 min;
  Vector3 max;

  Box() { }
  Box( const Vector3& min, const Vector3& max )
    : min( min ), max( max ) { }

  void bound( const Vector3& pos ) {
    if ( pos.x < min.x ) {
      min.x = pos.x;
    } else if ( pos.x > max.x ) {
      max.x = pos.x;
    }

    if ( pos.y < min.y ) {
      min.y = pos.y;
    } else if ( pos.y > max.y ) {
      max.y = pos.y;
    }

    if ( pos.z < min.z ) {
      min.z = pos.z;
    } else if ( pos.z > max.z ) {
      max.z = pos.z;
    }
  }
};

struct Sphere {
  Sphere() : radius( 0.0f ) { }
  float radius;
};

class Geometry : public IGeometry, public GeometryBuffer {
public:

  typedef std::shared_ptr<Geometry> Ptr;

  static Ptr create() { return make_shared<Geometry>(); }

  virtual THREE::GeometryType type() const { return THREE::Geometry; }

  /////////////////////////////////////////////////////////////////////////

  int id;

  std::string name;

  std::vector<Vertex> vertices;

  std::vector<Color> colors; // one-to-one vertex colors, used in ParticleSystem, Line and Ribbon

  std::vector<Material::Ptr> materials;
  Attributes attributes;

  std::vector<Face> faces;

  std::vector<std::vector<UV>> faceUvs;
  std::vector<std::vector<std::array<UV, 4>>> faceVertexUvs;

  std::vector<MorphTarget> morphTargets;
  std::vector<Color> morphColors;
  //std::vector<Vector3> morphNormals;
  std::vector<Face> morphNormals;

  std::vector<Vector3> skinVerticesA;
  std::vector<Vector3> skinVerticesB;
  std::vector<Vector4> skinWeights;
  struct SkinIndices { int x, y, z, w; };
  std::vector<SkinIndices> skinIndices;

  struct Offset { int index, count, start; };
  std::vector<Offset> offsets;

  Box    boundingBox;
  Sphere boundingSphere;

  bool hasTangents;
  bool dynamic;

  std::unordered_map<std::string, GeometryGroup::Ptr> geometryGroups;
  std::vector<GeometryGroup*> geometryGroupsList;

  bool verticesNeedUpdate;
  bool morphTargetsNeedUpdate;
  bool elementsNeedUpdate;
  bool uvsNeedUpdate;
  bool normalsNeedUpdate;
  bool tangentsNeedUpdate;
  bool colorsNeedUpdate;

  /////////////////////////////////////////////////////////////////////////

  virtual THREE_DECL void applyMatrix( const Matrix4& matrix );

  virtual THREE_DECL void computeCentroids();
  virtual THREE_DECL void computeFaceNormals();
  virtual THREE_DECL void computeVertexNormals();
  virtual THREE_DECL void computeTangents();
  virtual THREE_DECL void computeBoundingBox();
  virtual THREE_DECL void computeBoundingSphere();

  THREE_DECL void mergeVertices();

  /////////////////////////////////////////////////////////////////////////

protected:

  Geometry();

private:

  std::vector<Vector3> normals;

  static int& GeometryCount() {
    static int sGeometryCount = 0;
    return sGeometryCount;
  }

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/geometry.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GEOMETRY_HPP