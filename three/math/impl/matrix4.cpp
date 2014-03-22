#ifndef THREE_MATRIX4_CPP
#define THREE_MATRIX4_CPP

#include <three/constants.h>

#include <three/math/math.h>
#include <three/math/vector4.h>
#include <three/math/matrix4.h>
#include <three/math/euler.h>
#include <three/math/vector3.h>
#include <three/math/quaternion.h>

namespace three {

Matrix4::Matrix4() {
  identity();
}

Matrix4::Matrix4( float n11, float n12, float n13, float n14,
                  float n21, float n22, float n23, float n24,
                  float n31, float n32, float n33, float n34,
                  float n41, float n42, float n43, float n44 ) {

  set( n11, n12, n13, n14,
       n21, n22, n23, n24,
       n31, n32, n33, n34,
       n41, n42, n43, n44 );

}

Matrix4::Matrix4( const Matrix4& other ) {
  copy( other );
}

Matrix4& Matrix4::operator= ( const Matrix4& other ) {
  return copy( other );
}

Matrix4& Matrix4::set( float n11, float n12, float n13, float n14,
                       float n21, float n22, float n23, float n24,
                       float n31, float n32, float n33, float n34,
                       float n41, float n42, float n43, float n44 ) {

  te[0] = n11;
  te[4] = n12;
  te[8]  = n13;
  te[12] = n14;
  te[1] = n21;
  te[5] = n22;
  te[9]  = n23;
  te[13] = n24;
  te[2] = n31;
  te[6] = n32;
  te[10] = n33;
  te[14] = n34;
  te[3] = n41;
  te[7] = n42;
  te[11] = n43;
  te[15] = n44;

  return *this;

}

Matrix4& Matrix4::identity() {
  set(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
  );
  return *this;
}

Matrix4& Matrix4::copy ( const Matrix4& m ) {
  std::copy(std::begin(m.elements), std::end(m.elements), std::begin(this->elements));
//    elements.set( m.elements );
  return *this;
}

Matrix4& Matrix4::extractPosition( const Matrix4& m ) {
  //console.warn( 'DEPRECATED: Matrix4\'s .extractPosition() has been renamed to .copyPosition().' );
  return copyPosition( m );
}

Matrix4& Matrix4::copyPosition( const Matrix4& m ) {
  auto& te = elements;
  const auto& me = m.elements;

  te[12] = me[12];
  te[13] = me[13];
  te[14] = me[14];

  return *this;
}

Matrix4& Matrix4::extractRotation( const Matrix4& m ) {
  // @todo priv prop/reset
  auto v1 = Vector3();

  auto& te = elements;
  const auto& me = m.elements;

  auto scaleX = 1 / v1.set( me[0], me[1], me[2] ).length();
  auto scaleY = 1 / v1.set( me[4], me[5], me[6] ).length();
  auto scaleZ = 1 / v1.set( me[8], me[9], me[10] ).length();

  te[0] = me[0] * scaleX;
  te[1] = me[1] * scaleX;
  te[2] = me[2] * scaleX;

  te[4] = me[4] * scaleY;
  te[5] = me[5] * scaleY;
  te[6] = me[6] * scaleY;

  te[8] = me[8] * scaleZ;
  te[9] = me[9] * scaleZ;
  te[10] = me[10] * scaleZ;

  return *this;
}

Matrix4& Matrix4::makeRotationFromEuler( const Euler& euler ) {

  auto& te = elements;

  auto x = euler.x(), y = euler.y(), z = euler.z();
  auto a = Math::cos( x ), b = Math::sin( x );
  auto c = Math::cos( y ), d = Math::sin( y );
  auto e = Math::cos( z ), f = Math::sin( z );

  auto order = euler.order();

  if ( order == enums::EulerRotationOrder::XYZ ) {

    auto ae = a * e, af = a * f, be = b * e, bf = b * f;

    te[0] = c * e;
    te[4] = - c * f;
    te[8] = d;

    te[1] = af + be * d;
    te[5] = ae - bf * d;
    te[9] = - b * c;

    te[2] = bf - ae * d;
    te[6] = be + af * d;
    te[10] = a * c;

  } else if ( order == enums::EulerRotationOrder::YXZ ) {

    auto ce = c * e, cf = c * f, de = d * e, df = d * f;

    te[0] = ce + df * b;
    te[4] = de * b - cf;
    te[8] = a * d;

    te[1] = a * f;
    te[5] = a * e;
    te[9] = - b;

    te[2] = cf * b - de;
    te[6] = df + ce * b;
    te[10] = a * c;

  } else if ( order == enums::EulerRotationOrder::ZXY ) {

    auto ce = c * e, cf = c * f, de = d * e, df = d * f;

    te[0] = ce - df * b;
    te[4] = - a * f;
    te[8] = de + cf * b;

    te[1] = cf + de * b;
    te[5] = a * e;
    te[9] = df - ce * b;

    te[2] = - a * d;
    te[6] = b;
    te[10] = a * c;

  } else if ( order == enums::EulerRotationOrder::ZYX ) {

    auto ae = a * e, af = a * f, be = b * e, bf = b * f;

    te[0] = c * e;
    te[4] = be * d - af;
    te[8] = ae * d + bf;

    te[1] = c * f;
    te[5] = bf * d + ae;
    te[9] = af * d - be;

    te[2] = - d;
    te[6] = b * c;
    te[10] = a * c;

  } else if ( order == enums::EulerRotationOrder::YZX ) {

    auto ac = a * c, ad = a * d, bc = b * c, bd = b * d;

    te[0] = c * e;
    te[4] = bd - ac * f;
    te[8] = bc * f + ad;

    te[1] = f;
    te[5] = a * e;
    te[9] = - b * e;

    te[2] = - d * e;
    te[6] = ad * f + bc;
    te[10] = ac - bd * f;

  } else if ( order == enums::EulerRotationOrder::XZY ) {

    auto ac = a * c, ad = a * d, bc = b * c, bd = b * d;

    te[0] = c * e;
    te[4] = - f;
    te[8] = d * e;

    te[1] = ac * f + bd;
    te[5] = a * e;
    te[9] = ad * f - bc;

    te[2] = bc * f - ad;
    te[6] = b * e;
    te[10] = bd * f + ac;

  }

  // last column
  te[3] = 0;
  te[7] = 0;
  te[11] = 0;

  // bottom row
  te[12] = 0;
  te[13] = 0;
  te[14] = 0;
  te[15] = 1;

  return *this;
}

Matrix4& Matrix4::setRotationFromQuaternion( const Quaternion& q ) {
  //console.warn( 'DEPRECATED: Matrix4\'s .setRotationFromQuaternion() has been deprecated in favor of makeRotationFromQuaternion.  Please update your code.' );
  return makeRotationFromQuaternion( q );
}

Matrix4& Matrix4::makeRotationFromQuaternion( const Quaternion& q ) {

  auto& te = elements;

  auto x = q.x(), y = q.y(), z = q.z(), w = q.w();
  auto x2 = x + x, y2 = y + y, z2 = z + z;
  auto xx = x * x2, xy = x * y2, xz = x * z2;
  auto yy = y * y2, yz = y * z2, zz = z * z2;
  auto wx = w * x2, wy = w * y2, wz = w * z2;

  te[0] = 1.f - ( yy + zz );
  te[4] = xy - wz;
  te[8] = xz + wy;

  te[1] = xy + wz;
  te[5] = 1.f - ( xx + zz );
  te[9] = yz - wx;

  te[2] = xz - wy;
  te[6] = yz + wx;
  te[10] = 1.f - ( xx + yy );

  // last column
  te[3] = 0;
  te[7] = 0;
  te[11] = 0;

  // bottom row
  te[12] = 0;
  te[13] = 0;
  te[14] = 0;
  te[15] = 1;

  return *this;
}

Matrix4& Matrix4::lookAt( const Vector3& eye, const Vector3& target, const Vector3& up ) {

  // @todo private members
  auto x = Vector3();
  auto y = Vector3();
  auto z = Vector3();

  auto& te = elements;

  z.subVectors( eye, target ).normalize();

  if ( z.length() == 0 ) {
    z.z = 1;
  }

  x.crossVectors( up, z ).normalize();

  if ( x.length() == 0 ) {
    z.x += 0.0001f;
    x.crossVectors( up, z ).normalize();
  }

  y.crossVectors( z, x );

  te[0] = x.x;
  te[4] = y.x;
  te[8] = z.x;
  te[1] = x.y;
  te[5] = y.y;
  te[9] = z.y;
  te[2] = x.z;
  te[6] = y.z;
  te[10] = z.z;

  return *this;
}

Matrix4& Matrix4::multiply( const Matrix4& m ) {
  return multiplyMatrices( *this, m );
}

Matrix4& Matrix4::multiplyMatrices( const Matrix4& a, const Matrix4& b ) {

  const auto& ae = a.elements;
  const auto& be = b.elements;
  auto& te = elements;

  auto a11 = ae[0], a12 = ae[4], a13 = ae[8], a14 = ae[12];
  auto a21 = ae[1], a22 = ae[5], a23 = ae[9], a24 = ae[13];
  auto a31 = ae[2], a32 = ae[6], a33 = ae[10], a34 = ae[14];
  auto a41 = ae[3], a42 = ae[7], a43 = ae[11], a44 = ae[15];

  auto b11 = be[0], b12 = be[4], b13 = be[8], b14 = be[12];
  auto b21 = be[1], b22 = be[5], b23 = be[9], b24 = be[13];
  auto b31 = be[2], b32 = be[6], b33 = be[10], b34 = be[14];
  auto b41 = be[3], b42 = be[7], b43 = be[11], b44 = be[15];

  te[0] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
  te[4] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
  te[8] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
  te[12] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;

  te[1] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
  te[5] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
  te[9] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
  te[13] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;

  te[2] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
  te[6] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
  te[10] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
  te[14] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;

  te[3] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
  te[7] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
  te[11] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
  te[15] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;

  return *this;
}

Matrix4& Matrix4::multiplyToArray( const Matrix4& a, const Matrix4& b, Matrix4& r ) {

  const auto& te = elements;
  // @todo check ref passing
  multiplyMatrices( a, b );

  r[ 0 ] = te[0];
  r[ 1 ] = te[1];
  r[ 2 ] = te[2];
  r[ 3 ] = te[3];
  r[ 4 ] = te[4];
  r[ 5 ] = te[5];
  r[ 6 ] = te[6];
  r[ 7 ] = te[7];
  r[ 8 ]  = te[8];
  r[ 9 ]  = te[9];
  r[ 10 ] = te[10];
  r[ 11 ] = te[11];
  r[ 12 ] = te[12];
  r[ 13 ] = te[13];
  r[ 14 ] = te[14];
  r[ 15 ] = te[15];

  return *this;
}

Matrix4& Matrix4::multiplyScalar( float s ) {

  auto& te = elements;

  te[0] *= s;
  te[4] *= s;
  te[8] *= s;
  te[12] *= s;
  te[1] *= s;
  te[5] *= s;
  te[9] *= s;
  te[13] *= s;
  te[2] *= s;
  te[6] *= s;
  te[10] *= s;
  te[14] *= s;
  te[3] *= s;
  te[7] *= s;
  te[11] *= s;
  te[15] *= s;

  return *this;
}

Vector3& Matrix4::multiplyVector3( Vector3& vector ) const {
  //console.warn( 'DEPRECATED: Matrix4\'s .multiplyVector3() has been removed. Use vector.applyMatrix4( matrix ) or vector.applyProjection( matrix ) instead.' );
  return vector.applyProjection( *this );
}

Vector4& Matrix4::multiplyVector4( Vector4& vector ) const {
  //console.warn( 'DEPRECATED: Matrix4\'s .multiplyVector4() has been removed. Use vector.applyMatrix4( matrix ) instead.' );
  return vector.applyMatrix4( *this );
}

std::vector<float>& Matrix4::multiplyVector3Array( std::vector<float>& a) {

  //todo private
  auto v1 = Vector3();
  for ( auto i = a.size(); i < a.size(); i += 3 ) {

    v1.x =  a[i];
    v1.y =  a[i+1];
    v1.z =  a[i+2];

    v1.applyProjection( *this );

    a[i]   = v1.x;
    a[i+1] = v1.y;
    a[i+2] = v1.z;

  }
  return a;
}

Vector3& Matrix4::rotateAxis( Vector3& v ) const {
  //console.warn( 'DEPRECATED: Matrix4\'s .rotateAxis() has been removed. Use Vector3.transformDirection( matrix ) instead.' );
  v.transformDirection( *this );
  return v;
}

Vector3& Matrix4::crossVector( Vector3& vector ) const {
  //console.warn( 'DEPRECATED: Matrix4\'s .crossVector() has been removed. Use vector.applyMatrix4( matrix ) instead.' );
  return vector.applyMatrix4( *this );
}

float Matrix4::determinant() const {

  const auto& te = elements;

  auto n11 = te[0], n12 = te[4], n13 = te[8], n14 = te[12];
  auto n21 = te[1], n22 = te[5], n23 = te[9], n24 = te[13];
  auto n31 = te[2], n32 = te[6], n33 = te[10], n34 = te[14];
  auto n41 = te[3], n42 = te[7], n43 = te[11], n44 = te[15];

  //TODO: make this more efficient
  //( based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm )

  return (
           n41 * (
             +n14 * n23 * n32
             -n13 * n24 * n32
             -n14 * n22 * n33
             +n12 * n24 * n33
             +n13 * n22 * n34
             -n12 * n23 * n34
           ) +
           n42 * (
             +n11 * n23 * n34
             -n11 * n24 * n33
             +n14 * n21 * n33
             -n13 * n21 * n34
             +n13 * n24 * n31
             -n14 * n23 * n31
           ) +
           n43 * (
             +n11 * n24 * n32
             -n11 * n22 * n34
             -n14 * n21 * n32
             +n12 * n21 * n34
             +n14 * n22 * n31
             -n12 * n24 * n31
           ) +
           n44 * (
             -n13 * n22 * n31
             -n11 * n23 * n32
             +n11 * n22 * n33
             +n13 * n21 * n32
             -n12 * n21 * n33
             +n12 * n23 * n31
           )

         );

}

Matrix4& Matrix4::transpose() {

  auto& te = elements;
  float tmp;

  tmp = te[1];
  te[1] = te[4];
  te[4] = tmp;
  tmp = te[2];
  te[2] = te[8];
  te[8] = tmp;
  tmp = te[6];
  te[6] = te[9];
  te[9] = tmp;

  tmp = te[3];
  te[3] = te[12];
  te[12] = tmp;
  tmp = te[7];
  te[7] = te[13];
  te[13] = tmp;
  tmp = te[11];
  te[11] = te[14];
  te[14] = tmp;

  return *this;
}

std::array<float, 16>& Matrix4::flattenToArray( std::array<float, 16>& flat ) const {
  const auto& te = elements;

  flat[ 0 ] = te[0];
  flat[ 1 ] = te[1];
  flat[ 2 ] = te[2];
  flat[ 3 ] = te[3];
  flat[ 4 ] = te[4];
  flat[ 5 ] = te[5];
  flat[ 6 ] = te[6];
  flat[ 7 ] = te[7];
  flat[ 8 ] = te[8];
  flat[ 9 ] = te[9];
  flat[ 10 ] = te[10];
  flat[ 11 ] = te[11];
  flat[ 12 ] = te[12];
  flat[ 13 ] = te[13];
  flat[ 14 ] = te[14];
  flat[ 15 ] = te[15];
  return flat;
}

std::array<float, 16>& Matrix4::flattenToArrayOffset(std::array<float, 16>& flat, const size_t offset ) const {
  // @todo, offset check
  const auto& te = elements;
  flat[ offset ] = te[0];
  flat[ offset + 1 ] = te[1];
  flat[ offset + 2 ] = te[2];
  flat[ offset + 3 ] = te[3];

  flat[ offset + 4 ] = te[4];
  flat[ offset + 5 ] = te[5];
  flat[ offset + 6 ] = te[6];
  flat[ offset + 7 ] = te[7];

  flat[ offset + 8 ]  = te[8];
  flat[ offset + 9 ]  = te[9];
  flat[ offset + 10 ] = te[10];
  flat[ offset + 11 ] = te[11];

  flat[ offset + 12 ] = te[12];
  flat[ offset + 13 ] = te[13];
  flat[ offset + 14 ] = te[14];
  flat[ offset + 15 ] = te[15];

  return flat;
}

Vector3& Matrix4::getPosition() const {

  // @todo priv member
  auto v1 = Vector3();

  const auto& te = elements;
  return v1.set( te[12], te[13], te[14] );
}

Matrix4& Matrix4::setPosition( const Vector3& v ) {
  auto& te = elements;

  te[12] = v.x;
  te[13] = v.y;
  te[14] = v.z;

  return *this;
}

Matrix4& Matrix4::getInverse( const Matrix4& m, bool throwOnInvertible ) {

  // based on http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm
  auto& te = elements;
  const auto& me = m.elements;

  auto n11 = me[0], n12 = me[4], n13 = me[8], n14 = me[12];
  auto n21 = me[1], n22 = me[5], n23 = me[9], n24 = me[13];
  auto n31 = me[2], n32 = me[6], n33 = me[10], n34 = me[14];
  auto n41 = me[3], n42 = me[7], n43 = me[11], n44 = me[15];

  te[0] = n23*n34*n42 - n24*n33*n42 + n24*n32*n43 - n22*n34*n43 - n23*n32*n44 + n22*n33*n44;
  te[4] = n14*n33*n42 - n13*n34*n42 - n14*n32*n43 + n12*n34*n43 + n13*n32*n44 - n12*n33*n44;
  te[8] = n13*n24*n42 - n14*n23*n42 + n14*n22*n43 - n12*n24*n43 - n13*n22*n44 + n12*n23*n44;
  te[12] = n14*n23*n32 - n13*n24*n32 - n14*n22*n33 + n12*n24*n33 + n13*n22*n34 - n12*n23*n34;
  te[1] = n24*n33*n41 - n23*n34*n41 - n24*n31*n43 + n21*n34*n43 + n23*n31*n44 - n21*n33*n44;
  te[5] = n13*n34*n41 - n14*n33*n41 + n14*n31*n43 - n11*n34*n43 - n13*n31*n44 + n11*n33*n44;
  te[9] = n14*n23*n41 - n13*n24*n41 - n14*n21*n43 + n11*n24*n43 + n13*n21*n44 - n11*n23*n44;
  te[13] = n13*n24*n31 - n14*n23*n31 + n14*n21*n33 - n11*n24*n33 - n13*n21*n34 + n11*n23*n34;
  te[2] = n22*n34*n41 - n24*n32*n41 + n24*n31*n42 - n21*n34*n42 - n22*n31*n44 + n21*n32*n44;
  te[6] = n14*n32*n41 - n12*n34*n41 - n14*n31*n42 + n11*n34*n42 + n12*n31*n44 - n11*n32*n44;
  te[10] = n12*n24*n41 - n14*n22*n41 + n14*n21*n42 - n11*n24*n42 - n12*n21*n44 + n11*n22*n44;
  te[14] = n14*n22*n31 - n12*n24*n31 - n14*n21*n32 + n11*n24*n32 + n12*n21*n34 - n11*n22*n34;
  te[3] = n23*n32*n41 - n22*n33*n41 - n23*n31*n42 + n21*n33*n42 + n22*n31*n43 - n21*n32*n43;
  te[7] = n12*n33*n41 - n13*n32*n41 + n13*n31*n42 - n11*n33*n42 - n12*n31*n43 + n11*n32*n43;
  te[11] = n13*n22*n41 - n12*n23*n41 - n13*n21*n42 + n11*n23*n42 + n12*n21*n43 - n11*n22*n43;
  te[15] = n12*n23*n31 - n13*n22*n31 + n13*n21*n32 - n11*n23*n32 - n12*n21*n33 + n11*n22*n33;

  auto det = n11 * te[ 0 ] + n21 * te[ 4 ] + n31 * te[ 8 ] + n41 * te[ 12 ];

  if ( det == 0 ) {

    /*
    //@todo refactor
    auto msg = "Matrix4.getInverse(): can't invert matrix, determinant is 0";
    if ( throwOnInvertible || false ) {
      throw new Error( msg );
    } else {
      console.warn( msg );
    }*/

    identity();

    return *this;
  }

  multiplyScalar( 1 / det );

  return *this;
}

Matrix4& Matrix4::scale( const Vector3& v ) {

  auto& te = elements;
  auto x = v.x, y = v.y, z = v.z;

  te[0] *= x;
  te[4] *= y;
  te[8] *= z;
  te[1] *= x;
  te[5] *= y;
  te[9] *= z;
  te[2] *= x;
  te[6] *= y;
  te[10] *= z;
  te[3] *= x;
  te[7] *= y;
  te[11] *= z;

  return *this;
}

float Matrix4::getMaxScaleOnAxis() const {

  const auto& te = elements;

  auto scaleXSq = te[0] * te[0] + te[1] * te[1] + te[2] * te[2];
  auto scaleYSq = te[4] * te[4] + te[5] * te[5] + te[6] * te[6];
  auto scaleZSq = te[8] * te[8] + te[9] * te[9] + te[10] * te[10];

  return Math::sqrt( Math::max( scaleXSq, Math::max( scaleYSq, scaleZSq ) ) );

}

Matrix4& Matrix4::makeTranslation( float x, float y, float z ) {
  set(
    1, 0, 0, x,
    0, 1, 0, y,
    0, 0, 1, z,
    0, 0, 0, 1
  );
  return *this;
}

Matrix4& Matrix4::makeRotationX( float theta ) {
  auto c = Math::cos( theta ), s = Math::sin( theta );
  set(
    1, 0,  0, 0,
    0, c, -s, 0,
    0, s,  c, 0,
    0, 0,  0, 1
  );
  return *this;
}

Matrix4& Matrix4::makeRotationY( float theta ) {

  auto c = Math::cos( theta ), s = Math::sin( theta );
  set(
    c, 0, s, 0,
    0, 1, 0, 0,
    -s, 0, c, 0,
    0, 0, 0, 1
  );
  return *this;
}

Matrix4& Matrix4::makeRotationZ( float theta ) {

  auto c = Math::cos( theta ), s = Math::sin( theta );
  set(
    c, -s, 0, 0,
    s,  c, 0, 0,
    0,  0, 1, 0,
    0,  0, 0, 1
  );
  return *this;
}

Matrix4& Matrix4::makeRotationAxis( const Vector3& axis, float angle ) {
  // Based on http://www.gamedev.net/reference/articles/article1199.asp
  auto c = Math::cos( angle );
  auto s = Math::sin( angle );
  auto t = 1 - c;
  auto x = axis.x, y = axis.y, z = axis.z;
  auto tx = t * x, ty = t * y;

  set(
    tx * x + c, tx * y - s * z, tx * z + s * y, 0,
    tx * y + s * z, ty * y + c, ty * z - s * x, 0,
    tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
    0, 0, 0, 1
  );

  return *this;
}

Matrix4& Matrix4::makeScale( float x, float y, float z ) {
  set(
    x, 0, 0, 0,
    0, y, 0, 0,
    0, 0, z, 0,
    0, 0, 0, 1
  );
  return *this;
}

Matrix4& Matrix4::compose( const Vector3& position, const Quaternion& quaternion, const Vector3& scale ) {
  this->makeRotationFromQuaternion( quaternion );
  this->scale( scale );
  this->setPosition( position );
  return *this;
}

Matrix4& Matrix4::decompose( Vector3& position, Quaternion& quaternion, Vector3& scale ) {

  // @todo priv members
  auto vector = Vector3();
  auto matrix = Matrix4();

  const auto& te = elements;

  auto sx = vector.set( te[0], te[1], te[2] ).length();
  auto sy = vector.set( te[4], te[5], te[6] ).length();
  auto sz = vector.set( te[8], te[9], te[10] ).length();

  // if determine is negative, we need to invert one scale
  auto det = determinant();
  if( det < 0 ) {
    sx = -sx;
  }

  position.x = te[12];
  position.y = te[13];
  position.z = te[14];

  // scale the rotation part

  std::copy(std::begin(elements), std::end(elements), std::begin(matrix.elements));

  auto invSX = 1 / sx;
  auto invSY = 1 / sy;
  auto invSZ = 1 / sz;

  matrix.elements[0] *= invSX;
  matrix.elements[1] *= invSX;
  matrix.elements[2] *= invSX;

  matrix.elements[4] *= invSY;
  matrix.elements[5] *= invSY;
  matrix.elements[6] *= invSY;

  matrix.elements[8] *= invSZ;
  matrix.elements[9] *= invSZ;
  matrix.elements[10] *= invSZ;

  quaternion.setFromRotationMatrix( matrix );

  scale.x = sx;
  scale.y = sy;
  scale.z = sz;

  return *this;
}

Matrix4& Matrix4::makeFrustum( float left, float right, float bottom, float top, float near, float far ) {

  auto& te = elements;
  auto x = 2 * near / ( right - left );
  auto y = 2 * near / ( top - bottom );

  auto a = ( right + left ) / ( right - left );
  auto b = ( top + bottom ) / ( top - bottom );
  auto c = - ( far + near ) / ( far - near );
  auto d = - 2 * far * near / ( far - near );

  te[0] = x;
  te[4] = 0;
  te[8] = a;
  te[12] = 0;
  te[1] = 0;
  te[5] = y;
  te[9] = b;
  te[13] = 0;
  te[2] = 0;
  te[6] = 0;
  te[10] = c;
  te[14] = d;
  te[3] = 0;
  te[7] = 0;
  te[11] = - 1;
  te[15] = 0;

  return *this;
}

Matrix4& Matrix4::makePerspective( float fov, float aspect, float near, float far ) {

  auto ymax = near * Math::tan( Math::degToRad( fov * 0.5f ) );
  auto ymin = - ymax;
  auto xmin = ymin * aspect;
  auto xmax = ymax * aspect;

  return makeFrustum( xmin, xmax, ymin, ymax, near, far );
}

Matrix4& Matrix4::makeOrthographic( float left, float right, float top, float bottom, float near, float far ) {

  auto& te = elements;
  auto w = right - left;
  auto h = top - bottom;
  auto p = far - near;

  auto x = ( right + left ) / w;
  auto y = ( top + bottom ) / h;
  auto z = ( far + near ) / p;

  te[0] = 2 / w;
  te[4] = 0;
  te[8] = 0;
  te[12] = -x;
  te[1] = 0;
  te[5] = 2 / h;
  te[9] = 0;
  te[13] = -y;
  te[2] = 0;
  te[6] = 0;
  te[10] = -2/p;
  te[14] = -z;
  te[3] = 0;
  te[7] = 0;
  te[11] = 0;
  te[15] = 1;

  return *this;
}

} // namespace three

#endif // THREE_MATRIX4_CPP