#ifndef THREE_MATERIAL_H
#define THREE_MATERIAL_H

#include <three/common.h>

#include <three/core/interfaces.h>
#include <three/core/event_dispatcher.h>

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

class THREE_DECL Material : public NonCopyable, public DefaultEventDispatcher {

public:

  typedef Properties<std::string, any> Parameters;
  typedef std::unordered_set<std::string> ParameterKeys;

  // TODO(ea): make this work with any and stringstream
  typedef std::unordered_map<std::string, std::string> Defines;

  typedef std::shared_ptr<Material> Ptr;

  static Ptr create() {
    return make_shared<Material>( );
  }

  virtual ~Material();

  virtual THREE::MaterialType type() const {
    return THREE::Material;
  }

  Ptr clone() const;

public:

  int id;
  std::string uuid;

  std::string name;

  THREE::Side side;

  float opacity;
  bool transparent;

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

  float overdraw; // Overdrawn pixels (typically between 0 and 1) for fixing antialiasing gaps in CanvasRenderer

  bool visible;

  bool needsUpdate;

  Attributes attributes;

  Color color, ambient, emissive, specular;
  float shininess;

  float size;
  bool sizeAttenuation;

  THREE::Shading shading;
  THREE::Colors vertexColors;

  bool skinning;
  bool morphTargets;
  bool morphNormals;

  // TODO "Implement default values"

  //// When rendered geometry doesn't include these attributes but the material does,
  //// use these default values in WebGL. This avoids errors when buffer data is missing.
  std::unordered_map<std::string, std::vector<float> > defaultAttributeValues;

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

  void dispose() {
    dispatchEvent( "dispose" );
  };

protected:

  Material();

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

  virtual void __clone( Ptr& cloned ) const;

private:

};

} // namespace three

#endif // THREE_MATERIAL_H