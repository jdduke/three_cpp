#ifndef THREE_EULER_H
#define THREE_EULER_H

#include <three/common.h>

#include <three/constants.h>

#include <three/math/quaternion.h>

namespace three {

class Euler {
public:

  static const enums::EulerRotationOrder DefaultOrder = enums::EulerRotationOrder::XYZ;

  Euler()
    : x( 0.f ),
      y( 0.f ),
      z( 0.f ),
      _order( enums::EulerRotationOrder::XYZ ) {}

  Euler( const float xIn, const float yIn, const float zIn )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      _order( enums::EulerRotationOrder::XYZ ) {}

  Euler( const float xIn, const float yIn, const float zIn, const enums::EulerRotationOrder orderIn )
    : x( xIn ),
      y( yIn ),
      z( zIn ),
      _order( orderIn ) {}

  float x;
  float y;
  float z;

  inline const enums::EulerRotationOrder order() const {

    return _order;

  }

  inline Euler& set( float xIn, float yIn, float zIn) {

    x = xIn;
    y = yIn;
    z = zIn;

    return *this;

  }

  inline Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn ) {

    x = xIn;
    y = yIn;
    z = zIn;

    _order = orderIn;

    return *this;

  }

  inline Euler& copy ( const Euler& euler ) {

    x = euler.x;
    y = euler.y;
    z = euler.z;
    _order = euler._order;

    return *this;

  }

  Euler& setFromRotationMatrix( const Matrix4& m );

  Euler& setFromRotationMatrix( const Matrix4& m, const enums::EulerRotationOrder order );

  Euler& setFromQuaternion( const Quaternion& q ) {

    return setFromQuaternion( q, _order );

  }

  Euler& setFromQuaternion( const Quaternion& q, const enums::EulerRotationOrder order );

  inline Euler& reorder( enums::EulerRotationOrder newOrder ) {

    // WARNING: this discards revolution information -bhouston
    setFromQuaternion( Quaternion().setFromEuler( *this ), newOrder );

    return *this;

  }

  inline bool equals( const Euler& euler ) const {

    return ( euler.x == x ) && ( euler.y == y ) && ( euler.z == z ) && ( euler._order == _order );

  }

  inline Euler clone() {

    return Euler(*this);

  }

private:

  inline float _clamp( float x ) {

    return Math::min( Math::max( x, -1.f ), 1.f );

  }

  enums::EulerRotationOrder _order;

};

} // namespace three

#endif // THREE_EULER_H
