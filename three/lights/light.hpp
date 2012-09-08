#ifndef THREE_LIGHT_HPP
#define THREE_LIGHT_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>

namespace three {

class Light : public Object3D {
public:

	typedef std::shared_ptr<Light> Ptr;

	static Ptr create( int hex ) { return std::make_shared<Light>( hex ); }

	/////////////////////////////////////////////////////////////////////////

	Color color;

	/////////////////////////////////////////////////////////////////////////

protected:

	Light ( int hex ) : Object3D(), color( hex ) { }

	virtual THREE::Type getType() const { return THREE::Light; }

	virtual void visit( Visitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_LIGHT_HPP