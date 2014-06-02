#include <three/renderers/shaders/uniforms_lib.h>

#include <three/math/color.h>
#include <three/math/vector4.h>
#include <three/math/vector2.h>

namespace three {

typedef std::pair<std::string, Uniform> Pair;

Uniforms UniformsLib::common() {
  Uniforms uniforms;

  uniforms.add( "diffuse",               Uniform( THREE::c, Color( 0xeeeeee ) ) )
  .add( "opacity",               Uniform( THREE::f, 1.0f ) )

  .add( "map",                   Uniform( THREE::t, 0 ) )
  .add( "offsetRepeat",          Uniform( THREE::v4, Vector4( 0, 0, 1, 1 ) ) )

  .add( "lightMap",              Uniform( THREE::t, 2 ) )
  .add( "specularMap",           Uniform( THREE::t, 3 ) )

  .add( "envMap",                Uniform( THREE::t, 1 ) )
  .add( "flipEnvMap",            Uniform( THREE::f, -1 ) )
  .add( "useRefract",            Uniform( THREE::i, 0 ) )
  .add( "reflectivity",          Uniform( THREE::f, 1.0f ) )
  .add( "refractionRatio",       Uniform( THREE::f, 0.98f ) )
  .add( "combine",               Uniform( THREE::i, 0 ) )

  .add( "morphTargetInfluences", Uniform( THREE::f, 0 ) );

  return uniforms;
}

Uniforms UniformsLib::bump() {
  Uniforms uniforms;

  uniforms.add( "bumpMap",   Uniform( THREE::t, 4 ) )
  .add( "bumpScale", Uniform( THREE::f, 1 ) );

  return uniforms;
}

Uniforms UniformsLib::normalmap() {
  Uniforms uniforms;

  uniforms.add( "normalMap",   Uniform( THREE::t, 5 ) )
  .add( "normalScale", Uniform( THREE::v2, Vector2( 1.f, 1.f ) ) );

  return uniforms;
}

Uniforms UniformsLib::fog() {
  Uniforms uniforms;

  uniforms.add( "fogDensity", Uniform( THREE::f, 0.00025f ) )
  .add( "fogNear",    Uniform( THREE::f, 1.f ) )
  .add( "fogFar",     Uniform( THREE::f, 2000.f ) )
  .add( "fogColor",   Uniform( THREE::c, Color( 0xffffff ) ) );

  return uniforms;
}

Uniforms UniformsLib::lights() {
  Uniforms uniforms;

  uniforms.add( "ambientLightColor",  Uniform( THREE::fv ) )

  .add( "directionalLightDirection",  Uniform( THREE::fv ) )
  .add( "directionalLightColor",      Uniform( THREE::fv ) )

  .add( "hemisphereLightDirection",   Uniform( THREE::fv ) )
  .add( "hemisphereLightSkyColor",    Uniform( THREE::fv ) )
  .add( "hemisphereLightGroundColor", Uniform( THREE::fv ) )

  .add( "pointLightColor",            Uniform( THREE::fv ) )
  .add( "pointLightPosition",         Uniform( THREE::fv ) )
  .add( "pointLightDistance",         Uniform( THREE::fv1 ) )

  .add( "spotLightColor",             Uniform( THREE::fv ) )
  .add( "spotLightPosition",          Uniform( THREE::fv ) )
  .add( "spotLightDirection",         Uniform( THREE::fv ) )
  .add( "spotLightDistance",          Uniform( THREE::fv1 ) )
  .add( "spotLightAngle",             Uniform( THREE::fv1 ) )
  .add( "spotLightExponent",          Uniform( THREE::fv1 ) );

  return uniforms;
}

Uniforms UniformsLib::particle() {
  Uniforms uniforms;

  uniforms.add( "psColor",    Uniform( THREE::c, Color( 0xeeeeee ) ) )
  .add( "opacity",    Uniform( THREE::f, 1.0f ) )
  .add( "size",       Uniform( THREE::f, 1.0f ) )
  .add( "scale",      Uniform( THREE::f, 1.0f ) )
  .add( "map",        Uniform( THREE::t, 0 ) )

  .add( "fogDensity", Uniform( THREE::f, 0.00025f ) )
  .add( "fogNear",    Uniform( THREE::f, 1.f ) )
  .add( "fogFar",     Uniform( THREE::f, 2000.f ) )
  .add( "fogColor",   Uniform( THREE::c, Color( 0xffffff ) ) );

  return uniforms;
}

Uniforms UniformsLib::shadowmap() {
  Uniforms uniforms;

  uniforms.add( "shadowMap",      Uniform( THREE::tv, 6 ) )
  .add( "shadowMapSize",  Uniform( THREE::v2v ) )
  .add( "shadowBias",     Uniform( THREE::fv1 ) )
  .add( "shadowDarkness", Uniform( THREE::fv1 ) )
  .add( "shadowMatrix",   Uniform( THREE::m4v ) );

  return uniforms;
}

} // namespace three
