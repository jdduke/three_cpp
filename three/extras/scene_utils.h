#ifndef THREE_SCENE_UTILS_H
#define THREE_SCENE_UTILS_H

#include <three/common.h>
#include <three/core/object3d.h>
#include <three/scenes/scene.h>

#include <functional>

namespace three {

class SceneUtils {
public:

  typedef std::function<void(Object3D&)> Callback;

  static void showHierarchy( Object3D& root, bool visible );

  static void traverseHierarchy( Object3D& root, const Callback& callback );

  static Object3D::Ptr createMultiMaterialObject( const Geometry::Ptr& geometry, const std::vector<Material::Ptr>& materials );

  static void detach( Object3D::Ptr& child, Object3D::Ptr& parent, Scene::Ptr& scene );

  static void attach( Object3D::Ptr& child, Scene::Ptr& scene, Object3D::Ptr& parent );

};

} // namespace three

#endif // THREE_SCENE_UTILS_H



