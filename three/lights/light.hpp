#ifndef THREE_LIGHT_HPP
#define THREE_LIGHT_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>

namespace three {

class Light : public Object3D {
public:

	typedef shared_ptr<Light> Ptr;

	/////////////////////////////////////////////////////////////////////////

	Color color;

	/////////////////////////////////////////////////////////////////////////

	static Ptr create( int hex ) { return make_shared<Light>( hex ); }

protected:

	Light ( int hex ) : Object3D(), color( hex ) { }

	virtual THREE::Type getType() const { return THREE.Light; }

	virtual void visit()( Visitor& v ) { v( *this ); }

};

} // namespace three

#endif // THREE_LIGHT_HPP