#ifndef THREE_TRIANGLE_HPP
#define THREE_TRIANGLE_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>

namespace three {

class TriangleHelper {

public:

  static Vector3 normal( const Vector3& a, const Vector3& b, const Vector3& c);

  static Vector3& normal( const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target );

  static Vector3 barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c);

  static Vector3& barycoordFromPoint( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, Vector3& target );

  static bool containsPoint ( const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c );

private:

  TriangleHelper();

};

class Triangle {

public:

  Vector3 a, b, c;

  Triangle()
    : a( Vector3() ), b( Vector3() ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn )
    : a( aIn ), b( Vector3() ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn, const Vector3& bIn )
    : a( aIn ), b( bIn ), c( Vector3() ) {} 

  Triangle( const Vector3& aIn, const Vector3& bIn, const Vector3& cIn )
    : a( aIn ), b( bIn ), c( cIn ) {} 

  Triangle& set( const Vector3& a, const Vector3& b, const Vector3& c );

  Triangle& setFromPointsAndIndices( const std::vector<Vector3>& points, const size_t& i0, const size_t& i1, const size_t& i2 );

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

}; // end namespace

#endif // THREE_TRIANGLE_HPP