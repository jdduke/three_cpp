#ifndef THREE_VECTOR4_IPP
#define THREE_VECTOR4_IPP

#include <three/math/vector3.hpp>
#include <three/math/vector4.hpp>

namespace three {

  Vector4& Vector4::copy( const Vector3& v ) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.f;
    return *this;
  }

} // namespace three

#endif // THREE_VECTOR4_IPP