#ifndef THREE_GEOMETRY_GROUP_HPP
#define THREE_GEOMETRY_GROUP_HPP

#include <three/common.hpp>

#include <three/core/geometry_buffer.hpp>

#include <three/utils.hpp>

#include <vector>

namespace three {

class GeometryGroup : public GeometryBuffer {
public:

  typedef std::shared_ptr<GeometryGroup> Ptr;

  static Ptr create( int materialIndex = -1, int numMorphTargets = 0, int numMorphNormals = 0 ) {
    return three::make_shared<GeometryGroup>( materialIndex, numMorphTargets, numMorphNormals );
  }

  typedef GeometryBuffer::GLBuffer GLBuffer;

  int id;

  std::vector<std::vector<float>>   __morphNormalsArrays;
  std::vector<std::vector<float>>   __morphTargetsArrays;

  std::vector<int>  faces3;
  std::vector<int>  faces4;

  std::vector<int>  offsets;

  GLBuffer vertexColorBuffer;
  GLBuffer vertexIndexBuffer;
  GLBuffer vertexNormalBuffer;
  GLBuffer vertexPositionBuffer;
  GLBuffer vertexUvBuffer;

  int vertices;

protected:

  explicit GeometryGroup( int materialIndex = -1, int numMorphTargets = 0, int numMorphNormals = 0 )
    : GeometryBuffer( numMorphTargets, numMorphNormals, materialIndex ),
      id( -1 ),
      vertexColorBuffer( 0 ),
      vertexIndexBuffer( 0 ),
      vertexNormalBuffer( 0 ),
      vertexPositionBuffer( 0 ),
      vertexUvBuffer( 0 ),
      vertices( 0 ) { }
};

} // namespace three

#endif // THREE_GEOMETRY_GROUP_HPP