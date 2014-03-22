#ifndef THREE_EULER_CPP
#define THREE_EULER_CPP

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/euler.h>
#include <three/math/matrix4.h>
#include <three/math/quaternion.h>

namespace three {

Euler& Euler::setFromRotationMatrix( const Matrix4& m, enums::EulerRotationOrder order ) {

// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  const auto& te = m.elements;
  auto m11 = te[0], m12 = te[4], m13 = te[8];
  auto m21 = te[1], m22 = te[5], m23 = te[9];
  auto m31 = te[2], m32 = te[6], m33 = te[10];


  if ( order == enums::EulerRotationOrder::XYZ ) {

    _y = Math::asin( _clamp( m13 ) );

    if ( Math::abs( m13 ) < 0.99999f ) {
      _x = Math::atan2( - m23, m33 );
      _z = Math::atan2( - m12, m11 );
    } else {
      _x = Math::atan2( m32, m22 );
      _z = 0.f;
    }

  } else if ( order == enums::EulerRotationOrder::YXZ ) {

    _x = Math::asin( - _clamp( m23 ) );

    if ( Math::abs( m23 ) < 0.99999f ) {
      _y = Math::atan2( m13, m33 );
      _z = Math::atan2( m21, m22 );
    } else {
      _y = Math::atan2( - m31, m11 );
      _z = 0.f;
    }

  } else if ( order == enums::EulerRotationOrder::ZXY ) {

    _x = Math::asin( _clamp( m32 ) );

    if ( Math::abs( m32 ) < 0.99999f ) {
      _y = Math::atan2( - m31, m33 );
      _z = Math::atan2( - m12, m22 );
    } else {
      _y = 0.f;
      _z = Math::atan2( m21, m11 );
    }

  } else if ( order == enums::EulerRotationOrder::ZYX ) {

    _y = Math::asin( - _clamp( m31 ) );

    if ( Math::abs( m31 ) < 0.99999f ) {
      _x = Math::atan2( m32, m33 );
      _z = Math::atan2( m21, m11 );
    } else {
      _x = 0.f;
      _z = Math::atan2( - m12, m22 );

    }

  } else if ( order == enums::EulerRotationOrder::YZX ) {

    _z = Math::asin( _clamp( m21 ) );

    if ( Math::abs( m21 ) < 0.99999f ) {
      _x = Math::atan2( - m23, m22 );
      _y = Math::atan2( - m31, m11 );
    } else {
      _x = 0.f;
      _y = Math::atan2( m13, m33 );
    }

  } else if ( order == enums::EulerRotationOrder::XZY ) {

    _z = Math::asin( - _clamp( m12 ) );

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

Euler& Euler::setFromQuaternion( const Quaternion& q, enums::EulerRotationOrder order, bool update) {

// q is assumed to be normalized
// http://www.mathworks.com/matlabcentral/fileexchange/20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/content/SpinCalc.m

  auto sqx = q.x() * q.x();
  auto sqy = q.y() * q.y();
  auto sqz = q.z() * q.z();
  auto sqw = q.w() * q.w();

  if ( order == enums::EulerRotationOrder::XYZ) {

    _x = Math::atan2( 2 * ( q.x() * q.w() - q.y() * q.z() ), ( sqw - sqx - sqy + sqz ) );
    _y = Math::asin(  _clamp( 2 * ( q.x() * q.z() + q.y() * q.w() ) ) );
    _z = Math::atan2( 2 * ( q.z() * q.w() - q.x() * q.y() ), ( sqw + sqx - sqy - sqz ) );

  } else if ( order == enums::EulerRotationOrder::YXZ ) {

    _x = Math::asin(  _clamp( 2 * ( q.x() * q.w() - q.y() * q.z() ) ) );
    _y = Math::atan2( 2 * ( q.x() * q.z() + q.y() * q.w() ), ( sqw - sqx - sqy + sqz ) );
    _z = Math::atan2( 2 * ( q.x() * q.y() + q.z() * q.w() ), ( sqw - sqx + sqy - sqz ) );

  } else if ( order == enums::EulerRotationOrder::ZXY ) {

    _x = Math::asin(  _clamp( 2 * ( q.x() * q.w() + q.y() * q.z() ) ) );
    _y = Math::atan2( 2 * ( q.y() * q.w() - q.z() * q.x() ), ( sqw - sqx - sqy + sqz ) );
    _z = Math::atan2( 2 * ( q.z() * q.w() - q.x() * q.y() ), ( sqw - sqx + sqy - sqz ) );

  } else if ( order == enums::EulerRotationOrder::ZYX ) {

    _x = Math::atan2( 2 * ( q.x() * q.w() + q.z() * q.y() ), ( sqw - sqx - sqy + sqz ) );
    _y = Math::asin(  _clamp( 2 * ( q.y() * q.w() - q.x() * q.z() ) ) );
    _z = Math::atan2( 2 * ( q.x() * q.y() + q.z() * q.w() ), ( sqw + sqx - sqy - sqz ) );

  } else if ( order == enums::EulerRotationOrder::YZX ) {

    _x = Math::atan2( 2 * ( q.x() * q.w() - q.z() * q.y() ), ( sqw - sqx + sqy - sqz ) );
    _y = Math::atan2( 2 * ( q.y() * q.w() - q.x() * q.z() ), ( sqw + sqx - sqy - sqz ) );
    _z = Math::asin(  _clamp( 2 * ( q.x() * q.y() + q.z() * q.w() ) ) );

  } else if ( order == enums::EulerRotationOrder::XZY ) {

    _x = Math::atan2( 2 * ( q.x() * q.w() + q.y() * q.z() ), ( sqw - sqx + sqy - sqz ) );
    _y = Math::atan2( 2 * ( q.x() * q.z() + q.y() * q.w() ), ( sqw + sqx - sqy - sqz ) );
    _z = Math::asin(  _clamp( 2 * ( q.z() * q.w() - q.x() * q.y() ) ) );

  } else {
    //@todo
    //console.warn( 'WARNING: Euler.setFromQuaternion() given unsupported order: ' + order )
  }

  _order = order;

  if ( update ) _updateQuaternion();

  return *this;

}

} // namespace three

#endif // THREE_EULER_CPP