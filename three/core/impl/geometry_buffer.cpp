#include "three/core/geometry_buffer.h"

namespace three {

GeometryBuffer::GeometryBuffer( int numMorphTargets, int numMorphNormals, int materialIndex )
  : materialIndex( materialIndex ),
    hasPositions( false ),
    hasNormals( false ),
    hasUvs( false ),
    hasColors( false ),
    hasTangents( false ),
    dynamic( true ),
    __glInit( false ),
    __glColorBuffer( 0 ),
    __glFaceBuffer( 0 ),
    __glLineBuffer( 0 ),
    __glNormalBuffer( 0 ),
    __glSkinIndicesBuffer( 0 ),
    __glSkinWeightsBuffer( 0 ),
    __glTangentBuffer( 0 ),
    __glUV2Buffer( 0 ),
    __glUVBuffer( 0 ),
    __glVertexBuffer( 0 ),
    __glFaceCount( 0 ),
    __glLineCount( 0 ),
    __glParticleCount( 0 ),
    __glVertexCount( 0 ),
    numMorphTargets( numMorphTargets ),
    numMorphNormals( numMorphNormals ),
    __inittedArrays( false ) { }

GeometryBuffer::~GeometryBuffer() { }

enums::GeometryType GeometryBuffer::type() const { return enums::Geometry; }

void GeometryBuffer::dispose() {
  __inittedArrays = false;
  __colorArray.clear();
  __normalArray.clear();
  __lineDistanceArray.clear();
  __tangentArray.clear();
  __uvArray.clear();
  __uv2Array.clear();
  __faceArray.clear();
  __vertexArray.clear();
  __lineArray.clear();
  __skinIndexArray.clear();
  __skinWeightArray.clear();
}

} // namespace three
