#ifndef THREE_MESH_DEPTH_MATERIAL_H
#define THREE_MESH_DEPTH_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshDepthMaterial : public Material {
public:

  THREE_IMPL_MATERIAL(MeshDepthMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshDepthMaterial>( parameters );
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

#endif // THREE_MESH_DEPTH_MATERIAL_H

