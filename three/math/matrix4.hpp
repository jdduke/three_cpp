#ifndef THREE_MATRIX4_HPP
#define THREE_MATRIX4_HPP

#include <three/common.hpp>

#include <three/math/math.hpp>
#include <three/math/vector4.hpp>

namespace three {
    
class Matrix4 {
public:

  union {
    float elements[16];
    float te[16];
  };

  THREE_DECL Matrix4();
  THREE_DECL Matrix4( float n11, float n12, float n13, float n14,
                     float n21, float n22, float n23, float n24,
                     float n31, float n32, float n33, float n34,
                     float n41, float n42, float n43, float n44 );

  THREE_DECL Matrix4( const Matrix4& other );
  THREE_DECL Matrix4& operator= ( const Matrix4& other );
  THREE_DECL float& operator[]( const int i ) { return elements[i]; }
  THREE_DECL const float operator[]( const int i ) const { return elements[i]; }

  THREE_DECL Matrix4& set( float n11, float n12, float n13, float n14,
                           float n21, float n22, float n23, float n24,
                           float n31, float n32, float n33, float n34,
                           float n41, float n42, float n43, float n44 );

  THREE_DECL Matrix4& identity();
  THREE_DECL Matrix4& copy ( const Matrix4& m );
  THREE_DECL Matrix4& extractPosition( const Matrix4& m );
  THREE_DECL Matrix4& copyPosition( const Matrix4& m );
  THREE_DECL Matrix4& extractRotation( const Matrix4& m );
  THREE_DECL Matrix4& makeRotationFromEuler( const Euler& euler );
  THREE_DECL Matrix4& setRotationFromQuaternion( const Quaternion& q );
  THREE_DECL Matrix4& makeRotationFromQuaternion( const Quaternion& q );
  THREE_DECL Matrix4& lookAt( const Vector3& eye, const Vector3& target, const Vector3& up );
  THREE_DECL Matrix4& multiply( const Matrix4& m );
  THREE_DECL Matrix4& multiplyMatrices( const Matrix4& a, const Matrix4& b );
  THREE_DECL Matrix4& multiplyToArray( const Matrix4& a, const Matrix4& b, Matrix4& r );
  THREE_DECL Matrix4& multiplyScalar( float s );
  THREE_DECL Vector3& multiplyVector3( Vector3& vector ) const;
  THREE_DECL Vector4& multiplyVector4( Vector4& vector ) const;
  THREE_DECL std::vector<float>& multiplyVector3Array( std::vector<float>& a);
  THREE_DECL Vector3& rotateAxis( Vector3& v ) const;
  THREE_DECL Vector3& crossVector( Vector3& vector ) const;
  THREE_DECL float determinant() const;
  THREE_DECL Matrix4& transpose();
  THREE_DECL std::vector<float>& flattenToArray( std::vector<float>& flat ) const;
  THREE_DECL std::vector<float>& flattenToArrayOffset( std::vector<float>& flat, const size_t offset ) const;
  THREE_DECL Vector3& getPosition() const;
  THREE_DECL Matrix4& setPosition( const Vector3& v );
  THREE_DECL Matrix4& getInverse( const Matrix4& m, bool throwOnInvertible = false );
  THREE_DECL Matrix4& scale( const Vector3& v );
  THREE_DECL float getMaxScaleOnAxis() const;
  THREE_DECL Matrix4& makeTranslation( float x, float y, float z );
  THREE_DECL Matrix4& makeRotationX( float theta );
  THREE_DECL Matrix4& makeRotationY( float theta );
  THREE_DECL Matrix4& makeRotationZ( float theta );
  THREE_DECL Matrix4& makeRotationAxis( const Vector3& axis, float angle );
  THREE_DECL Matrix4& makeScale( float x, float y, float z );
  THREE_DECL Matrix4& compose( const Vector3& position, const Quaternion& quaternion, const Vector3& scale );
  THREE_DECL Matrix4& decompose( Vector3& position, Quaternion& quaternion, Vector3& scale );
  THREE_DECL Matrix4& makeFrustum( float left, float right, float bottom, float top, float near, float far );
  THREE_DECL Matrix4& makePerspective( float fov, float aspect, float near, float far );
  THREE_DECL Matrix4& makeOrthographic( float left, float right, float top, float bottom, float near, float far );

  Matrix4 clone() {
    return *this;
  }

};

static_assert( sizeof( Matrix4 ) == sizeof( float ) * 16, "Invalid Matrix4 storage size" );

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/matrix4.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATRIX4_HPP