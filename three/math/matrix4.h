#ifndef THREE_MATRIX4_H
#define THREE_MATRIX4_H

#include <three/common.h>
#include <three/math/math.h>

#include <three/utils/memory.h>

#include <array>
#include <vector>

namespace three {

class Matrix4 {
public:

  union {
    float elements[16];
    float te[16];
  };

  Matrix4();

  Matrix4( float n11, float n12, float n13, float n14,
           float n21, float n22, float n23, float n24,
           float n31, float n32, float n33, float n34,
           float n41, float n42, float n43, float n44 );

  Matrix4( const Matrix4& other );

  Matrix4& operator= ( const Matrix4& other );

  float& operator[]( const int i ) {
    return elements[i];
  }

  const float operator[]( const int i ) const {
    return elements[i];
  }

  Matrix4& set( float n11, float n12, float n13, float n14,
                float n21, float n22, float n23, float n24,
                float n31, float n32, float n33, float n34,
                float n41, float n42, float n43, float n44 );

  Matrix4& identity();

  Matrix4& copy ( const Matrix4& m );

  Matrix4& extractPosition( const Matrix4& m );

  Matrix4& copyPosition( const Matrix4& m );

  Matrix4& extractRotation( const Matrix4& m );

  Matrix4& makeRotationFromEuler( const Euler& euler );

  Matrix4& setRotationFromQuaternion( const Quaternion& q );

  Matrix4& makeRotationFromQuaternion( const Quaternion& q );

  Matrix4& lookAt( const Vector3& eye, const Vector3& target, const Vector3& up );

  Matrix4& multiply( const Matrix4& m );

  Matrix4& multiplyMatrices( const Matrix4& a, const Matrix4& b );

  Matrix4& multiplyToArray( const Matrix4& a, const Matrix4& b, Matrix4& r );

  Matrix4& multiplyScalar( float s );

  Vector3& multiplyVector3( Vector3& vector ) const;

  Vector4& multiplyVector4( Vector4& vector ) const;

  std::vector<float>& multiplyVector3Array( std::vector<float>& a);

  Vector3& rotateAxis( Vector3& v ) const;

  Vector3& crossVector( Vector3& vector ) const;

  float determinant() const;

  Matrix4& transpose();

  std::array<float, 16>& flattenToArray( std::array<float, 16>& flat ) const;

  std::array<float, 16>& flattenToArrayOffset( std::array<float, 16>& flat, const size_t offset ) const;

  Vector3& getPosition() const;

  Matrix4& setPosition( const Vector3& v );

  Matrix4& getInverse( const Matrix4& m, bool throwOnInvertible = false );

  Matrix4& scale( const Vector3& v );

  float getMaxScaleOnAxis() const;

  Matrix4& makeTranslation( float x, float y, float z );

  Matrix4& makeRotationX( float theta );

  Matrix4& makeRotationY( float theta );

  Matrix4& makeRotationZ( float theta );

  Matrix4& makeRotationAxis( const Vector3& axis, float angle );

  Matrix4& makeScale( float x, float y, float z );

  Matrix4& compose( const Vector3& position, const Quaternion& quaternion, const Vector3& scale );

  Matrix4& decompose( Vector3& position, Quaternion& quaternion, Vector3& scale );

  Matrix4& makeFrustum( float left, float right, float bottom, float top, float near, float far );

  Matrix4& makePerspective( float fov, float aspect, float near, float far );

  Matrix4& makeOrthographic( float left, float right, float top, float bottom, float near, float far );

  Matrix4 clone() {
    return *this;
  }

};

static_assert( sizeof( Matrix4 ) == sizeof( float ) * 16, "Invalid Matrix4 storage size" );

} // namespace three

#endif // THREE_MATRIX4_H