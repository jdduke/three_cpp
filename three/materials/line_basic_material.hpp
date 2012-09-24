#ifndef THREE_LINE_BASIC_MATERIAL_HPP
#define THREE_LINE_BASIC_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class LineBasicMaterial : public Material {
public:

	typedef std::shared_ptr<LineBasicMaterial> Ptr;

	static Ptr create( const Parameters& parameters = Parameters() ) {
		return three::make_shared<LineBasicMaterial>( parameters );
	}

	/////////////////////////////////////////////////////////////////////////

	Ptr clone ( ) {
		return Material::clone( *this );
	}

protected:

	LineBasicMaterial ( const Parameters& parameters )
	  : Material() {
		fog = true;
		setParameters( parameters, DefaultKeys() );
	}

	static const ParameterKeys& DefaultKeys() {
		static std::array<std::string, 6> sKeys = {
			"color",
			"linewidth",
			"linecap",
			"linejoin",
			"vertexColors",
			"fog"
		};
		static ParameterKeys sKeysSet(sKeys.begin(), sKeys.end());
		return sKeysSet;
	}

};

} // namespace three

#endif // THREE_LINE_BASIC_MATERIAL_HPP

