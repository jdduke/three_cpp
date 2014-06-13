#ifndef THREE_EXTRA_ICOSAHEDRON_GEOMETRY_H
#define THREE_EXTRA_ICOSAHEDRON_GEOMETRY_H

#include <three/extras/geometries/polyhedron_geometry.h>

namespace three {

class THREE_DECL IcosahedronGeometry : public PolyhedronGeometry {
public:

  typedef std::shared_ptr<IcosahedronGeometry> Ptr;

  static Ptr create( float radius, int detail );

protected:

  IcosahedronGeometry( float radius, int detail );

};

} // end namespace three

#endif // THREE_EXTRA_ICOSAHEDRON_GEOMETRY_H