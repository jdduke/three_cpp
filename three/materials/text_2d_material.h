#ifndef THREE_TEXT_2D_MATERIAL_HPP
#define THREE_TEXT_2D_MATERIAL_HPP

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

#if defined(THREE_HEADER_ONLY)
# include <three/materials/impl/text_2d_material.cpp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_TEXT_2D_MATERIAL_HPP

