#ifndef THREE_TORUS_KNOT_GEOMETRY_H
#define THREE_TORUS_KNOT_GEOMETRY_H

#include <three/core/geometry.h>

namespace three {

class THREE_DECL TorusKnotGeometry : public Geometry {
public:

  typedef std::shared_ptr<TorusKnotGeometry> Ptr;

  static Ptr create( float radius = 100,
                     float tube = 40,
                     size_t radialSegments = 64,
                     size_t tubularSegments = 8,
                     float p = 2,
                     float q = 3,
                     float heightScale = 1 );

protected:

  TorusKnotGeometry();

  void initialize( float radius,
                   float tube,
                   size_t radialSegments,
                   size_t tubularSegments,
                   float p,
                   float q,
                   float heightScale );

private:

  Vector3 getPos( float u, float in_q, float in_p, float radius, float heightScale ) const;

};

} // namespace three

#endif // THREE_TORUS_KNOT_GEOMETRY_H