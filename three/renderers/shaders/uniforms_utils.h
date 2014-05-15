#ifndef THREE_UNIFORMS_UTILS_H
#define THREE_UNIFORMS_UTILS_H

#include <three/materials/uniform.h>

namespace three {

class UniformsUtils {

public:

  template < typename UniformsList >
  static Uniforms merge( UniformsList& uniformsList, bool overwrite = false ) {

    Uniforms merged;

    for ( auto& uniforms : uniformsList ) {
      merged.contents.insert( uniforms.begin(), uniforms.end() );
    }

    return merged;
  }

};


} // namespace three

#endif //THREE_UNIFORMS_UTILS_H