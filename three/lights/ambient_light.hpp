#ifndef THREE_AMBIENT_LIGHT_HPP
#define THREE_AMBIENT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class AmbientLight : public Light {
public:

	typedef shared_ptr<AmbientLight> Ptr;

	/////////////////////////////////////////////////////////////////////////

	static void create( int hex ) { return make_shared<AmbientLight>(); }

private:

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_HPP