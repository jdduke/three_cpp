#ifndef THREE_EXTRA_TETRAHEDRON_GEOMETRY_H
#define THREE_EXTRA_TETRAHEDRON_GEOMETRY_H

#include <three/extras/geometries/polyhedron_geometry.h>

namespace three {

class THREE_DECL TetrahedronGeometry : public PolyhedronGeometry {
public:

  typedef std::shared_ptr<TetrahedronGeometry> Ptr;

  static Ptr create( float radius, int detail );

protected:

  TetrahedronGeometry( float radius, int detail );

};

} // end namespace three

#endif // THREE_EXTRA_TETRAHEDRON_GEOMETRY_H