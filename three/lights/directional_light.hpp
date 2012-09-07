#ifndef THREE_DIRECTIONAL_LIGHT_HPP
#define THREE_DIRECTIONAL_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class DirectionalLight : public Light {
public:

	typedef shared_ptr<DirectionalLight> Ptr;

	/////////////////////////////////////////////////////////////////////////

	Object3D* target;

	float intensity;
	float distance;

	bool castShadow, onlyShadow;

	float shadowCameraNear;
	float shadowCameraFar;

	float shadowCameraLeft;
	float shadowCameraRight;
	float shadowCameraTop;
	float shadowCameraBottom;

	bool shadowCameraVisible;

	float shadowBias;
	float shadowDarkness;

	int shadowMapWidth;
	int shadowMapHeight;

	// TODO:
	/*
	this.shadowCascade = false;

	this.shadowCascadeOffset = new THREE.Vector3( 0, 0, -1000 );
	this.shadowCascadeCount = 2;

	this.shadowCascadeBias = [ 0, 0, 0 ];
	this.shadowCascadeWidth = [ 512, 512, 512 ];
	this.shadowCascadeHeight = [ 512, 512, 512 ];

	this.shadowCascadeNearZ = [ -1.000, 0.990, 0.998 ];
	this.shadowCascadeFarZ  = [  0.990, 0.998, 1.000 ];

	this.shadowCascadeArray = [];

	//

	this.shadowMap = null;
	this.shadowMapSize = null;
	this.shadowCamera = null;
	this.shadowMatrix = null;
	*/

	/////////////////////////////////////////////////////////////////////////

	static void create( int hex, float intensity = 1, float distance = 0 ) {

		return make_shared<AmbientLight>( hex, intensity, distance );

	}

private:

	DirectionalLight ( int hex, float intensity, float distance )
	 : Light ( hex ),
	 target ( nullptr ),
	 intensity ( intensity ), distance ( distance ),
	 castShadow ( false ), onlyShadow ( false ),
	 shadowCameraNear ( 50 ), shadowCameraFar ( 5000 ),
	 shadowCameraLeft ( -500 ), shadowCameraRight ( 500 ),
	 shadowCameraTop ( 500 ), shadowCameraBottom ( -500 ),
	 shadowCameraVisible ( false ),
	 shadowBias ( 0 ),
	 shadowDarkness ( 0.5 ),
	 shadowMapWidth ( 512 ),
	 shadowMapHeight ( 512 ) {

		position.set ( 0, 1, 0 );

	}

};

} // namespace three

#endif // THREE_DIRECTIONAL_LIGHT_HPP