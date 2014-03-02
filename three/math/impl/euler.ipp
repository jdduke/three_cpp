#ifndef THREE_EULER_IPP
#define THREE_EULER_IPP

#include <three/common.hpp>

#include <three/utils/macros.hpp>
#include <three/math/math.hpp>
#include <three/math/euler.hpp>

namespace three {

    void Euler::_updateQuaternion() {
      _quaternion.setFromEuler( *this, _order );
    }

    inline float Euler::clamp( const float& x ) {
      return Math::min( Math::max( x, -1.f ), 1.f );
    }


	Euler& Euler::set( float xIn, float yIn, float zIn) {

     _x = xIn;
     _y = yIn;
     _z = zIn;

     _updateQuaternion();

     return *this;
   }

   Euler& Euler::set( float xIn, float yIn, float zIn, enums::EulerRotationOrder orderIn ) {

     _x = xIn;
     _y = yIn;
     _z = zIn;
     _order = orderIn;

     _updateQuaternion();

     return *this;
   }

   Euler& Euler::copy ( const Euler& euler ) {

     _x = euler._x;
     _y = euler._y;
     _z = euler._z;
     _order = euler._order;

     _updateQuaternion();

     return *this;
   }

   Euler& Euler::setFromRotationMatrix( const Matrix3& m ) {
    return setFromRotationMatrix(m, _order);
  }

  Euler& Euler::setFromRotationMatrix( const Matrix3& m, enums::EulerRotationOrder order ) {

      // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    const auto& te = m.elements;
    auto m11 = te[0], m12 = te[4], m13 = te[8];
    auto m21 = te[1], m22 = te[5], m23 = te[9];
    auto m31 = te[2], m32 = te[6], m33 = te[10];


    if ( order == enums::EulerRotationOrder::XYZ ) {

      _y = Math::asin( clamp( m13 ) );

      if ( Math::abs( m13 ) < 0.99999f ) {
        _x = Math::atan2( - m23, m33 );
        _z = Math::atan2( - m12, m11 );
      } else {
        _x = Math::atan2( m32, m22 );
        _z = 0.f;
      }

    } else if ( order == enums::EulerRotationOrder::YXZ ) {

      _x = Math::asin( - clamp( m23 ) );

      if ( Math::abs( m23 ) < 0.99999f ) {
        _y = Math::atan2( m13, m33 );
        _z = Math::atan2( m21, m22 );
      } else {
        _y = Math::atan2( - m31, m11 );
        _z = 0.f;
      }

    } else if ( order == enums::EulerRotationOrder::ZXY ) {

      _x = Math::asin( clamp( m32 ) );

      if ( Math::abs( m32 ) < 0.99999f ) {
        _y = Math::atan2( - m31, m33 );
        _z = Math::atan2( - m12, m22 );
      } else {
        _y = 0.f;
        _z = Math::atan2( m21, m11 );
      }

    } else if ( order == enums::EulerRotationOrder::ZYX ) {

      _y = Math::asin( - clamp( m31 ) );

      if ( Math::abs( m31 ) < 0.99999f ) {
        _x = Math::atan2( m32, m33 );
        _z = Math::atan2( m21, m11 );
      } else {
        _x = 0.f;
        _z = Math::atan2( - m12, m22 );

      }

    } else if ( order == enums::EulerRotationOrder::YZX ) {

      _z = Math::asin( clamp( m21 ) );

      if ( Math::abs( m21 ) < 0.99999f ) {
        _x = Math::atan2( - m23, m22 );
        _y = Math::atan2( - m31, m11 );
      } else {
        _x = 0.f;
        _y = Math::atan2( m13, m33 );
      }

    } else if ( order == enums::EulerRotationOrder::XZY ) {

      _z = Math::asin( - clamp( m12 ) );

      if ( Math::abs( m12 ) < 0.99999f ) {
        _x = Math::atan2( m32, m22 );
        _y = Math::atan2( m13, m11 );
      } else {
        _x = Math::atan2( - m23, m33 );
        _y = 0.f;
      }

    } else {
        // @todo
        //console().warn( 'WARNING: Euler.setFromRotationMatrix() given unsupported order: ' + order )
    }

    _order = order;

    _updateQuaternion();

    return *this;
  }

  Euler& Euler::setFromQuaternion( const Quaternion& q, bool update) {
    return setFromQuaternion(q, _order, update);
  }

  Euler& Euler::setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update) {

      // q is assumed to be normalized
      // http://www.mathworks.com/matlabcentral/fileexchange/20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/content/SpinCalc.m

    auto sqx = q.x * q.x;
    auto sqy = q.y * q.y;
    auto sqz = q.z * q.z;
    auto sqw = q.w * q.w;

    if ( order == enums::EulerRotationOrder::XYZ) {

      _x = Math::atan2( 2 * ( q.x * q.w - q.y * q.z ), ( sqw - sqx - sqy + sqz ) );
      _y = Math::asin(  clamp( 2 * ( q.x * q.z + q.y * q.w ) ) );
      _z = Math::atan2( 2 * ( q.z * q.w - q.x * q.y ), ( sqw + sqx - sqy - sqz ) );

    } else if ( order == enums::EulerRotationOrder::YXZ ) {

      _x = Math::asin(  clamp( 2 * ( q.x * q.w - q.y * q.z ) ) );
      _y = Math::atan2( 2 * ( q.x * q.z + q.y * q.w ), ( sqw - sqx - sqy + sqz ) );
      _z = Math::atan2( 2 * ( q.x * q.y + q.z * q.w ), ( sqw - sqx + sqy - sqz ) );

    } else if ( order == enums::EulerRotationOrder::ZXY ) {

      _x = Math::asin(  clamp( 2 * ( q.x * q.w + q.y * q.z ) ) );
      _y = Math::atan2( 2 * ( q.y * q.w - q.z * q.x ), ( sqw - sqx - sqy + sqz ) );
      _z = Math::atan2( 2 * ( q.z * q.w - q.x * q.y ), ( sqw - sqx + sqy - sqz ) );

    } else if ( order == enums::EulerRotationOrder::ZYX ) {

      _x = Math::atan2( 2 * ( q.x * q.w + q.z * q.y ), ( sqw - sqx - sqy + sqz ) );
      _y = Math::asin(  clamp( 2 * ( q.y * q.w - q.x * q.z ) ) );
      _z = Math::atan2( 2 * ( q.x * q.y + q.z * q.w ), ( sqw + sqx - sqy - sqz ) );

    } else if ( order == enums::EulerRotationOrder::YZX ) {

      _x = Math::atan2( 2 * ( q.x * q.w - q.z * q.y ), ( sqw - sqx + sqy - sqz ) );
      _y = Math::atan2( 2 * ( q.y * q.w - q.x * q.z ), ( sqw + sqx - sqy - sqz ) );
      _z = Math::asin(  clamp( 2 * ( q.x * q.y + q.z * q.w ) ) );

    } else if ( order == enums::EulerRotationOrder::XZY ) {

      _x = Math::atan2( 2 * ( q.x * q.w + q.y * q.z ), ( sqw - sqx + sqy - sqz ) );
      _y = Math::atan2( 2 * ( q.x * q.z + q.y * q.w ), ( sqw + sqx - sqy - sqz ) );
      _z = Math::asin(  clamp( 2 * ( q.z * q.w - q.x * q.y ) ) );

    } else {
        //@todo
        //console.warn( 'WARNING: Euler.setFromQuaternion() given unsupported order: ' + order )
    }

    _order = order;

    if ( update ) _updateQuaternion();

    return *this;
  }

  Euler& Euler::reorder(enums::EulerRotationOrder newOrder) {
      // WARNING: this discards revolution information -bhouston
    auto q = Quaternion();
    q.setFromEuler( *this );
    setFromQuaternion( q, newOrder );
    return *this;
  }

  bool Euler::equals( const Euler& euler ) const {
    return ( euler._x == _x ) && ( euler._y == _y ) && ( euler._z == _z ) && ( euler._order == _order );
  }

};

#endif //THREE_EULER_IPP