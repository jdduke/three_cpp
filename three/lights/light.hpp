#ifndef THREE_LIGHT_HPP
#define THREE_LIGHT_HPP

#include <three/common.hpp>

#include <three/core/color.hpp>
#include <three/core/object3d.hpp>

namespace three {

class Light : public Object3D {
public:

	typedef std::shared_ptr<Light> Ptr;

	/////////////////////////////////////////////////////////////////////////

	Color color;

	Object3D* target;

	/////////////////////////////////////////////////////////////////////////

protected:

	explicit Light ( int hex ) : Object3D(), color( hex ), target ( nullptr ) { }

	virtual THREE::Type type() const { return THREE::Light; }

	virtual void visit( Visitor& v ) { v( *this ); }
	virtual void visit( ConstVisitor& v ) const { v( *this ); }

};

} // namespace three

#endif // THREE_LIGHT_HPP