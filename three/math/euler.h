#ifndef THREE_EULER_H
#define THREE_EULER_H

#include <three/common.h>

#include <three/constants.h>

namespace three {

class THREE_DECL Euler {
public:

  static const THREE::EulerRotationOrder DefaultOrder = THREE::EulerRotationOrder::XYZ;

  Euler()
    : x( 0.f ),
      y( 0.f ),
      z( 0.f ),
      _order( THREE::EulerRotationOrder::XYZ ) {}

  Euler( const float xIn, const float yIn, const float zIn )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      _order( THREE::EulerRotationOrder::XYZ ) {}

  Euler( const float xIn, const float yIn, const float zIn, const THREE::EulerRotationOrder orderIn )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      _order( orderIn ) {}

  float x;
  float y;
  float z;

  inline const THREE::EulerRotationOrder order() const { return _order; }

  Euler& set( float xIn, float yIn, float zIn);
  Euler& set( float xIn, float yIn, float zIn, THREE::EulerRotationOrder orderIn );

  Euler& copy ( const Euler& euler );

  Euler& setFromRotationMatrix( const Matrix4& m );
  Euler& setFromRotationMatrix( const Matrix4& m, const THREE::EulerRotationOrder order );

  Euler& setFromQuaternion( const Quaternion& q );
  Euler& setFromQuaternion( const Quaternion& q, const THREE::EulerRotationOrder order );

  Euler& reorder( THREE::EulerRotationOrder newOrder );

  bool equals( const Euler& euler ) const;

  Euler clone() const;

private:
  THREE::EulerRotationOrder _order;

};

} // namespace three

#endif // THREE_EULER_H
