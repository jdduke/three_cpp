#ifndef THREE_EXTRAS_AXIS_HELPER_H
#define THREE_EXTRAS_AXIS_HELPER_H

#include <three/objects/line.h>
#include <three/utils/conversion.h>

namespace three {

class AxisHelper : public Line {
public:

  typedef std::shared_ptr<AxisHelper> Ptr;

  static Ptr create( float sizeIn = 1 );

  float size;

protected:

  AxisHelper( float sizeIn );

};

} // end namespace three

#endif // THREE_EXTRAS_AXIS_HELPER_H