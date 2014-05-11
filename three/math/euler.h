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
    : _x(0.f), _y(0.f), _z(0.f), _order(enums::EulerRotationOrder::XYZ), _quaternion( nullptr ) {};

  Euler(float xIn, float yIn, float zIn)
    : _x(xIn), _y(yIn), _z(zIn), _order(enums::EulerRotationOrder::XYZ), _quaternion( nullptr ) {};

  Euler(float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn)
    : _x(xIn), _y(yIn), _z(zIn), _order(orderIn), _quaternion( nullptr ) {};

  inline float x() const {

    return _x;

  }

  inline Euler& x(float value) {

    _x = value;

    _updateQuaternion();

    return *this;

  }

  inline float y() const {

    return _y;

  }

  inline Euler& y(float value) {

    _y = value;

    _updateQuaternion();

    return *this;

  }

  inline float z() const {

    return _z;

  }

  inline Euler& z(float value) {

    _z = value;

    _updateQuaternion();

    return *this;

  }

  inline enums::EulerRotationOrder order() const {

    return _order;

  }

  inline Euler& w(const enums::EulerRotationOrder& value) {

    _order = value;

    _updateQuaternion();

    return *this;
  }

  inline Euler& set( float xIn, float yIn, float zIn) {

    _x = xIn;
    _y = yIn;
    _z = zIn;

    _updateQuaternion();

    return *this;

  }

  inline Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn ) {

    _x = xIn;
    _y = yIn;
    _z = zIn;
    _order = orderIn;

    _updateQuaternion();

    return *this;

  }

  inline Euler& copy ( const Euler& euler ) {

    _x = euler._x;
    _y = euler._y;
    _z = euler._z;
    _order = euler._order;

    _updateQuaternion();

    return *this;

  }

  inline Euler& setFromRotationMatrix( const Matrix4& m ) {

    return setFromRotationMatrix(m, _order);

  }

  Euler& setFromRotationMatrix( const Matrix4& m, enums::EulerRotationOrder order );

  inline Euler& setFromQuaternion( const Quaternion& q, bool update = false) {

    return setFromQuaternion(q, _order, update);

  }

  Euler& setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update = false );

  inline Euler& reorder(enums::EulerRotationOrder newOrder) {

    // WARNING: this discards revolution information -bhouston

    auto q = Quaternion();
    q.setFromEuler( *this );
    setFromQuaternion( q, newOrder );

    return *this;

  }

  inline bool equals( const Euler& euler ) const {

    return ( euler._x == _x ) && ( euler._y == _y ) && ( euler._z == _z ) && ( euler._order == _order );

  }

  inline Euler clone() {

    return *this;

  }

private:

  friend class Object3D;

  inline void _updateQuaternion() {

    if(_quaternion) {
        _quaternion->setFromEuler( *this, false );
    }

  }

  inline float _clamp( float x ) {

    return Math::min( Math::max( x, -1.f ), 1.f );

  }

  float _x, _y, _z;

  enums::EulerRotationOrder _order;

  Quaternion* _quaternion;

};

} // namespace three

#endif // THREE_EULER_H