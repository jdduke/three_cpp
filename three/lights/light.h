#ifndef THREE_LIGHT_H
#define THREE_LIGHT_H

#include <three/common.h>

#include <three/visitor.h>
#include <three/math/color.h>
#include <three/core/object3d.h>

namespace three {

class Light : public Object3D {

public:

  typedef std::shared_ptr<Light> Ptr;

  virtual enums::Type type() const {
    return enums::Light;
  }

  Color color;
  float intensity;
  float distance;

  Object3D::Ptr target;

  bool castShadow, onlyShadow, shadowCascade;

protected:

  explicit Light( int hex, float intensity = 1, float distance = 0 )
    : Object3D(),
      color( hex ),
      intensity( intensity ),
      distance( distance ),
      target( nullptr ),
      castShadow( false ),
      onlyShadow( false ),
      shadowCascade( false ) { }

  virtual void visit( Visitor& v ) {
    v( *this );
  }

  virtual void visit( ConstVisitor& v ) const {
    v( *this );
  }

};

} // namespace three

#endif // THREE_LIGHT_H