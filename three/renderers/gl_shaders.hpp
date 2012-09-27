#ifndef THREE_GL_SHADERS_HPP
#define THREE_GL_SHADERS_HPP

#include <three/materials/uniform.hpp>

namespace three {

struct ShaderChunk {

  THREE_DECL static const char* fog_pars_fragment();
  THREE_DECL static const char* fog_fragment();

  THREE_DECL static const char* envmap_pars_fragment();
  THREE_DECL static const char* envmap_fragment();
  THREE_DECL static const char* envmap_pars_vertex();
  THREE_DECL static const char* envmap_vertex();

  THREE_DECL static const char* map_particle_pars_fragment();
  THREE_DECL static const char* map_particle_fragment();

  THREE_DECL static const char* map_pars_vertex();
  THREE_DECL static const char* map_pars_fragment();
  THREE_DECL static const char* map_vertex();
  THREE_DECL static const char* map_fragment();

  THREE_DECL static const char* lightmap_pars_fragment();
  THREE_DECL static const char* lightmap_pars_vertex();
  THREE_DECL static const char* lightmap_fragment();
  THREE_DECL static const char* lightmap_vertex();

  THREE_DECL static const char* bumpmap_pars_fragment();

  THREE_DECL static const char* specularmap_pars_fragment();
  THREE_DECL static const char* specularmap_fragment();

  THREE_DECL static const char* lights_lambert_pars_vertex();
  THREE_DECL static const char* lights_lambert_vertex();

  THREE_DECL static const char* lights_phong_pars_vertex();
  THREE_DECL static const char* lights_phong_vertex();

  THREE_DECL static const char* lights_phong_pars_fragment();
  THREE_DECL static const char* lights_phong_fragment();

  THREE_DECL static const char* color_pars_fragment();
  THREE_DECL static const char* color_fragment();
  THREE_DECL static const char* color_pars_vertex();
  THREE_DECL static const char* color_vertex();

  THREE_DECL static const char* skinning_pars_vertex();

  THREE_DECL static const char* skinbase_vertex();
  THREE_DECL static const char* skinning_vertex();
  THREE_DECL static const char* morphtarget_pars_vertex();
  THREE_DECL static const char* morphtarget_vertex();

  THREE_DECL static const char* default_vertex();
  THREE_DECL static const char* morphnormal_vertex();
  THREE_DECL static const char* skinnormal_vertex();
  THREE_DECL static const char* defaultnormal_vertex();

  THREE_DECL static const char* shadowmap_pars_fragment();
  THREE_DECL static const char* shadowmap_fragment();
  THREE_DECL static const char* shadowmap_pars_vertex();
  THREE_DECL static const char* shadowmap_vertex();

  THREE_DECL static const char* alphatest_fragment();

  THREE_DECL static const char* linear_to_gamma_fragment();
};

/////////////////////////////////////////////////////////////////////////

struct UniformsUtils {

  template < typename UniformsList >
  static Uniforms merge( UniformsList& uniformsList, bool overwrite = false ) {

    Uniforms merged;

    for ( auto& uniforms : uniformsList ) {
      /*if ( overwrite ) {
        for (auto& uniform : uniforms ) {
          merged[ uniform.first ] = uniform.second;
        }
      } else */{
        merged.insert( uniforms.begin(), uniforms.end() );
      }
    }

    return std::move( merged );
  }

};

struct UniformsLib {

  typedef std::pair<std::string, Uniform> Pair;

  THREE_DECL static Uniforms common();
  THREE_DECL static Uniforms bump();
  THREE_DECL static Uniforms fog();
  THREE_DECL static Uniforms lights();
  THREE_DECL static Uniforms particle();
  THREE_DECL static Uniforms shadowmap();

};

/////////////////////////////////////////////////////////////////////////

struct Shader {
  Shader( Uniforms uniforms, std::string vs, std::string fs )
    : uniforms( std::move( uniforms ) ), vertexShader( std::move( vs ) ), fragmentShader( std::move( fs ) ) { }
  Uniforms uniforms;
  std::string vertexShader, fragmentShader;
};

/////////////////////////////////////////////////////////////////////////

struct ShaderLib {

  typedef std::pair<std::string, Uniform> Pair;

  THREE_DECL static const Shader& depth();
  THREE_DECL static const Shader& normal();
  THREE_DECL static const Shader& basic();
  THREE_DECL static const Shader& lambert();
  THREE_DECL static const Shader& phong();
  THREE_DECL static const Shader& particleBasic();
  THREE_DECL static const Shader& depthRGBA();

private:

  THREE_DECL static Shader depthCreate();
  THREE_DECL static Shader normalCreate();
  THREE_DECL static Shader basicCreate();
  THREE_DECL static Shader lambertCreate();
  THREE_DECL static Shader phongCreate();
  THREE_DECL static Shader particleBasicCreate();
  THREE_DECL static Shader depthRGBACreate();

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/renderers/impl/gl_shaders.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_SHADERS_HPP