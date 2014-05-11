#ifndef THREE_FRUSTUM_H
#define THREE_FRUSTUM_H

#include <three/common.h>

#include <three/utils/noncopyable.h>
#include <three/math/plane.h>

#include <array>

namespace three {

class Frustum : public NonCopyable {
public:

  Frustum() {}

  Frustum( const Matrix4& m ) {
    setFromMatrix( m );
  }

  std::array<Plane, 6> planes;

  Frustum& set( const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5 );

  Frustum& copy( const Frustum& frustum );

  Frustum& setFromMatrix( const Matrix4& m );

  bool intersectsObject( const Object3D& object );

  bool intersectsSphere( const Sphere& sphere ) const;

  bool intersectsBox( const Box3& box );

  bool containsPoint( const Vector3& point ) const;

  // TODO "Frustum::clone"


private:
  Sphere _sphere;
  Vector3 _p1;
  Vector3 _p2;
};

} // namespace three

#endif // THREE_FRUSTUM_H