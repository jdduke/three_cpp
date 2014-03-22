#ifndef THREE_PARTICLE_H
#define THREE_PARTICLE_H

#include <three/common.h>

#include <three/core/object3d.h>
#include <three/materials/material.h>

namespace three {

class Particle : public Object3D {
public:

  typedef std::shared_ptr<Particle> Ptr;

  static Ptr create( const Material::Ptr& material ) {
    return three::make_shared<Particle>( material );
  }

  THREE_IMPL_OBJECT(Particle)

protected:

  Particle( const Material::Ptr& material )
    : Object3D( material ) { }

};

} // namespace three

#endif // THREE_PARTICLE_H