#ifndef THREE_SHADERS_SHADER_CHUNK_H
#define THREE_SHADERS_SHADER_CHUNK_H

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

} // namespace three

#endif // THREE_SHADERS_SHADER_CHUNK_H