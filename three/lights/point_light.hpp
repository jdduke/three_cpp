#ifndef THREE_POINT_LIGHT_HPP
#define THREE_POINT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class PointLight : public Light {
public:

	typedef shared_ptr<PointLight> Ptr;

	/////////////////////////////////////////////////////////////////////////

	float intensity;
	float distance;

	/////////////////////////////////////////////////////////////////////////

	static void create( int hex, float intensity = 1, float distance = 0 ) {

		return make_shared<AmbientLight>( hex, intensity, distance );

	}

private:

	PointLight ( int hex, float intensity, float distance )
	 : Light ( hex ), intensity ( intensity ), distance ( distance ) {

		position.set ( 0, 0, 0 );

	}

};

} // namespace three

#endif // THREE_POINT_LIGHT_HPP