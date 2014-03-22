#ifndef THREE_SCENE_IPP
#define THREE_SCENE_IPP

#include <three/scenes/scene.h>

#include <three/visitor.h>

#include <three/objects/particle.h>
#include <three/objects/line.h>
#include <three/lights/light.h>

#include <three/utils/template.h>

namespace three {

Scene::Scene()
  : Object3D(),
    overrideMaterial( nullptr ),
    autoUpdate( true ),
    matrixAutoUpdate( false ) { }

Scene::~Scene() { }

void Scene::visit( Visitor& v )            {
  v( *this );
}
void Scene::visit( ConstVisitor& v ) const {
  v( *this );
}

/////////////////////////////////////////////////////////////////////////

namespace detail {

struct FallbackVisitor : public Visitor {
  //TODO: virtual void operator()( Bone& o )     { fallback(o); }
  virtual void operator()( Camera& o )   {
    fallback(o);
  }
  virtual void operator()( Light& o )    {
    fallback(o);
  }
  virtual void operator()( Scene& o )    {
    fallback(o);
  }
  virtual void operator()( Particle& o ) {
    fallback(o);
  }
  //TODO: virtual void operator()( Sprite& o )   { fallback(o); }
  virtual void operator()( Mesh& o )     {
    fallback(o);
  }
  virtual void operator()( Line& o )     {
    fallback(o);
  }
};

struct Add : public FallbackVisitor {
  Add( Scene& s, const Object3D::Ptr& o )
    : s( s ), object( o ) { }

  void operator()( Object3D& o ) {
    if ( push_unique( s.__objects, &o ) ) {

      s.__objectsAdded.push_back( object );

      erase( s.__objectsRemoved, object );
    }
  }
  void operator()( Light& l ) {
    push_unique( s.__lights, &l );
    if ( l.target && l.target->parent == nullptr ) {
      s.add( l.target );
    }
  }
  void operator()( Bone& ) { }
  void operator()( Camera& ) { }

  Scene& s;
  const Object3D::Ptr& object;
};

struct Remove : public FallbackVisitor {
  Remove( Scene& s, const Object3D::Ptr& o ) : s( s ), object( o ) { }
  void operator()( Object3D& o ) {
    if ( erase( s.__objects, &o ) ) {
      s.__objectsRemoved.push_back( object );
      erase( s.__objectsAdded, object );
    }
  }

  void operator()( Light& o ) {
    erase( s.__lights, &o );
  }
  void operator()( Camera& o ) { }

  Scene& s;
  const Object3D::Ptr& object;
};

} // namespace detail

void Scene::__addObject( const Object3D::Ptr& object ) {
  if ( !object )
    return;

  detail::Add objectAdd( *this, object );
  object->visit( objectAdd );

  for ( auto& child : object->children ) {
    __addObject( child );
  }
}

void Scene::__removeObject( const Object3D::Ptr& object ) {
  if ( !object )
    return;

  detail::Remove objectRemove( *this, object );
  object->visit( objectRemove );

  for ( auto& child : object->children ) {
    __removeObject( child );
  }
}

} // namespace three

#endif // THREE_SCENE_IPP