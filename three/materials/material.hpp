#ifndef THREE_MATERIAL_HPP
#define THREE_MATERIAL_HPP

#include <three/common.hpp>

#include <three/math/color.hpp>

#include <three/materials/attribute.hpp>
#include <three/materials/program.hpp>
#include <three/materials/uniform.hpp>

#include <three/textures/texture.hpp>

#include <three/utils/any.hpp>
#include <three/utils/memory.hpp>
#include <three/utils/noncopyable.hpp>
#include <three/utils/properties.hpp>

#include <unordered_map>
#include <unordered_set>
#include <array>

namespace three {

class Material : public NonCopyable {
public:

  typedef Properties<std::string, any> Parameters;
  typedef std::unordered_set<std::string> ParameterKeys;

  typedef std::shared_ptr<Material> Ptr;

  static Ptr create() { return make_shared<Material>( ); }

  virtual enums::MaterialType type() const { return enums::Material; }

  /////////////////////////////////////////////////////////////////////////

  int id;

  std::string name;

  Attributes attributes;
  //AttributeLocations attributeLocations;

  enums::Side side;

  Color color, ambient, emissive, specular;
  float shininess;
  float opacity;
  bool transparent;

  float size;
  bool sizeAttenuation;

  enums::Shading shading;
  enums::Colors vertexColors;

  enums::Blending blending;
  enums::BlendFactor blendSrc;
  enums::BlendFactor blendDst;
  enums::BlendEquation blendEquation;

  bool depthTest;
  bool depthWrite;

  bool polygonOffset;
  float polygonOffsetFactor;
  float polygonOffsetUnits;

  float alphaTest;

  bool overdraw; // Boolean for fixing antialiasing gaps in CanvasRenderer

  bool visible;

  bool needsUpdate;

  bool skinning;
  bool morphTargets;
  bool morphNormals;

  float reflectivity;
  float refractionRatio;
  enums::TextureConstant combine;

  bool metal;
  bool perPixel;
  bool wrapAround;
  Vector3 wrapRGB;

  bool wireframe;
  float wireframeLinewidth;
  float linewidth;
  enums::LineEndType linecap, linejoin;

  int numSupportedMorphTargets;
  int numSupportedMorphNormals;

  Program::Ptr program;

  std::string fragmentShader;
  std::string vertexShader;

  Uniforms uniforms;
  UniformsList uniformsList;

  Texture::Ptr map, envMap, lightMap, bumpMap, specularMap;
  float bumpScale;
  Vector3 normalScale;

  bool fog;
  bool lights;
  bool shadowPass;

  /////////////////////////////////////////////////////////////////////////

  Material& clone( Material& material ) const;

protected:

  Material( );

  /////////////////////////////////////////////////////////////////////////

  template < typename MaterialType >
  static std::shared_ptr<MaterialType> clone( const MaterialType& src ) {
    auto material = MaterialType::create();
    static_cast<const Material&>( src ).clone( *material );
    return material;
  }

  /////////////////////////////////////////////////////////////////////////

  void setParameters( const Parameters& parameters,
                                 const ParameterKeys& keys = ParameterKeys() );

  static const ParameterKeys& defaultKeys() {
    static std::array<std::string, 15> sKeys = {
      "side",
      "opacity",
      "transparent",
      "blending",
      "blendSrc",
      "blendDst",
      "blendEquation",
      "depthTest",
      "depthWrite",
      "polygonOffset",
      "polygonOffsetFactor",
      "polygonOffsetUnits",
      "alphaTest",
      "overdraw",
      "visible"
    };
    static ParameterKeys sKeysSet( sKeys.begin(), sKeys.end() );
    return sKeysSet;
  }

private:

  static int& MaterialCount() {
    static int sMaterialCount = 0;
    return sMaterialCount;
  }

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/material.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATERIAL_HPP