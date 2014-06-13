#ifndef THREE_EXTRA_OCTAHEDRON_GEOMETRY_H
#define THREE_EXTRA_OCTAHEDRON_GEOMETRY_H

#include <three/extras/geometries/polyhedron_geometry.h>

namespace three {

class THREE_DECL OctahedronGeometry : public PolyhedronGeometry {
public:

  typedef std::shared_ptr<OctahedronGeometry> Ptr;

  static Ptr create( float radius, int detail );

protected:

  OctahedronGeometry( float radius, int detail );

};

} // end namespace three

#endif // THREE_EXTRA_OCTAHEDRON_GEOMETRY_H