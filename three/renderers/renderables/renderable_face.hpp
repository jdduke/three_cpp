#ifndef THREE_RENDERABLE_FACE_HPP
#define THREE_RENDERABLE_FACE_HPP

#include <three/common.hpp>

#include <three/renderers/renderables/renderable.hpp>
#include <three/renderers/renderables/renderable_vertex.hpp>

namespace three {

class RenderableFace : public Renderable {
public:

	RenderableFace( THREE::FaceType type = THREE::Face3 )
	: Renderable ( 0 ), material ( nullptr ), faceMaterial ( nullptr ),
	  mType ( type ), mSize ( type == THREE::Face3 ? 3 : 4 ) { }

	RenderableFace( RenderableFace&& )      = default;
	RenderableFace( const RenderableFace& ) = default;
	RenderableFace& operator=( RenderableFace&& )      = default;
	RenderableFace& operator=( const RenderableFace& ) = default;

	/////////////////////////////////////////////////////////////////////////

	RenderableVertex v1, v2, v3, v4;

	Vector3 centroidWorld;
	Vector3 centroidScreen;

	Vector3 normalWorld;
	Vector3 vertexNormalsWorld[ 4 ];

	Material* material;
	Material* faceMaterial;

	std::array<std::vector<UV>, 4> uvs;

	THREE::FaceType type() const { return mType; }

	int size() { return mSize; }

private:

	THREE::FaceType mType;
	int mSize;
};

} // namespace three

#endif // THREE_RENDERABLE_FACE_HPP