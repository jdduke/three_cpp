#ifndef THREE_SCENE_HPP
#define THREE_SCENE_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>

#include <three/renderers/renderables/renderable_object.hpp>

namespace three {

class Scene : public Object3D {
public:

  typedef std::shared_ptr<Scene> Ptr;

  static Ptr create() { 

    return make_shared<Scene>(); 

  }

  virtual enums::Type type() const { 

    return enums::Scene; 

  }

  IFog::Ptr fog;
  Material* overrideMaterial;

  bool autoUpdate = true;
  bool matrixAutoUpdate = false;

  std::vector<Light*>    __lights;

  std::vector<Object3D::Ptr> __objectsAdded;
  std::vector<Object3D::Ptr> __objectsRemoved;

  struct GLObject : RenderableObject {
    explicit GLObject( GeometryBuffer* buffer = nullptr,
                       Object3D* object       = nullptr,
                       bool render            = false,
                       Material* opaque       = nullptr,
                       Material* transparent  = nullptr,
                       float z                = 0.f )
      : RenderableObject( object, z ),
        buffer( buffer ),
        render( render ),
        opaque( opaque ),
        transparent( transparent ) { }

    GeometryBuffer* buffer;
    bool render;
    Material* opaque;
    Material* transparent;
  };

  std::vector<GLObject>  __glObjects;
  std::vector<GLObject>  __glObjectsImmediate;
  std::vector<Object3D*> __glSprites;
  std::vector<Object3D*> __glFlares;

  std::vector<Object3D*> __objects;

protected:

  Scene();

  ~Scene();

  virtual void __addObject( const Object3D::Ptr& object );

  virtual void __removeObject( const Object3D::Ptr& object );

  virtual void visit( Visitor& v );

  virtual void visit( ConstVisitor& v ) const;

};

} // namespace three


#if defined(THREE_HEADER_ONLY)
# include <three/scenes/impl/scene.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_SCENE_HPP