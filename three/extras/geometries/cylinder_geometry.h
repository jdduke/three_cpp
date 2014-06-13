#ifndef THREE_EXTRA_CYLINDER_GEOMETRY_H
#define THREE_EXTRA_CYLINDER_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face3.h>

namespace three {

class THREE_DECL CylinderGeometry : public Geometry {
public:

  typedef std::shared_ptr<CylinderGeometry> Ptr;

  static Ptr create( float radiusTop = 20,
                     float radiusBottom = 20,
                     float height = 100,
                     size_t radialSegments = 8,
                     size_t heightSegments = 1,
                     bool openEnded = false );

protected:

  CylinderGeometry();

  void initialize( float radiusTop,
                   float radiusBottom,
                   float height,
                   size_t radialSegments,
                   size_t heightSegments,
                   bool openEnded );

};

} // end namespace three

#endif // THREE_EXTRA_CYLINDER_GEOMETRY_H