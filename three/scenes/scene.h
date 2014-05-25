#ifndef THREE_SCENE_H
#define THREE_SCENE_H

#include <three/common.h>

#include <three/core/object3d.h>

#include <three/renderers/renderables/renderable_object.h>

namespace three {

class THREE_DECL Scene : public Object3D {

public:

  typedef std::shared_ptr<Scene> Ptr;

  static Ptr create() {
    return make_shared<Scene>();
  }

  virtual enums::Type type() const {
    return enums::Scene;
  }

public:

  IFog::Ptr fog;
  Material* overrideMaterial;

  bool autoUpdate;
  bool matrixAutoUpdate;

  THREE_REVIEW("Proper cleanup of light pointers?")
  std::vector<Light*>    __lights;

  std::vector<Object3D::Ptr> __objectsAdded;
  std::vector<Object3D::Ptr> __objectsRemoved;

  struct GLObject : RenderableObject {
    explicit GLObject( unsigned int id        = 0,
                       GeometryBuffer* buffer = nullptr,
                       Object3D* object       = nullptr,
                       bool render            = false,
                       Material* opaque       = nullptr,
                       Material* transparent  = nullptr,
                       float z                = 0.f )
      : RenderableObject( object, z ),
        id( id ),
        buffer( buffer ),
        render( render ),
        opaque( opaque ),
        transparent( transparent ) { }
    unsigned int id;
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

  // TODO "Scene::clone"


protected:

  Scene();

  ~Scene();

  virtual void __addObject( const Object3D::Ptr& object );

  virtual void __removeObject( const Object3D::Ptr& object );

  virtual void visit( Visitor& v );

  virtual void visit( ConstVisitor& v ) const;

  virtual void visit( ConstRawPointerVisitor& v ) const;


};

} // namespace three

#endif // THREE_SCENE_H