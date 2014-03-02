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

    inline float x() const {
     return _x;
    }

    inline Euler& x(const float& value) {
      _x = value;
      _updateQuaternion();
	  return *this;
    }

     inline float y() const {
     return _y;
    }

    inline Euler& y(const float& value) {
      _y = value;
      _updateQuaternion();
	  return *this;
    }

     inline float z() const {
     return _z;
    }

    inline Euler& z(const float& value) {
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