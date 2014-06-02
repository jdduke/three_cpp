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

  virtual THREE::MaterialType type() const {
    return THREE::MeshFaceMaterial;
  }

  std::vector<Material::Ptr> materials;

protected:

  MeshFaceMaterial() {}

  MeshFaceMaterial( const std::vector<Material::Ptr>& materialsIn )
    : materials( materialsIn ) {}

  virtual void __clone( Material::Ptr& target ) const THREE_OVERRIDE {
    if ( !target )
      target = create( materials );

    Material::__clone( target );
  }

};

} // namespace three

#endif // THREE_MATERIAL_H

