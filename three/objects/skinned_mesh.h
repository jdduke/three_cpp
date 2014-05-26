#ifndef THREE_SKINNED_MESH_H
#define THREE_SKINNED_MESH_H

#include <three/objects/mesh.h>

namespace three {

class SkinnedMesh : public Mesh {

public:

  THREE_IMPL_OBJECT(SkinnedMesh);

  static Ptr create( const Geometry::Ptr& geometry, const Material::Ptr& material, bool useVertexTexture = true ) {
    return three::make_shared<SkinnedMesh>( geometry, material, useVertexTexture );
  }

  bool useVertexTexture;
  Matrix4 offsetMatrix;
  std::vector<Matrix4> boneInverses;


  // Bone& addBone( Bone& bone = Bone() ) {
  //   /*if ( !bone ) {
  //     bone.reset( Bone::create( this ) );
  //   }*/

  //   bones.push_back( bone );
  //   return bone;
  // }

  // void updateMatrixWorld ( bool force ) {

  //   if ( matrixAutoUpdate ) updateMatrix();

  //   // update matrixWorld
  //   if ( matrixWorldNeedsUpdate || force ) {
  //     if ( parent ) {
  //       matrixWorld.multiply( parent.matrixWorld, matrix );
  //     } else {
  //       matrixWorld.copy( this.matrix );
  //     }
  //     matrixWorldNeedsUpdate = false;
  //     force = true;
  //   }

  //   // update children
  //   for ( size_t i = 0, l = children.size(); i < l; i ++ ) {
  //     auto& child = children[ i ];
  //     if ( child->type() == THREE.Bone ) {
  //       child->update( identityMatrix, false );
  //     } else {
  //       child->updateMatrixWorld( true );
  //     }
  //   }

  //   // make a snapshot of the bones' rest position
  //   if ( boneInverses.size() == 0 ) {
  //     for ( const auto& bone : bones ) {
  //       Matrix4 inverse;
  //       inverse.getInverse( bone->skinMatrix );
  //       boneInverses.push_back( inverse );
  //     }
  //   }

  //   // flatten bone matrices to array
  //   for ( size_t b = 0, bl = bones.size(); b < bl; b ++ ) {
  //     // compute the offset between the current and the original transform;

  //     //TODO: we could get rid of this multiplication step if the skinMatrix
  //     // was already representing the offset; however, this requires some
  //     // major changes to the animation system
  //     offsetMatrix.multiply( bones[ b ].skinMatrix, boneInverses[ b ] );
  //     //offsetMatrix.flattenToArrayOffset( boneMatrices, b * 16 );
  //     boneMatrices[ b ] = offsetMatrix;
  //   }

  //   if ( this.useVertexTexture ) {
  //     boneTexture.needsUpdate = true;
  //   }

  // }

  // /*
  //  * Pose
  //  */

  // void pose () {

  //   this.updateMatrixWorld( true );
  //   for ( size_t i = 0; i < geometry.skinIndices.size(); i ++ ) {
  //     // normalize weights
  //     const auto& sw = geometry->skinWeights[ i ];
  //     const auto scale = 1.0.f / sw.lengthManhattan();
  //     if ( scale != Math::Inf() ) {
  //       sw.multiplyScalar( scale );
  //     } else {
  //       sw.set( 1 ); // this will be normalized by the shader anyway
  //     }
  //   }
  // }

protected:

  // init bones
  SkinnedMesh( const Geometry::Ptr& geometry, const Material::Ptr& material, bool useVertexTexture )
    : Mesh( geometry, material ), useVertexTexture( useVertexTexture ) {

    // Matrix4 identityMatrix;

    // if ( geometry && geometry->bones.size() > 0 ) {

    //   for ( auto& gbone : geometry->bones ) {

    //     const auto& p = gbone.pos;
    //     const auto& q = gbone.rotq;
    //     const auto& s = gbone.scl;

    //     auto& bone = addBone();

    //     bone.name = gbone.name;
    //     bone.position.set( p[0], p[1], p[2] );
    //     bone.quaternion.set( q[0], q[1], q[2], q[3] );
    //     bone.useQuaternion = true;

    //     bone.scale.set( s[0], s[1], s[2] );
    //     //bone.scale.set( 1, 1, 1 );

    //   }

    //   for ( size_t b = 0; b < bones.size(); b ++ ) {

    //     auto& gbone = geometry.bones[ b ];
    //     auto& bone = bones[ b ];

    //     if ( gbone.parent == -1 ) {

    //       add( bone );

    //     } else {

    //       bones[ gbone.parent ].add( bone );

    //     }

    //   }

    //   //

    //   const auto nBones = bones.size();

    //   if ( this.useVertexTexture ) {

    //     // layout (1 matrix = 4 pixels)
    //     //  RGBA RGBA RGBA RGBA (=> column1, column2, column3, column4)
    //     //  with  8x8  pixel texture max   16 bones  (8 * 8  / 4)
    //     //     16x16 pixel texture max   64 bones (16 * 16 / 4)
    //     //     32x32 pixel texture max  256 bones (32 * 32 / 4)
    //     //     64x64 pixel texture max 1024 bones (64 * 64 / 4)

    //     var size;

    //     if ( nBones > 256 )
    //       size = 64;
    //     else if ( nBones > 64 )
    //       size = 32;
    //     else if ( nBones > 16 )
    //       size = 16;
    //     else
    //       size = 8;

    //     this.boneTextureWidth = size;
    //     this.boneTextureHeight = size;

    //     this.boneMatrices = new Float32Array( this.boneTextureWidth * this.boneTextureHeight * 4 ); // 4 floats per RGBA pixel
    //     this.boneTexture = new THREE.DataTexture( this.boneMatrices, this.boneTextureWidth, this.boneTextureHeight, THREE.RGBAFormat, THREE.FloatType );
    //     this.boneTexture.minFilter = THREE.NearestFilter;
    //     this.boneTexture.magFilter = THREE.NearestFilter;
    //     this.boneTexture.generateMipmaps = false;
    //     this.boneTexture.flipY = false;

    //   } else {

    //     this.boneMatrices = new Float32Array( 16 * nBones );

    //   }

    //   this.pose();

    // }

  }

  // TODO "SkinnedMesh::_clone"

}; // SkinnedMesh

} // namespace three

#endif // THREE_SKINNED_MESH_H