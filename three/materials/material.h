#ifndef THREE_MATERIAL_H
#define THREE_MATERIAL_H

#include <three/common.h>

#include <three/core/interfaces.h>

#include <three/math/color.h>
#include <three/math/vector3.h>

#include <three/materials/attribute.h>
#include <three/materials/program.h>
#include <three/materials/uniform.h>

#include <three/textures/texture.h>

#include <three/utils/any.h>
#include <three/utils/memory.h>
#include <three/utils/noncopyable.h>
#include <three/utils/properties.h>

#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <string>

namespace three {

class THREE_DECL Material : public NonCopyable {

public:

  typedef Properties<std::string, any> Parameters;
  typedef std::unordered_set<std::string> ParameterKeys;

  // TODO(ea): make this work with any and stringstream
  typedef std::unordered_map<std::string, std::string> Defines;

  typedef std::shared_ptr<Material> Ptr;

  static Ptr create() {
    return make_shared<Material>( );
  }

  ~Material();

  virtual enums::MaterialType type() const {
    return enums::Material;
  }

public:

  int id;
  std::string uuid;

  std::string name;

  enums::Side side;

  float opacity;
  bool transparent;

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

  THREE_REVIEW("Needed? Also: changed to float in r65, was bool in r50.")
  float overdraw; // Overdrawn pixels (typically between 0 and 1) for fixing antialiasing gaps in CanvasRenderer

  bool visible;

  bool needsUpdate;

  Attributes attributes;

  Color color, ambient, emissive, specular;
  float shininess;

  float size;
  bool sizeAttenuation;

  enums::Shading shading;
  enums::Colors vertexColors;

  bool skinning;
  bool morphTargets;
  bool morphNormals;

  // TODO "Implement default values"

  //// When rendered geometry doesn't include these attributes but the material does,
  //// use these default values in WebGL. This avoids errors when buffer data is missing.
  std::unordered_map<std::string, std::vector<float> > defaultAttributeValues;

  //// By default, bind position to attribute index 0. In WebGL, attribute 0
  //// should always be used to avoid potentially expensive emulation.
  std::string index0AttributeName;

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

  Defines defines;

  Texture::Ptr map, envMap, lightMap, bumpMap, normalMap, specularMap;
  float bumpScale;
  Vector3 normalScale;

  bool fog;
  bool lights;
  bool shadowPass;

  float scale;
  float gapSize;
  float dashSize;

  void setParameters( const Parameters& parameters,
                      const ParameterKeys& keys = ParameterKeys() );

  // TODO "Clone without passing a parameter"

  Material& clone( Material& material ) const;

  // TODO "Implement"

  void dispose() {
    //this.dispatchEvent( { type: 'dispose' } );
  };

protected:

  Material();

  template < typename MaterialType >
  static std::shared_ptr<MaterialType> clone( const MaterialType& src ) {
    auto material = MaterialType::create();
    static_cast<const Material&>( src ).clone( *material );
    return material;
  }

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

#endif // THREE_MATERIAL_H