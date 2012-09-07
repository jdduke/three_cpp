#ifndef THREE_VECTOR4_HPP
#define THREE_VECTOR4_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>

namespace three {

class Vector4 {
public:

    union {
        struct { float x, y, z, w; };
        float xyzw[4];
    };

    Vector4() : x ( 0 ), y ( 0 ), z ( 0 ), w ( 1.f ) { }
    Vector4 ( float xIn, float yIn, float zIn, float wIn = 1.f ) : x ( xIn ), y ( yIn ), z ( zIn ), w ( wIn ) { }
    Vector4 ( const Vector4& v ) : x ( v.x ), y ( v.y ), z ( v.z ), w ( v.w ) { }
    Vector4& operator= ( const Vector4& v ) { copy ( v ); }

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

    Vector4& add ( const Vector4& a, const Vector4& b ) {
        x = a.x + b.x;
        y = a.y + b.y;
        z = a.z + b.z;
        w = a.w + b.w;
        return *this;
    }

    Vector4& addSelf ( const Vector4& v ) {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
        return *this;
    }

    Vector4& sub ( const Vector4& a, const Vector4& b ) {
        x = a.x - b.x;
        y = a.y - b.y;
        z = a.z - b.z;
        w = a.w - b.w;
        return *this;
    }

    Vector4& subSelf ( const Vector4& v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
        return *this;
    }

    Vector4& multiplyScalar ( float s ) {
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
            return set ( 0, 0, 0, 1 );
        }
    }

    Vector4& negate() {
        return multiplyScalar ( -1.f );
    }

    float dot ( const Vector4& v ) {
        return x * v.x + y * v.y + z * v.z + w + v.w;
    }

    float lengthSq() const {
        return dot ( *this );
    }

    float length() const {
        return Math.sqrt ( lengthSq() );
    }

    Vector4& normalize() {
        return divideScalar ( length() );
    }

    Vector4& setLength ( float l ) {
        return normalize().multiplyScalar ( l );
    }

    Vector4& lerpSelf ( const Vector4& v, float alpha ) {
        x += ( v.x - x ) * alpha;
        y += ( v.y - y ) * alpha;
        z += ( v.z - z ) * alpha;
        w += ( v.w - w ) * alpha;
        return *this;
    }

    bool equals ( const Vector4& v ) {
        return ( ( v.x == x ) && ( v.y == y ) && ( v.z == z ) );
    }

    Vector4 clone() {
        return *this;
    }

    Vector4& setAxisAngleFromQuaternion ( const Quaternion& q ) {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/index.htm
        // q is assumed to be normalized
        w = 2 * Math.acos ( q.w );

        auto s = Math.sqrt ( 1.f - q.w * q.w );

        if ( s < 0.0001f ) {
            x = 1;
            y = 0;
            z = 0;
        } else {
            x = q.x / s;
            y = q.y / s;
            z = q.z / s;
        }
        return *this;
    }

    Vector4& setAxisAngleFromRotationMatrix ( const Matrix4& m ) {
        // http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToAngle/index.htm
        // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)

        auto& te = m.elements;

        float angle, x, y, z,  // variables for result
              epsilon = 0.01f,   // margin to allow for rounding errors
              epsilon2 = 0.1f,   // margin to distinguish between 0 and 180 degrees
              m11 = te[0], m12 = te[4], m13 = te[8],
              m21 = te[1], m22 = te[5], m23 = te[9],
              m31 = te[2], m32 = te[6], m33 = te[10];

        if ( ( Math.abs ( m12 - m21 ) < epsilon )
                && ( Math.abs ( m13 - m31 ) < epsilon )
                && ( Math.abs ( m23 - m32 ) < epsilon ) ) {

            // singularity found
            // first check for identity matrix which must have +1 for all terms
            // in leading diagonal and zero in other terms

            if ( ( Math.abs ( m12 + m21 ) < epsilon2 )
                    && ( Math.abs ( m13 + m31 ) < epsilon2 )
                    && ( Math.abs ( m23 + m32 ) < epsilon2 )
                    && ( Math.abs ( m11 + m22 + m33 - 3 ) < epsilon2 ) ) {

                // this singularity is identity matrix so angle = 0
                this.set ( 1, 0, 0, 0 );
                return *this; // zero angle, arbitrary axis
            }

            // otherwise this singularity is angle = 180
            angle = Math.PI;

            var xx = ( m11 + 1.f ) / 2.f;
            var yy = ( m22 + 1.f ) / 2.f;
            var zz = ( m33 + 1.f ) / 2.f;
            var xy = ( m12 + m21 ) / 4.f;
            var xz = ( m13 + m31 ) / 4.f;
            var yz = ( m23 + m32 ) / 4.f;

            if ( ( xx > yy ) && ( xx > zz ) ) { // m11 is the largest diagonal term
                if ( xx < epsilon ) {
                    x = 0;
                    y = 0.707106781f;
                    z = 0.707106781f;
                } else {
                    x = Math.sqrt ( xx );
                    y = xy / x;
                    z = xz / x;
                }
            } else if ( yy > zz ) { // m22 is the largest diagonal term
                if ( yy < epsilon ) {
                    x = 0.707106781f;
                    y = 0;
                    z = 0.707106781f;
                } else {
                    y = Math.sqrt ( yy );
                    x = xy / y;
                    z = yz / y;
                }
            } else { // m33 is the largest diagonal term so base result on this
                if ( zz < epsilon ) {
                    x = 0.707106781f;
                    y = 0.707106781f;
                    z = 0;
                } else {
                    z = Math.sqrt ( zz );
                    x = xz / z;
                    y = yz / z;
                }
            }

            set ( x, y, z, angle );
            return *this; // return 180 deg rotation
        }

        // as we have reached here there are no singularities so we can handle normally
        auto s = Math.sqrt ( ( m32 - m23 ) * ( m32 - m23 )
                             + ( m13 - m31 ) * ( m13 - m31 )
                             + ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize

        if ( Math.abs ( s ) < 0.001f ) s = 1;

        // prevent divide by zero, should not happen if matrix is orthogonal and should be
        // caught by singularity test above, but I've left it in just in case

        this.x = ( m32 - m23 ) / s;
        this.y = ( m13 - m31 ) / s;
        this.z = ( m21 - m12 ) / s;
        this.w = Math.acos ( ( m11 + m22 + m33 - 1 ) / 2 );

        return *this;
    }
};

} // namespace three

#endif // THREE_VECTOR4_HPP