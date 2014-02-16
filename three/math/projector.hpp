#ifndef THREE_PROJECTOR_HPP
#define THREE_PROJECTOR_HPP

#include <three/common.hpp>

#include <three/math/ray.hpp>
#include <three/math/vector3.hpp>

#include <three/renderers/renderables/renderable_object.hpp>

#include <three/utils/noncopyable.hpp>

#include <vector>

namespace three {

class Projector : public NonCopyable {
public:

  THREE_DECL Projector();

  THREE_DECL Vector3& projectVector( Vector3& vector, const Camera& camera );
  THREE_DECL Vector3& unprojectVector( Vector3& vector, const Camera& camera );

  THREE_DECL Ray pickingRay( Vector3 vector, const Camera& camera );

  struct RenderData {
    // TODO: Fill these with pointers AFTER creating with the pool
    std::vector<RenderableObject> objects;
    std::vector<RenderableObject> sprites;
    std::vector<Object3D*>        lights;
    std::vector<Renderable*>      elements;
  };

  THREE_DECL RenderData& projectGraph( Object3D& root, bool sort );
  THREE_DECL RenderData& projectScene( Scene& scene, Camera& camera, bool sort );

  struct Impl;

protected:

  std::unique_ptr<Impl> impl;

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/projector.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_PROJECTOR_HPP