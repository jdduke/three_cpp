#ifndef THREE_LINE_BASIC_MATERIAL_H
#define THREE_LINE_BASIC_MATERIAL_H

#include <three/common.h>
#include <three/core/interfaces.h>
#include <three/materials/material.h>

namespace three {

class LineBasicMaterial : public Material {

public:

  THREE_IMPL_MATERIAL(LineBasicMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<LineBasicMaterial>( parameters );
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

