#ifndef THREE_BUFFER_GEOMETRY_HPP
#define THREE_BUFFER_GEOMETRY_HPP

#include <three/common.hpp>

#include <three/core/geometry.hpp>

namespace three {

struct Offset {
  Offset() : start( 0 ), count( 0 ), index( 0 ) { }
  int start, count, index;
};

class BufferGeometry : public Geometry {
public:

  typedef std::shared_ptr<BufferGeometry> Ptr;

  Ptr create() { return make_shared<BufferGeometry>(); }

  virtual enums::GeometryType type() const { return enums::BufferGeometry; }

  /////////////////////////////////////////////////////////////////////////

  std::vector<Offset> offsets;

  /////////////////////////////////////////////////////////////////////////

  virtual THREE_DECL void applyMatrix( Matrix4& matrix ) ;

  THREE_DECL void computeBoundingBox();
  THREE_DECL void computeBoundingSphere();
  THREE_DECL void computeVertexNormals();
  THREE_DECL void computeTangents();

protected:

  THREE_DECL BufferGeometry();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/buffer_geometry.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_BUFFER_GEOMETRY_HPP