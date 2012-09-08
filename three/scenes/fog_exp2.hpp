#ifndef THREE_FOG_EXP2_HPP
#define THREE_FOG_EXP2_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>

namespace three {

class FogExp2 {

	FogExp2 ( int hex, float density = 0.00025f )
	 : color ( hex ), density ( density ) { }

	Color color;
	float density;

};

} // namespace three

#endif // THREE_FOG_EXP2_HPP