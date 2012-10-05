#ifndef THREE_PROGRAM_HPP
#define THREE_PROGRAM_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/utils/noncopyable.hpp>

#include <three/materials/uniform.hpp>

#include <map>
#include <string>

namespace three {

class Program : public NonCopyable {
public:

  typedef std::shared_ptr<Program> Ptr;

  static Ptr create( Buffer program, int id ) {
    return three::make_shared<Program>( program, id );
  }

  /////////////////////////////////////////////////////////////////////////

  UniformLocations uniforms;
  UniformLocations attributes;

  Buffer program;
  int id;

protected:

  Program( Buffer program, int id )
    : program( program ), id( id ) { }
};

} // namespace three

#endif // THREE_PROGRAM_HPP