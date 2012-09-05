#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <cmath>

namespace three {

class Color {
public:

    Color ( unsigned hex ) { setHex ( hex ); }

    Color ( float rIn, float gIn, float bIn )
        : r ( rIn ), g ( gIn ), b ( bIn ) { }

    Color ( const Color& other )
        : r ( other.r ), g ( other.g ), b ( other.b ) { }

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
            auto i = ( int ) std::floor ( h * 6.f );
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

    union {
        struct { float r, g, b; };
        float rgb[3];
    };

};

}

#endif