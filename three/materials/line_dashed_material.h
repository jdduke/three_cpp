#ifndef THREE_LINE_DASHED_MATERIAL_H
#define THREE_LINE_DASHED_MATERIAL_H

#include <three/common.h>
#include <three/core/interfaces.h>
#include <three/materials/material.h>

namespace three {

class LineDashedMaterial : public Material {

public:

  typedef std::shared_ptr<LineDashedMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<LineDashedMaterial>( parameters );
  }

  virtual enums::MaterialType type() const {
    return enums::LineDashedMaterial;
  }

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  LineDashedMaterial( const Parameters& parameters )
    : Material() {
    fog = true;
    setParameters( parameters, defaultKeys() );
  }

  static const ParameterKeys& defaultKeys() {
    static std::array<std::string, 6> sKeys = {
      "color",
      "linewidth",
      "scale",
      "dashSize",
      "gapSize",
      "vertexColors"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_LINE_DASHED_MATERIAL_H

