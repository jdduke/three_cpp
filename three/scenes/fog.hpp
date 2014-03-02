#ifndef THREE_FOG_HPP
#define THREE_FOG_HPP

#include <three/common.hpp>

#include <three/math/color.hpp>

#include <three/utils/noncopyable.hpp>

namespace three {

class Fog : public IFog, NonCopyable {
public:

  typedef std::shared_ptr<Fog> Ptr;

  static Ptr create( int hex, float near = 1, float far = 1000 ) {
    return three::make_shared<Fog>( hex, near, far );
  }

  enums::FogType type() const { return enums::Fog; }

  /////////////////////////////////////////////////////////////////////////

  Color color;
  float near, far;

protected:

  Fog( int hex, float near, float far )
    : IFog(), color( hex ), near( near ), far( far ) { }

};

} // namespace three

#endif // THREE_FOG_HPP