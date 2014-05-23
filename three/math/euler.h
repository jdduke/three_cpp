#ifndef THREE_EULER_H
#define THREE_EULER_H

#include <three/common.h>

#include <three/constants.h>

#include <three/math/quaternion.h>
#include <three/utils/functional.h>

namespace three {

class Euler {
public:

  static const enums::EulerRotationOrder DefaultOrder = enums::EulerRotationOrder::XYZ;

  Euler()
    : x( FLOAT_HOOK( Euler, _updateQuaternion, 0.f ) ),
      y( FLOAT_HOOK( Euler, _updateQuaternion, 0.f ) ),
      z( FLOAT_HOOK( Euler, _updateQuaternion, 0.f ) ),
      _order( enums::EulerRotationOrder::XYZ ), 
      _quaternion( nullptr ) {};

  Euler( const float xIn, const float yIn, const float zIn )
    : x( FLOAT_HOOK(Euler, _updateQuaternion, xIn ) ), 
      y( FLOAT_HOOK(Euler, _updateQuaternion, yIn ) ), 
      z( FLOAT_HOOK(Euler, _updateQuaternion, zIn ) ), 
      _order( enums::EulerRotationOrder::XYZ ), 
      _quaternion( nullptr ) {};

  Euler( const float xIn, const float yIn, const float zIn, const enums::EulerRotationOrder orderIn )
    : x( FLOAT_HOOK( Euler, _updateQuaternion, xIn ) ), 
      y( FLOAT_HOOK( Euler, _updateQuaternion, yIn ) ), 
      z( FLOAT_HOOK( Euler, _updateQuaternion, zIn ) ),  
      _order( orderIn ), 
      _quaternion( nullptr ) {};

  NumericalHook<float, Euler> x;
  NumericalHook<float, Euler> y;
  NumericalHook<float, Euler> z;
    
  inline const enums::EulerRotationOrder order() const {

    return _order;

  }

  inline Euler& set( float xIn, float yIn, float zIn) {

    x.value = xIn;
    y.value = yIn;
    z.value = zIn;

    _updateQuaternion();

    return *this;

  }

  inline Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn ) {

    x.value = xIn;
    y.value = yIn;
    z.value = zIn;

    _order = orderIn;

    _updateQuaternion();

    return *this;

  }

  inline Euler& copy ( const Euler& euler ) {

    x.value = euler.x.value;
    y.value = euler.y.value;
    z.value = euler.z.value;
    _order = euler._order;

    _updateQuaternion();

    return *this;

  }

  Euler& setFromRotationMatrix( const Matrix4& m );

  Euler& setFromRotationMatrix( const Matrix4& m, const enums::EulerRotationOrder order );

  Euler& setFromQuaternion( const Quaternion& q, bool update = false) {

    return setFromQuaternion(q, _order, update);

  }

  Euler& setFromQuaternion( const Quaternion& q, const enums::EulerRotationOrder order, bool update = false );

  inline Euler& reorder( enums::EulerRotationOrder newOrder ) {

    // WARNING: this discards revolution information -bhouston

    auto q = Quaternion();
    q.setFromEuler( *this );
    setFromQuaternion( q, newOrder );

    return *this;

  }

  inline bool equals( const Euler& euler ) const {

    return ( euler.x.value == x.value ) && ( euler.y.value == y.value ) && ( euler.z.value == z.value ) && ( euler._order == _order );

  }

  inline Euler clone() {

    return Euler(*this);

  }

private:

  friend class Object3D;

  void _updateQuaternion() {

    if( _quaternion ) {
      _quaternion->setFromEuler( *this, false );
    }

  }

  inline float _clamp( float x ) {

    return Math::min( Math::max( x, -1.f ), 1.f );

  }

  enums::EulerRotationOrder _order;

  Quaternion* _quaternion;

};

} // namespace three

#endif // THREE_EULER_H