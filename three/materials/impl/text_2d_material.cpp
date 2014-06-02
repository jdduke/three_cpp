#include <three/materials/text_2d_material.h>

#include <three/constants.h>

#include <three/extras/utils/font.h>

#include <three/math/color.h>

namespace three {
namespace detail {
inline const char* textVertexShader() {
  return
    "\
varying vec2 vUv;\
void main() {\
  vUv = uv;\
  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\
  gl_Position = projectionMatrix * mvPosition;\
}\
";
}

inline const char* textFragmentShader() {
  return
    "\
uniform sampler2D texture;\
uniform vec3 diffuse;\
uniform float opacity;\
varying vec2 vUv;\
void main() {\
  float texOpacity = texture2D( texture, vUv ).a;\
  gl_FragColor = vec4( diffuse, opacity * texOpacity );\
}\
";
}
}

ShaderMaterial::Ptr Text2DMaterial::create( const Font& font,
    const Color& color,
    float opacity ) {
  auto material = ShaderMaterial::create(
                    std::string(detail::textVertexShader()),
                    std::string(detail::textFragmentShader()),
                    Uniforms().add( UniformKey::diffuse(), Uniform( THREE::c, color ) )
                    .add( UniformKey::opacity(), Uniform( THREE::f, opacity ) )
                    .add( "texture", Uniform( THREE::t, font.texture().get() ) )
                  );

  //material->map = font.texture();
  material->depthTest = material->depthWrite = false;
  material->side = THREE::NoSide;
  material->transparent = true;

  return material;
}
} // namespace three
