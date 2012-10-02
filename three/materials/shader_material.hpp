#ifndef THREE_SHADER_MATERIAL_HPP
#define THREE_SHADER_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class ShaderMaterial : public Material {
public:

  typedef std::shared_ptr<ShaderMaterial> Ptr;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<ShaderMaterial>( parameters );
  }

  static Ptr create( const std::string& fragmentShader,
                     const std::string& vertexShader,
                     const Uniforms& uniforms,
                     const Parameters& parameters = Parameters() ) {
    return three::make_shared<ShaderMaterial>( fragmentShader,
                                               vertexShader,
                                               uniforms,
                                               parameters );
  }

  virtual THREE::MaterialType type() const { return THREE::ShaderMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    return Material::clone( *this );
  }

protected:

  ShaderMaterial( const Parameters& parameters )
    : Material() {
    setParameters( parameters, DefaultKeys() );
  }

  ShaderMaterial( const std::string& fragmentShader,
                  const std::string& vertexShader,
                  const Uniforms& uniforms,
                  const Parameters& parameters )
    : Material() {
    this->fragmentShader = fragmentShader;
    this->vertexShader   = vertexShader;
    this->uniforms       = uniforms;
    setParameters( parameters, DefaultKeys() );
  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 15> sKeys = {
      "fragmentShader",
      "vertexShader",
      "uniforms",
      "attributes",
      "shading",
      "blending",
      "depthTest",
      "wireframe",
      "wireframeLinewidth",
      "lights",
      "vertexColors",
      "skinning",
      "morphTargets",
      "morphNormals",
      "fog"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

};

} // namespace three

#endif // THREE_MATERIAL_HPP

