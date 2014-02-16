#ifndef THREE_FOG_EXP2_HPP
#define THREE_FOG_EXP2_HPP

#include <three/common.hpp>

#include <three/math/color.hpp>

#include <three/utils/noncopyable.hpp>

namespace three {

class FogExp2 : public IFog, NonCopyable {
public:

  typedef std::shared_ptr<FogExp2> Ptr;

  static Ptr create( int hex, float density = 0.00025f ) {
    return three::make_shared<FogExp2>( hex, density );
  }

  THREE::FogType type() const { return THREE::FogExp2; }

  /////////////////////////////////////////////////////////////////////////

  Color color;
  float density;

protected:

   FogExp2( int hex, float density )
    : IFog(), color( hex ), density( density ) { }

};

} // namespace three

#endif // THREE_FOG_EXP2_HPP