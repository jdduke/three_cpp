#ifndef THREE_MESH_BASIC_MATERIAL_H
#define THREE_MESH_BASIC_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshBasicMaterial : public Material {

public:

  THREE_IMPL_MATERIAL(MeshBasicMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshBasicMaterial>( parameters );
  }

protected:

  MeshBasicMaterial( const Parameters& parameters )
    : Material() {
    //fog = true;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {

    static std::array<std::string, 17> sKeys = {
      "color",
      "map",
      "lightMap",
      "specularMap",
      "envMap",
      "combine",
      "reflectivity",
      "refractionRatio",
      "fog",
      "shading",
      "wireframe",
      "wireframeLinewidth",
      "wireframeLinecap",
      "wireframeLinejoin",
      "vertexColors",
      "skinning",
      "morphTargets"
    };

    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );

    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MESH_BASIC_MATERIAL_H

