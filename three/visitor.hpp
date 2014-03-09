#ifndef THREE_VISITOR_HPP
#define THREE_VISITOR_HPP

#include <three/config.hpp>
#include <three/fwd.hpp>

namespace three {

class Visitor {
public:
  virtual void operator()( Object3D& ) { }
  virtual void operator()( Bone& )     { }
  virtual void operator()( Camera& )   { }
  virtual void operator()( Light& )    { }
  virtual void operator()( Scene& )    { }
  virtual void operator()( Particle& ) { }
  virtual void operator()( Sprite& )   { }
  virtual void operator()( Mesh& )     { }
  virtual void operator()( Line3& )     { }
  virtual ~Visitor() { }
  template < typename T >
  void fallback( T& object ) { ( *this )( static_cast<Object3D&>( object ) ); }
};

class ConstVisitor {
public:
  virtual void operator()( const Object3D& ) { }
  virtual void operator()( const Bone& )     { }
  virtual void operator()( const Camera& )   { }
  virtual void operator()( const Light& )    { }
  virtual void operator()( const Scene& )    { }
  virtual void operator()( const Particle& ) { }
  virtual void operator()( const Sprite& )   { }
  virtual void operator()( const Mesh& )     { }
  virtual void operator()( const Line3& )     { }
  virtual ~ConstVisitor() { }
  template < typename T >
  void fallback( const T& object ) { ( *this )( static_cast<const Object3D&>( object ) ); }
};

} // namespace three

#endif // THREE_VISITOR_HPP
