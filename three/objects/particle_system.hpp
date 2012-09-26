#ifndef THREE_PARTICLE_SYSTEM_HPP
#define THREE_PARTICLE_SYSTEYM_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>
#include <three/materials/material.hpp>

namespace three {

class ParticleSystem : public Object3D {
public:

  typedef std::shared_ptr<ParticleSystem> Ptr;

  static Ptr create( Geometry::Ptr geometry, Material::Ptr material ) {
    return three::make_shared<ParticleSystem> ( geometry, material );
  }

  /////////////////////////////////////////////////////////////////////////

  virtual THREE::Type type() const { return THREE::ParticleSystem; }

  virtual void visit( Visitor& v ) { v( *this ); }
  virtual void visit( ConstVisitor& v ) const { v( *this ); }

  /////////////////////////////////////////////////////////////////////////

  bool softParticles;

protected:

  ParticleSystem( Geometry::Ptr geometry, Material::Ptr material )
    : Object3D( material, geometry ), softParticles( false ) {

    if ( geometry ) {
      if ( geometry->boundingSphere.radius == 0 ) {
        geometry->computeBoundingSphere();
      }
      boundRadius = geometry->boundingSphere.radius;
    }

    frustumCulled = false;

  }

};

} // namespace three

#endif // THREE_PARTICLE_SYSTEM_HPP