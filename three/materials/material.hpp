#ifndef THREE_MATERIAL_HPP
#define THREE_MATERIAL_HPP

#include <three/common.hpp>

#include <three/extras/noncopyable.hpp>

#include <three/materials/attribute.hpp>
#include <three/materials/custom_attribute.hpp>
#include <three/materials/program.hpp>
#include <three/materials/uniform.hpp>

#include <three/textures/texture.hpp>

#include <unordered_set>

namespace three {

typedef std::pair<std::string, any> Parameter;
typedef std::unordered_map<std::string, any> Parameters;

class Material : public NonCopyable {
public:

	typedef std::shared_ptr<Material> Ptr;

	static Ptr create() { return make_shared<Material>( ); }

	virtual THREE::MaterialType type() const { return THREE::Material; }

	/////////////////////////////////////////////////////////////////////////

	int id;

	std::string name;

	CustomAttributes customAttributes;
	Attributes attributes;

	THREE::Side side;

	Color color, ambient, emissive, specular;
	float shininess;
	float opacity;
	bool transparent;

	float size;
	float sizeAttenuation;

	THREE::Shading shading;
	THREE::Colors vertexColors;

	THREE::Blending blending;
	THREE::BlendFactor blendSrc;
	THREE::BlendFactor blendDst;
	THREE::BlendEquation blendEquation;

	bool depthTest;
	bool depthWrite;

	bool polygonOffset;
	float polygonOffsetFactor;
	float polygonOffsetUnits;

	float alphaTest;

	bool overdraw; // Boolean for fixing antialiasing gaps in CanvasRenderer

	bool visible;

	bool needsUpdate;

	bool skinning;
	bool morphTargets;
	bool morphNormals;

	float reflectivity;
	float refractionRatio;
	THREE::TextureConstant combine;

	bool metal;
	bool perPixel;
	bool wrapAround;
	Vector3 wrapRGB;

	bool wireframe;
	float wireframeLinewidth;
	float linewidth;
	THREE::LineEndType linecap, linejoin;

	int numSupportedMorphTargets;
	int	numSupportedMorphNormals;

	Program::Ptr program;

	std::string fragmentShader;
	std::string vertexShader;
	Uniforms uniforms;

	UniformsList uniformsList;

	Texture::Ptr map, envMap, lightMap, bumpMap, specularMap;
	float bumpScale;
	Vector3 normalScale;

	bool fog;
	bool lights;
	bool shadowPass;

	/////////////////////////////////////////////////////////////////////////

	Material& clone ( Material& material ) const {

		material.name                = name;

		material.side                = side;

		material.attributes          = material.attributes;
		material.customAttributes    = material.customAttributes;

		material.color               = color;
		material.ambient             = ambient;
		material.emissive            = emissive;
		material.specular            = specular;

		material.shininess           = shininess;
		material.opacity             = opacity;
		material.transparent         = transparent;

		material.size                = size;
		material.sizeAttenuation     = sizeAttenuation;

		material.shading             = shading;

		material.vertexColors        = vertexColors;

		material.blending            = blending;

		material.blendSrc            = blendSrc;
		material.blendDst            = blendDst;
		material.blendEquation       = blendEquation;

		material.depthTest           = depthTest;
		material.depthWrite          = depthWrite;

		material.polygonOffset       = polygonOffset;
		material.polygonOffsetFactor = polygonOffsetFactor;
		material.polygonOffsetUnits  = polygonOffsetUnits;

		material.alphaTest           = alphaTest;

		material.overdraw            = overdraw;

		material.visible             = visible;

		material.skinning            = skinning;
		material.morphTargets        = morphTargets;
		material.morphNormals        = morphNormals;

		material.reflectivity        = reflectivity;
		material.refractionRatio     = refractionRatio;
		material.combine             = combine;

		material.metal               = metal;
		material.perPixel            = perPixel;
		material.wrapAround          = wrapAround;
		material.wrapRGB             = wrapRGB;

		material.wireframe           = wireframe;
		material.wireframeLinewidth  = wireframeLinewidth;
		material.linewidth           = linewidth;
		material.linejoin            = linejoin;
		material.linecap             = linecap;

		material.numSupportedMorphTargets = numSupportedMorphTargets;
		material.numSupportedMorphNormals = numSupportedMorphNormals;

		material.fragmentShader      = fragmentShader;
		material.vertexShader        = vertexShader;
		material.uniforms            = uniforms;

		material.map         = map;
		material.envMap      = envMap;
		material.lightMap    = lightMap;
		material.bumpMap     = bumpMap;
		material.specularMap = specularMap;

		material.bumpScale       = bumpScale;

		material.fog             = fog;
		material.lights          = lights;
		material.shadowPass      = shadowPass;

		return material;

	}

protected:

	THREE_DECL Material ( )
	: id ( MaterialCount()++ ),
	side ( THREE::FrontSide ),
	color ( 0xffffff ),
	ambient ( 0xffffff ),
	emissive ( 0x000000 ),
	specular ( 0x111111 ),
	shininess ( 30 ),
	opacity ( 1 ),
	transparent ( false ),
	size ( 1.f ),
	sizeAttenuation ( true ),
	shading ( THREE::NoShading ),
	vertexColors ( THREE::NoColors ),
	blending ( THREE::NormalBlending ),
	blendSrc ( THREE::SrcAlphaFactor ),
	blendDst ( THREE::OneMinusSrcAlphaFactor ),
	blendEquation ( THREE::AddEquation ),
	depthTest ( true ),
	depthWrite ( true ),
	polygonOffset ( false ),
	polygonOffsetFactor ( 0 ),
	polygonOffsetUnits ( 0 ),
	alphaTest ( 0 ),
	overdraw ( false ),
	visible ( true ),
	needsUpdate ( true ),
	skinning ( false ),
	morphTargets ( false ),
	morphNormals ( false ),
	reflectivity ( 1.f ),
	refractionRatio ( 0.98f ),
	combine ( THREE::MultiplyOperation ),
	metal ( false ),
	perPixel ( false ),
	wrapAround ( false ),
	wrapRGB ( 1, 1, 1 ),
	wireframe ( false ),
	wireframeLinewidth ( 1 ),
	linewidth ( 1 ),
	linecap ( THREE::Round ),
	linejoin ( THREE::Round ),
	program	( 0 ),
	numSupportedMorphTargets ( 0 ),
	numSupportedMorphNormals ( 0 ),
	fragmentShader ( "void main() { }" ),
	vertexShader ( "void main() { }" ),
	bumpScale ( 1 ),
	normalScale ( 1, 1, 1 ),
	fog ( false ),
	lights ( false ),
	shadowPass ( false ) { }

	template < typename MaterialType >
	static std::shared_ptr<MaterialType> clone ( const MaterialType& src ) {

		auto material = MaterialType::create();

		static_cast<const Material&>(src).clone( *material );

		return material;

	}

	template < typename T >
	static bool load( const std::unordered_map<std::string, any>& parameters,
	                  const std::string& key,
	                  T& value ) {
		auto paramIt = parameters.find( key );
		if ( paramIt != parameters.end() ) {
			try {
				value = paramIt->second.cast<T>();
			} catch ( ... ) {
				return false;
			}
			return true;
		}
		return false;
	}

#define PARAM_LOAD(PARAM_NAME) \
	if ( loadAll || keys.count( #PARAM_NAME ) > 0 ) \
		load ( parameters, #PARAM_NAME, PARAM_NAME )

	typedef std::unordered_set<std::string> ParameterKeys;

	THREE_DECL void setParameters( const Parameters& parameters,
	                               const ParameterKeys& keys = ParameterKeys() ) {
		if ( parameters.empty() )
			return;

		bool loadAll = keys.empty();

		PARAM_LOAD( side );
		PARAM_LOAD( color );
		PARAM_LOAD( ambient );
		PARAM_LOAD( emissive );
		PARAM_LOAD( specular );
		PARAM_LOAD( shininess );
		PARAM_LOAD( opacity );
		PARAM_LOAD( transparent );
		PARAM_LOAD( size );
		PARAM_LOAD( sizeAttenuation );
		PARAM_LOAD( shading );
		PARAM_LOAD( vertexColors );
		PARAM_LOAD( blending );
		PARAM_LOAD( blendSrc );
		PARAM_LOAD( blendDst );
		PARAM_LOAD( blendEquation );
		PARAM_LOAD( depthTest );
		PARAM_LOAD( depthWrite );
		PARAM_LOAD( polygonOffset );
		PARAM_LOAD( polygonOffsetFactor );
		PARAM_LOAD( polygonOffsetUnits );
		PARAM_LOAD( alphaTest );
		PARAM_LOAD( overdraw );
		PARAM_LOAD( visible );
		PARAM_LOAD( needsUpdate );
		PARAM_LOAD( skinning );
		PARAM_LOAD( morphTargets );
		PARAM_LOAD( morphNormals );
		PARAM_LOAD( reflectivity );
		PARAM_LOAD( refractionRatio );
		PARAM_LOAD( combine );
		PARAM_LOAD( metal );
		PARAM_LOAD( perPixel );
		PARAM_LOAD( wrapAround );
		PARAM_LOAD( wrapRGB );
		PARAM_LOAD( wireframe );
		PARAM_LOAD( wireframeLinewidth );
		PARAM_LOAD( linewidth );
		PARAM_LOAD( linecap );
		PARAM_LOAD( linejoin );
		PARAM_LOAD( program );
		PARAM_LOAD( numSupportedMorphNormals );
		PARAM_LOAD( fragmentShader );
		PARAM_LOAD( vertexShader );
		PARAM_LOAD( bumpScale );
		PARAM_LOAD( normalScale );
		PARAM_LOAD( fog );
		PARAM_LOAD( lights );
		PARAM_LOAD( shadowPass );
	}

#undef PARAM_LOAD

private:

	static int& MaterialCount() {
		static int sMaterialCount = 0;
		return sMaterialCount;
	}

};


#if THREE_HAS_VARIADIC_TEMPLATES

template < typename... Params >
Parameters parameters( Params&&... params ) {
	return Parameters( {std::move(params)...} );
}

#else

Parameters parameters( Parameter&& p0 ) {
	Parameters params;
	params.insert( p0 );
	return std::move(params);
}
Parameters parameters( Parameter&& p0, Parameter&& p1 ) {
	Parameters params;
	params.insert( p0 );
	params.insert( p1 );
	return std::move(params);
}
Parameters parameters( Parameter&& p0, Parameter&& p1, Parameter&& p2 ) {
	Parameters params;
	params.insert( p0 );
	params.insert( p1 );
	params.insert( p2 );
	return std::move(params);
}
Parameters parameters( Parameter&& p0, Parameter&& p1, Parameter&& p2, Parameter&& p3 ) {
	Parameters params;
	params.insert( p0 );
	params.insert( p1 );
	params.insert( p2 );
	params.insert( p3 );
	return std::move(params);
}
Parameters parameters( Parameter&& p0, Parameter&& p1, Parameter&& p2, Parameter&& p3, Parameter&& p4 ) {
	Parameters params;
	params.insert( p0 );
	params.insert( p1 );
	params.insert( p2 );
	params.insert( p3 );
	params.insert( p4 );
	return std::move(params);
}
Parameters parameters( Parameter&& p0, Parameter&& p1, Parameter&& p2, Parameter&& p3, Parameter&& p4, Parameter&& p5 ) {
  Parameters params;
  params.insert( p0 );
  params.insert( p1 );
  params.insert( p2 );
  params.insert( p3 );
  params.insert( p4 );
  params.insert( p5 );
  return std::move(params);
}
#endif


} // namespace three

#endif // THREE_MATERIAL_HPP