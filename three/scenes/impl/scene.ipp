#ifndef THREE_SCENE_IPP
#define THREE_SCENE_IPP

#include <three/scenes/scene.hpp>

#include <three/utils.hpp>

#include <three/objects/particle.hpp>
#include <three/objects/line.hpp>
#include <three/lights/light.hpp>

namespace three {

Scene::Scene()
  : Object3D(),
    overrideMaterial( nullptr ),
    matrixAutoUpdate( false ) { }

/////////////////////////////////////////////////////////////////////////

namespace detail {

struct Add : public Visitor {
  Add( Scene& s, Object3D::Ptr& o )
    : s( s ), object( o ) { }

  void operator()( Object3D& o ) {
    if ( push_unique( s.__objects, &o ) ) {

      s.__objectsAdded.push_back( object );

      erase( s.__objectsRemoved, object );
    }
  }
  void operator()( Scene& s ) { fallback( s ); }
  void operator()( Light& l ) {
    push_unique( s.__lights, &l );
    if ( l.target && l.target->parent == nullptr ) {
      s.add( l.target );
    }
  }
  void operator()( Bone& ) { }
  void operator()( Camera& ) { }
  void operator()( Particle& p ) { fallback( p ); }
  //void operator() ( Sprite& s)    { fallback(s); }
  void operator()( Mesh& m )      { fallback( m ); }
  void operator()( Line& l )      { fallback( l ); }

  Scene& s;
  Object3D::Ptr& object;
};

struct Remove : public Visitor {
  Remove( Scene& s, Object3D::Ptr& o ) : s( s ), object( o ) { }
  void operator()( Object3D& o ) {
    if ( erase( s.__objects, &o ) ) {
      s.__objectsRemoved.push_back( object );
      erase( s.__objectsAdded, object );
    }
  }

  void operator()( Scene& s ) { fallback( s ); }
  void operator()( Light& l ) { erase( s.__lights, &l ); }
  void operator()( Bone& ) { }
  void operator()( Camera& ) { }

  Scene& s;
  Object3D::Ptr& object;
};

} // namespace detail

void Scene::__addObject( Object3D::Ptr& object ) {
  if ( !object )
    return;

  detail::Add objectAdd( *this, object );
  object->visit( objectAdd );

  for ( auto& child : object->children ) {
    __addObject( child );
  }
}

void Scene::__removeObject( Object3D::Ptr& object ) {
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