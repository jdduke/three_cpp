#ifndef THREE_TEXT_2D_GEOMETRY_HPP
#define THREE_TEXT_2D_GEOMETRY_HPP

#include <three/core/geometry.hpp>

#include <three/extras/utils/font.hpp>

namespace three {

class Text2DGeometry : public Geometry {
public:

  typedef std::shared_ptr<Text2DGeometry> Ptr;

  THREE_DECL static Ptr create( const std::string& text,
                                const Font::Ptr& font );

#ifdef TODO_THREE_DYNAMIC_GEOMETRY
  THREE_DECL void update( const std::string& text );
#endif

protected:

  THREE_DECL Text2DGeometry( const std::string& text,
                             const Font::Ptr& font );

  THREE_DECL void update();

  std::string text;
  Font::Ptr font;

};

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/extras/geometries/impl/text_2d_geometry.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_TEXT_2D_GEOMETRY_HPP