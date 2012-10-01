#ifndef THREE_UNIFORM_HPP
#define THREE_UNIFORM_HPP

#include <three/common.hpp>

#include <three/utils/any.hpp>

#include <unordered_map>

namespace three {

class Uniform {
public:

  enum Type {
    c = 0, i, iv, iv1, f, fv, fv1, v2, v3, v4, v2v, v3v, v4v, t, tv, m4, m4v, INVALID
  };

  THREE_DECL Uniform( );
  THREE_DECL Uniform( Uniform&& );
  THREE_DECL Uniform( const Uniform& );
  THREE_DECL explicit Uniform( Type type, any value = any(), Texture* texture = nullptr );
  THREE_DECL Uniform& operator=( Uniform );
  THREE_DECL void load( int location );

  Type type;
  any value;
  Texture* texture;

private:
  THREE_DECL Uniform& swap( Uniform& other );
};

typedef std::unordered_map<std::string, Uniform> Uniforms;
typedef std::unordered_map<std::string, int> UniformsIndices;
typedef std::vector<std::pair<Uniform*, std::string>> UniformsList;

} // namespace three


#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/uniform.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_SHADERS_H