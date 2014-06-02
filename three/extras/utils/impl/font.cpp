#include <three/extras/utils/font.h>

#include <three/gl.h>
#include <three/math/color.h>
#include <three/math/vector2.h>
#include <three/math/vector4.h>
#include <three/math/matrix4.h>
#include <three/core/face.h>
#include <three/utils/conversion.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <three/extras/utils/impl/stb_truetype.h>

namespace three {

namespace detail {

inline std::vector<unsigned char> load( const std::string& file ) {
  FILE* fp = fopen( file.c_str(), "rb" );
  if (!fp) return std::vector<unsigned char>();

  fseek( fp, 0, SEEK_END );
  int size = ftell( fp );
  fseek( fp, 0, SEEK_SET );

  std::vector<unsigned char> buffer( size );
  fread( buffer.data(), 1, size, fp );
  fclose( fp );
  return buffer;
}

} // namespace detail


struct Font::Impl {
  Impl( )
    : fontSize( 15 ),
      textureWidth( 512 ),
      textureHeight( 512 ),
      firstCharacter( 0 ),
      countCharacter( 0 ),
      texture( 0 ) { }

  std::vector<stbtt_bakedchar> characterData;
  float fontSize;
  int textureWidth;
  int textureHeight;
  int firstCharacter;
  int countCharacter;
  Texture::Ptr texture;
};

Font::Ptr Font::create( const std::string& ttf,
                        float size,
                        int textureWidth,
                        int textureHeight,
                        int firstCharacter,
                        int countCharacter ) {

  Font::Ptr font = three::make_shared<Font>();
  if ( !font->initialize( ttf,
                          size,
                          textureWidth,
                          textureHeight,
                          firstCharacter,
                          countCharacter) ) {
    font.reset();
  }
  return font;
}

void Font::generate( const std::string& text,
                     std::vector<Vertex>& vertices,
                     std::vector<Face>& faces,
                     std::vector<std::array<Vector2, 3>>& faceUvs ) {

  Vector3 normal( 0, 0, 1 );

  vertices.clear();
  faces.clear();
  faceUvs.clear();

  auto x = 0.f, y = 0.f;

  for ( size_t i = 0; i < text.size(); ++i ) {

    const auto c = text.at( i );

    if( c >= impl->firstCharacter &&
        c <= ( impl->firstCharacter + impl->countCharacter ) ) {

      std::array<Vector3, 4> vert;
      std::array<Vector2, 4> uv;

#if 1

      const auto& bakedchar = impl->characterData[( c - impl->firstCharacter )];
      float round_x = Math::round( x + bakedchar.xoff);
      float round_y = Math::round( y - bakedchar.yoff);

      stbtt_aligned_quad quad;
      quad.x0 = ( float )round_x;
      quad.y0 = ( float )round_y;
      quad.x1 = ( float )round_x + bakedchar.x1 - bakedchar.x0;
      quad.y1 = ( float )round_y - bakedchar.y1 + bakedchar.y0;

      quad.s0 = bakedchar.x0 / ( float )impl->textureWidth;
      quad.t0 = bakedchar.y0 / ( float )impl->textureWidth;
      quad.s1 = bakedchar.x1 / ( float )impl->textureHeight;
      quad.t1 = bakedchar.y1 / ( float )impl->textureHeight;

      x += bakedchar.xadvance;

      vert[ 0 ].x = quad.x1;
      vert[ 0 ].y = quad.y0;
      uv  [ 0 ].x = quad.s1;
      uv  [ 0 ].y = quad.t0;

      vert[ 1 ].x = quad.x0;
      vert[ 1 ].y = quad.y0;
      uv  [ 1 ].x = quad.s0;
      uv  [ 1 ].y = quad.t0;

      vert[ 2 ].x = quad.x0;
      vert[ 2 ].y = quad.y1;
      uv  [ 2 ].x = quad.s0;
      uv  [ 2 ].y = quad.t1;

      vert[ 3 ].x = quad.x1;
      vert[ 3 ].y = quad.y1;
      uv  [ 3 ].x = quad.s1;
      uv  [ 3 ].y = quad.t1;

#elif 1

      stbtt_aligned_quad quad;
      stbtt_GetBakedQuad(impl->characterData.data(),
                         impl->textureWidth,
                         impl->textureHeight,
                         c - impl->firstCharacter,
                         &x, &y,
                         &quad,
                         1);//1=opengl,0=old d3d

      vert[ 0 ].x = quad.x0;
      vert[ 0 ].y = quad.y0;
      uv  [ 0 ].u = quad.s0;
      uv  [ 0 ].v = quad.t1;

      vert[ 1 ].x = quad.x1;
      vert[ 1 ].y = quad.y0;
      uv  [ 1 ].u = quad.s1;
      uv  [ 1 ].v = quad.t1;

      vert[ 2 ].x = quad.x1;
      vert[ 2 ].y = quad.y1;
      uv  [ 2 ].u = quad.s1;
      uv  [ 2 ].v = quad.t0;

      vert[ 3 ].x = quad.x0;
      vert[ 3 ].y = quad.y1;
      uv  [ 3 ].u = quad.s0;
      uv  [ 3 ].v = quad.t0;

#else

      const auto& bakedchar = impl->characterData[( c - impl->firstCharacter )];
      int round_x = STBTT_ifloor( x + bakedchar.xoff );
      int round_y = STBTT_ifloor( y + bakedchar.yoff );

      stbtt_aligned_quad quad;
      quad.x0 = ( float )round_x;
      quad.y0 = ( float )round_y;
      quad.x1 = ( float )round_x + bakedchar.x1 - bakedchar.x0;
      quad.y1 = ( float )round_y - bakedchar.y1 + bakedchar.y0;

      quad.s0 = bakedchar.x0 / ( float )impl->textureWidth;
      quad.t0 = bakedchar.y0 / ( float )impl->textureWidth;
      quad.s1 = bakedchar.x1 / ( float )impl->textureHeight;
      quad.t1 = bakedchar.y1 / ( float )impl->textureHeight;

      x += bakedchar.xadvance;

      vert[ 0 ].x = quad.x1;
      vert[ 0 ].y = quad.y0;
      uv  [ 0 ].u = quad.s1;
      uv  [ 0 ].v = quad.t0;

      vert[ 1 ].x = quad.x0;
      vert[ 1 ].y = quad.y0;
      uv  [ 1 ].u = quad.s0;
      uv  [ 1 ].v = quad.t0;

      vert[ 2 ].x = quad.x0;
      vert[ 2 ].y = quad.y1;
      uv  [ 2 ].u = quad.s0;
      uv  [ 2 ].v = quad.t1;

      vert[ 3 ].x = quad.x1;
      vert[ 3 ].y = quad.y1;
      uv  [ 3 ].u = quad.s1;
      uv  [ 3 ].v = quad.t1;

#endif

      const auto offset = (int)vertices.size();

      vertices.insert( vertices.end(), vert.data(), vert.data() + vert.size() );

      auto face = Face( offset, offset + 1, offset + 3 );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );
      faces.push_back( face );

      faceUvs.push_back( toArray( uv[0], uv[1], uv[3] ) );

      face = Face( offset + 1, offset + 2, offset + 3 );
      face.normal.copy( normal );
      face.vertexNormals.fill( normal );
      faces.push_back( face );

      faceUvs.push_back( toArray( uv[1], uv[2], uv[3] ) );

    }
  }
}

Font::Font() : impl( new Impl() ) { }

Font::~Font() { }

bool Font::initialize( const std::string& ttf,
                       float fontSize,
                       int textureWidth,
                       int textureHeight,
                       int firstCharacter,
                       int countCharacter ) {

  auto buffer = detail::load( ttf );
  std::vector<unsigned char> texels( textureWidth * textureHeight );
  impl->characterData.resize( countCharacter );
  impl->fontSize = fontSize;
  impl->textureWidth = textureWidth;
  impl->textureHeight = textureHeight;
  impl->firstCharacter = firstCharacter;
  impl->countCharacter = countCharacter;
  stbtt_BakeFontBitmap( buffer.data(),
                        0,
                        fontSize,
                        texels.data(),
                        textureWidth,
                        textureHeight,
                        firstCharacter,
                        countCharacter,
                        impl->characterData.data() );
  impl->texture = Texture::create(
                    TextureDesc( Image(texels, textureWidth, textureHeight),
                                 THREE::AlphaFormat )
                  );

  return !!impl->texture;
}

const Texture::Ptr& Font::texture() const {
  return impl->texture;
}

} // namespace three
