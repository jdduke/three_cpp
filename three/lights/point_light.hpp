#ifndef THREE_POINT_LIGHT_HPP
#define THREE_POINT_LIGHT_HPP

#include <three/common.hpp>

#include <three/lights/light.hpp>

namespace three {

class PointLight : public Light {
public:

    typedef std::shared_ptr<PointLight> Ptr;

    static Ptr create( int hex, float intensity = 1, float distance = 0 ) {
        return make_shared<PointLight>( hex, intensity, distance );
    }

    virtual THREE::Type type() const { return THREE::PointLight; }

    /////////////////////////////////////////////////////////////////////////

    float intensity;
    float distance;

    /////////////////////////////////////////////////////////////////////////


protected:

    PointLight ( int hex, float intensity, float distance )
        : Light ( hex ), intensity ( intensity ), distance ( distance ) {

            position.set ( 0, 0, 0 );

    }

};

} // namespace three

#endif // THREE_POINT_LIGHT_HPP