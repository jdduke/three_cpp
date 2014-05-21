#ifndef THREE_MATRIX3_CPP
#define THREE_MATRIX3_CPP

#include <three/math/matrix3.h>

#include <three/math/matrix4.h>
#include <three/math/vector3.h>
#include <three/math/math.h>

namespace three {

Matrix3& Matrix3::getInverse( const Matrix4& m, bool throwOnInvertable ) {

  // input: THREE.Matrix4
  // ( based on http://code.google.com/p/webgl-mjs/ )

  const auto& me = m.elements;

  const auto a11 =   me[10] * me[5] - me[6] * me[9];
  const auto a21 = - me[10] * me[1] + me[2] * me[9];
  const auto a31 =   me[6] * me[1] - me[2] * me[5];
  const auto a12 = - me[10] * me[4] + me[6] * me[8];
  const auto a22 =   me[10] * me[0] - me[2] * me[8];
  const auto a32 = - me[6] * me[0] + me[2] * me[4];
  const auto a13 =   me[9] * me[4] - me[5] * me[8];
  const auto a23 = - me[9] * me[0] + me[1] * me[8];
  const auto a33 =   me[5] * me[0] - me[1] * me[4];

  const auto det = me[0] * a11 + me[1] * a12 + me[2] * a13;

  // no inverse

  if ( det == 0.f  && throwOnInvertable) {

      throw -1;

  }

  const auto idet = 1.0f / det;

  te[ 0 ] = idet * a11;
  te[ 1 ] = idet * a21;
  te[ 2 ] = idet * a31;
  te[ 3 ] = idet * a12;
  te[ 4 ] = idet * a22;
  te[ 5 ] = idet * a32;
  te[ 6 ] = idet * a13;
  te[ 7 ] = idet * a23;
  te[ 8 ] = idet * a33;

  return *this;

}

} // namespace three

#endif // THREE_MATRIX3_H