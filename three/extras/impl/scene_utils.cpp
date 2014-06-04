#ifndef THREE_SCENE_UTILS_CPP
#define THREE_SCENE_UTILS_CPP

#include <three/objects/mesh.h>
#include <three/extras/scene_utils.h>

namespace three {

 void SceneUtils::showHierarchy( Object3D& root, bool visible ) {
    traverseHierarchy( root, [visible]( Object3D& node ) {
      node.visible = visible;
    } );
  }

  void SceneUtils::traverseHierarchy( Object3D& root, const Callback& callback ) {
    for ( auto& n : root.children ) {
      callback( *n );
      traverseHierarchy( *n, callback );
    }
  }

  Object3D::Ptr SceneUtils::createMultiMaterialObject( const Geometry::Ptr& geometry, const std::vector<Material::Ptr>& materials ) {

    auto group = Object3D::create();

    for ( size_t i = 0; i < materials.size(); i++ ) {
      group->add( Mesh::create( geometry, materials[ i ] ) );
    }

    return group;

  }

  void SceneUtils::detach( Object3D::Ptr& child, Object3D::Ptr& parent, Scene::Ptr& scene ) {

    child->applyMatrix( parent->matrixWorld );
    parent->remove( child );
    scene->add( child );

  }

  void SceneUtils::attach( Object3D::Ptr& child, Scene::Ptr& scene, Object3D::Ptr& parent ) {

    auto matrixWorldInverse = Matrix4();
    matrixWorldInverse.getInverse( parent->matrixWorld );
    child->applyMatrix( matrixWorldInverse );

    scene->remove( child );
    parent->add( child );

  }
  
};

#endif // THREE_SCENE_UTILS_CPP