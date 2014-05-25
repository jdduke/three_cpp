#include <three/objects/mesh.h>

#include <three/console.h>

namespace three {

Mesh::Ptr Mesh::create( const Geometry::Ptr& geometry, const Material::Ptr& material ) {
  return three::make_shared<Mesh>( geometry, material );
}

int Mesh::getMorphTargetIndexByName( const std::string& name ) {
  auto morphTargetIndexIter = morphTargetDictionary.find( name );
  if ( morphTargetIndexIter != morphTargetDictionary.end() )
    return morphTargetIndexIter->second;

  console().log( "Three.Mesh.getMorphTargetIndexByName: morph target does not exist, returning 0" );
  return 0;
}

Mesh::Mesh( const Geometry::Ptr& geometry, const Material::Ptr& material )
  : Object3D( material, geometry ),
  boundRadius( 0 ), morphTargetBase( -1 ) {

  if ( geometry ) {

    if ( !geometry->boundingSphere ) {
      geometry->computeBoundingSphere();
    }

    boundRadius = geometry->boundingSphere->radius;

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

} // namespace three
