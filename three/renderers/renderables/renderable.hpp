#ifndef THREE_RENDERABLE_HPP
#define THREE_RENDERABLE_HPP

#include <three/common.hpp>

namespace three {

class Renderable {
public:

  float z;

protected:

  Renderable( float z ) : z( z ) { }
};

} // namespace three

#endif // THREE_RENDERABLE_OBJECT_HPP