#ifndef THREE_EULER_HPP
#define THREE_EULER_HPP

#include <three/common.hpp>

namespace three {   
	class Euler {
	private:

    float _x, _y, _z;

    enums::EulerRotationOrder _order;
    Quaternion _quaternion;


    void _updateQuaternion();

    inline float clamp( const float& x );


  public:
        

    static const enums::EulerRotationOrder DefaultOrder = enums::EulerRotationOrder::XYZ;

    Euler() : _x(0.f), _y(0.f), _z(0.f), _order(enums::EulerRotationOrder::XYZ) {};

    Euler(float xIn, float yIn, float zIn) : _x(xIn), _y(yIn), _z(zIn), _order(enums::EulerRotationOrder::XYZ) { };

    Euler(float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn) : _x(xIn), _y(yIn), _z(zIn), _order(orderIn) {};

	PROPERTY(float, x);
    GET(x) const {
     return _x;
    }

    inline Euler& SET(x) {
      _x = value;
      _updateQuaternion();
	  return *this;
    }

    PROPERTY(float, y);
    GET(y) const {
     return _y;
    }

    inline Euler& SET(y) {
      _y = value;
      _updateQuaternion();
      return *this;
    }

    PROPERTY(float, z);
    GET(z) const {
     return _z;
    }

    inline Euler& SET(z) {
      _z = value;
      _updateQuaternion();
      return *this;
    }

	PROPERTY(enums::EulerRotationOrder, order);
    GET(order) const {
     return _order;
    }

    inline Euler& SET(order) {
      _order = value;
      _updateQuaternion();
      return *this;
    }

   THREE_DECL Euler& set( float xIn, float yIn, float zIn);

   THREE_DECL Euler& set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn );

   THREE_DECL Euler& copy ( const Euler& euler );

   THREE_DECL Euler& setFromRotationMatrix( const Matrix3& m );

  THREE_DECL Euler& setFromRotationMatrix( const Matrix3& m, enums::EulerRotationOrder order );

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, bool update = true);

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update = true);

  THREE_DECL Euler& reorder(enums::EulerRotationOrder newOrder);

  THREE_DECL bool equals( const Euler& euler ) const;

  THREE_DECL Euler clone() {
    return *this;
  }

};

} // namespace three

#endif // THREE_Euler_HPP