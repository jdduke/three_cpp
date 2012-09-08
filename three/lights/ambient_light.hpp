#ifndef THREE_AMBIENT_LIGHT_HPP
#define THREE_AMBIENT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class AmbientLight : public Light {
public:

	typedef std::shared_ptr<AmbientLight> Ptr;

	static Ptr create( int hex ) { return std::make_shared<AmbientLight>(); }

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_HPP