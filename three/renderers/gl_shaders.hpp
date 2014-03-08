#ifndef THREE_GL_SHADERS_HPP
#define THREE_GL_SHADERS_HPP

#include <three/materials/uniform.hpp>

namespace three {

class ShaderChunk {
public:

  static const char* fog_pars_fragment();
  static const char* fog_fragment();

  static const char* envmap_pars_fragment();
  static const char* envmap_fragment();
  static const char* envmap_pars_vertex();
  static const char* envmap_vertex();

  static const char* map_particle_pars_fragment();
  static const char* map_particle_fragment();

  static const char* map_pars_vertex();
  static const char* map_pars_fragment();
  static const char* map_vertex();
  static const char* map_fragment();

  static const char* lightmap_pars_fragment();
  static const char* lightmap_pars_vertex();
  static const char* lightmap_fragment();
  static const char* lightmap_vertex();

  static const char* bumpmap_pars_fragment();

  static const char* specularmap_pars_fragment();
  static const char* specularmap_fragment();

  static const char* lights_lambert_pars_vertex();
  static const char* lights_lambert_vertex();

  static const char* lights_phong_pars_vertex();
  static const char* lights_phong_vertex();

  static const char* lights_phong_pars_fragment();
  static const char* lights_phong_fragment();

  static const char* color_pars_fragment();
  static const char* color_fragment();
  static const char* color_pars_vertex();
  static const char* color_vertex();

  static const char* skinning_pars_vertex();

  static const char* skinbase_vertex();
  static const char* skinning_vertex();
  static const char* morphtarget_pars_vertex();
  static const char* morphtarget_vertex();

  static const char* default_vertex();
  static const char* morphnormal_vertex();
  static const char* skinnormal_vertex();
  static const char* defaultnormal_vertex();

  static const char* shadowmap_pars_fragment();
  static const char* shadowmap_fragment();
  static const char* shadowmap_pars_vertex();
  static const char* shadowmap_vertex();

  static const char* alphatest_fragment();

  static const char* linear_to_gamma_fragment();
};

/////////////////////////////////////////////////////////////////////////

class UniformsLib {
public:

  static Uniforms common();
  static Uniforms bump();
  static Uniforms fog();
  static Uniforms lights();
  static Uniforms particle();
  static Uniforms shadowmap();

};

/////////////////////////////////////////////////////////////////////////

class Shader {
public:
  Shader( Uniforms uniforms, std::string vs, std::string fs )
    : uniforms( std::move( uniforms ) ), vertexShader( std::move( vs ) ), fragmentShader( std::move( fs ) ) { }
  Uniforms uniforms;
  std::string vertexShader, fragmentShader;
};

/////////////////////////////////////////////////////////////////////////

class ShaderLib {
public:

  static const Shader& depth();
  static const Shader& normal();
  static const Shader& basic();
  static const Shader& lambert();
  static const Shader& phong();
  static const Shader& particleBasic();
  static const Shader& depthRGBA();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/renderers/impl/gl_shaders.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_SHADERS_HPP