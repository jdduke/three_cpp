#ifndef THREE_TRIANGLE_H
#define THREE_TRIANGLE_H

#include <three/common.h>

#include <three/math/math.h>
#include <three/math/plane.h>

namespace three {

class THREE_DECL Triangle {

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

  template <typename Points>
  inline Triangle& setFromPointsAndIndices( const Points& points, size_t i0, size_t i1, size_t i2 ) {

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

  float area() const;

  Vector3 midpoint() const;
  Vector3& midpoint( Vector3& target ) const;

  Vector3 normal() const;
  Vector3& normal( Vector3& target ) const;

  Plane plane() const;
  Plane& plane( Plane& target ) const;

  Vector3 barycoordFromPoint( const Vector3& point ) const;
  Vector3& barycoordFromPoint( const Vector3& point, Vector3& target ) const;

  bool containsPoint( const Vector3& point ) const;

  inline bool equals( const Triangle& triangle ) const {

    return triangle.a.equals( a ) && triangle.b.equals( b ) && triangle.c.equals( c );

  }

  inline Triangle clone() const {

    return *this;

  }

};

} // end namespace

#endif // THREE_TRIANGLE_H