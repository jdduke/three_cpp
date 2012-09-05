#ifndef _VECTOR2_HPP_
#define _VECTOR2_HPP_

namespace three {

class Vector2 {
public:

    union {
        struct { float x, y; };
        float xy[2];
    };

    Vector2() : x ( 0 ), y ( 0 ) { }
    Vector2 ( float xIn, float yIn ) : x ( xIn ), y ( yIn ) { }
    Vector2 ( const Vector2& v ) : x ( v.x ), y ( v.y ) { }

    Vector2& set ( float xIn, float yIn ) {
        x = xIn;
        y = yIn;
        return *this;
    }

    Vector2& copy ( const Vector2& v ) {
        x = v.x;
        y = v.y;
        return *this;
    }

    Vector2& add ( const Vector2& a, const Vector2& b ) {
        x = a.x + b.x;
        y = a.y + b.y;
        return *this;
    }

    Vector2& addSelf ( const Vector2& v ) {
        x += v.x;
        y += v.y;
        return *this;
    }

    Vector2& sub ( const Vector2& a, const Vector2& b ) {
        x = a.x - b.x;
        y = a.y - b.y;
        return *this;
    }

    Vector2& subSelf ( const Vector2& v ) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    Vector2& multiplyScalar ( float s ) {
        x *= s;
        y *= s;
        return *this;
    }

    Vector2& divideScalar ( float s ) {
        if ( s ) {
            return multiplyScalar ( 1.f / s );
        } else {
            return set ( 0, 0 );
        }
    }

    Vector2& negate() {
        return multiplyScalar ( -1.f );
    }

    float dot ( const Vector2& v ) {
        return x * v.x + y * v.y;
    }

    float lengthSq() {
        return x * x + y * y;
    }

    float length() {
        return std::sqrt ( lengthSq() );
    }

    Vector2& normalize() {
        return divideScalar ( length() );
    }

    float distanceTo ( const Vector2& v ) {
        return std::sqrt ( distanceToSquared ( v ) );
    }

    float distanceToSquared ( const Vector2& v ) {
        auto dx = x - v.x, dy = y - v.y;
        return dx * dx + dy * dy;
    }

    Vector2& setLength ( float l ) {
        return normalize().multiplyScalar ( l );
    }

    Vector2& lerpSelf ( const Vector2& v, float alpha ) {
        x += ( v.x - x ) * alpha;
        y += ( v.y - y ) * alpha;
        return *this;
    }

    bool equals ( const Vector2& v ) {
        return ( ( v.x == x ) && ( v.y == y ) );
    }

    bool isZero() {
        return ( lengthSq() < 0.0001 /* almostZero */ );
    }

    Vector2 clone() {
        return *this;
    }
};

}

#endif