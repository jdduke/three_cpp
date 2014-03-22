#ifndef THREE_PARTICLE_BASIC_MATERIAL_H
#define THREE_PARTICLE_BASIC_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class ParticleBasicMaterial : public Material {
public:

  typedef std::shared_ptr<ParticleBasicMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<ParticleBasicMaterial>( parameters );
  }

  virtual enums::MaterialType type() const {
    return enums::ParticleBasicMaterial;
  }

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  ParticleBasicMaterial( const Parameters& parameters )
    : Material() {
    fog = true;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 8> sKeys = {
      "color",
      "map",
      "size",
      "sizeAttenuation",
      "opacity",
      "depthTest",
      "vertexColors",
      "fog"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MATERIAL_H

