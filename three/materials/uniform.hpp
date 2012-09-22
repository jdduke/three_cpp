#ifndef THREE_UNIFORM_HPP
#define THREE_UNIFORM_HPP

#include <three/common.hpp>

#include <three/utils/any.hpp>

#include <unordered_map>

namespace three {

struct Uniform {
  
  enum Type {
    c = 0, f, i, fv, fv1, v2, v2v, v3, v4, t, tv, m4v
  };

  Uniform( Type type, any value = any(), Texture* texture = nullptr )
    : type ( type ), value ( std::move(value) ), texture ( texture ) { }

  Type type;
  any value;
  Texture* texture;
};

typedef std::unordered_map<std::string, Uniform> Uniforms;

} // namespace three

#endif // THREE_GL_SHADERS_H