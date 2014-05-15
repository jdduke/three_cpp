#ifndef THREE_UNIFORMS_LIB_CPP
#define THREE_UNIFORMS_LIB_CPP

#include <three/renderers/shaders/uniforms_lib.h>

#include <three/math/color.h>
#include <three/math/vector4.h>
#include <three/math/vector2.h>

namespace three {

typedef std::pair<std::string, Uniform> Pair;

Uniforms UniformsLib::common() {
  Uniforms uniforms;

  uniforms.add( "diffuse",               Uniform( enums::c, Color( 0xeeeeee ) ) )
  .add( "opacity",               Uniform( enums::f, 1.0f ) )

  .add( "map",                   Uniform( enums::t, 0 ) )
  .add( "offsetRepeat",          Uniform( enums::v4, Vector4( 0, 0, 1, 1 ) ) )

  .add( "lightMap",              Uniform( enums::t, 2 ) )
  .add( "specularMap",           Uniform( enums::t, 3 ) )

  .add( "envMap",                Uniform( enums::t, 1 ) )
  .add( "flipEnvMap",            Uniform( enums::f, -1 ) )
  .add( "useRefract",            Uniform( enums::i, 0 ) )
  .add( "reflectivity",          Uniform( enums::f, 1.0f ) )
  .add( "refractionRatio",       Uniform( enums::f, 0.98f ) )
  .add( "combine",               Uniform( enums::i, 0 ) )

  .add( "morphTargetInfluences", Uniform( enums::f, 0 ) );

  return uniforms;
}

Uniforms UniformsLib::bump() {
  Uniforms uniforms;

  uniforms.add( "bumpMap",   Uniform( enums::t, 4 ) )
  .add( "bumpScale", Uniform( enums::f, 1 ) );

  return uniforms;
}

Uniforms UniformsLib::normalmap() {
  Uniforms uniforms;

  uniforms.add( "normalMap",   Uniform( enums::t, 5 ) )
  .add( "normalScale", Uniform( enums::v2, Vector2( 1.f, 1.f ) ) );

  return uniforms;
}

Uniforms UniformsLib::fog() {
  Uniforms uniforms;

  uniforms.add( "fogDensity", Uniform( enums::f, 0.00025f ) )
  .add( "fogNear",    Uniform( enums::f, 1.f ) )
  .add( "fogFar",     Uniform( enums::f, 2000.f ) )
  .add( "fogColor",   Uniform( enums::c, Color( 0xffffff ) ) );

  return uniforms;
}

Uniforms UniformsLib::lights() {
  Uniforms uniforms;

  uniforms.add( "ambientLightColor",  Uniform( enums::fv ) )

  .add( "directionalLightDirection",  Uniform( enums::fv ) )
  .add( "directionalLightColor",      Uniform( enums::fv ) )

  .add( "hemisphereLightDirection",   Uniform( enums::fv ) )
  .add( "hemisphereLightSkyColor",    Uniform( enums::fv ) )
  .add( "hemisphereLightGroundColor", Uniform( enums::fv ) )

  .add( "pointLightColor",            Uniform( enums::fv ) )
  .add( "pointLightPosition",         Uniform( enums::fv ) )
  .add( "pointLightDistance",         Uniform( enums::fv1 ) )

  .add( "spotLightColor",             Uniform( enums::fv ) )
  .add( "spotLightPosition",          Uniform( enums::fv ) )
  .add( "spotLightDirection",         Uniform( enums::fv ) )
  .add( "spotLightDistance",          Uniform( enums::fv1 ) )
  .add( "spotLightAngle",             Uniform( enums::fv1 ) )
  .add( "spotLightExponent",          Uniform( enums::fv1 ) );

  return uniforms;
}

Uniforms UniformsLib::particle() {
  Uniforms uniforms;

  uniforms.add( "psColor",    Uniform( enums::c, Color( 0xeeeeee ) ) )
  .add( "opacity",    Uniform( enums::f, 1.0f ) )
  .add( "size",       Uniform( enums::f, 1.0f ) )
  .add( "scale",      Uniform( enums::f, 1.0f ) )
  .add( "map",        Uniform( enums::t, 0 ) )

  .add( "fogDensity", Uniform( enums::f, 0.00025f ) )
  .add( "fogNear",    Uniform( enums::f, 1.f ) )
  .add( "fogFar",     Uniform( enums::f, 2000.f ) )
  .add( "fogColor",   Uniform( enums::c, Color( 0xffffff ) ) );

  return uniforms;
}

Uniforms UniformsLib::shadowmap() {
  Uniforms uniforms;

  uniforms.add( "shadowMap",      Uniform( enums::tv, 6 ) )
  .add( "shadowMapSize",  Uniform( enums::v2v ) )
  .add( "shadowBias",     Uniform( enums::fv1 ) )
  .add( "shadowDarkness", Uniform( enums::fv1 ) )
  .add( "shadowMatrix",   Uniform( enums::m4v ) );

  return uniforms;
}

} // namespace three

#endif // THREE_UNIFORMS_LIB_CPP