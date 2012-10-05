#ifndef THREE_UNIFORM_HPP
#define THREE_UNIFORM_HPP

#include <three/common.hpp>

#include <three/utils/any.hpp>
#include <three/utils/properties.hpp>

namespace three {

class Uniform {
public:

  //////////////////////////////////////////////////////////////////////////

  THREE_DECL Uniform( );
  THREE_DECL Uniform( Uniform&& );
  THREE_DECL Uniform( const Uniform& );
  THREE_DECL explicit Uniform( THREE::UniformType type, any value = any() );
  THREE_DECL Uniform& operator=( Uniform );

  THREE_DECL void load( int location );

  //////////////////////////////////////////////////////////////////////////

  THREE::UniformType type;
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

namespace UniformKey {

#if 1
#define DECLARE_UNIFORM_KEY(a)                   \
  inline const std::string& a () {               \
    static const std::string uniformKey( (#a) ); \
    return uniformKey;                           \
  }
#else
#define DECLARE_UNIFORM_KEY(a) inline const char* a () { return #a; }
#endif

DECLARE_UNIFORM_KEY(modelMatrix)
DECLARE_UNIFORM_KEY(modelViewMatrix)
DECLARE_UNIFORM_KEY(normalMatrix)
DECLARE_UNIFORM_KEY(projectionMatrix)
DECLARE_UNIFORM_KEY(viewMatrix)
DECLARE_UNIFORM_KEY(cameraPosition)
DECLARE_UNIFORM_KEY(morphTargetInfluence)
DECLARE_UNIFORM_KEY(boneTexture)
DECLARE_UNIFORM_KEY(boneGlobalMatrices)
DECLARE_UNIFORM_KEY(mNear)
DECLARE_UNIFORM_KEY(mFar)
DECLARE_UNIFORM_KEY(map)
DECLARE_UNIFORM_KEY(envMap)
DECLARE_UNIFORM_KEY(flipEnvMap)
DECLARE_UNIFORM_KEY(nreflectivity)
DECLARE_UNIFORM_KEY(refractionRatio)
DECLARE_UNIFORM_KEY(reflectivity)
DECLARE_UNIFORM_KEY(combine)
DECLARE_UNIFORM_KEY(useRefract)
DECLARE_UNIFORM_KEY(lightMap)
DECLARE_UNIFORM_KEY(specularMap)
DECLARE_UNIFORM_KEY(bumpMap)
DECLARE_UNIFORM_KEY(bumpScale)
DECLARE_UNIFORM_KEY(offsetRepeat)
DECLARE_UNIFORM_KEY(psColor)
DECLARE_UNIFORM_KEY(size)
DECLARE_UNIFORM_KEY(scale)
DECLARE_UNIFORM_KEY(fogColor)
DECLARE_UNIFORM_KEY(fogNear)
DECLARE_UNIFORM_KEY(fogFar)
DECLARE_UNIFORM_KEY(fogDensity)
DECLARE_UNIFORM_KEY(ambient)
DECLARE_UNIFORM_KEY(diffuse)
DECLARE_UNIFORM_KEY(opacity)
DECLARE_UNIFORM_KEY(emissive)
DECLARE_UNIFORM_KEY(shininess)
DECLARE_UNIFORM_KEY(specular)
DECLARE_UNIFORM_KEY(wrapRGB)
DECLARE_UNIFORM_KEY(ambientLightColor)
DECLARE_UNIFORM_KEY(directionalLightColor)
DECLARE_UNIFORM_KEY(directionalLightDirection)
DECLARE_UNIFORM_KEY(pointLightColor)
DECLARE_UNIFORM_KEY(pointLightPosition)
DECLARE_UNIFORM_KEY(pointLightDistance)
DECLARE_UNIFORM_KEY(spotLightColor)
DECLARE_UNIFORM_KEY(spotLightPosition)
DECLARE_UNIFORM_KEY(spotLightDistance)
DECLARE_UNIFORM_KEY(spotLightDirection)
DECLARE_UNIFORM_KEY(spotLightAngle)
DECLARE_UNIFORM_KEY(spotLightExponent)
DECLARE_UNIFORM_KEY(hemisphereLightSkyColor)
DECLARE_UNIFORM_KEY(hemisphereLightGroundColor)
DECLARE_UNIFORM_KEY(hemisphereLightPosition)
DECLARE_UNIFORM_KEY(shadowMatrix)
DECLARE_UNIFORM_KEY(shadowMap)
DECLARE_UNIFORM_KEY(shadowSize)
DECLARE_UNIFORM_KEY(shadowDarkness)
DECLARE_UNIFORM_KEY(shadowBias)

#undef DECLARE_UNIFORM_KEY

} // namespace UniformKey


} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/uniform.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_SHADERS_H