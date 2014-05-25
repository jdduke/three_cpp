#ifndef THREE_TORUS_GEOMETRY_H
#define THREE_TORUS_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face3.h>

#include <three/math/math.h>

#include <three/utils/conversion.h>

namespace three {

class THREE_DECL TorusGeometry : public Geometry {
public:

  typedef std::shared_ptr<TorusGeometry> Ptr;

  static Ptr create( float radius = 50,
                     float tube = 40,
                     int segmentsR = 8,
                     int segmentsT = 6,
                     float arc = Math::PI() * 2 );

protected:

  TorusGeometry();

  void initialize( float radius,
                   float tube,
                   int segmentsR,
                   int segmentsT,
                   float arc );

};

} // namespace three

#endif // THREE_TORUS_GEOMETRY_H