#ifndef THREE_ORTHOGRAPHIC_CAMERA_H
#define THREE_ORTHOGRAPHIC_CAMERA_H

#include <three/common.h>
#include <three/math/math.h>
#include <three/cameras/camera.h>

#include <memory>

namespace three {

class OrthographicCamera : public Camera {

public:

  typedef std::shared_ptr<OrthographicCamera> Ptr;

  static Ptr create( float left, float right, float top, float bottom, float near = 0.1f, float far = 2000 ) {

    return make_shared<OrthographicCamera>( left, right, top, bottom, near, far );

  }

  float left, right;

  float top, bottom;

  void updateProjectionMatrix() {

    projectionMatrix.makeOrthographic( left, right, top, bottom, near, far );

  }

  THREE_REVIEW("Correct cloning here?")
  Ptr clone() {

    Camera::Ptr camera = Camera::clone();
    
    Ptr orthographicCamera = std::static_pointer_cast<OrthographicCamera>(camera);

    orthographicCamera->left = left;
    orthographicCamera->right = right;
    orthographicCamera->top = top;
    orthographicCamera->bottom = bottom;

    return orthographicCamera;

  }

protected:

  OrthographicCamera( float left, float right, float top, float bottom, float near, float far )
    : Camera( near, far ), left( left ), right( right ), top( top ), bottom( bottom ) {

    updateProjectionMatrix();

  }

};

} // namespace three

#endif // THREE_ORTHOGRAPHIC_CAMERA_H