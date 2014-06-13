#ifndef THREE_EXTRA_LATHE_GEOMETRY_H
#define THREE_EXTRA_LATHE_GEOMETRY_H

#include <three/core/geometry.h>

namespace three {

class THREE_DECL LatheGeometry : public Geometry {
public:

  typedef std::shared_ptr<LatheGeometry> Ptr;

  static Ptr create( const std::vector<Vector3>& points,
                     size_t segments = 12,
                     float phiStart = 0,
                     float phiLength = Math::PI() * 2.f );

protected:

  LatheGeometry();

  void initialize( const std::vector<Vector3>& points,
                   size_t segments,
                   float phiStart,
                   float phiLength );

};

} // end namespace three

#endif // THREE_EXTRA_LATHE_GEOMETRY_H