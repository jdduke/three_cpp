#ifndef THREE_RAYCASTER_H
#define THREE_RAYCASTER_H

#include <three/utils/noncopyable.h>

#include <three/math/math.h>
#include <three/math/vector3.h>
#include <three/math/ray.h>
#include <three/core/face.h>
#include <three/core/object3d.h>

namespace three {

struct Intersect {

public:

  Intersect( float distanceIn, Vector3& pointIn, Face* faceIn, size_t faceIndexIn, Object3D* objectIn )
    : distance( distanceIn), point( pointIn ), face( faceIn ), faceIndex( faceIndexIn ), object( objectIn ) {}

  float distance;

  Vector3 point;

  Face* face;
  size_t faceIndex;

  Object3D* object;

};

typedef std::vector<Intersect> Intersects;

class Raycaster : public NonCopyable {

public:

  static float precision;
  static float linePrecision;

  struct Impl;

  typedef std::shared_ptr<Raycaster> Ptr;

  Ptr static create( Vector3& origin, Vector3& direction, float near = 0.f, float far = Math::INF() ) {
    return three::make_shared<Raycaster>( origin, direction, near, far );
  }

  Raycaster( Vector3& origin, Vector3& direction, float near = 0.f, float far = Math::INF() );

  Ray ray;

  float near;
  float far;

  Raycaster& set( const Vector3& origin, const Vector3& direction );

  Intersects intersectObject ( const Object3D::Ptr& object, bool recursive = false );

  Intersects intersectObjects ( const std::vector<Object3D::Ptr>& objects, bool recursive = false );

protected:

  std::unique_ptr<Impl> impl;

private:
  void _intersectDescendants( const Object3D::Ptr& object, Intersects& intersects );
  void _intersectObject( const Object3D::Ptr& object, Intersects& intersects );
};

} // end namespace

#endif // THREE_RAYCASTER_H