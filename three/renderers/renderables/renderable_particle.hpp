#ifndef THREE_RENDERABLE_PARTICLE_HPP
#define THREE_RENDERABLE_PARTICLE_HPP

#include <three/common.hpp>

#include <three/core/vector2.hpp>

#include <three/renderers/renderables/renderable.hpp>

namespace three {

class RenderableParticle : public Renderable {
public:

	RenderableParticle()
	 : Renderable( 0 ),
	 object ( nullptr ),
	 x ( 0 ), y ( 0 ), rotation ( 0 ), material ( nullptr ) { }

	Object3D* object;

	float x, y;

	float rotation;
	Vector2 scale;

	Material* material;
};

} // namespace three

#endif // THREE_RENDERABLE_LINE_HPP