#ifndef THREE_TRIANGLE_H
#define THREE_TRIANGLE_H

#include <three/common.h>

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

  Triangle& set( const Vector3& aIn, const Vector3& bIn, const Vector3& cIn );
  Triangle& setFromPointsAndIndices( const Vector3 points[], size_t i0, size_t i1, size_t i2 );

  Triangle& copy( const Triangle& triangle );

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

  bool equals( const Triangle& triangle ) const;

  Triangle clone() const;

};

} // end namespace

#endif // THREE_TRIANGLE_H