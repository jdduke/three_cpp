#ifndef THREE_EULER_HPP
#define THREE_EULER_HPP

#include <three/common.hpp>

namespace three {

class Euler {

private:

  float _x, _y, _z;

  enums::EulerRotationOrder _order;

  Quaternion _quaternion;

public:

  static const enums::EulerRotationOrder DefaultOrder = enums::EulerRotationOrder::XYZ;

  Euler()
    : _x(0.f), _y(0.f), _z(0.f), _order(enums::EulerRotationOrder::XYZ) {};

  Euler(const float& xIn, const float& yIn, const float& zIn)
    : _x(xIn), _y(yIn), _z(zIn), _order(enums::EulerRotationOrder::XYZ) {};

  Euler(const float& xIn, const float& yIn, const float& zIn, enums::EulerRotationOrder orderIn)
    : _x(xIn), _y(yIn), _z(zIn), _order(orderIn) {};

  inline float x() const;

  inline Euler& x(const float& value);

  inline float y() const;

  inline Euler& y(const float& value);

  inline float z() const;

  inline Euler& z(const float& value);

  inline enums::EulerRotationOrder order() const;

  inline Euler& w(const enums::EulerRotationOrder& value);

  Euler& set( float xIn, float yIn, float zIn);

  Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn );

  Euler& copy ( const Euler& euler );

  Euler& setFromRotationMatrix( const Matrix4& m );

  Euler& setFromRotationMatrix( const Matrix4& m, enums::EulerRotationOrder order );

  Euler& setFromQuaternion( const Quaternion& q, bool update = true);

  Euler& setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update = true);

  Euler& reorder(enums::EulerRotationOrder newOrder);

  inline bool equals( const Euler& euler ) const;

  Euler clone() {
    return *this;
  }

private:

  void _updateQuaternion();

  inline float _clamp( const float& x );

};

} // namespace three

#endif // THREE_Euler_HPP