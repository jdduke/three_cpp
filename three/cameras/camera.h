#ifndef THREE_CAMERA_H
#define THREE_CAMERA_H

#include <three/common.h>

#include <three/visitor.h>
#include <three/core/object3d.h>
#include <three/math/matrix4.h>

#if defined(_WIN32)
#undef near
#undef far
#endif

namespace three {

class Camera : public Object3D {

public:

  THREE_IMPL_OBJECT(Camera);

  static Ptr create( float near = .1f, float far = 2000.f ) {

    return three::make_shared<Camera>( near, far );

  }

  mutable Matrix4 matrixWorldInverse;

  Matrix4 projectionMatrix;

  float near, far;

  void lookAt( const Vector3& vector ) {

    auto m1 = Matrix4();

    m1.lookAt( position(), vector, up() );

    quaternion().setFromRotationMatrix( m1 );

  }

protected:

  explicit Camera( float near = .1f, float far = 2000.f )
    : Object3D(), near( near ), far( far ) { }

  virtual Object3D::Ptr __clone( Object3D::Ptr target, bool recursive ) const THREE_OVERRIDE {

    Ptr camera = target ? std::static_pointer_cast<Camera>(target) : create(near, far);

    Object3D::__clone( camera, recursive );

    camera->near = near;
    camera->far = far;
    camera->matrixWorldInverse = matrixWorldInverse;
    camera->projectionMatrix = projectionMatrix;
    camera->_viewMatrixArray = _viewMatrixArray;
    camera->_projectionMatrixArray = _projectionMatrixArray;

    return camera;

  }

private:

  friend class GLRenderer;

  std::array<float, 16> _viewMatrixArray;

  std::array<float, 16> _projectionMatrixArray;

};

} // namespace three

#endif // THREE_CAMERA_H
