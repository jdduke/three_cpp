#ifndef THREE_EXTRA_RING_GEOMETRY_H
#define THREE_EXTRA_RING_GEOMETRY_H

#include <three/core/geometry.h>

namespace three {

class THREE_DECL RingGeometry : public Geometry {
public:

  typedef std::shared_ptr<RingGeometry> Ptr;

  static Ptr create( float innerRadius = 0,
                     float outerRadius = 50,
                     size_t thetaSegments = 8,
                     size_t phiSegments = 8,
                     float thetaStart = 0,
                     float thetaLength = Math::PI() * 2 );

protected:

  RingGeometry();

  void initialize( float innerRadius,
                   float outerRadius,
                   size_t thetaSegments,
                   size_t phiSegments,
                   float thetaStart,
                   float thetaLength);

};

} // end namespace three

#endif // THREE_EXTRA_RING_GEOMETRY_H