#ifndef THREE_MATERIAL_HPP
#define THREE_MATERIAL_HPP

#include <three/common.hpp>

#include <three/extras/noncopyable.hpp>

#include <three/materials/attribute.hpp>
#include <three/materials/custom_attribute.hpp>
#include <three/materials/program.hpp>
#include <three/materials/uniform.hpp>

#include <three/textures/texture.hpp>

#include <unordered_set>

namespace three {

typedef std::pair<std::string, any>          Parameter;
typedef std::unordered_map<std::string, any> Parameters;
typedef std::unordered_set<std::string>      ParameterKeys;

class Material : public NonCopyable {
public:

  typedef std::shared_ptr<Material> Ptr;

  static Ptr create() { return make_shared<Material>( ); }

  virtual THREE::MaterialType type() const { return THREE::Material; }

  /////////////////////////////////////////////////////////////////////////

  int id;

  std::string name;

  CustomAttributes customAttributes;
  Attributes attributes;

  THREE::Side side;

  Color color, ambient, emissive, specular;
  float shininess;
  float opacity;
  bool transparent;

  float size;
  bool sizeAttenuation;

  THREE::Shading shading;
  THREE::Colors vertexColors;

  THREE::Blending blending;
  THREE::BlendFactor blendSrc;
  THREE::BlendFactor blendDst;
  THREE::BlendEquation blendEquation;

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
  THREE::TextureConstant combine;

  bool metal;
  bool perPixel;
  bool wrapAround;
  Vector3 wrapRGB;

  bool wireframe;
  float wireframeLinewidth;
  float linewidth;
  THREE::LineEndType linecap, linejoin;

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

  THREE_DECL Material& clone( Material& material ) const;

protected:

  THREE_DECL Material( );

  /////////////////////////////////////////////////////////////////////////

  template < typename MaterialType >
  static std::shared_ptr<MaterialType> clone( const MaterialType& src ) {
    auto material = MaterialType::create();
    static_cast<const Material&>( src ).clone( *material );
    return material;
  }

  /////////////////////////////////////////////////////////////////////////

  THREE_DECL void setParameters( const Parameters& parameters,
                                 const ParameterKeys& keys = ParameterKeys() );

private:

  static int& MaterialCount() {
    static int sMaterialCount = 0;
    return sMaterialCount;
  }

};


#if THREE_HAS_VARIADIC_TEMPLATES

template < typename... Params >
Parameters parameters( Params && ... params ) {
  return Parameters( {std::move( params )...} );
}

#else

THREE_DECL Parameters parameters( Parameter && p0 );
THREE_DECL Parameters parameters( Parameter && p0, Parameter && p1 );
THREE_DECL Parameters parameters( Parameter && p0, Parameter && p1, Parameter && p2 );
THREE_DECL Parameters parameters( Parameter && p0, Parameter && p1, Parameter && p2, Parameter && p3 );
THREE_DECL Parameters parameters( Parameter && p0, Parameter && p1, Parameter && p2, Parameter && p3, Parameter && p4 );
THREE_DECL Parameters parameters( Parameter && p0, Parameter && p1, Parameter && p2, Parameter && p3, Parameter && p4, Parameter && p5 );

#endif

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/material.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MATERIAL_HPP