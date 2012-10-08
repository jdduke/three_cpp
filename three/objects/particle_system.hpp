#ifndef THREE_PARTICLE_SYSTEM_HPP
#define THREE_PARTICLE_SYSTEM_HPP

#include <three/common.hpp>

#include <three/visitor.hpp>
#include <three/core/object3d.hpp>
#include <three/materials/material.hpp>

namespace three {

class ParticleSystem : public Object3D {
public:

  typedef std::shared_ptr<ParticleSystem> Ptr;

  static Ptr create( const Geometry::Ptr& geometry, const Material::Ptr& material ) {
    return three::make_shared<ParticleSystem> ( geometry, material );
  }

  /////////////////////////////////////////////////////////////////////////

  THREE_IMPL_OBJECT(ParticleSystem)

  /////////////////////////////////////////////////////////////////////////

protected:

  ParticleSystem( const Geometry::Ptr& geometry, const Material::Ptr& material )
    : Object3D( material, geometry ) {

    if ( geometry ) {
      if ( geometry->boundingSphere.radius <= 0.0001f ) {
        geometry->computeBoundingSphere();
      }
      boundRadius = geometry->boundingSphere.radius;
    }

    frustumCulled = false;

  }

};

} // namespace three

#endif // THREE_PARTICLE_SYSTEM_HPP