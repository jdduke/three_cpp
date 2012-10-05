#ifndef THREE_PROGRAM_HPP
#define THREE_PROGRAM_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/utils/noncopyable.hpp>

#include <three/materials/uniform.hpp>

#include <map>
#include <string>

namespace three {

typedef Properties<std::string, int> AttributeLocations;

class Program : public NonCopyable {
public:

  typedef std::shared_ptr<Program> Ptr;

  static Ptr create( Buffer program, int id ) {
    return three::make_shared<Program>( program, id );
  }

  /////////////////////////////////////////////////////////////////////////

  UniformLocations uniforms;
  AttributeLocations attributes;

  Buffer program;
  int id;

protected:

  Program( Buffer program, int id )
    : program( program ), id( id ) { }
};

//////////////////////////////////////////////////////////////////////////

namespace AttributeKey {

#if 1
#define DECLARE_ATTRIBUTE_KEY(a)                 \
  inline const std::string& a () {               \
  static const std::string attributeKey( (#a) ); \
  return attributeKey;                           \
}
#else
#define DECLARE_ATTRIBUTE_KEY(a) inline const char* a () { return #a; }
#endif

DECLARE_ATTRIBUTE_KEY(position)
DECLARE_ATTRIBUTE_KEY(normal)
DECLARE_ATTRIBUTE_KEY(color)
DECLARE_ATTRIBUTE_KEY(uv)
DECLARE_ATTRIBUTE_KEY(uv2)
DECLARE_ATTRIBUTE_KEY(tangent)
DECLARE_ATTRIBUTE_KEY(skinVertexA)
DECLARE_ATTRIBUTE_KEY(skinVertexB)
DECLARE_ATTRIBUTE_KEY(skinWeight)
DECLARE_ATTRIBUTE_KEY(skinIndex)
DECLARE_ATTRIBUTE_KEY(morphTarget)

#undef DECLARE_ATTRIBUTE_KEY

} // namespace AttributeKey

} // namespace three

#endif // THREE_PROGRAM_HPP