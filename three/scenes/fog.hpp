#ifndef THREE_FOG_HPP
#define THREE_FOG_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>

namespace three {

class Fog {

	Fog ( int hex, float near = 1, float far = 1000 )
	 : color ( hex ), near ( near ), far ( far ) { }

	Color color;
	float near, far;

};

} // namespace three

#endif // THREE_FOG_HPP