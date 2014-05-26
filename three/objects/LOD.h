#ifndef THREE_LOD_H
#define THREE_LOD_H

#include <three/common.h>
#include <three/core/object3d.h>
#include <three/cameras/camera.h>
#include <three/objects/LOD.h>
#include <three/utils/memory.h>

namespace three {

struct DistanceObject {
  DistanceObject( float distanceIn, Object3D::Ptr objectIn )
      : distance( distanceIn ), object( objectIn ) {}

  float distance;
  Object3D::Ptr object;
};

class THREE_DECL LOD : public Object3D {

public:

  THREE_IMPL_OBJECT(LOD);

  static Ptr create() {
    return three::make_shared<LOD>();
  }

  std::vector<DistanceObject> objects;

  LOD& addLevel( const Object3D::Ptr& object, float distance = 0 );

  Object3D::Ptr getObjectForDistance( float distance ) const;

  void update( const Camera::Ptr& camera ) const;

protected:

  LOD() {}

  // TODO "LOD::__clone"

};

}

#endif // THREE_LOD_H