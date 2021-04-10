#ifndef THREE_MESH_PHONG_MATERIAL_HPP
#define THREE_MESH_PHONG_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {

class MeshPhongMaterial : public Material
{
public:
    typedef std::shared_ptr<MeshPhongMaterial> Ptr;

    static Ptr create(const Parameters& parameters = Parameters())
    {
        return three::make_shared<MeshPhongMaterial>(parameters);
    }

    virtual THREE::MaterialType type() const { return THREE::MeshPhongMaterial; }

    /////////////////////////////////////////////////////////////////////////

    Ptr clone()
    {
        return Material::clone(*this);
    }

protected:
    MeshPhongMaterial(const Parameters& parameters)
        : Material()
    {
        fog = true;
        setParameters(parameters, DefaultKeys());
    }

    static const ParameterKeys& DefaultKeys()
    {
        static std::array<std::string, 30> sKeys = {
            "color",
            "ambient",
            "emissive",
            "specular",
            "shininess",
            "metal",
            "perPixel",
            "wrapAround",
            "wrapRGB",
            "map",
            "lightMap",
            "bumpMap",
            "bumpScale",
            "normalMap",
            "normalScale",
            "specularMap",
            "envMap",
            "combine",
            "reflectivity",
            "refractionRatio",
            "fog",
            "shading",
            "wireframe",
            "wireframeLinewidth",
            "wireframeLinecap",
            "wireframeLinejoin",
            "vertexColors",
            "skinning",
            "morphTargets",
            "morphNormals"
        };
        static ParameterKeys sKeysSet(sKeys.begin(), sKeys.end());
        return sKeysSet;
    }
};

} // namespace three

#endif // THREE_MESH_PHONG_MATERIAL_HPP
