#ifndef THREE_COLOR_HPP
#define THREE_COLOR_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>

namespace three {

class Color {
public:

    union {
        struct { float r, g, b; };
        float rgb[3];
    };

    Color () : r ( 1 ), g ( 1 ), b ( 1 ) { }

    explicit Color ( unsigned hex ) { setHex ( hex ); }

    Color ( float r, float g, float b )
        : r ( r ), g ( g ), b ( b ) { }

    Color ( Color&& other )      = default;
    Color ( const Color& other ) = default;
    Color& operator=( Color&& other )      = default;
    Color& operator=( const Color& other ) = default;

    Color& copyGammaToLinear ( const Color& color ) {
        r = color.r * color.r;
        g = color.g * color.g;
        b = color.b * color.b;
        return *this;
    }

    Color& setRGB ( float rIn, float gIn, float bIn ) {
        r = rIn;
        g = gIn;
        b = bIn;
        return *this;
    }

    Color& setHSV ( float h, float s, float v ) {
        if ( v == 0.f ) {
            r = g = b = 0;
        } else {
            auto i = ( int ) Math::floor ( h * 6.f );
            auto f = ( h * 6 ) - i;
            auto p = v * ( 1.f - s );
            auto q = v * ( 1.f - ( s * f ) );
            auto t = v * ( 1.f - ( s * ( 1.f - f ) ) );

            switch ( i ) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            case 5:
            default:
                r = v;
                g = p;
                b = q;
                break;
            }
        }

        return *this;
    }

    Color& setHex ( unsigned hex ) {
        r = ( float ) ( ( hex >> 16 ) & 255 ) / 255.f;
        g = ( float ) ( ( hex >>  8 ) & 255 ) / 255.f;
        b = ( float ) ( ( hex >>  0 ) & 255 ) / 255.f;
        return *this;
    }

    Color clone() {
        return *this;
    }

};

} // namespace three

#endif // THREE_COLOR_HPP