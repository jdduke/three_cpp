#ifndef THREE_RAYCASTER_H
#define THREE_RAYCASTER_H

#include <three/utils/noncopyable.h>

#include <three/math/math.h>
#include <three/math/vector3.h>
#include <three/math/ray.h>
#include <three/core/object3d.h>

namespace three {

struct Intersect;

typedef std::vector<Intersect> Intersects;

struct DescSort;

class THREE_DECL Raycaster : public NonCopyable {

public:

  static float precision;
  static float linePrecision;

  struct Impl;

  Raycaster( Vector3& origin, Vector3& direction, float near = 0.f, float far = Math::INF() );

  Ray ray;

  float near;
  float far;

  Raycaster& set( const Vector3& origin, const Vector3& direction );

  Intersects intersectObject( const Object3D::Ptr& object, bool recursive = false );
  Intersects intersectObjects( const std::vector<Object3D::Ptr>& objects, bool recursive = false );

protected:

  std::unique_ptr<Impl> impl;

private:

  void _intersectDescendants( const Object3D::Ptr& object, Intersects& intersects );

  void _intersectObject( const Object3D::Ptr& object, Intersects& intersects );

};


} // end namespace

#endif // THREE_RAYCASTER_H