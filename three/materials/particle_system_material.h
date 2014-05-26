#ifndef THREE_PARTICLE_SYSTEM_MATERIAL_H
#define THREE_PARTICLE_SYSTEM_MATERIAL_H

#include <three/common.h>

#include <three/materials/material.h>

namespace three {

class ParticleSystemMaterial : public Material {
public:

  THREE_IMPL_MATERIAL(ParticleSystemMaterial);

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<ParticleSystemMaterial>( parameters );
  }

protected:

  ParticleSystemMaterial( const Parameters& parameters )
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

#endif // THREE_PARTICLE_SYSTEM_MATERIAL_H

