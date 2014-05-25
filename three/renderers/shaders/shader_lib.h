#ifndef THREE_SHADER_LIB_H
#define THREE_SHADER_LIB_H

#include <three/common.h>
#include <three/renderers/gl/gl_shader.h>

namespace three {

class THREE_DECL ShaderLib {

public:

  static const Shader& depth();
  static const Shader& normal();
  static const Shader& basic();
  static const Shader& dashed();
  static const Shader& lambert();
  static const Shader& phong();
  static const Shader& particleBasic();
  static const Shader& depthRGBA();

};

} // namespace three

#endif //THREE_SHADER_LIB_H