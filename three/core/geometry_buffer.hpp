#ifndef THREE_GEOMETRY_BUFFER_HPP
#define THREE_GEOMETRY_BUFFER_HPP

#include <three/constants.hpp>
#include <three/core/face.hpp>
#include <three/materials/attribute.hpp>
#include <three/utils/noncopyable.hpp>

#include <memory>

namespace three {

class GeometryBuffer : NonCopyable
{
public:
    virtual THREE::GeometryType type() const { return THREE::Geometry; }

    virtual ~GeometryBuffer() {}

    typedef Buffer GLBuffer;

    Index materialIndex;

    bool hasPositions;
    bool hasNormals;
    bool hasUvs;
    bool hasColors;
    bool hasTangents;
    bool dynamic;

    // Only delete if the buffer has created a new attribute
    //      from an original...
    struct AttributeDeleter
    {
        void operator()(Attribute* a)
        {
            if (a && a->__original != nullptr)
                delete a;
        }
    };
    typedef std::unique_ptr<Attribute, AttributeDeleter> AttributePtr;
    std::vector<AttributePtr> __glCustomAttributesList;

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
    std::vector<float> __skinWeightArray;

    std::vector<uint16_t> __faceArray;
    std::vector<uint16_t> __lineArray;

    bool __inittedArrays;

    void dispose()
    {
        __inittedArrays = false;
        __colorArray.clear();
        __normalArray.clear();
        __tangentArray.clear();
        __uvArray.clear();
        __uv2Array.clear();
        __faceArray.clear();
        __vertexArray.clear();
        __lineArray.clear();
        __skinVertexAArray.clear();
        __skinVertexBArray.clear();
        __skinIndexArray.clear();
        __skinWeightArray.clear();
    }

    GeometryBuffer(int numMorphTargets = 0, int numMorphNormals = 0, int materialIndex = -1)
        : materialIndex(materialIndex), hasPositions(false), hasNormals(false), hasUvs(false), hasColors(false), hasTangents(false), dynamic(true), __glColorBuffer(0), __glFaceBuffer(0), __glLineBuffer(0), __glNormalBuffer(0), __glSkinIndicesBuffer(0), __glSkinVertexABuffer(0), __glSkinVertexBBuffer(0), __glSkinWeightsBuffer(0), __glTangentBuffer(0), __glUV2Buffer(0), __glUVBuffer(0), __glVertexBuffer(0), __glFaceCount(0), __glLineCount(0), __glParticleCount(0), __glVertexCount(0), numMorphTargets(numMorphTargets), numMorphNormals(numMorphNormals), __inittedArrays(false) {}
};

}

#endif // THREE_GEOMETRY_BUFFER_HPP