#ifndef THREE_TEXT_2D_MATERIAL_H
#define THREE_TEXT_2D_MATERIAL_H

#include <three/common.h>

#include <three/materials/shader_material.h>

namespace three {

class Text2DMaterial {
public:

  typedef std::shared_ptr<ShaderMaterial> Ptr;

  static Ptr create( const Font& font,
                     const Color& color = Color( 0xffffff ),
                     float opacity = 1.f );

};

} // namespace three

#endif // THREE_TEXT_2D_MATERIAL_H

