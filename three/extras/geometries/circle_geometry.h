#ifndef THREE_CUBE_GEOMETRY_H
#define THREE_CUBE_GEOMETRY_H

#include <three/core/geometry.h>

namespace three {

class THREE_DECL CircleGeometry : public Geometry {
public:

  typedef std::shared_ptr<CircleGeometry> Ptr;

  static Ptr create(float radius = 50, 
                    size_t segments = 8, 
                    float thetaStart = 0, 
                    float thetaLength = Math::PI() * 2 );

protected:

  CircleGeometry();

  void initialize( float radius, size_t segments, float thetaStart, float thetaLength );

};

} // namespace three

#endif // THREE_CIRCLE_GEOMETRY_H