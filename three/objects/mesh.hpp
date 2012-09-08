#ifndef THREE_MESH_HPP
#define THREE_MESH_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>

#include <vector>
#include <unordered_map>
#include <memory>

namespace three {

class Mesh : public Object3D {
public:

	typedef std::shared_ptr<Mesh> Ptr;

	static Ptr create( Geometry::Ptr& geometry, Material::Ptr& material ) {
		return std::make_shared<Mesh>( geometry, material );
	}

	/////////////////////////////////////////////////////////////////////////

	Geometry::Ptr geometry;
	Material::Ptr material;

	float boundRadius;
	int morphTargetBase;

	std::vector<int> morphTargetForcedOrder;
	std::vector<int> morphTargetInfluences;
	std::unordered_map<std::string, int> morphTargetDictionary;

	/////////////////////////////////////////////////////////////////////////

	int getMorphTargetIndexByName ( const std::string& name ) {
		auto morphTargetIndexIter = morphTargetDictionary.find( name );
		if ( morphTargetIndexIter != morphTargetDictionary.end() )
			return morphTargetIndexIter->second;

		console().log( "Three.Mesh.getMorphTargetIndexByName: morph target does not exist, returning 0");
		return 0;
	}

protected:

	Mesh ( Geometry::Ptr& geometry, Material::Ptr& material )
	 : Object3D(), geometry ( geometry ), material ( material ),
	 boundRadius ( 0 ), morphTargetBase ( -1 ) {

		if ( geometry ) {

			if ( geometry->boundingSphere.radius == 0) {
				geometry->computeBoundingSphere();
			}

			boundRadius = geometry->boundingSphere.radius;

			// setup morph targets

			if ( geometry->morphTargets.size() > 0 ) {

				morphTargetBase = -1;

				int m = 0;
				for ( const auto& morphTarget : geometry->morphTargets ) {

					morphTargetInfluences.push_back( 0 );
					morphTargetDictionary[ morphTarget.name ] = m++;

				}
			}
		}
	 }

	virtual THREE::Type getType() const { return THREE::Mesh; }

	virtual void visit( Visitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_MESH_HPP