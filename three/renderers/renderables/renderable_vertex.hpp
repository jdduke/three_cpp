#ifndef THREE_RENDERABLE_VERTEX_HPP
#define THREE_RENDERABLE_VERTEX_HPP

#include <three/common.hpp>

namespace three {

class RenderableVertex {
public:

	Vector3 positionWorld;
	Vector4 positionScreen;

	bool visible;

	RenderableVertex() : visible ( true ) { }

	RenderableVertex& copy ( const RenderableVertex& vertex ) {
		return (*this) = vertex;
	}
};

} // namespace three

#endif // THREE_RENDERABLE_VERTEX_HPP