#ifndef THREE_LINE_BASIC_MATERIAL_H
#define THREE_LINE_BASIC_MATERIAL_H

#include <three/common.h>
#include <three/core/interfaces.h>
#include <three/materials/material.h>

namespace three {

class LineBasicMaterial : public Material {

public:

  typedef std::shared_ptr<LineBasicMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<LineBasicMaterial>( parameters );
  }

  virtual enums::MaterialType type() const {
    return enums::LineBasicMaterial;
  }

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  LineBasicMaterial( const Parameters& parameters )
    : Material() {
    fog = true;
    setParameters( parameters, defaultKeys() );
  }

  static const ParameterKeys& defaultKeys() {
    static std::array<std::string, 6> sKeys = {
      "color",
      "linewidth",
      "linecap",
      "linejoin",
      "vertexColors",
      "fog"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_LINE_BASIC_MATERIAL_H

