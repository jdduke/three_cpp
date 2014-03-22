#ifndef THREE_MESH_HPP
#define THREE_MESH_HPP

#include <three/core/object3d.h>
#include <three/core/geometry.h>
#include <three/visitor.h>
#include <three/materials/material.h>

#include <vector>
#include <unordered_map>
#include <memory>

namespace three {

  class Mesh : public Object3D {
  public:

    typedef std::shared_ptr<Mesh> Ptr;

    static Ptr create( const Geometry::Ptr& geometry, const Material::Ptr& material );

    /////////////////////////////////////////////////////////////////////////

    virtual enums::Type type() const { return enums::Mesh; }
    virtual void visit( Visitor& v ) { v( *this ); }
    virtual void visit( ConstVisitor& v ) const { v( *this ); }

    /////////////////////////////////////////////////////////////////////////

    float boundRadius;
    int morphTargetBase;

    std::vector<int> morphTargetForcedOrder;
    std::vector<int> morphTargetInfluences;
    std::unordered_map<std::string, int> morphTargetDictionary;

    /////////////////////////////////////////////////////////////////////////

    int getMorphTargetIndexByName( const std::string& name );

  protected:

    Mesh( const Geometry::Ptr& geometry, const Material::Ptr& material );

  };

} // namespace three


#if defined(THREE_HEADER_ONLY)
# include <three/objects/impl/mesh.cpp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_MESH_HPP