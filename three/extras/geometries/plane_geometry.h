#ifndef THREE_PLANE_GEOMETRY_H
#define THREE_PLANE_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face3.h>

namespace three {

class THREE_DECL PlaneGeometry : public Geometry {
public:

 typedef std::shared_ptr<PlaneGeometry> Ptr;

  static Ptr create( float width,
                     float height,
                     int segmentsWidth = 1,
                     int segmentsHeight = 1 );

protected:

  PlaneGeometry();

  void initialize( float width,
                   float height,
                   int segmentsWidth,
                   int segmentsHeight );

};

} // namespace three

#endif // THREE_PLANE_GEOMETRY_H