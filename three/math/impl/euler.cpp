#include <three/math/euler.h>

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/matrix4.h>
#include <three/math/quaternion.h>

namespace three {
namespace {

float _clamp( float x ) {

  return Math::min( Math::max( x, -1.f ), 1.f );

}

} // namespace

Euler& Euler::set( float xIn, float yIn, float zIn) {

  x = xIn;
  y = yIn;
  z = zIn;

  return *this;

}

Euler& Euler::set( float xIn, float yIn, float zIn, THREE::EulerRotationOrder orderIn ) {

  x = xIn;
  y = yIn;
  z = zIn;

  _order = orderIn;

  return *this;

}

Euler& Euler::copy ( const Euler& euler ) {

  x = euler.x;
  y = euler.y;
  z = euler.z;
  _order = euler._order;

  return *this;

}

Euler& Euler::setFromRotationMatrix( const Matrix4& m ) {
    return setFromRotationMatrix(m, _order);
}

Euler& Euler::setFromRotationMatrix( const Matrix4& m, const THREE::EulerRotationOrder order ) {

// assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

  const auto& te = m.elements;
  auto m11 = te[0], m12 = te[4], m13 = te[8];
  auto m21 = te[1], m22 = te[5], m23 = te[9];
  auto m31 = te[2], m32 = te[6], m33 = te[10];


  if ( order == THREE::EulerRotationOrder::XYZ ) {

    y = Math::asin( _clamp( m13 ) );

    if ( Math::abs( m13 ) < 0.99999f ) {
      x = Math::atan2( - m23, m33 );
      z = Math::atan2( - m12, m11 );
    } else {
      x = Math::atan2( m32, m22 );
      z = 0.f;
    }

  } else if ( order == THREE::EulerRotationOrder::YXZ ) {

    x = Math::asin( - _clamp( m23 ) );

    if ( Math::abs( m23 ) < 0.99999f ) {
      y = Math::atan2( m13, m33 );
      z = Math::atan2( m21, m22 );
    } else {
      y = Math::atan2( - m31, m11 );
      z = 0.f;
    }

  } else if ( order == THREE::EulerRotationOrder::ZXY ) {

    x = Math::asin( _clamp( m32 ) );

    if ( Math::abs( m32 ) < 0.99999f ) {
      y = Math::atan2( - m31, m33 );
      z = Math::atan2( - m12, m22 );
    } else {
      y = 0.f;
      z = Math::atan2( m21, m11 );
    }

  } else if ( order == THREE::EulerRotationOrder::ZYX ) {

    y = Math::asin( - _clamp( m31 ) );

    if ( Math::abs( m31 ) < 0.99999f ) {
      x = Math::atan2( m32, m33 );
      z = Math::atan2( m21, m11 );
    } else {
      x = 0.f;
      z = Math::atan2( - m12, m22 );

    }

  } else if ( order == THREE::EulerRotationOrder::YZX ) {

    z = Math::asin( _clamp( m21 ) );

    if ( Math::abs( m21 ) < 0.99999f ) {
      x = Math::atan2( - m23, m22 );
      y = Math::atan2( - m31, m11 );
    } else {
      x = 0.f;
      y = Math::atan2( m13, m33 );
    }

  } else if ( order == THREE::EulerRotationOrder::XZY ) {

    z = Math::asin( - _clamp( m12 ) );

    if ( Math::abs( m12 ) < 0.99999f ) {
      x = Math::atan2( m32, m22 );
      y = Math::atan2( m13, m11 );
    } else {
      x = Math::atan2( - m23, m33 );
      y = 0.f;
    }

  } else {
    // @todo
    //console().warn( 'WARNING: Euler.setFromRotationMatrix() given unsupported order: ' + order )
  }

  _order = order;

  return *this;

}

Euler& Euler::setFromQuaternion( const Quaternion& q ) {

  return setFromQuaternion( q, _order );

}


Euler& Euler::setFromQuaternion( const Quaternion& q, const THREE::EulerRotationOrder order ) {

// q is assumed to be normalized
// http://www.mathworks.com/matlabcentral/fileexchange/20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/content/SpinCalc.m

  auto sqx = q.x * q.x;
  auto sqy = q.y * q.y;
  auto sqz = q.z * q.z;
  auto sqw = q.w * q.w;

  if ( order == THREE::EulerRotationOrder::XYZ) {

    x = Math::atan2( 2 * ( q.x * q.w - q.y * q.z ), ( sqw - sqx - sqy + sqz ) );
    y = Math::asin(  _clamp( 2 * ( q.x * q.z + q.y * q.w ) ) );
    z = Math::atan2( 2 * ( q.z * q.w - q.x * q.y ), ( sqw + sqx - sqy - sqz ) );

  } else if ( order == THREE::EulerRotationOrder::YXZ ) {

    x = Math::asin(  _clamp( 2 * ( q.x * q.w - q.y * q.z ) ) );
    y = Math::atan2( 2 * ( q.x * q.z + q.y * q.w ), ( sqw - sqx - sqy + sqz ) );
    z = Math::atan2( 2 * ( q.x * q.y + q.z * q.w ), ( sqw - sqx + sqy - sqz ) );

  } else if ( order == THREE::EulerRotationOrder::ZXY ) {

    x = Math::asin(  _clamp( 2 * ( q.x * q.w + q.y * q.z ) ) );
    y = Math::atan2( 2 * ( q.y * q.w - q.z * q.x ), ( sqw - sqx - sqy + sqz ) );
    z = Math::atan2( 2 * ( q.z * q.w - q.x * q.y ), ( sqw - sqx + sqy - sqz ) );

  } else if ( order == THREE::EulerRotationOrder::ZYX ) {

    x = Math::atan2( 2 * ( q.x * q.w + q.z * q.y ), ( sqw - sqx - sqy + sqz ) );
    y = Math::asin(  _clamp( 2 * ( q.y * q.w - q.x * q.z ) ) );
    z = Math::atan2( 2 * ( q.x * q.y + q.z * q.w ), ( sqw + sqx - sqy - sqz ) );

  } else if ( order == THREE::EulerRotationOrder::YZX ) {

    x = Math::atan2( 2 * ( q.x * q.w - q.z * q.y ), ( sqw - sqx + sqy - sqz ) );
    y = Math::atan2( 2 * ( q.y * q.w - q.x * q.z ), ( sqw + sqx - sqy - sqz ) );
    z = Math::asin(  _clamp( 2 * ( q.x * q.y + q.z * q.w ) ) );

  } else if ( order == THREE::EulerRotationOrder::XZY ) {

    x = Math::atan2( 2 * ( q.x * q.w + q.y * q.z ), ( sqw - sqx + sqy - sqz ) );
    y = Math::atan2( 2 * ( q.x * q.z + q.y * q.w ), ( sqw + sqx - sqy - sqz ) );
    z = Math::asin(  _clamp( 2 * ( q.z * q.w - q.x * q.y ) ) );

  } else {
    //@todo
    //console.warn( 'WARNING: Euler.setFromQuaternion() given unsupported order: ' + order )
  }

  _order = order;

  return *this;

}

Euler& Euler::reorder( THREE::EulerRotationOrder newOrder ) {

  // WARNING: this discards revolution information -bhouston
  return setFromQuaternion( Quaternion().setFromEuler( *this ), newOrder );

}

bool Euler::equals( const Euler& euler ) const {

  return ( euler.x == x ) && ( euler.y == y ) && ( euler.z == z ) && ( euler._order == _order );

}

Euler Euler::clone() const {

  return Euler(*this);

}

} // namespace three
