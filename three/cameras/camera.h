#ifndef THREE_CAMERA_H
#define THREE_CAMERA_H

#include <three/common.h>

#include <three/visitor.h>
#include <three/core/object3d.h>
#include <three/math/matrix4.h>

namespace three {

class Camera : public Object3D {

public:

  typedef std::shared_ptr<Camera> Ptr;

  static Ptr create( float near = .1f, float far = 2000.f ) {

    return three::make_shared<Camera>( near, far );

  }

  mutable Matrix4 matrixWorldInverse;

  Matrix4 projectionMatrix;

  float near, far;

  void lookAt( const Vector3& vector ) {

    auto m1 = Matrix4();

    m1.lookAt( position, vector, up );

    quaternion.setFromRotationMatrix( m1 );

  }

  THREE_REVIEW("Correct cloning here?")
  Ptr clone() const {

    Ptr camera =  three::make_shared<Camera>( near, far );

    camera->matrixWorldInverse = matrixWorldInverse;
    camera->projectionMatrix = projectionMatrix;
    camera->_viewMatrixArray = _viewMatrixArray;
    camera->_projectionMatrixArray = _projectionMatrixArray;

    return camera;

  }

protected:

  Camera( float near = .1f, float far = 2000.f )
    : Object3D(), near( near ), far( far ) { }

  THREE_IMPL_OBJECT(Camera)

private:
  
  friend class GLRenderer;

  std::array<float, 16> _viewMatrixArray;
  
  std::array<float, 16> _projectionMatrixArray;

};

} // namespace three

#endif // THREE_CAMERA_H