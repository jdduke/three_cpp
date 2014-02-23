#ifndef THREE_EULER_HPP
#define THREE_EULER_HPP

#include <three/common.hpp>

#include <three/utils/macros.hpp>
#include <three/math/math.hpp>

#include <three/math/quaternion.hpp>

namespace three {

	class Euler {
	private:

    float _x, _y, _z;

    THREE::EulerRotationOrder _order;
    Quaternion* _quaternion;


    void _updateQuaternion();

    inline float clamp( const float& x );


  public:
        

    const THREE::EulerRotationOrder DefaultOrder = THREE::EulerRotationOrder::XYZ;

    Euler() : _x(0.f), _y(0.f), _z(0.f), _order(THREE::EulerRotationOrder::XYZ) {
      _quaternion = new Quaternion();
    };

    Euler(float xIn, float yIn, float zIn) : _x(xIn), _y(yIn), _z(zIn), _order(THREE::EulerRotationOrder::XYZ) {
      _quaternion = new Quaternion();
    };

    Euler(float xIn, float yIn, float zIn, THREE::EulerRotationOrder orderIn) : _x(xIn), _y(yIn), _z(zIn), _order(orderIn) {
      _quaternion = new Quaternion();
    };

  THREE_DECL inline float getX() const;
  THREE_DECL inline Quaternion& setX(const float& value);
  THREE_DECL inline float getY() const;
  THREE_DECL inline Quaternion& setY(const float& value);
  THREE_DECL inline float getZ() const;
  THREE_DECL inline Quaternion& setOrder(const THREE::EulerRotationOrder& value);
  THREE_DECL inline THREE::EulerRotationOrder& getOrder() const;


   THREE_DECL Euler& set( float xIn, float yIn, float zIn);

   THREE_DECL Euler& set( float xIn, float yIn, float zIn, THREE::EulerRotationOrder orderIn );

   THREE_DECL Euler& copy ( const Euler& euler );

   THREE_DECL Euler& setFromRotationMatrix( const Matrix3& m );

  THREE_DECL Euler& setFromRotationMatrix( const Matrix3& m, THREE::EulerRotationOrder order );

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, bool update = true);

  THREE_DECL Euler& setFromQuaternion( const Quaternion& q, THREE::EulerRotationOrder order, bool update = true);

  THREE_DECL Euler& reorder(THREE::EulerRotationOrder newOrder);

  THREE_DECL bool equals( const Euler& euler ) const;

  THREE_DECL Euler clone() {
    return *this;
  }

};

} // namespace three

#endif // THREE_Euler_HPP