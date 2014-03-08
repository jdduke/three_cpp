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

  virtual void applyMatrix( Matrix4& matrix ) ;

  void computeBoundingBox();
  void computeBoundingSphere();
  void computeVertexNormals();
  void computeTangents();

protected:

  BufferGeometry();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/buffer_geometry.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_BUFFER_GEOMETRY_HPP