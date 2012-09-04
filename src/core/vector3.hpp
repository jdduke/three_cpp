#ifndef _VECTOR3_HPP_
#define _VECTOR3_HPP_

#include <cmath>

namespace three {

class Vector3 {
public:

    union {
        struct { float x, y, z; };
        float xyz[3];
    };

    Vector3() : x ( 0 ), y ( 0 ), z ( 0 ) { }
    Vector3 ( float xIn, float yIn, float zIn ) : x ( xIn ), y ( yIn ), z ( zIn ) { }
    Vector3 ( const Vector3& v ) : x ( v.x ), y ( v.y ), z ( v.z ) { }
    Vector3& operator= ( const Vector3& v ) { copy ( v ); }

    Vector3& set ( float xIn, float yIn, float zIn ) {
        x = xIn;
        y = yIn;
        z = zIn;
        return *this;
    }

    Vector3& copy ( const Vector3& v ) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    Vector3& add ( const Vector3& a, const Vector3& b ) {
        x = a.x + b.x;
        y = a.y + b.y;
        z = a.z + b.z;
        return *this;
    }

    Vector3& addSelf ( const Vector3& v ) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3& sub ( const Vector3& a, const Vector3& b ) {
        x = a.x - b.x;
        y = a.y - b.y;
        z = a.z - b.z;
        return *this;
    }

    Vector3& subSelf ( const Vector3& v ) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3& multiplyScalar ( float s ) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3& divideScalar ( float s ) {
        if ( s != 0.f ) {
            x /= s;
            y /= s;
            z /= s;
        } else {
            set ( 0, 0, 0 );
        }
        return *this;
    }

    Vector3& negate() {
        return multiplyScalar ( -1.f );
    }

    float dot ( const Vector3& v ) {
        return x * v.x + y * v.y + z * v.z;
    }

    float lengthSq() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return std::sqrt ( lengthSq() );
    }

    float lengthManhattan() const {
        return std::abs ( x ) + std::abs ( y ) + std::abs ( z );
    }

    Vector3& normalize() {
        return divideScalar ( length() );
    }

    float distanceTo ( const Vector3& v ) {
        return std::sqrt ( distanceToSquared ( v ) );
    }

    float distanceToSquared ( const Vector3& v ) {
        return Vector3().sub ( *this, v ).lengthSq();
    }

    Vector3& setLength ( float l ) {
        return normalize().multiplyScalar ( l );
    }

    Vector3& lerpSelf ( const Vector3& v, float alpha ) {
        x += ( v.x - x ) * alpha;
        y += ( v.y - y ) * alpha;
        z += ( v.z - z ) * alpha;
        return *this;
    }

    Vector3& cross ( const Vector3& a, const Vector3& b ) {
        x = a.y * b.z - a.z * b.y;
        y = a.z * b.x - a.x * b.z;
        z = a.x * b.y - a.y * b.x;
        return *this;
    }

    Vector3& crossSelf ( const Vector3& v ) {
        return cross ( Vector3 ( *this ), v );
    }

    Vector3& setEulerFromQuaternion ( const Quaternion& q, Order order = XYZ ) {
        // q is assumed to be normalized

        // clamp, to handle numerical problems
        auto clamp = [] ( float x ) {
            return std::min ( std::max ( x, -1.f ), 1.f );
        }

        auto sqx = q.x * q.x;
        auto sqy = q.y * q.y;
        auto sqz = q.z * q.z;
        auto sqw = q.w * q.w;

        if ( order == XYZ ) {
            this.x = std::atan2 ( 2.f * ( q.x * q.w - q.y * q.z ), ( sqw - sqx - sqy + sqz ) );
            this.y = std::asin ( clamp ( 2.f * ( q.x * q.z + q.y * q.w ) ) );
            this.z = std::atan2 ( 2.f * ( q.z * q.w - q.x * q.y ), ( sqw + sqx - sqy - sqz ) );
        } else if ( order == YXZ ) {
            this.x = std::asin ( clamp ( 2.f * ( q.x * q.w - q.y * q.z ) ) );
            this.y = std::atan2 ( 2.f * ( q.x * q.z + q.y * q.w ), ( sqw - sqx - sqy + sqz ) );
            this.z = std::atan2 ( 2.f * ( q.x * q.y + q.z * q.w ), ( sqw - sqx + sqy - sqz ) );
        } else if ( order == ZXY ) {
            this.x = std::asin ( clamp ( 2.f * ( q.x * q.w + q.y * q.z ) ) );
            this.y = std::atan2 ( 2.f * ( q.y * q.w - q.z * q.x ), ( sqw - sqx - sqy + sqz ) );
            this.z = std::atan2 ( 2.f * ( q.z * q.w - q.x * q.y ), ( sqw - sqx + sqy - sqz ) );
        } else if ( order == ZYX ) {
            this.x = std::atan2 ( 2.f * ( q.x * q.w + q.z * q.y ), ( sqw - sqx - sqy + sqz ) );
            this.y = std::asin ( clamp ( 2.f * ( q.y * q.w - q.x * q.z ) ) );
            this.z = std::atan2 ( 2.f * ( q.x * q.y + q.z * q.w ), ( sqw + sqx - sqy - sqz ) );
        } else if ( order == YZX ) {
            this.x = std::atan2 ( 2.f * ( q.x * q.w - q.z * q.y ), ( sqw - sqx + sqy - sqz ) );
            this.y = std::atan2 ( 2.f * ( q.y * q.w - q.x * q.z ), ( sqw + sqx - sqy - sqz ) );
            this.z = std::asin ( clamp ( 2.f * ( q.x * q.y + q.z * q.w ) ) );
        } else if ( order == XZY ) {
            this.x = std::atan2 ( 2.f * ( q.x * q.w + q.y * q.z ), ( sqw - sqx + sqy - sqz ) );
            this.y = std::atan2 ( 2.f * ( q.x * q.z + q.y * q.w ), ( sqw + sqx - sqy - sqz ) );
            this.z = std::asin ( clamp ( 2.f * ( q.z * q.w - q.x * q.y ) ) );
        }

        return *this;
    }

    Vector3& getScaleFromMatrix ( const Matrix4& m ) {
        auto sx = set ( m.elements[0], m.elements[1], m.elements[2] ).length();
        auto sy = set ( m.elements[4], m.elements[5], m.elements[6] ).length();
        auto sz = set ( m.elements[8], m.elements[9], m.elements[10] ).length();

        return set ( sx, sy, sz );
    }

    bool equals ( const Vector3& v ) {
        return ( ( v.x == x ) && ( v.y == y ) && ( v.z == z ) );
    }

    bool isZero() {
        return ( lengthSq() < 0.0001f /* almostZero */ );
    }

    Vector3 clone() {
        return *this;
    }
};

}

#endif