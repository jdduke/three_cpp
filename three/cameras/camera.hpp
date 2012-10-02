#ifndef THREE_CAMERA_HPP
#define THREE_CAMERA_HPP

#include <three/common.hpp>

#include <three/visitor.hpp>
#include <three/core/object3d.hpp>
#include <three/core/matrix4.hpp>

namespace three {

class Camera : public Object3D {
public:

  typedef std::shared_ptr<Camera> Ptr;

  static Ptr create() {
    return three::make_shared<Camera>();
  }

  /////////////////////////////////////////////////////////////////////////

  mutable Matrix4 matrixWorldInverse;
  Matrix4 projectionMatrix;
  mutable Matrix4 projectionMatrixInverse;

  float near, far;

  std::array<float, 16> _viewMatrixArray;
  std::array<float, 16> _projectionMatrixArray;

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL void lookAt( const Vector3& vector ) {
    matrix.lookAt( position, vector, up );
    if ( rotationAutoUpdate ) {
      rotation = matrix.getEulerRotation( eulerOrder );
    }
  }

  /////////////////////////////////////////////////////////////////////////

protected:

  Camera( float near = .1f, float far = 2000.f )
    : Object3D(), near( near ), far( far ) { }

  /////////////////////////////////////////////////////////////////////////

  virtual THREE::Type type() const { return THREE::Camera; }

  virtual void visit( Visitor& v ) { v( *this ); }
  virtual void visit( ConstVisitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_CAMERA_HPP