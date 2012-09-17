#ifndef THREE_PROGRAM_HPP
#define THREE_PROGRAM_HPP

#include <three/common.hpp>

#include <three/extras/noncopyable.hpp>

#include <three/renderers/gl_custom_attribute.hpp>

namespace three {

class Program : public NonCopyable {
public:

	typedef std::shared_ptr<Program> Ptr;

	static Ptr create( ) { return make_shared<Program>( ); }

	/////////////////////////////////////////////////////////////////////////

	struct Attributes {
		int position;
	} attributes;

	std::vector<float> uniforms;
	Buffer program;

protected:

	Program ()
	: program ( 0 ) { }

private:

	/*static int& MaterialCount() {
		static int sMaterialCount = 0;
		return sMaterialCount;
	}*/

};

} // namespace three

#endif // THREE_PROGRAM_HPP