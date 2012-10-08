#ifndef THREE_LINE_HPP
#define THREE_LINE_HPP

#include <three/common.hpp>

#include <three/core/object3d.hpp>
#include <three/core/geometry.hpp>
#include <three/materials/line_basic_material.hpp>

namespace three {

class Line : public Object3D {
public:

  typedef std::shared_ptr<Line> Ptr;

  static Ptr create( const Geometry::Ptr& geometry,
                     const Material::Ptr& material,
                     THREE::LineType lineType = THREE::LineStrip ) {
    if ( material ) {
      return three::make_shared<Line>( geometry, material, lineType );
    } else {
      return three::make_shared<Line>( geometry, defaultMaterial(), lineType );
    }
  }

  /////////////////////////////////////////////////////////////////////////

  THREE_IMPL_OBJECT(Line)

  /////////////////////////////////////////////////////////////////////////

  THREE::LineType lineType;

protected:

  Line( const Geometry::Ptr& geometry, const Material::Ptr& material, THREE::LineType lineType )
    : Object3D( material, geometry ), lineType( lineType ) {

    if ( geometry ) {

      if ( geometry->boundingSphere.radius == 0 ) {
        geometry->computeBoundingSphere();
      }

    }
  }

  static Material::Ptr defaultMaterial() {
    return LineBasicMaterial::create(
      Material::Parameters().add( "color", Color( (int)(Math::random() * 0xFFFFFF) ) )
    );
  }
};

} // namespace three

#endif // THREE_MESH_HPP