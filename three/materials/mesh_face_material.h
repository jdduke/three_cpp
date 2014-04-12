#ifndef THREE_MESH_FACE_MATERIAL_H
#define THREE_MESH_FACE_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

THREE_REVIEW("Unfortunate bloat here, due to 'required' inheritance of Material")
class MeshFaceMaterial : public Material {

public:

  typedef std::shared_ptr<MeshFaceMaterial> Ptr;

  static Ptr create() {
    return three::make_shared<MeshFaceMaterial>();
  }

  static Ptr create( const std::vector<Material::Ptr>& materials ) {
    return three::make_shared<MeshFaceMaterial>( materials );
  }

  virtual enums::MaterialType type() const {
    return enums::MeshFaceMaterial;
  }

  std::vector<Material::Ptr> materials;

  // TODO "MeshFaceMaterial::clone"


protected:

  MeshFaceMaterial() {}

  MeshFaceMaterial( const std::vector<Material::Ptr>& materialsIn )
    : materials( materialsIn ) {}

};

} // namespace three

#endif // THREE_MATERIAL_H

