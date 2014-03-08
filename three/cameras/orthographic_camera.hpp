#ifndef THREE_ORTHOGRAPHIC_CAMERA_HPP
#define THREE_ORTHOGRAPHIC_CAMERA_HPP

#include <three/common.hpp>

#include <three/cameras/camera.hpp>

namespace three {

class OrthographicCamera : public Camera {
public:

  typedef std::shared_ptr<OrthographicCamera> Ptr;

  static Ptr create( float left, float right, float top, float bottom, float near = 0.1f, float far = 2000 ) {
    return make_shared<OrthographicCamera>( left, right, top, bottom, near, far );
  }

  /////////////////////////////////////////////////////////////////////////

  float left, right;
  float top, bottom;

  /////////////////////////////////////////////////////////////////////////

  void updateProjectionMatrix() {
    projectionMatrix.makeOrthographic( left, right, top, bottom, near, far );
  }

  /////////////////////////////////////////////////////////////////////////

protected:

  OrthographicCamera( float left, float right, float top, float bottom, float near, float far )
    : Camera( near, far ), left( left ), right( right ), top( top ), bottom( bottom ) {

    updateProjectionMatrix();

  }

};

} // namespace three

#endif // THREE_ORTHOGRAPHIC_CAMERA_HPP