#ifndef THREE_LINE_H
#define THREE_LINE_H

#include <three/common.h>

#include <three/core/object3d.h>
#include <three/core/geometry.h>
#include <three/materials/line_basic_material.h>

namespace three {

class Line : public Object3D {
public:

  THREE_IMPL_OBJECT(Line);

  static Ptr create( const Geometry::Ptr& geometry,
                     const Material::Ptr& material,
                     THREE::LineType lineType = THREE::LineStrip ) {
    if ( material ) {
      return three::make_shared<Line>( geometry, material, lineType );
    } else {
      return three::make_shared<Line>( geometry, defaultMaterial(), lineType );
    }
  }

  THREE::LineType lineType;

protected:

  Line( const Geometry::Ptr& geometry, const Material::Ptr& material, THREE::LineType lineType )
    : Object3D( material, geometry ), lineType( lineType ) {

    if ( geometry ) {

      if ( !geometry->boundingSphere ) {
        geometry->computeBoundingSphere();
      }

    }
  }

  // TODO "Line::__clone"


  static Material::Ptr defaultMaterial() {
    return LineBasicMaterial::create(
             Material::Parameters().add( "color", Color( (int)(Math::random() * 0xFFFFFF) ) )
           );
  }
};

} // namespace three

#endif // THREE_MESH_H