#ifndef THREE_MESH_LAMBERT_MATERIAL_H
#define THREE_MESH_LAMBERT_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshLambertMaterial : public Material {
public:

  THREE_IMPL_MATERIAL(MeshLambertMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshLambertMaterial>( parameters );
  }

protected:

  MeshLambertMaterial( const Parameters& parameters )
    : Material() {
    fog = true;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 22> sKeys = {
      "color",
      "ambient",
      "emissive",
      "wrapAround",
      "wrapRGB",
      "map",
      "lightMap",
      "specularMap",
      "envMap"
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
      "morphTargets",
      "morphNormals"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MESH_LAMBERT_MATERIAL_H

