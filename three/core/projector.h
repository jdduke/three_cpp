#ifndef THREE_PROJECTOR_H
#define THREE_PROJECTOR_H

#include <three/common.h>

#include <three/math/ray.h>
#include <three/math/vector3.h>
#include <three/renderers/renderables/renderable_object.h>
#include <three/utils/noncopyable.h>

#include <vector>

namespace three {

class Projector : public NonCopyable {

public:

  struct RenderData {
        
    THREE_TODO("JD: Fill these with pointers AFTER creating with the pool")
        
    std::vector<RenderableObject> objects;
    std::vector<RenderableObject> sprites;
    std::vector<Object3D*>        lights;
    std::vector<Renderable*>      elements;
        
  };
    
  struct Impl;
    
  Projector();

  Vector3& projectVector( Vector3& vector, const Camera& camera );

  Vector3& unprojectVector( Vector3& vector, const Camera& camera );

  Ray pickingRay( Vector3 vector, const Camera& camera );

  RenderData& projectGraph( Object3D& root, bool sort );

  RenderData& projectScene( Scene& scene, Camera& camera, bool sortObjects, bool sortElements );

protected:

  std::unique_ptr<Impl> impl;

};

} // namespace three

#endif // THREE_PROJECTOR_H