#ifndef THREE_MESH_PHONG_MATERIAL_H
#define THREE_MESH_PHONG_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshPhongMaterial : public Material {
public:

  THREE_IMPL_MATERIAL(MeshPhongMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshPhongMaterial>( parameters );
  }

protected:

  MeshPhongMaterial( const Parameters& parameters )
    : Material() {
    fog = true;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 30> sKeys = {
      "color",
      "ambient",
      "emissive",
      "specular",
      "shininess",
      "metal",
      "perPixel",
      "wrapAround",
      "wrapRGB",
      "map",
      "lightMap",
      "bumpMap",
      "bumpScale",
      THREE_REVIEW("Check if normal map is working properly")
      "normalMap",
      THREE_REVIEW("Normal scale in three.js is a vector2. Trivial?")
      "normalScale",
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
      "morphTargets",
      "morphNormals"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MESH_PHONG_MATERIAL_H

