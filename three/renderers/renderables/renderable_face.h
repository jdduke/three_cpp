#ifndef THREE_RENDERABLE_FACE_H
#define THREE_RENDERABLE_FACE_H

#include <three/common.h>
#include <three/constants.h>

#include <three/renderers/renderables/renderable.h>
#include <three/renderers/renderables/renderable_vertex.h>

namespace three {

class RenderableFace : public Renderable {

public:

  RenderableFace( )
    : Renderable( 0 ), vertexNormalsLength( 0 ), material( nullptr ) { }

  RenderableVertex v1, v2, v3;

  Vector3 centroidWorld;

  Vector3 normalModel;
  Vector3 normalModelView;

  int vertexNormalsLength;
  Vector3 vertexNormalsModel[3];
  Vector3 vertexNormalsModelView[3];

  optional<Color> color;
  Material* material;

  std::array<std::vector<Vector2>, 3> uvs;

};

} // namespace three

#endif // THREE_RENDERABLE_FACE_H