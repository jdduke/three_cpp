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

  THREE_DECL Matrix4& set( float n11, float n12, float n13, float n14,
                           float n21, float n22, float n23, float n24,
                           float n31, float n32, float n33, float n34,
                           float n41, float n42, float n43, float n44 );

  THREE_DECL Matrix4& identity();
  THREE_DECL Matrix4& copy( const Matrix4& m );

  THREE_DECL Matrix4& lookAt( const Vector3& eye, const Vector3& target, const Vector3& up );
  THREE_DECL Matrix4& multiply( const Matrix4& a, const Matrix4& b );

  THREE_DECL Matrix4& multiplySelf( const Matrix4& m );
  THREE_DECL Matrix4& multiplyToArray( const Matrix4& a, const Matrix4& b, float* r );
  THREE_DECL Matrix4& multiplyScalar( float s );

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL Vector3 multiplyVector3( const Vector3& v ) const;
  THREE_DECL Vector4 multiplyVector4( const Vector4& v ) const;
  THREE_DECL void multiplyVector3( Vector3& v ) const;
  THREE_DECL void multiplyVector4( Vector4& v ) const;
  THREE_DECL std::vector<float>& multiplyVector3Array( std::vector<float>& a ) const;

  THREE_DECL Vector3& rotateAxis( Vector3& v ) const;
  THREE_DECL Vector4 crossVector( Vector4 a ) const;
  THREE_DECL float determinant() const;
  THREE_DECL Matrix4& transpose();

  THREE_DECL Vector3    getScale() const;
  THREE_DECL Vector3    getPosition() const;
  THREE_DECL Quaternion getRotation() const;
  THREE_DECL Vector3    getEulerRotation( THREE::EulerRotationOrder order = THREE::XYZ ) const;
  THREE_DECL Vector4    getAxisAngle( ) const;

  Vector3 getColumnX() const;
  Vector3 getColumnY() const;
  Vector3 getColumnZ() const;

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL Matrix4& getInverse( const Matrix4& m );

  THREE_DECL Matrix4& setRotationFromEuler( const Vector3& v, THREE::EulerRotationOrder order = THREE::XYZ );
  THREE_DECL Matrix4& setRotationFromQuaternion( const Quaternion& q );

  THREE_DECL Matrix4& compose( const Vector3& translation, const Quaternion& rotation, const Vector3& scale );

  THREE_DECL void decompose( Vector3& translation, Quaternion& rotation, Vector3& scale );

  THREE_DECL Matrix4& extractPosition( const Matrix4& m );
  THREE_DECL Matrix4& extractRotation( const Matrix4& m );

  Matrix4& setPosition( const Vector3& v );

  THREE_DECL Matrix4& translate( const Vector3& v );
  THREE_DECL Matrix4& rotateX( float angle );
  THREE_DECL Matrix4& rotateY( float angle );
  THREE_DECL Matrix4& rotateZ( float angle );
  THREE_DECL Matrix4& rotateByAxis( const Vector3& axis, float angle );
  THREE_DECL Matrix4& scale( const Vector3& v );

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL float getMaxScaleOnAxis() const;

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL Matrix4& makeTranslation( float x, float y, float z );
  THREE_DECL Matrix4& makeRotationX( float theta );
  THREE_DECL Matrix4& makeRotationY( float theta );
  THREE_DECL Matrix4& makeRotationZ( float theta );
  THREE_DECL Matrix4& makeRotationAxis( const Vector3& axis, float angle );
  THREE_DECL Matrix4& makeScale( float x, float y, float z );
  THREE_DECL Matrix4& makeFrustum( float left, float right, float bottom, float top, float near, float far );
  THREE_DECL Matrix4& makePerspective( float fov, float aspect, float near, float far );
  THREE_DECL Matrix4& makeOrthographic( float left, float right, float top, float bottom, float near, float far );

  Matrix4 clone() const {
    return Matrix4( *this );
  }

  /////////////////////////////////////////////////////////////////////////


  template < typename T >
  T& flattenToArray( T& flat ) const {

    flat[ 0 ] = te[0]; flat[ 1 ] = te[1]; flat[ 2 ] = te[2]; flat[ 3 ] = te[3];
    flat[ 4 ] = te[4]; flat[ 5 ] = te[5]; flat[ 6 ] = te[6]; flat[ 7 ] = te[7];
    flat[ 8 ]  = te[8]; flat[ 9 ]  = te[9]; flat[ 10 ] = te[10]; flat[ 11 ] = te[11];
    flat[ 12 ] = te[12]; flat[ 13 ] = te[13]; flat[ 14 ] = te[14]; flat[ 15 ] = te[15];

    return flat;

  }

  template < typename T >
  T& flattenToArrayOffset( T& flat, size_t offset ) const {

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

};

static_assert( sizeof( Matrix4 ) == sizeof( float ) * 16, "Invalid Matrix4 storage size" );

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/core/impl/matrix4.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATRIX4_HPP