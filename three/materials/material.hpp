#ifndef THREE_MATERIAL_HPP
#define THREE_MATERIAL_HPP

#include <three/common.hpp>

#include <three/extras/noncopyable.hpp>

namespace three {

class Material : public NonCopyable {
public:

	typedef std::shared_ptr<Material> Ptr;

	static Ptr create( int hex ) { return make_shared<Material>( ); }

	virtual THREE::MaterialType type() const { return THREE::Material; }

	/////////////////////////////////////////////////////////////////////////

	int id;

	std::string name;

	THREE::Side side;

	float opacity;
	bool transparent;

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

	/////////////////////////////////////////////////////////////////////////

	Material& clone ( Material& material ) {

		material.name                = name;

		material.side                = side;

		material.opacity             = opacity;
		material.transparent         = transparent;

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

		return material;

	}

protected:

	Material ()
	: id ( 0 ),//MaterialCount++ ),
	side ( THREE::FrontSide ),
	opacity ( 1 ),
	transparent ( false ),
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
	needsUpdate ( true ) { }

private:

	/*static int& MaterialCount() {
		static int sMaterialCount = 0;
		return sMaterialCount;
	}*/

};

} // namespace three

#endif // THREE_MATERIAL_HPP