#ifndef THREE_MESH_BASIC_MATERIAL_HPP
#define THREE_MESH_BASIC_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class MeshBasicMaterial : public Material {
public:

  typedef std::shared_ptr<MeshBasicMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshBasicMaterial>( parameters );
  }

  virtual enums::MaterialType type() const { return enums::MeshBasicMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    return Material::clone( *this );
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

#endif // THREE_MESH_BASIC_MATERIAL_HPP

