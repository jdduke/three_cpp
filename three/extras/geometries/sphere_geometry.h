#ifndef THREE_SPHERE_GEOMETRY_H
#define THREE_SPHERE_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face.h>

#include <three/math/math.h>

#include <three/utils/conversion.h>

namespace three {

class THREE_DECL SphereGeometry : public Geometry {
public:

  typedef std::shared_ptr<SphereGeometry> Ptr;

  static Ptr create( float radius = 50,
                     float segmentsWidth = 8,
                     float segmentsHeight = 6,
                     float phiStart = 0,
                     float phiLength = Math::PI() * 2,
                     float thetaStart = 0,
                     float thetaLength = Math::PI() );

protected:

  SphereGeometry();

  void initialize( float radius,
                   float segmentsWidth,
                   float segmentsHeight,
                   float phiStart,
                   float phiLength,
                   float thetaStart,
                   float thetaLength );

};

} // namespace three

#endif // THREE_SPHERE_GEOMETRY_H