#ifndef THREE_PROGRAM_HPP
#define THREE_PROGRAM_HPP

#include <three/common.hpp>

#include <three/extras/noncopyable.hpp>

#include <map>
#include <string>

namespace three {

class Program : public NonCopyable {
public:

	typedef std::shared_ptr<Program> Ptr;

	static Ptr create( Buffer program, int id ) { 
		return make_shared<Program>( program, id );	
	}

	/////////////////////////////////////////////////////////////////////////

	std::map<std::string, int> uniforms;
	std::map<std::string, int> attributes;

	Buffer program;
	int id;

protected:

	Program ( Buffer program, int id )
	: program ( program ), id ( id ) { }
};

} // namespace three

#endif // THREE_PROGRAM_HPP