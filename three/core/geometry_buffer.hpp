#ifndef THREE_GEOMETRY_BUFFER_HPP
#define THREE_GEOMETRY_BUFFER_HPP

#include <three/core/face.hpp>

#include <three/renderers/gl_custom_attribute.hpp>

namespace three {

struct GeometryBuffer {

	typedef unsigned GLBuffer;

	std::vector<GLCustomAttribute> __glCustomAttributesList;

	GLBuffer __glColorBuffer;
	GLBuffer __glFaceBuffer;
	GLBuffer __glLineBuffer;
	GLBuffer __glNormalBuffer;
	GLBuffer __glSkinIndicesBuffer;
	GLBuffer __glSkinVertexABuffer;
	GLBuffer __glSkinVertexBBuffer;
	GLBuffer __glSkinWeightsBuffer;
	GLBuffer __glTangentBuffer;
	GLBuffer __glUV2Buffer;
	GLBuffer __glUVBuffer;
	GLBuffer __glVertexBuffer;

	std::vector<GLBuffer> __glMorphNormalsBuffers;
	std::vector<GLBuffer> __glMorphTargetsBuffers;

	int __glFaceCount;
	int __glLineCount;
	int __glParticleCount;
	int __glVertexCount;

	int numMorphTargets;
	int numMorphNormals;

	std::vector<float> __vertexArray;
	std::vector<float> __normalArray;
	std::vector<float> __tangentArray;
	std::vector<float> __colorArray;
	std::vector<std::pair<float, int>> __sortArray;

	std::vector<float> __uvArray;
	std::vector<float> __uv2Array;

	std::vector<float> __skinVertexAArray;
	std::vector<float> __skinVertexBArray;
	std::vector<int> __skinIndexArray;
	std::vector<int> __skinWeightArray;

	std::vector<uint16_t> __faceArray;
	std::vector<uint16_t> __lineArray;

	bool __inittedArrays;

};

}

#endif // THREE_GEOMETRY_BUFFER_HPP