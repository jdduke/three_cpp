#ifndef THREE_LIGHT_HPP
#define THREE_LIGHT_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>
#include <three/core/object3d.hpp>

namespace three {

class Light : public Object3D {
public:

    typedef std::shared_ptr<Light> Ptr;

    virtual THREE::Type type() const { return THREE::Light; }

    /////////////////////////////////////////////////////////////////////////

    Color color;
    float intensity;
    float distance;

    std::shared_ptr<Object3D> target;

    bool castShadow, onlyShadow, shadowCascade;

    /////////////////////////////////////////////////////////////////////////

protected:

    explicit Light ( int hex, float intensity = 1, float distance = 0 )
        : Object3D(),
        color( hex ),
        intensity ( intensity ),
        distance ( distance ),
        target ( nullptr ),
        castShadow (false ),
        onlyShadow ( false ),
        shadowCascade ( false ) { }

    virtual void visit( Visitor& v ) { v( *this ); }
    virtual void visit( ConstVisitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_LIGHT_HPP