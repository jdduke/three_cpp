#include <three/objects/bone.h>

namespace three {

  Bone::Bone( const SkinnedMesh::Ptr& belongsToSkin, float scale, int parentIn )
    : skin ( belongsToSkin ), skinMatrix( Matrix4() ), scl( scale ), parent( parentIn ) {}

  void Bone::update( const Matrix4* parentSkinMatrix, bool forceUpdate) {

  	// // update local

  	// if ( matrixAutoUpdate ) {
   //     // uhh? binary or on void?
  	// 	forceUpdate |= updateMatrix();

  	// }

  	// // update skin matrix

  	// if ( forceUpdate || matrixWorldNeedsUpdate ) {

  	// 	if( parentSkinMatrix ) {

  	// 		skinMatrix.multiplyMatrices( parentSkinMatrix, matrix );

  	// 	} else {

  	// 		skinMatrix.copy( matrix );

  	// 	}

  	// 	matrixWorldNeedsUpdate = false;
  	// 	this->forceUpdate = true;

  	// }

  	// // update children

  	// size_t i, l = children.size();

  	// for ( auto& child : children ) {

  	// 	child.update( skinMatrix, forceUpdate );

  	// }

  }


}; // end namespace

