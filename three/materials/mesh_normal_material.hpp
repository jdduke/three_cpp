#ifndef THREE_MESH_NORMAL_MATERIAL_HPP
#define THREE_MESH_NORMAL_MATERIAL_HPP

#include <three/common.hpp>

#include <three/materials/material.hpp>

namespace three {
class MeshNormalMaterial : public Material
{
public:
    typedef std::shared_ptr<MeshNormalMaterial> Ptr;

    static Ptr create(const Parameters& parameters = Parameters())
    {
        return three::make_shared<MeshNormalMaterial>(parameters);
    }

    virtual THREE::MaterialType type() const { return THREE::MeshNormalMaterial; }

    /////////////////////////////////////////////////////////////////////////

    Ptr clone()
    {
        return Material::clone(*this);
    }

protected:
    MeshNormalMaterial(const Parameters& parameters)
        : Material()
    {
        shading = THREE::FlatShading;
        setParameters(parameters, DefaultKeys());
    }

    static const ParameterKeys& DefaultKeys()
    {
        static std::array<std::string, 3> sKeys = {
            "shading",
            "wireframe",
            "wireframeLinewidth"
        };
        static ParameterKeys sKeysSet(sKeys.begin(), sKeys.end());
        return sKeysSet;
    }
};

} // namespace three

#endif // THREE_MESH_NORMAL_MATERIAL_HPP
