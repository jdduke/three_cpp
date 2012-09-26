#ifndef THREE_MESH_DEPTH_MATERIAL_HPP
#define THREE_MESH_DEPTH_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class MeshDepthMaterial : public Material {
public:

  typedef std::shared_ptr<MeshDepthMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshDepthMaterial>( parameters );
  }

  virtual THREE::MaterialType type() const { return THREE::MeshDepthMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  MeshDepthMaterial( const Parameters& parameters )
    : Material() {
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 2> sKeys = {
      "wireframe",
      "wireframeLinewidth"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MESH_DEPTH_MATERIAL_HPP

