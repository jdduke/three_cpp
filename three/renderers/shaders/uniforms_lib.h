#ifndef THREE_UNIFORMS_LIB_H
#define THREE_UNIFORMS_LIB_H

#include <three/materials/uniform.h>

namespace three {

class UniformsLib {

public:

  static Uniforms common();
  static Uniforms bump();
  static Uniforms normalmap();
  static Uniforms fog();
  static Uniforms lights();
  static Uniforms particle();
  static Uniforms shadowmap();

};

} // namespace three

#endif // THREE_UNIFORMS_LIB_H