#ifndef THREE_TRIANGLE_HPP
#define THREE_TRIANGLE_HPP

#include <three/common.h>

#include <three/math/math.h>

namespace three {

class TriangleHelper {

public:

  inline static Vector3 normal( const Vector3& a, const Vector3& b, const Vector3& c) {
    
    auto target = Vector3();
    
    TriangleHelper::normal( a, b, c, target );
    
    return target;

  }

  static Vector3& normal( const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target );

  inline static Vector3 barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c) {

    auto target = Vector3();

    TriangleHelper::barycoordFromPoint( point, a, b, c, target );

    return target;

  }

  static Vector3& barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target );

  inline static bool containsPoint ( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c ) {

    auto v1 = Vector3();
    auto result = TriangleHelper::barycoordFromPoint( point, a, b, c, v1 );

    return ( result.x >= 0 ) && ( result.y >= 0 ) && ( ( result.x + result.y ) <= 1 );

  }

private:

  TriangleHelper();

};

class Triangle {

public:

  Triangle()
    : a( Vector3() ), b( Vector3() ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn )
    : a( aIn ), b( Vector3() ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn, const Vector3& bIn )
    : a( aIn ), b( bIn ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn, const Vector3& bIn, const Vector3& cIn )
    : a( aIn ), b( bIn ), c( cIn ) {} 

  Vector3 a, b, c;

  inline Triangle& set( const Vector3& aIn, const Vector3& bIn, const Vector3& cIn ) {

    a.copy( aIn );
    b.copy( bIn );
    c.copy( cIn );

    return *this;

  }

  inline Triangle& setFromPointsAndIndices ( const std::vector<Vector3>& points, size_t i0, size_t i1, size_t i2 ) {

    a.copy( points[i0] );
    b.copy( points[i1] );
    c.copy( points[i2] );

    return *this;

  }

  inline Triangle& copy( const Triangle& triangle ) {

    a.copy( triangle.a );
    b.copy( triangle.b );
    c.copy( triangle.c );

    return *this;
    
  }

  inline float area() const {

    auto v0 = Vector3();
    auto v1 = Vector3();

    v0.subVectors( c, b );
    v1.subVectors( a, b );

    return v0.cross( v1 ).length() * 0.5f;

  }

  inline Vector3 midpoint() const {

    auto target = Vector3();
    
    midpoint( target );

    return target;

  }

  inline Vector3& midpoint( Vector3& target ) const {

    return target.addVectors( a, b ).add( c ).multiplyScalar( 1 / 3 );

  }

  inline Vector3 normal() const {

    return TriangleHelper::normal( a, b, c );

  }

  inline Vector3& normal( Vector3& target ) const {

    return TriangleHelper::normal( a, b, c, target );

  }

  inline Plane plane() const {

    auto target = Plane();

    plane( target );

    return target;

  }

  inline Plane& plane( Plane& target ) const {

    return target.setFromCoplanarPoints( a, b, c );

  }

  inline Vector3 barycoordFromPoint( const Vector3& point ) const {

    return TriangleHelper::barycoordFromPoint( point, a, b, c );

  }

  inline Vector3& barycoordFromPoint( const Vector3& point, Vector3& target ) const {

    return TriangleHelper::barycoordFromPoint( point, a, b, c, target );

  }

  inline bool containsPoint( const Vector3& point ) const {

    return TriangleHelper::containsPoint( point, a, b, c );

  }

  inline bool equals( const Triangle& triangle ) const {

    return triangle.a.equals( a ) && triangle.b.equals( b ) && triangle.c.equals( c );

  }

  inline Triangle clone() const {

    return *this;

  }

};

} // end namespace

#endif // THREE_TRIANGLE_HPP