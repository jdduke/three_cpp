#ifndef THREE_PLANE_HPP
#define THREE_PLANE_HPP

#include <three/math/line3.hpp>

namespace three {

  class Plane {
  public: 

    Vector3 normal = Vector3( 1.f, 0.f, 0.f );
    float constant = 0.f;

    THREE_DECL Plane() : normal(Vector3( 1.f, 0.f, 0.f)), constant(0.f) {}
    THREE_DECL Plane( Vector3 v, float constant) : normal(v), constant(constant) {}

    THREE_DECL Plane&   set( const Vector3& normalIn, float constantIn );
    THREE_DECL Plane&   setComponents( float x, float y, float z, float w );
    THREE_DECL Plane&   setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point );
    THREE_DECL Plane&   setFromCoplanarPoints( const Vector3& a, const Vector3& b, const Vector3& c);
    THREE_DECL Plane&   copy( const Plane& plane );
    THREE_DECL Plane&   normalize();
    THREE_DECL Plane&   negate();
    THREE_DECL float    distanceToPoint( const Vector3& point ) const;
    THREE_DECL float    distanceToSphere( const Sphere& sphere ) const;
    THREE_DECL Vector3  projectPoint( const Vector3& point ) const;
    THREE_DECL Vector3  projectPoint( const Vector3& point, const Vector3& target ) const;
    THREE_DECL Vector3  orthoPoint( const Vector3& point) const;
    THREE_DECL Vector3  orthoPoint( const Vector3& point, const Vector3& target ) const;
    THREE_DECL bool     isIntersectionLine( const Line3& line ) const;
    THREE_DECL Vector3* intersectLine( const Line3& line) const;
    THREE_DECL Vector3* intersectLine( const Line3& line, const Vector3& target) const;
    THREE_DECL Vector3  coplanarPoint() const;
    THREE_DECL Vector3  coplanarPoint( const Vector3& target ) const;
    THREE_DECL Plane&   applyMatrix4( const Matrix4& matrix );
    THREE_DECL Plane&   applyMatrix4( const Matrix4& matrix, const Matrix3& normalMatrix);
    THREE_DECL Plane&   translate( const Vector3& offset );
    THREE_DECL bool     equals( const Plane& plane ) const;
    THREE_DECL Plane    clone();

  };

}

#endif //THREE_PLANE_HPP