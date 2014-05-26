#ifndef THREE_ORTHOGRAPHIC_CAMERA_H
#define THREE_ORTHOGRAPHIC_CAMERA_H

#include <three/common.h>
#include <three/math/math.h>
#include <three/cameras/camera.h>

#include <memory>

namespace three {

class OrthographicCamera : public Camera {

public:

  THREE_IMPL_OBJECT(OrthographicCamera);

  static Ptr create( float left, float right, float top, float bottom, float near = 0.1f, float far = 2000.f ) {

    return make_shared<OrthographicCamera>( left, right, top, bottom, near, far );

  }

  float left, right;

  float top, bottom;

  void updateProjectionMatrix() {

    projectionMatrix.makeOrthographic( left, right, top, bottom, near, far );

  }

protected:

  OrthographicCamera( float left, float right, float top, float bottom, float near, float far )
    : Camera( near, far ), left( left ), right( right ), top( top ), bottom( bottom ) {

    updateProjectionMatrix();

  }

  virtual void __clone( Object3D::Ptr& cloned, bool recursive ) const THREE_OVERRIDE {

    if ( !cloned ) cloned = create( left, right, top, bottom );

    Camera::__clone( cloned, recursive );

    auto& camera = static_cast<OrthographicCamera&>( *cloned );
    camera.left = left;
    camera.right = right;
    camera.top = top;
    camera.bottom = bottom;

  }

};

} // namespace three

#endif // THREE_ORTHOGRAPHIC_CAMERA_H