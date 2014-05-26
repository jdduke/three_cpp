#ifndef THREE_QUATERNION_H
#define THREE_QUATERNION_H

#include <three/common.h>

namespace three {

class THREE_DECL Quaternion {

public:

  Quaternion()
    : x( 0.f ),
      y( 0.f ),
      z( 0.f ),
      w( 1.f ) {}

  Quaternion( const float xIn, const float yIn, const float zIn, const float wIn = 1.f )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      w( wIn ) {}

  float x;
  float y;
  float z;
  float w;

  Quaternion& set( const float xIn, const float yIn, const float zIn, const float wIn );

  Quaternion& copy( const Quaternion& quaternion );

  Quaternion& setFromEuler( const Euler& euler );
  Quaternion& setFromAxisAngle( const Vector3& axis, const float angle );
  Quaternion& setFromRotationMatrix( const Matrix4& m );

  Quaternion& inverse();
  Quaternion& conjugate();

  float lengthSq() const;
  float length() const;

  Quaternion& normalize();

  Quaternion& multiply( const Quaternion& q );
  Quaternion& multiplyQuaternions( const Quaternion& a, const Quaternion& b );

  Quaternion& slerp( const Quaternion& qb, float t );

  bool equals( const Quaternion& quaternion ) const;

  Quaternion clone() const;

};

} // namespace three

#endif // THREE_QUATERNION_H
