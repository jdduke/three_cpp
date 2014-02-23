#ifndef THREE_FRUSTUM_HPP
#define THREE_FRUSTUM_HPP

#include <three/common.hpp>

#include <three/utils/noncopyable.hpp>
#include <three/math/plane.hpp>

#include <array>

namespace three {

  class Frustum : public NonCopyable {
  public:

    Frustum() { }
    Frustum( const Matrix4& m ) { setFromMatrix( m ); }

    THREE_DECL Frustum& set( const Plane& p0, const Plane& p1, const Plane& p2, const Plane& p3, const Plane& p4, const Plane& p5 );
    THREE_DECL Frustum& copy( const Frustum& frustum );

    THREE_DECL Frustum& setFromMatrix( const Matrix4& m );
    THREE_DECL bool intersectsObject( const Object3D& object );
    THREE_DECL bool intersectsSphere( const Sphere& sphere ) const;
    THREE_DECL bool intersectsBox( const Box3& box );
    THREE_OBSOLETE bool contains( const Object3D& point ) const;
    THREE_DECL bool containsPoint( const Vector3& point ) const;
    THREE_DECL Frustum clone();

    std::array<Plane, 6> planes;

  private:
    // @todo lazy init?
    Sphere _sphere;
    Vector3 _p1;
    Vector3 _p2;
  };

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/frustum.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_FRUSTUM_HPP