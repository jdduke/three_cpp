#ifndef THREE_FRUSTUM_HPP
#define THREE_FRUSTUM_HPP

#include <three/common.hpp>

#include <three/utils/noncopyable.hpp>

#include <three/math/vector4.hpp>

#include <array>

namespace three {

class Frustum : public NonCopyable {
public:

  Frustum() { }
  Frustum( const Matrix4& m ) { setFromMatrix( m ); }

  THREE_DECL void setFromMatrix( const Matrix4& m );
  THREE_DECL bool contains( const Object3D& object ) const;

  std::array<Vector4, 6> planes;
};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/frustum.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FRUSTUM_HPP