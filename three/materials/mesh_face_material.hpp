#ifndef THREE_MESH_FACE_MATERIAL_HPP
#define THREE_MESH_FACE_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class MeshFaceMaterial : public Material {
public:

	typedef std::shared_ptr<MeshFaceMaterial> Ptr;

	static Ptr create( const Parameters& parameters = Parameters() ) {
		return three::make_shared<MeshFaceMaterial>( parameters );
	}

	/////////////////////////////////////////////////////////////////////////

	Ptr clone ( ) {
		return Material::clone( *this );
	}

protected:

	MeshFaceMaterial ( const Parameters& parameters )
	  : Material() {
		setParameters( parameters, DefaultKeys() );
	}

	static const ParameterKeys& DefaultKeys() {
		static ParameterKeys sKeysSet;
		return sKeysSet;
	}

};

} // namespace three

#endif // THREE_MATERIAL_HPP

