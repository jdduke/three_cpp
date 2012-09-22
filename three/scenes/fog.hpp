#ifndef THREE_FOG_HPP
#define THREE_FOG_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>

namespace three {

class Fog : public IFog {
public:

	Fog ( int hex, float near = 1, float far = 1000 )
	 : color ( hex ), near ( near ), far ( far ) { }

	THREE::FogType type() const	{ return THREE::Fog; }

	Color color;
	float near, far;

};

} // namespace three

#endif // THREE_FOG_HPP