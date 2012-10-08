#ifndef THREE_TEXT_2D_GEOMETRY_IPP
#define THREE_TEXT_2D_GEOMETRY_IPP

#include <three/extras/geometries/text_2d_geometry.hpp>

namespace three {

Text2DGeometry::Ptr Text2DGeometry::create( const std::string& text,
                                            const Font::Ptr& font ) {

  auto geometry = make_shared<Text2DGeometry>( text, font );
  geometry->update();

  return geometry;
}

/////////////////////////////////////////////////////////////////////////

Text2DGeometry( const std::string& text,
                const Font::Ptr& font )
 : Geometry(), text( text ), font( font ) { }

void Text2DGeometry::update( const std::string& text ) {

  if ( this->text == text )
    return;

  this->text = text;

  update();

}

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

};

} // namespace three

#endif // THREE_TEXT_2D_GEOMETRY_IPP