#ifndef THREE_MESH_FACE_MATERIAL_H
#define THREE_MESH_FACE_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class MeshFaceMaterial : public Material {
public:

  typedef std::shared_ptr<MeshFaceMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<MeshFaceMaterial>( parameters );
  }

  virtual enums::MaterialType type() const { return enums::MeshFaceMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  MeshFaceMaterial( const Parameters& parameters )
    : Material() {
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static ParameterKeys sKeysSet;
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MATERIAL_H

