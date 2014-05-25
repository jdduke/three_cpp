#ifndef THREE_BOX_GEOMETRY_H
#define THREE_BOX_GEOMETRY_H

#include <three/core/geometry.h>
#include <three/core/face3.h>
#include <three/utils/conversion.h>

namespace three {

class THREE_DECL BoxGeometry : public Geometry {
public:

  typedef std::shared_ptr<BoxGeometry> Ptr;

  static Ptr create( float width,
                     float height,
                     float depth,
                     int segmentsWidth = 1,
                     int segmentsHeight = 1,
                     int segmentsDepth = 1 );

protected:

  enum Side { X = 0, Y = 1, Z = 2 };

  BoxGeometry();

  void initialize( float width,
                   float height,
                   float depth,
                   int segmentsWidth,
                   int segmentsHeight,
                   int segmentsDepth );

  void buildPlane( Side u, Side v,
                   float udir, float vdir,
                   float width, float height, float depth,
                   int materialIndex,
                   int segmentsWidth, int segmentsHeight, int segmentsDepth );
};

} // namespace three

#endif // THREE_BOX_GEOMETRY_H