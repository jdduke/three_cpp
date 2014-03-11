#ifndef THREE_FRUSTUM_HPP
#define THREE_FRUSTUM_HPP

#include <three/common.hpp>

#include <three/utils/noncopyable.hpp>
#include <three/math/plane.hpp>

#include <array>

namespace three {

  class Frustum : public NonCopyable {
  public:

    Frustum() {}

    Frustum( const Matrix4& m ) { setFromMatrix( m ); }

    std::array<Plane, 6> planes;

    Frustum& set( const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5 );

    Frustum& copy( const Frustum& frustum );

    Frustum& setFromMatrix( const Matrix4& m );

    bool intersectsObject( const Object3D& object );

    bool intersectsSphere( const Sphere& sphere ) const;

    bool intersectsBox( const Box3& box );

    THREE_OBSOLETE bool contains( const Object3D& point ) const;

    bool containsPoint( const Vector3& point ) const;

    Frustum clone();


  private:
    Sphere _sphere;
    Vector3 _p1;
    Vector3 _p2;
  };

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/frustum.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FRUSTUM_HPP