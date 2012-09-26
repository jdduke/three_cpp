#ifndef THREE_CAMERA_HPP
#define THREE_CAMERA_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>
#include <three/core/matrix4.hpp>

namespace three {

class Camera : public Object3D {
public:

  typedef std::shared_ptr<Camera> Ptr;

  /////////////////////////////////////////////////////////////////////////

  mutable Matrix4 matrixWorldInverse;
  Matrix4 projectionMatrix;
  mutable Matrix4 projectionMatrixInverse;

  float near, far;

  std::array<float, 16> _viewMatrixArray;
  std::array<float, 16> _projectionMatrixArray;

  /////////////////////////////////////////////////////////////////////////

  void lookAt( const Vector3& vector ) {
    matrix.lookAt( position, vector, up );
    if ( rotationAutoUpdate ) {
      rotation = matrix.getEulerRotation( eulerOrder );
    }
  }

  /////////////////////////////////////////////////////////////////////////

protected:

  Camera( float near, float far )
    : Object3D(), near( near ), far( far ) { }

  /////////////////////////////////////////////////////////////////////////

  virtual THREE::Type type() const { return THREE::Camera; }

  virtual void visit( Visitor& v ) { v( *this ); }
  virtual void visit( ConstVisitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_CAMERA_HPP