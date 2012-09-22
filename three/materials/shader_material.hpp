#if 0

#ifndef THREE_SHADER_MATERIAL_HPP
#define THREE_SHADER_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

/**
 * parameters = {
 *  fragmentShader: <string>,
 *  vertexShader: <string>,
 *
 *  uniforms: { "parameter1": { type: "f", value: 1.0 }, "parameter2": { type: "i" value2: 2 } },
 *
 *  shading: THREE::SmoothShading,
 *  blending: THREE::NormalBlending,
 *  depthTest: <bool>,
 *
 *  wireframe: <boolean>,
 *  wireframeLinewidth: <float>,
 *
 *  lights: <bool>,
 *
 *  vertexColors: THREE::NoColors / THREE::VertexColors / THREE::FaceColors,
 *
 *  skinning: <bool>,
 *  morphTargets: <bool>,
 *  morphNormals: <bool>,
 *
 *	fog: <bool>
 * }
 */

class ShaderMaterial : public Material {
public:

	typedef std::shared_ptr<ShaderMaterial> Ptr;

	static Ptr create( /* parameters */ ) { return make_shared<ShaderMaterial>( ); }

	/////////////////////////////////////////////////////////////////////////

	bool wireframe;
	float wireframeLinewidth;

	bool fog;

	bool lights;

	bool skinning; // set to use skinning attribute streams

	bool morphTargets; // set to use morph targets
	bool morphNormals; // set to use morph normals

	/////////////////////////////////////////////////////////////////////////

	Ptr clone ( ) {

		auto materialPtr = create();

		auto& material = *materialPtr;

		Material::clone ( material );

		material.fragmentShader = fragmentShader;
		material.vertexShader = vertexShader;
		//material.uniforms = uniforms;
		//material.attributes = attributes;

		material.wireframe = wireframe;
		material.wireframeLinewidth = wireframeLinewidth;

		material.fog = fog;

		material.lights = lights;

		material.skinning = skinning;

		material.morphTargets = morphTargets;
		material.morphNormals = morphNormals;

		return materialPtr;

	}

protected:

	ShaderMaterial ( /* parameters */ )
	: Material(),
	fragmentShader ( "void main() {}" ),
	vertexShader ( "void main() {}" ),
	//uniforms ( {} ),
	//attributes ( null ),
	wireframe ( false ),
	wireframeLinewidth ( 1 ),
	fog ( false ),
	lights ( false ),
	skinning ( false ),
	morphTargets ( false ),
	morphNormals ( false ) {

		shading = THREE::SmoothShading;

	}

};

} // namespace three

#endif // THREE_MATERIAL_HPP

#endif // 0