#ifndef THREE_MESH_NORMAL_MATERIAL_H
#define THREE_MESH_NORMAL_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshNormalMaterial : public Material {
public:

  THREE_IMPL_MATERIAL(MeshNormalMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshNormalMaterial>( parameters );
  }

protected:

  MeshNormalMaterial( const Parameters& parameters )
    : Material() {
    shading = THREE::FlatShading;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 3> sKeys = {
      "shading",
      "wireframe",
      "wireframeLinewidth"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MESH_NORMAL_MATERIAL_H

