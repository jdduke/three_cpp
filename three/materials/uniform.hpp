#ifndef THREE_UNIFORM_HPP
#define THREE_UNIFORM_HPP

#include <three/common.hpp>

#include <three/utils/any.hpp>
#include <three/utils/properties.hpp>

namespace three {

class Uniform {
public:

  enum Type {
    c = 0, i, iv, iv1, f, fv, fv1, v2, v3, v4, v2v, v3v, v4v, t, tv, m4, m4v, INVALID
  };

  //////////////////////////////////////////////////////////////////////////

  THREE_DECL Uniform( );
  THREE_DECL Uniform( Uniform&& );
  THREE_DECL Uniform( const Uniform& );
  THREE_DECL explicit Uniform( Type type, any value = any() );
  THREE_DECL Uniform& operator=( Uniform );

  THREE_DECL void load( int location );

  //////////////////////////////////////////////////////////////////////////

  Type type;
  any value;

private:
  THREE_DECL Uniform& swap( Uniform& other );
};

/////////////////////////////////////////////////////////////////////////

typedef Properties<std::string, Uniform> Uniforms;
typedef std::unordered_map<std::string, int> UniformLocations;
typedef std::vector<std::pair<Uniform*, std::string>> UniformsList;

/////////////////////////////////////////////////////////////////////////

inline int uniformLocation( const UniformLocations& uniforms, const std::string& name ) {
  auto uniformIndexIt = uniforms.find( name );
  if ( uniformIndexIt != uniforms.end() ) {
    return uniformIndexIt->second;
  } else {
    return -1;
  }
}

inline bool validUniformLocation( int index ) {
  return index != -1;
}

/////////////////////////////////////////////////////////////////////////

namespace CommonUniforms {

enum CommonUniform {

  modelMatrix = 0,
  modelViewMatrix,
  normalMatrix,
  projectionMatrix,
  viewMatrix,
  cameraPosition,

  morphTargetInfluence,
  boneTexture,
  boneGlobalMatrices,

  near,
  far,

  map,
  envMap,
  flipEnvMap,
  nreflectivity,
  refractionRatio,
  reflectivity,
  combine,
  useRefract,
  lightMap,
  bumpMap,
  bumpScale,

  offsetRepeat,

  psColor,
  size,
  scale,

  fogColor,
  fogNear,
  fogFar,
  fogDensity,

  ambient,
  diffuse,
  opacity,
  emissive,
  shininess,
  specular,
  wrapRGB,

  ambientLightColor,
  directionalLightColor,
  directionalLightDirection,
  pointLightColor,
  pointLightPosition,
  pointLightDistance,
  spotLightColor,
  spotLightPosition,
  spotLightDistance,
  spotLightDirection,
  spotLightAngle,
  spotLightExponent,
  hemisphereLightSkyColor,
  hemisphereLightGroundColor,
  hemisphereLightPosition,

  shadowMatrix,
  shadowMap,
  shadowSize,
  shadowDarkness,
  shadowBias,

  CommonUniformCount
};

} // namespace CommonUniforms


} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/uniform.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_SHADERS_H