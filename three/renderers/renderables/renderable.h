#ifndef THREE_RENDERABLE_H
#define THREE_RENDERABLE_H

#include <three/common.h>

namespace three {

class Renderable {
public:

  int id;

  float z;

protected:

  Renderable( float z ) : z( z ) { }

};

} // namespace three

#endif // THREE_RENDERABLE_OBJECT_H