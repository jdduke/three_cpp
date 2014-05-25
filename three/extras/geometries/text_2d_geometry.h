#ifndef THREE_TEXT_2D_GEOMETRY_H
#define THREE_TEXT_2D_GEOMETRY_H

#include <three/core/geometry.h>

#include <three/extras/utils/font.h>

namespace three {

class THREE_DECL Text2DGeometry : public Geometry {
public:

  typedef std::shared_ptr<Text2DGeometry> Ptr;

  static Ptr create( const std::string& text,
                     const Font::Ptr& font );

#ifdef TODO_THREE_DYNAMIC_GEOMETRY
  void update( const std::string& text );
#endif

protected:

  Text2DGeometry( const std::string& text,
                  const Font::Ptr& font );

  void update();

  std::string text;
  Font::Ptr font;

};

} // namespace three

#endif // THREE_TEXT_2D_GEOMETRY_H