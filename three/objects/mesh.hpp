#ifndef THREE_MESH_HPP
#define THREE_MESH_HPP

#include <three/core/object3d.hpp>
#include <three/core/geometry.hpp>
#include <three/visitor.hpp>
#include <three/materials/material.hpp>

#include <vector>
#include <unordered_map>
#include <memory>

namespace three {

  class Mesh : public Object3D {
  public:

    typedef std::shared_ptr<Mesh> Ptr;

    THREE_DECL static Ptr create( const Geometry::Ptr& geometry, const Material::Ptr& material );

    /////////////////////////////////////////////////////////////////////////

    virtual THREE::Type type() const { return THREE::Mesh; }
    virtual void visit( Visitor& v ) { v( *this ); }
    virtual void visit( ConstVisitor& v ) const { v( *this ); }

    /////////////////////////////////////////////////////////////////////////

    float boundRadius;
    int morphTargetBase;

    std::vector<int> morphTargetForcedOrder;
    std::vector<int> morphTargetInfluences;
    std::unordered_map<std::string, int> morphTargetDictionary;

    /////////////////////////////////////////////////////////////////////////

    THREE_DECL int getMorphTargetIndexByName( const std::string& name );

  protected:

    THREE_DECL Mesh( const Geometry::Ptr& geometry, const Material::Ptr& material );

  };

} // namespace three


#if defined(THREE_HEADER_ONLY)
# include <three/objects/impl/mesh.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MESH_HPP