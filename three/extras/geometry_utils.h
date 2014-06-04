#ifndef THREE_GEOMETRY_UTILS_H
#define THREE_GEOMETRY_UTILS_H

#include <three/core/face3.h>

namespace three {

class GeometryUtils {
public:

  static void merge( Geometry& geometry1, Object3D& object2, int materialIndexOffset = 0 );

  static void merge( Geometry& geometry1,
                     Geometry& geometry2,
                     int materialIndexOffset = 0,
                     const Matrix4* matrix = nullptr,
                     const Matrix3* normalMatrix = nullptr );

  // NOTE: Skip rest of r65, because GeometryUtils is becoming obsolete in ~r67

};

} // namespace three

#endif // THREE_GEOMETRY_UTILS_H