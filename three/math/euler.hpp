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

  THREE_DECL inline float x() const;

  THREE_DECL inline Euler& x(const float& value);

  THREE_DECL inline float y() const;

  THREE_DECL inline Euler& y(const float& value);

  THREE_DECL inline float z() const;

  THREE_DECL inline Euler& z(const float& value);

  THREE_DECL inline enums::EulerRotationOrder order() const;

  THREE_DECL inline Euler& w(const enums::EulerRotationOrder& value);

  THREE_DECL Euler& set( float xIn, float yIn, float zIn);

  THREE_DECL Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn );

  THREE_DECL Euler& copy ( const Euler& euler );

  THREE_DECL Euler& setFromRotationMatrix( const Matrix4& m );

  THREE_DECL Euler& setFromRotationMatrix( const Matrix4& m, enums::EulerRotationOrder order );

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, bool update = true);

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update = true);

  THREE_DECL Euler& reorder(enums::EulerRotationOrder newOrder);

  THREE_DECL inline bool equals( const Euler& euler ) const;

  THREE_DECL Euler clone() {
    return *this;
  }

private:

  void _updateQuaternion();

  inline float _clamp( const float& x );

};

} // namespace three

#endif // THREE_Euler_HPP