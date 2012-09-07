#ifndef THREE_SPOT_LIGHT_HPP
#define THREE_SPOT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class SpotLight : public Light {
public:

	typedef shared_ptr<PointLight> Ptr;

	/////////////////////////////////////////////////////////////////////////

	Object3D* target;

	float intensity;
	float distance;
	float angle;
	float exponent;

	bool castShadow, onlyShadow;

	float shadowCameraNear;
	float shadowCameraFar;
	float shadowCameraFov;

	bool shadowCameraVisible;

	float shadowBias;
	float shadowDarkness;

	int shadowMapWidth;
	int shadowMapHeight;

	// TODO:

	// shadowMap = null;
	//this.shadowMapSize = null;
	//this.shadowCamera = null;
	//this.shadowMatrix = null;

	/////////////////////////////////////////////////////////////////////////

	static Ptr create( int hex, float intensity = 1, float distance = 0, float angle = Math.PI / 2, float exponent = 10 ) {

		return make_shared<AmbientLight>( hex, intensity, distance, angle, exponent );

	}

private:

	SpotLight ( int hex, float intensity, float distance, float angle, float exponent )
	 : Light ( hex ),
	 target ( nullptr ),
	 intensity ( intensity ), distance ( distance ), angle ( angle ), exponent ( exponent ),
	 castShadow ( false ), onlyShadow ( false )
	 shadowCameraNear ( 50 ), shadowCameraFar ( 5000 ), shadowCameraFov ( 50 ),
	 shadowCameraVisible ( false ),
	 shadowBias ( 0 ),
	 shadowDarkness ( 0.5 ),
	 shadowMapWidth ( 512 ),
	 shadowMapHeight ( 512 ) {

		position.set ( 0, 1, 0 );

	}

};

} // namespace three

#endif // THREE_SPOT_LIGHT_HPP