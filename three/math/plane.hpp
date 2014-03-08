#ifndef THREE_PLANE_HPP
#define THREE_PLANE_HPP

#include <three/math/line3.hpp>

namespace three {

  class Plane {
  public: 

    Vector3 normal;
    float constant;

    Plane() : normal(Vector3( 1.f, 0.f, 0.f)), constant(0.f) {}
    Plane( Vector3 v, float constant) : normal(v), constant(constant) {}

    Plane& set( const Vector3& normalIn, float constantIn );
    Plane& setComponents( float x, float y, float z, float w );
    Plane& setFromNormalAndCoplanarPoint( const Vector3& normal, const Vector3& point );
    Plane& setFromCoplanarPoints( const Vector3& a, const Vector3& b, const Vector3& c);
    Plane& copy( const Plane& plane );
    Plane& normalize();
    Plane& negate();
    float distanceToPoint( const Vector3& point ) const;
    float distanceToSphere( const Sphere& sphere ) const;
    Vector3 projectPoint( const Vector3& point ) const;
    Vector3& projectPoint( const Vector3& point, Vector3& target );
    Vector3 orthoPoint( const Vector3& point) const;
    Vector3& orthoPoint( const Vector3& point, Vector3& target );
    bool isIntersectionLine( const Line3& line ) const;
    Vector3 intersectLine( const Line3& line) const;
    Vector3& intersectLine( const Line3& line, Vector3& target);
    Vector3 coplanarPoint() const;
    Vector3 coplanarPoint( const Vector3& target ) const;
    Plane& applyMatrix4( const Matrix4& matrix );
    Plane& applyMatrix4( const Matrix4& matrix, const Matrix3& normalMatrix);
    Plane& translate( const Vector3& offset );
    bool equals( const Plane& plane ) const;
    Plane clone();

  };

}

#endif //THREE_PLANE_HPP