#ifndef THREE_AMBIENT_LIGHT_HPP
#define THREE_AMBIENT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class AmbientLight : public Light {
public:

    typedef std::shared_ptr<AmbientLight> Ptr;

    static Ptr create( int hex ) { return make_shared<AmbientLight>( hex ); }

    virtual THREE::Type type() const { return THREE::AmbientLight; }

protected:

    AmbientLight( int hex ) : Light( hex ) { }

};

} // namespace three

#endif // THREE_AMBIENT_LIGHT_HPP