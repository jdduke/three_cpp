#ifndef THREE_SHADER_MATERIAL_HPP
#define THREE_SHADER_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class ShaderMaterial : public Material {
public:

  typedef std::shared_ptr<ShaderMaterial> Ptr;

  static Ptr create( std::string vertexShader,
                     std::string fragmentShader,
                     const Uniforms& uniforms = Uniforms(),
                     const Attributes& attributes = Attributes(),
                     const Parameters& parameters = Parameters() ) {
    return three::make_shared<ShaderMaterial>( std::move(vertexShader),
                                               std::move(fragmentShader),
                                               uniforms,
                                               attributes,
                                               parameters );
  }

  virtual THREE::MaterialType type() const { return THREE::ShaderMaterial; }

  /////////////////////////////////////////////////////////////////////////

  Ptr clone( ) {
    auto material = create();
    static_cast<const Material&>(*this).clone( *material );
    return material;
  }

protected:

  friend class Material;

  static Ptr create( const Parameters& parameters = Parameters() ) {
    return three::make_shared<ShaderMaterial>( parameters );
  }

  ShaderMaterial( const Parameters& parameters )
    : Material() {
    setParameters( parameters, DefaultKeys() );
  }

  ShaderMaterial( std::string vertexShader,
                  std::string fragmentShader,
                  const Uniforms& uniforms,
                  const Attributes& attributes,
                  const Parameters& parameters )
    : Material() {

    setParameters( parameters, DefaultKeys() );

    this->vertexShader   = std::move(vertexShader);
    this->fragmentShader = std::move(fragmentShader);

    if (!attributes.empty()) {
      this->attributes = attributes;
    }
    if (!uniforms.empty()) {
      this->uniforms = uniforms;
    }

  }

  static const ParameterKeys& DefaultKeys() {
    static std::array<std::string, 12> sKeys = {
      //"fragmentShader",
      //"vertexShader",
      //"uniforms",
      //"attributes",
      "shading",
      "blending",
      "depthTest",
      "transparent",
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

