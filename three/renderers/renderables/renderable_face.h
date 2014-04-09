#ifndef THREE_RENDERABLE_FACE_H
#define THREE_RENDERABLE_FACE_H

#include <three/common.h>
#include <three/constants.h>

#include <three/renderers/renderables/renderable.h>
#include <three/renderers/renderables/renderable_vertex.h>

namespace three {

class RenderableFace : public Renderable {

public:

  RenderableFace( enums::FaceType type = enums::Face3 )
    : Renderable( 0 ), vertexNormalsLength( 0 ), color( nullptr ), material( nullptr ),
      mType( type ), mSize( type == enums::Face3 ? 3 : 4 ) { }

  RenderableVertex v1, v2, v3;

  Vector3 centroidWorld;

  Vector3 normalModel;
  Vector3 normalModelView;

  float vertexNormalsLength;
  Vector3 vertexNormalsModel[3];
  Vector3 vertexNormalsModelView[3];

  Color* color;
  Material* material;

  std::array<std::vector<Vector2>, 4> uvs;

  enums::FaceType type() const {
    return mType;
  }

  int size() {
    return mSize;
  }

private:

  enums::FaceType mType;
  int mSize;
  
};

} // namespace three

#endif // THREE_RENDERABLE_FACE_H