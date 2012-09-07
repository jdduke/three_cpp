#ifndef THREE_QUATERNION_HPP
#define THREE_QUATERNION_HPP

#include <three/common.hpp>

#include <cmath>

namespace three {

class Quaternion {
public:

    union {
        struct { float x, y, z, w; };
        float xyzw[4];
    };

    Quaternion() : x ( 0 ), y ( 0 ), z ( 0 ), w ( 1.f ) { }
    Quaternion ( float xIn, float yIn, float zIn, float wIn = 1.f ) : x ( xIn ), y ( yIn ), z ( zIn ), w ( wIn ) { }
    Quaternion ( const Quaternion& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( v.w ) { }
    Quaternion& operator= ( const Quaternion& v ) { copy ( v ); }

    Vector4& set ( float xIn, float yIn, float zIn, float wIn ) {
        x = xIn;
        y = yIn;
        z = zIn;
        w = wIn;
        return *this;
    }

    Vector4& copy ( const Vector4& v ) {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }

    Quaternion& setFromEuler ( const Vector3& v, Order order = XYZ ) {

        // http://www.mathworks.com/matlabcentral/fileexchange/
        //  20696-function-to-convert-between-dcm-euler-angles-quaternions-and-euler-vectors/
        //  content/SpinCalc.m

        auto c1 = Math.cos ( v.x / 2 );
        auto c2 = Math.cos ( v.y / 2 );
        auto c3 = Math.cos ( v.z / 2 );
        auto s1 = Math.sin ( v.x / 2 );
        auto s2 = Math.sin ( v.y / 2 );
        auto s3 = Math.sin ( v.z / 2 );

        if ( order == XYZ ) {

            x = s1 * c2 * c3 + c1 * s2 * s3;
            y = c1 * s2 * c3 - s1 * c2 * s3;
            z = c1 * c2 * s3 + s1 * s2 * c3;
            w = c1 * c2 * c3 - s1 * s2 * s3;

        } else if ( order == YXZ ) {

            x = s1 * c2 * c3 + c1 * s2 * s3;
            y = c1 * s2 * c3 - s1 * c2 * s3;
            z = c1 * c2 * s3 - s1 * s2 * c3;
            w = c1 * c2 * c3 + s1 * s2 * s3;

        } else if ( order == ZXY ) {

            x = s1 * c2 * c3 - c1 * s2 * s3;
            y = c1 * s2 * c3 + s1 * c2 * s3;
            z = c1 * c2 * s3 + s1 * s2 * c3;
            w = c1 * c2 * c3 - s1 * s2 * s3;

        } else if ( order == ZYX ) {

            x = s1 * c2 * c3 - c1 * s2 * s3;
            y = c1 * s2 * c3 + s1 * c2 * s3;
            z = c1 * c2 * s3 - s1 * s2 * c3;
            w = c1 * c2 * c3 + s1 * s2 * s3;

        } else if ( order == YZX ) {

            x = s1 * c2 * c3 + c1 * s2 * s3;
            y = c1 * s2 * c3 + s1 * c2 * s3;
            z = c1 * c2 * s3 - s1 * s2 * c3;
            w = c1 * c2 * c3 - s1 * s2 * s3;

        } else if ( order == XZY ) {

            x = s1 * c2 * c3 - c1 * s2 * s3;
            y = c1 * s2 * c3 - s1 * c2 * s3;
            z = c1 * c2 * s3 + s1 * s2 * c3;
            w = c1 * c2 * c3 + s1 * s2 * s3;

        }

        return *this;
    }

    Quaternion& setFromAxisAngle ( const Vector3& axis, float angle ) {
        // from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/index.htm
        // axis have to be normalized

        float halfAngle = angle / 2.f;
        float s = Math.sin ( halfAngle );

        x = axis.x * s;
        y = axis.y * s;
        z = axis.z * s;
        w = Math.cos ( halfAngle );

        return *this;
    }

    Quaternion& setFromRotationMatrix ( const Matrix4& m ) {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

        const auto& te = m.elements;
        float
        m11 = te[0], m12 = te[4], m13 = te[8],
        m21 = te[1], m22 = te[5], m23 = te[9],
        m31 = te[2], m32 = te[6], m33 = te[10],

        trace = m11 + m22 + m33;

        if ( trace > 0 ) {

            float s = 0.5f / Math.sqrt ( trace + 1.0f );

            w = 0.25f / s;
            x = ( m32 - m23 ) * s;
            y = ( m13 - m31 ) * s;
            z = ( m21 - m12 ) * s;

        } else if ( m11 > m22 && m11 > m33 ) {

            float s = 2.0f * Math.sqrt ( 1.0f + m11 - m22 - m33 );

            w = ( m32 - m23 ) / s;
            x = 0.25f * s;
            y = ( m12 + m21 ) / s;
            z = ( m13 + m31 ) / s;

        } else if ( m22 > m33 ) {

            float s = 2.0f * Math.sqrt ( 1.0f + m22 - m11 - m33 );

            w = ( m13 - m31 ) / s;
            x = ( m12 + m21 ) / s;
            y = 0.25f * s;
            z = ( m23 + m32 ) / s;

        } else {

            float s = 2.0f * Math.sqrt ( 1.0f + m33 - m11 - m22 );

            w = ( m21 - m12 ) / s;
            x = ( m13 + m31 ) / s;
            y = ( m23 + m32 ) / s;
            z = 0.25f * s;

        }

        return *this;

    }

    Quaternion& calculateW() {
        w = - Math.sqrt ( Math.abs ( 1.f - x * x - y * y - z * z ) );
        return *this;
    }

    Quaternion& inverse() {
        x *= -1.f;
        y *= -1.f;
        z *= -1.f;
        return *this;
    }

    float length() const {
        return Math.sqrt ( lengthSq() );
    }

    Vector4& normalize() {
        return divideScalar ( length() );
    }

    Quaternion& multiply ( const Quaternion& a, const Quaternion& b ) {
        // from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/code/index.htm
        float qax = a.x, qay = a.y, qaz = a.z, qaw = a.w,
              qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

        x =  qax * qbw + qay * qbz - qaz * qby + qaw * qbx;
        y = -qax * qbz + qay * qbw + qaz * qbx + qaw * qby;
        z =  qax * qby - qay * qbx + qaz * qbw + qaw * qbz;
        w = -qax * qbx - qay * qby - qaz * qbz + qaw * qbw;

        return *this;
    }

    Quaternion& multiplySelf ( const Quaternion& b ) {
        float qax = x,   qay = y,   qaz = z,   qaw = w,
              qbx = b.x, qby = b.y, qbz = b.z, qbw = b.w;

        x = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
        y = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
        z = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
        w = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;

        return *this;
    }

    Vector3 multiplyVector3 ( const Vector3& v ) {
        float qx = x, qy = y, qz = z, qw = w;

        // calculate quat * vector
        float ix =  qw * v.x + qy * v.z - qz * v.y,
              iy =  qw * v.y + qz * v.x - qx * v.z,
              iz =  qw * v.z + qx * v.y - qy * v.x,
              iw = -qx * v.x - qy * v.y - qz * v.z;

        // calculate result * inverse quat
        Vector3 dest;
        dest.x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
        dest.y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
        dest.z = iz * qw + iw * -qz + ix * -qy - iy * -qx;

        return dest;
    }

    Quaternion& slerpSelf ( const Quaternion& qb, float t ) {

        float x = x, y = y, z = z, w = w;

        // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
        auto cosHalfTheta = w * qb.w + x * qb.x + y * qb.y + z * qb.z;

        if ( cosHalfTheta < 0 ) {
            w = -qb.w;
            x = -qb.x;
            y = -qb.y;
            z = -qb.z;

            cosHalfTheta = -cosHalfTheta;
        } else {
            copy ( qb );
        }

        if ( cosHalfTheta >= 1.0f ) {
            w = w;
            x = x;
            y = y;
            z = z;
            return *this;
        }

        auto halfTheta    = Math.acos ( cosHalfTheta );
        auto sinHalfTheta = Math.sqrt ( 1.0f - cosHalfTheta * cosHalfTheta );

        if ( Math.abs ( sinHalfTheta ) < 0.001f ) {
            w = 0.5f * ( w + w );
            x = 0.5f * ( x + x );
            y = 0.5f * ( y + y );
            z = 0.5f * ( z + z );
            return *this;
        }

        auto ratioA = Math.sin ( ( 1. - t ) * halfTheta ) / sinHalfTheta;
        auto ratioB = Math.sin ( t * halfTheta ) / sinHalfTheta;

        w = ( w * ratioA + w * ratioB );
        x = ( x * ratioA + x * ratioB );
        y = ( y * ratioA + y * ratioB );
        z = ( z * ratioA + z * ratioB );

        return *this;
    }

    Quaternion clone() {
        return *this;
    }

private:

    float lengthSq() const {
        return x * x + y * y + z * z + w * w;
    }

    Quaternion& multiplyScalar ( float s ) {
        x *= s;
        y *= s;
        z *= s;
        w *= s;
        return *this;
    }

    Vector4& divideScalar ( float s ) {
        if ( s != 0.f ) {
            return multiplyScalar ( 1.f / s );
        } else {
            return set ( 0, 0, 0, 0 );
        }
    }

};

} // namespace three

#endif // THREE_QUATERNION_HPP