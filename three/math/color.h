#ifndef THREE_COLOR_H
#define THREE_COLOR_H

#include <three/common.h>

namespace three {

class THREE_DECL HSL {

public:

  union {
    struct { float h, s, l; };
    float hsl[3];
  };

  HSL () : h ( 1.f ), s ( 1.f ), l ( 1.f ) { }

  float& operator[] (const int i) { return hsl[i]; }
  const float operator[] (const int i) const { return hsl[i]; }

};

class THREE_DECL Color {

public:

  union {
    struct { float r, g, b; };
    float rgb[3];
  };

  Color () : r ( 1.f ), g ( 1.f ), b ( 1.f ) { }

  explicit Color ( unsigned hex ) { set ( hex ); }

  Color ( float r, float g, float b )
  : r ( r ), g ( g ), b ( b ) { }

  float& operator[] (const int i) { return rgb[i]; }
  const float operator[] (const int i) const { return rgb[i]; }

  Color& set( unsigned hex );
  Color& setRGB ( float rIn, float gIn, float bIn );
  Color& setHSL ( float h, float s, float l );

  Color& copy( const Color& color );

  Color& copyGammaToLinear( const Color& color );
  Color& copyLinearToGamma( const Color& color );

  Color& convertGammaToLinear();
  Color& convertLinearToGamma();

  int getHex() const;
  HSL getHSL() const;

  Color& offsetHSL( float h, float s, float l );

  Color& add( const Color& color );
  Color& addColors( const Color& color1, const Color&  color2 );
  Color& addScalar( float s );

  Color& multiply( const Color& color );
  Color& multiplyScalar( float s );

  Color& lerp( const Color& color, float alpha );

  bool equals( const Color& c ) const;

  Color clone() const;

};

} // namespace three

#endif // THREE_COLOR_H
