#ifndef THREE_CAMERA_HPP
#define THREE_CAMERA_HPP

#include <three/common.hpp>

#include <three/visitor.hpp>
#include <three/core/object3d.hpp>
#include <three/math/matrix4.hpp>

namespace three {

class Camera : public Object3D {

public:

  typedef std::shared_ptr<Camera> Ptr;

  static Ptr create() {

    return three::make_shared<Camera>();

  }

  mutable Matrix4 matrixWorldInverse;

  Matrix4 projectionMatrix;

  float near, far;

  std::array<float, 16> _viewMatrixArray;
  std::array<float, 16> _projectionMatrixArray;

  void lookAt( const Vector3& vector ) {

    auto m1 = Matrix4();

    m1.lookAt( position, vector, up );

    quaternion.setFromRotationMatrix( m1 );

  }

protected:

  Camera( float near = .1f, float far = 2000.f )
    : Object3D(), near( near ), far( far ) { }

  THREE_IMPL_OBJECT(Camera)

};

} // namespace three

#endif // THREE_CAMERA_HPP