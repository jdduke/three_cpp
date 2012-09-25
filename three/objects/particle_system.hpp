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
		//return make_shared<Line>( geometry, material, lineType );
		return Ptr( new ParticleSystem( geometry, material ) );
	}

	/////////////////////////////////////////////////////////////////////////

	virtual THREE::Type type() const { return THREE::Particle; }

	virtual void visit( Visitor& v ) { v( *this ); }
	virtual void visit( ConstVisitor& v ) const { v( *this ); }

	/////////////////////////////////////////////////////////////////////////

	Geometry::Ptr geometry;
	Material::Ptr material;

	bool softParticles;

protected:

	ParticleSystem ( Geometry::Ptr geometry, Material::Ptr material )
	 : Object3D(), geometry ( geometry ), material ( material ), softParticles ( false ) {

	 	if ( geometry ) {

	 		if ( geometry->boundingSphere.radius == 0 ) {
	 			geometry->computeBoundingSphere();
	 		}

	 		boundRadius = geometry->boundingSphere.radius;
	 	}

	 }

};

} // namespace three

#endif // THREE_PARTICLE_SYSTEM_HPP