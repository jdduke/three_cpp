#ifndef THREE_MESH_FACE_MATERIAL_H
#define THREE_MESH_FACE_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

THREE_REVIEW("Unfortunate bloat here, due to 'required' inheritance of Material")
class MeshFaceMaterial : public Material {

public:

  typedef std::shared_ptr<MeshFaceMaterial> Ptr;

  static Ptr create( const std::vector<Material>& materials ) {
    return three::make_shared<MeshFaceMaterial>( materials );
  }

  virtual enums::MaterialType type() const {
    return enums::MeshFaceMaterial;
  }

  std::vector<Material> materials;

  THREE_TODO("Clone implementation")
  Ptr clone( ) {
    auto clone = *this;
    for( auto& mat : clone.materials) {

    }
  }

protected:

  MeshFaceMaterial( const std::vector<Material>& materialsIn )
    : materials( materialsIn ) {}

};

} // namespace three

#endif // THREE_MATERIAL_H

