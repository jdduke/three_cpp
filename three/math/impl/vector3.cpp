#ifndef THREE_VECTOR3_CPP
#define THREE_VECTOR3_CPP

#include <three/math/vector3.h>
#include <three/math/vector4.h>
#include <three/math/matrix3.h>
#include <three/math/matrix4.h>
#include <three/math/quaternion.h>

namespace three {

Vector3& Vector3::applyMatrix3( const Matrix3& m ) {

    auto x = this->x, y = this->y, z = this->z;
    
    const auto& e = m.elements;
    
    this->x = e[0] * x + e[3] * y + e[6] * z;
    this->y = e[1] * x + e[4] * y + e[7] * z;
    this->z = e[2] * x + e[5] * y + e[8] * z;
    
    return *this;
}

Vector3& Vector3::applyMatrix4( const Matrix4& m ) {

    // input: THREE.Matrix4 affine matrix
    
    auto x = this->x, y = this->y, z = this->z;
    
    const auto& e = m.elements;
    
    this->x = e[0] * x + e[4] * y + e[8]  * z + e[12];
    this->y = e[1] * x + e[5] * y + e[9]  * z + e[13];
    this->z = e[2] * x + e[6] * y + e[10] * z + e[14];
    
    return *this;

}

Vector3& Vector3::applyProjection( const Matrix4& m ) {

    // input: THREE.Matrix4 projection matrix
    
    auto x = this->x, y = this->y, z = this->z;
    
    const auto& e = m.elements;
    auto d = 1.f / ( e[3] * x + e[7] * y + e[11] * z + e[15] ); // perspective divide
    
    this->x = ( e[0] * x + e[4] * y + e[8]  * z + e[12] ) * d;
    this->y = ( e[1] * x + e[5] * y + e[9]  * z + e[13] ) * d;
    this->z = ( e[2] * x + e[6] * y + e[10] * z + e[14] ) * d;
    
    return *this;

}

Vector3& Vector3::applyQuaternion( const Quaternion& q ) {

    auto x = this->x, y = this->y, z = this->z;
    
    auto qx = q.x;
    auto qy = q.y;
    auto qz = q.z;
    auto qw = q.w;
    
    // calculate quat * vector
    
    auto ix =  qw * x + qy * z - qz * y;
    auto iy =  qw * y + qz * x - qx * z;
    auto iz =  qw * z + qx * y - qy * x;
    auto iw = -qx * x - qy * y - qz * z;
    
    // calculate result * inverse quat
    
    this->x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    this->y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    this->z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
    
    return *this;

}

Vector3& Vector3::copy( const Vector4& v ) {

    x = v.x;
    y = v.y;
    z = v.z;

    return *this;

}

Vector3& Vector3::transformDirection( const Matrix4& m ) {

    // input: THREE.Matrix4 affine matrix
    // vector interpreted as a direction
    
    auto x = this->x, y = this->y, z = this->z;
    
    const auto& e = m.elements;
    
    this->x = e[0] * x + e[4] * y + e[8]  * z;
    this->y = e[1] * x + e[5] * y + e[9]  * z;
    this->z = e[2] * x + e[6] * y + e[10] * z;
    
    normalize();
    
    return *this;

}

Vector3& Vector3::setFromMatrixPosition( const Matrix4& m ) {

  x = m.elements[ 12 ];
  y = m.elements[ 13 ];
  z = m.elements[ 14 ];

  return *this;

}

Vector3& Vector3::setFromMatrixScale( const Matrix4& m ) {

  auto sx = set( m.elements[ 0 ], m.elements[ 1 ], m.elements[  2 ] ).length();
  auto sy = set( m.elements[ 4 ], m.elements[ 5 ], m.elements[  6 ] ).length();
  auto sz = set( m.elements[ 8 ], m.elements[ 9 ], m.elements[ 10 ] ).length();

  x = sx;
  y = sy;
  z = sz;

  return *this;

}

Vector3& Vector3::setFromMatrixColumn( size_t index, const Matrix4& matrix ) {

  auto offset = index * 4;

  x = matrix.elements[ offset ];
  y = matrix.elements[ offset + 1 ];
  z = matrix.elements[ offset + 2 ];

  return *this;

}

Vector3& Vector3::applyEuler( const Euler& euler ) {

  applyQuaternion( Quaternion().setFromEuler( euler ) );

  return *this;

}

Vector3& Vector3::applyAxisAngle( const Vector3& axis, float angle ) {

  applyQuaternion( Quaternion().setFromAxisAngle( axis, angle ) );

  return *this;

}

} // namespace three

#endif // THREE_VECTOR3_CPP