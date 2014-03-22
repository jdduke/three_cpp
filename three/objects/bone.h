#ifndef THREE_BONE_HPP
#define THREE_BONE_HPP


#include <three/common.h>
#include <three/utils/memory.h>
#include <three/core/object3d.h>
#include <three/objects/skinned_mesh.h>
#include <memory>

namespace three {

class Bone  : Object3d {

  typedef std::shared_ptr<Bone> Ptr;

  static Ptr create( const SkinnedMesh::Ptr& belongsToSkin ) {
  	return make_shared<Bone>(belongsToSkin)
  }

  SkinnedMesh::Ptr skin;
  Matrix4 skinMatrix;

  THREE_IMPL_OBJECT(Bone)

  void update( const Matrix4::Ptr& parentSkinMatrix, bool forceUpdate = false) {
  	
  	// update local

  	if ( matrixAutoUpdate ) {

  		forceUpdate |= updateMatrix();

  	}

  	// update skin matrix

  	if ( forceUpdate || matrixWorldNeedsUpdate ) {

  		if( parentSkinMatrix ) {

  			skinMatrix.multiplyMatrices( parentSkinMatrix, matrix );

  		} else {

  			skinMatrix.copy( matrix );

  		}

  		matrixWorldNeedsUpdate = false;
  		this->forceUpdate = true;

  	}

  	// update children

  	size_t i, l = children.size();

  	for ( auto& child : children ) {

  		child.update( skinMatrix, forceUpdate );

  	}
    
  }

protected:

  Bone( const SkinnedMesh::Ptr& belongsToSkin ) 
    : skin ( belongsToSkin ), skinMatrix( Matrix4() ) {}
	
};

}; // end namespace

#endif // THREE_BONE_HPP
