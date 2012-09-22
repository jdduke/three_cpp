#ifndef THREE_VECTOR3_HPP
#define THREE_VECTOR3_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>

namespace three {

class Vector3 {
public:

    union {
        struct { float x, y, z; };
        float xyz[3];
    };

    Vector3() : x ( 0 ), y ( 0 ), z ( 0 ) { }
    Vector3 ( float xIn, float yIn, float zIn ) : x ( xIn ), y ( yIn ), z ( zIn ) { }
    explicit Vector3( float value ) : x ( value ), y ( value ), z ( value ) { }
    explicit Vector3 ( float* values ) : x ( values[0] ), y ( values[1] ), z ( values[2] ) { }

    float& operator[] (const int i) { return xyz[i]; }
    const float operator[] (const int i) const { return xyz[i]; }

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

    float dot ( const Vector3& v ) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float lengthSq() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return Math::sqrt ( lengthSq() );
    }

    float lengthManhattan() const {
        return Math::abs ( x ) + Math::abs ( y ) + Math::abs ( z );
    }

    Vector3& normalize() {
        return divideScalar ( length() );
    }

    float distanceTo ( const Vector3& v ) const {
        return Math::sqrt ( distanceToSquared ( v ) );
    }

    float distanceToSquared ( const Vector3& v ) const {
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

static_assert( sizeof(Vector3) == sizeof(float) * 3, "Invalid Vector3 storage size");

inline Vector3 add ( const Vector3& a, const Vector3& b ) {
    return Vector3().add ( a, b );
}

inline Vector3 sub ( const Vector3& a, const Vector3& b ) {
    return Vector3().sub ( a, b );
}

inline float dot ( const Vector3& a, const Vector3& b ) {
    return a.dot( b );
}

inline Vector3 cross ( const Vector3& a, const Vector3& b ) {
    return Vector3().cross ( a, b );
}

} // namespace three

#endif // THREE_VECTOR3_HPP