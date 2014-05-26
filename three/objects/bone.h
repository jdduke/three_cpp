#ifndef THREE_BONE_H
#define THREE_BONE_H

#include <three/common.h>
#include <three/utils/memory.h>
#include <three/core/object3d.h>
#include <three/objects/skinned_mesh.h>
#include <three/math/matrix4.h>
#include <memory>

namespace three {

class THREE_DECL Bone : public Object3D {

  THREE_IMPL_OBJECT(Bone);

  static Ptr create( const SkinnedMesh::Ptr& belongsToSkin, float scale = 1, int parent = -1 ) {
    return three::make_shared<Bone>(belongsToSkin, scale);
  }

  SkinnedMesh::Ptr skin;
  Matrix4 skinMatrix;

  float pos;
  float rotq;
  float scl;

  int parent;

  void update( const Matrix4* parentSkinMatrix, bool forceUpdate = false);

protected:

  Bone( const SkinnedMesh::Ptr& belongsToSkin, float scale = 1, int parent = -1 );

};

}; // end namespace

#endif // THREE_BONE_H
