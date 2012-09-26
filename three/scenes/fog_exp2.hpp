#ifndef THREE_FOG_EXP2_HPP
#define THREE_FOG_EXP2_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>

namespace three {

class FogExp2 : public IFog {
public:

  FogExp2( int hex, float density = 0.00025f )
    : IFog( hex ), density( density ) { }

  THREE::FogType type() const    { return THREE::FogExp2; }

  float density;

};

} // namespace three

#endif // THREE_FOG_EXP2_HPP