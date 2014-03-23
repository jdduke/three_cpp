#ifndef THREE_MESH_FACE_MATERIAL_H
#define THREE_MESH_FACE_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshFaceMaterial : public IMaterial {
public:

  typedef std::shared_ptr<MeshFaceMaterial> Ptr;

  static Ptr create( const std::vector<IMaterial>& materials ) {
    return three::make_shared<MeshFaceMaterial>( materials );
  }

  virtual enums::MaterialType type() const {
    return enums::MeshFaceMaterial;
  }

  std::vector<IMaterial> materials;

  THREE_TODO("Clone implementation")
  Ptr clone( ) {
    auto clone = *this;
    for( auto& mat : clone.materials) {

    }
  }

protected:

  MeshFaceMaterial( const std::vector<IMaterial>& materialsIn )
    : materials( materialsIn ) {}

};

} // namespace three

#endif // THREE_MATERIAL_H

