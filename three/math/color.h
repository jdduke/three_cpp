#ifndef THREE_COLOR_H
#define THREE_COLOR_H

#include <three/common.h>

#include <three/math/math.h>

namespace three {

class HSL {

public:

  union {
    struct { float h, s, l; };
    float hsl[3];
  };

  HSL () : h ( 1.f ), s ( 1.f ), l ( 1.f ) { }

  float& operator[] (const int i) { return hsl[i]; }

  const float operator[] (const int i) const { return hsl[i]; }

};

class Color {
  
public:

  union {
    struct { float r, g, b; };
    float rgb[3];
  };

  Color () : r ( 1.f ), g ( 1.f ), b ( 1.f ) { }

  explicit Color ( unsigned hex ) { setHex ( hex ); }

  Color ( float r, float g, float b )
  : r ( r ), g ( g ), b ( b ) { }

  float& operator[] (const int i) { return rgb[i]; }
  const float operator[] (const int i) const { return rgb[i]; }

  void set() {
    //Not implemented
  }

  inline Color& setHex ( unsigned hex ) {

    r =  (float) ( ( hex >> 16 ) & 255 ) / 255;
    g =  (float) ( ( hex >>  8 ) & 255 ) / 255;
    b =  (float) ( ( hex >>  0 ) & 255 ) / 255;

    return *this;
  }

  inline Color& setRGB ( float rIn, float gIn, float bIn ) {

    r = rIn;
    g = gIn;
    b = bIn;

    return *this;

  }

  inline Color& setHSL ( float h, float s, float l ) {

    if ( s == 0.f ) {

      r = g = b = l;

    } else {

      auto p = l <= 0.5f ? l * ( 1.f + s ) : l + s - ( l * s );
      auto q = ( 2.f * l ) - p;

      r = hue2rgb( q, p, h + 1.f / 3.f );
      g = hue2rgb( q, p, h );
      b = hue2rgb( q, p, h - 1.f / 3.f );

    }

    return *this;

  }

  inline void setStyle() {
    //Not implemented
  }

  inline Color& copy( const Color& color ) {

    return *this = color;

  }

  inline Color& copyGammaToLinear( const Color& color ) {

    r = color.r * color.r;
    g = color.g * color.g;
    b = color.b * color.b;

    return *this;

  }

  inline Color& copyLinearToGamma( const Color& color ) {

    r = Math::sqrt( color.r );
    g = Math::sqrt( color.g );
    b = Math::sqrt( color.b );

    return *this;

  }

  inline Color& convertGammaToLinear() {

    r = r * r;
    g = g * g;
    b = b * b;

    return *this;

  }

  inline Color& convertLinearToGamma() {

    r = Math::sqrt( r );
    g = Math::sqrt( g );
    b = Math::sqrt( b );

    return *this;

  }

  inline int getHex() const {

    return (int)( r * 255 ) << 16 ^ (int)( g * 255 ) << 8 ^ (int)( b * 255 ) << 0;

  }

  inline void getHexString() {
    //Not implemented
  }

  HSL getHSL() {

    // h,s,l ranges are in 0.0 - 1.0
    auto hsl = HSL();

    const float max = Math::max( r, g, b );
    const float min = Math::min( r, g, b );

    float hue = 0.f, saturation = 0.f;
    auto lightness = ( min + max ) / 2.0f;

    if ( min != max ) {
      auto delta = max - min;

      saturation = lightness <= 0.5f ? delta / ( max + min ) : delta / ( 2 - max - min );

      if(max == r) {
        hue = ( g - b ) / delta + ( g < b ? 6.f : 0.f );
      } else if (max == g) {
        hue = ( b - r ) / delta + 2.f; 
      } else if (max == b) {
        hue = ( r - g ) / delta + 4.f;
      }

      hue /= 6;
    }

    hsl.h = hue;
    hsl.s = saturation;
    hsl.l = lightness;

    return hsl;

  }

  inline void getStyle() {
    //Not implemented
  }

  inline Color& offsetHSL( float h, float s, float l ) {

    auto hsl = getHSL();

    hsl.h += h; hsl.s += s; hsl.l += l;

    setHSL( hsl.h, hsl.s, hsl.l );

    return *this;

  }

  inline Color& add( const Color& color ) {

    r += color.r;
    g += color.g;
    b += color.b;

    return *this;

  }

  inline Color& addColors( const Color& color1, const Color&  color2 ) {

    r = color1.r + color2.r;
    g = color1.g + color2.g;
    b = color1.b + color2.b;

    return *this;

  }

  inline Color& addScalar( float s ) {

    r += s;
    g += s;
    b += s;

    return *this;

  }

  inline Color& multiply( const Color& color ) {

    r *= color.r;
    g *= color.g;
    b *= color.b;

    return *this;

  }

  inline Color& multiplyScalar( float s ) {

    r *= s;
    g *= s;
    b *= s;

    return *this;

  }

  inline Color& lerp( const Color& color, float alpha ) {

    r += ( color.r - r ) * alpha;
    g += ( color.g - g ) * alpha;
    b += ( color.b - b ) * alpha;

    return *this;

  }

  inline bool equals( const Color& c ) const {

    return ( c.r == r ) && ( c.g == g ) && ( c.b == b );

  }

  inline Color clone() {

    return *this;

  }

private:

  float hue2rgb( float p, float q, float t ) {

    if ( t < 0 ) t += 1;
    if ( t > 1 ) t -= 1;
    if ( t < 1 / 6 ) return p + ( q - p ) * 6 * t;
    if ( t < 1 / 2 ) return q;
    if ( t < 2 / 3 ) return p + ( q - p ) * 6 * ( 2 / 3 - t );

    return p;

  }

};

} // namespace three

#endif // THREE_COLOR_H