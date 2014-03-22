#ifndef THREE_PROGRAM_H
#define THREE_PROGRAM_H

#include <three/common.h>

#include <three/utils/memory.h>
#include <three/utils/noncopyable.h>

#include <three/materials/attribute.h>
#include <three/materials/uniform.h>

#include <map>
#include <string>

namespace three {

class Program : public NonCopyable {
public:

  typedef std::shared_ptr<Program> Ptr;

  static Ptr create( Buffer program, int id ) {
    return three::make_shared<Program>( program, id );
  }

  UniformLocations uniforms;
  AttributeLocations attributes;

  Buffer program;
  int id;

protected:

  Program( Buffer program, int id )
    : program( program ), id( id ) { }
};

} // namespace three

#endif // THREE_PROGRAM_H