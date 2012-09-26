#ifndef THREE_MESH_LAMBERT_MATERIAL_HPP
#define THREE_MESH_LAMBERT_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class MeshLambertMaterial : public Material {
public:

  typedef std::shared_ptr<MeshLambertMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshLambertMaterial>( parameters );
  }

  virtual THREE::MaterialType type() const { return THREE::MeshLambertMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    return Material::clone( *this );
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

#endif // THREE_MESH_LAMBERT_MATERIAL_HPP

