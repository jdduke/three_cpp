#ifndef THREE_GEOMETRY_HPP
#define THREE_GEOMETRY_HPP

#include <three/common.hpp>

#include <three/core/math.hpp>
#include <three/core/color.hpp>
#include <three/core/vector3.hpp>
#include <three/core/uv.hpp>

#include <three/materials/material.hpp>

namespace three {

class Geometry {
public:

    typedef shared_ptr<Geometry> Ptr;

    Ptr create() { return make_shared<Geometry>(); }

    /////////////////////////////////////////////////////////////////////////

    int id;

    std::string name;

    std::vector<Vector3> vertices;
    std::vector<Color> colors; // one-to-one vertex colors, used in ParticleSystem, Line and Ribbon

    std::vector<Material::Ptr> materials;

    std::vector<Face3> faces;

    std::vector<std::vector<UV>> faceUvs;
    std::vector<std::vector<UV>> faceVertexUvs;

    std::vector<Vector3> morphTargets; // TODO: Type
    std::vector<Color> morphColors;
    std::vector<Vector3> morphNormals;

    std::vector<float> skinWeights;
    std::vector<int> skinIndices;

    void* boundingBox;    // TODO: Type
    struct { float radius; } boundingSphere; // TODO: Type

    bool hasTangents;

    bool dynamic;

    /////////////////////////////////////////////////////////////////////////

protected:

    Geometry()
    : id ( GeometryCount()++ ),
    boundingBox ( nullptr ),
    boundingSphere ( { 0 } ),
    hasTangents ( false ),
    dynamic ( true ) { }

private:

    static int& GeometryCount() {
        static int sGeometryCount = 0;
        return sGeometryCount;
    }

};

} // namespace three

#endif // THREE_GEOMETRY_HPP