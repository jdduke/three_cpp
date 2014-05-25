#include <three/extras/geometries/text_2d_geometry.h>

#include <three/console.h>

namespace three {

Text2DGeometry::Ptr Text2DGeometry::create( const std::string& text,
                                            const Font::Ptr& font ) {

  auto geometry = three::make_shared<Text2DGeometry>( text, font );
  geometry->update();

  return geometry;
}

Text2DGeometry::Text2DGeometry( const std::string& text,
                                const Font::Ptr& font )
  : Geometry(), text( text ), font( font ) { }

#ifdef TODO_THREE_DYNAMIC_GEOMETRY
void Text2DGeometry::update( const std::string& text ) {

  if ( this->text == text )
    return;

  this->text = text;

  update();

}
#endif

void Text2DGeometry::update() {

  if ( !font ) {
    console().warn( "Text2DGeometry: Invalid font" );
    return;
  }

  font->generate( text,
                  vertices,
                  faces,
                  faceVertexUvs[ 0 ] );

  computeCentroids();

  verticesNeedUpdate = true;
  uvsNeedUpdate      = true;

}

} // namespace three
