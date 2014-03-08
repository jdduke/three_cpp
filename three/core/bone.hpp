#ifndef THREE_BONE_HPP
#define THREE_BONE_HPP

#include <three/math/vector3.hpp>
#include <three/math/quaternion.hpp>

#include <string>

namespace three {

// TODO:
class Bone {
  Bone( std::string name,
        Vector3 position,
        Quaternion quaternion,
        bool useQuaternion = true,
        Vector3 scale = Vector3( 1, 1, 1 ) )
    : name( name ),
      position( position ),
      quaternion( quaternion ),
      useQuaternion( useQuaternion ),
      scale( scale ) { }

  std::string name;
  Vector3 position;
  Quaternion quaternion;
  bool useQuaternion;
  Vector3 scale;

};

} // namespace three

#endif // THREE_BONE_HPP
