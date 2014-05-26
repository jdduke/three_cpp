#ifndef THREE_VISITOR_H
#define THREE_VISITOR_H

#include <three/config.h>
#include <three/fwd.h>

namespace three {

class Visitor {
public:
  virtual void operator()( Object3D& ) { }
  virtual void operator()( Bone& ) { }
  virtual void operator()( Camera& ) { }
  virtual void operator()( Light& ) { }
  virtual void operator()( Scene& ) { }
  virtual void operator()( Particle& ) { }
  virtual void operator()( Sprite& ) { }
  virtual void operator()( Mesh& ) { }
  virtual void operator()( Line3& ) { }
  virtual void operator()( LOD& ) { }
  virtual void operator()( Geometry& ) { }
  virtual void operator()( BufferGeometry& ) { }
  virtual ~Visitor() { }
  template < typename T >
  void fallback( T& object ) {
    ( *this )( static_cast<Object3D&>( object ) );
  }
};

class ConstRawPointerVisitor {
public:
  virtual void operator()( const Object3D* ) { }
  virtual void operator()( const Bone* ) { }
  virtual void operator()( const Camera* ) { }
  virtual void operator()( const Light* ) { }
  virtual void operator()( const Scene* ) { }
  virtual void operator()( const Particle* ) { }
  virtual void operator()( const Sprite* ) { }
  virtual void operator()( const Mesh* ) { }
  virtual void operator()( const Line3* ) { }
  virtual void operator()( const LOD* ) { }
  virtual void operator()( const Geometry* ) { }
  virtual void operator()( const BufferGeometry* ) { }
  virtual ~ConstRawPointerVisitor() { }
  template < typename T >
  void fallback( T& object ) {
    ( *this )( std::static_pointer_cast<Object3D>( object ) );
  }
};

class ConstVisitor {
public:
  virtual void operator()( const Object3D& ) { }
  virtual void operator()( const Bone& ) { }
  virtual void operator()( const Camera& ) { }
  virtual void operator()( const Light& ) { }
  virtual void operator()( const Scene& ) { }
  virtual void operator()( const Particle& ) { }
  virtual void operator()( const Sprite& ) { }
  virtual void operator()( const Mesh& ) { }
  virtual void operator()( const Line3& ) { }
  virtual void operator()( const LOD& ) { }
  virtual void operator()( const Geometry& ) { }
  virtual void operator()( const BufferGeometry& ) { }
  virtual ~ConstVisitor() { }
  template < typename T >
  void fallback( const T& object ) {
    ( *this )( static_cast<const Object3D&>( object ) );
  }
};

} // namespace three

#endif // THREE_VISITOR_H
