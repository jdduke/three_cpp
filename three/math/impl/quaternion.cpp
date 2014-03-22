#ifndef THREE_QUATERNION_IPP
#define THREE_QUATERNION_IPP


#include <three/common.hpp>
#include <three/math/math.hpp>
#include <three/math/euler.hpp>
#include <three/math/quaternion.hpp>

namespace three {

  Quaternion& Quaternion::setFromEuler( const Euler& euler, bool update ) {

    // http://www.mathworks.com/matlabcentral/fileexchange/
    //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
    //  content/SpinCalc.m

    auto c1 = Math::cos( euler.x() / 2 );
    auto c2 = Math::cos( euler.y() / 2 );
    auto c3 = Math::cos( euler.z() / 2 );
    auto s1 = Math::sin( euler.x() / 2 );
    auto s2 = Math::sin( euler.y() / 2 );
    auto s3 = Math::sin( euler.z() / 2 );

    if ( euler.order() == enums::EulerRotationOrder::XYZ ) {

      _x = s1 * c2 * c3 + c1 * s2 * s3;
      _y = c1 * s2 * c3 - s1 * c2 * s3;
      _z = c1 * c2 * s3 + s1 * s2 * c3;
      _w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == enums::EulerRotationOrder::YXZ ) {

      _x = s1 * c2 * c3 + c1 * s2 * s3;
      _y = c1 * s2 * c3 - s1 * c2 * s3;
      _z = c1 * c2 * s3 - s1 * s2 * c3;
      _w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.order() == enums::EulerRotationOrder::ZXY ) {

      _x = s1 * c2 * c3 - c1 * s2 * s3;
      _y = c1 * s2 * c3 + s1 * c2 * s3;
      _z = c1 * c2 * s3 + s1 * s2 * c3;
      _w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == enums::EulerRotationOrder::ZYX ) {

      _x = s1 * c2 * c3 - c1 * s2 * s3;
      _y = c1 * s2 * c3 + s1 * c2 * s3;
      _z = c1 * c2 * s3 - s1 * s2 * c3;
      _w = c1 * c2 * c3 + s1 * s2 * s3;

    } else if ( euler.order() == enums::EulerRotationOrder::YZX ) {

      _x = s1 * c2 * c3 + c1 * s2 * s3;
      _y = c1 * s2 * c3 + s1 * c2 * s3;
      _z = c1 * c2 * s3 - s1 * s2 * c3;
      _w = c1 * c2 * c3 - s1 * s2 * s3;

    } else if ( euler.order() == enums::EulerRotationOrder::XZY ) {

      _x = s1 * c2 * c3 - c1 * s2 * s3;
      _y = c1 * s2 * c3 - s1 * c2 * s3;
      _z = c1 * c2 * s3 + s1 * s2 * c3;
      _w = c1 * c2 * c3 + s1 * s2 * s3;

    }

    if ( update ) _updateEuler();

    return *this;

  }

  Quaternion& Quaternion::setFromRotationMatrix( const Matrix4& m ) {

    // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm

    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

    const auto& te = m.elements;

      auto m11 = te[0], m12 = te[4], m13 = te[8],
      m21 = te[1], m22 = te[5], m23 = te[9],
      m31 = te[2], m32 = te[6], m33 = te[10];

      auto trace = m11 + m22 + m33;
      float s;

    if ( trace > 0.f ) {

      s = 0.5f / Math::sqrt( trace + 1.0f );

      _w = 0.25f / s;
      _x = ( m32 - m23 ) * s;
      _y = ( m13 - m31 ) * s;
      _z = ( m21 - m12 ) * s;

    } else if ( m11 > m22 && m11 > m33 ) {

      s = 2.0f * Math::sqrt( 1.0f + m11 - m22 - m33 );

      _w = (m32 - m23 ) / s;
      _x = 0.25f * s;
      _y = (m12 + m21 ) / s;
      _z = (m13 + m31 ) / s;

    } else if ( m22 > m33 ) {

      s = 2.0f * Math::sqrt( 1.0f + m22 - m11 - m33 );

      _w = (m13 - m31 ) / s;
      _x = (m12 + m21 ) / s;
      _y = 0.25f * s;
      _z = (m23 + m32 ) / s;

    } else {

      s = 2.0f * Math::sqrt( 1.0f + m33 - m11 - m22 );

      _w = ( m21 - m12 ) / s;
      _x = ( m13 + m31 ) / s;
      _y = ( m23 + m32 ) / s;
      _z = 0.25f * s;

    }

    _updateEuler();

    return *this;

  }

  Quaternion& Quaternion::normalize() {

    auto l = length();

    if ( l == 0.f ) {

      _x = 0;
      _y = 0;
      _z = 0;
      _w = 1;

    } else {

      l = 1.f / l;

      _x = _x * l;
      _y = _y * l;
      _z = _z * l;
      _w = _w * l;

    }

    return *this;
  }

  Quaternion& Quaternion::multiplyQuaternions( const Quaternion& a, const Quaternion& b ) {

    // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm

    auto qax = a.x(), qay = a.y(), qaz = a.z(), qaw = a.w();
    auto qbx = b.x(), qby = b.y(), qbz = b.z(), qbw = b.w();

    _x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    _y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    _z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    _w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

    _updateEuler();

    return *this;

  }

  Quaternion& Quaternion::slerp( const Quaternion& qb, float t ) {

    auto x = _x, y = _y, z = _z, w = _w;

    // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/

    auto cosHalfTheta = w * qb.w() + x * qb.x() + y * qb.y() + z * qb.z();

    if ( cosHalfTheta < 0.f ) {

      _w = -qb.w();
      _x = -qb.x();
      _y = -qb.y();
      _z = -qb.z();

      cosHalfTheta = -cosHalfTheta;

    } else {

      copy( qb );

    }

    if ( cosHalfTheta >= 1.0f ) {

      _w = w;
      _x = x;
      _y = y;
      _z = z;

      return *this;

    }

    auto halfTheta = Math::acos( cosHalfTheta );
    auto sinHalfTheta = Math::sqrt( 1.0f - cosHalfTheta * cosHalfTheta );

    if ( Math::abs( sinHalfTheta ) < 0.001f ) {

      _w = 0.5f * ( w + _w );
      _x = 0.5f * ( x + _x );
      _y = 0.5f * ( y + _y );
      _z = 0.5f * ( z + _z );

      return *this;

    }

    auto ratioA = Math::sin( ( 1.f - t ) * halfTheta ) / sinHalfTheta,
    ratioB = Math::sin( t * halfTheta ) / sinHalfTheta;

    _w = ( w * ratioA + _w * ratioB );
    _x = ( x * ratioA + _x * ratioB );
    _y = ( y * ratioA + _y * ratioB );
    _z = ( z * ratioA + _z * ratioB );

    _updateEuler();

    return *this;

  }

  void Quaternion::_updateEuler() {
      if(_euler == nullptr) {
        _euler = make_shared<Euler>(Euler()); 
      }
      _euler->setFromQuaternion( *this, false );
    }

} // namespace three

#endif // THREE_QUATERNION_IPP