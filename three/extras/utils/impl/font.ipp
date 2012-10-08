#ifndef THREE_FONT_IPP
#define THREE_FONT_IPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/extras/utils/font.hpp>

#if !THREE_NEW_FONT

#include <three/gl.hpp>
#include <three/core/color.hpp>
#include <three/core/vector2.hpp>
#include <three/core/vector4.hpp>
#include <three/core/matrix4.hpp>

#define STB_TRUETYPE_IMPLEMENTATION
#include <three/extras/utils/impl/stb_truetype.h>

namespace three {
namespace utils {

namespace detail {

inline bool compile( GLenum shaderType, unsigned& shader, const char* source ) {

  if( shader ) return false;

  shader = glCreateShader( shaderType );
  glShaderSource( shader, 1, &source, NULL );
  glCompileShader( shader );

  if ( !glTrue( glGetShaderParameter( shader, GL_COMPILE_STATUS ) ) ) {
    int loglen;
    char logbuffer[1000];
    glGetShaderInfoLog( shader, sizeof( logbuffer ), &loglen, logbuffer );
    console().error( logbuffer );
    glDeleteShader( shader );
    shader = 0;

    return false;
  }

  return true;

}

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


/////////////////////////////////////////////////////////////////////////

struct Font::Impl {
  Impl( )
    : fontSize( 15 ),
      textureWidth( 512 ),
      textureHeight( 512 ),
      firstCharacter( 0 ),
      countCharacter( 0 ),
      program( 0 ),
      vertexShader( 0 ),
      fragmentShader( 0 ),
      tid( 0 ),
      vertexAttrib( 0 ), texcoordAttrib( 0 ),
      mvpUniform( 0 ), diffuseUniform( 0 ), colorUniform( 0 ) { }

  ~Impl() {
    if ( program ) {
      glDeleteProgram( program );
      program = 0;
      glDeleteShader( vertexShader );
      vertexShader = 0;
      glDeleteShader( fragmentShader );
      fragmentShader = 0;
    }

    if ( tid ) {
      glDeleteTexture( tid );
    }
  }

  std::vector<stbtt_bakedchar> characterData;
  float fontSize;
  int textureWidth;
  int textureHeight;
  int firstCharacter;
  int countCharacter;
  unsigned program, vertexShader, fragmentShader;
  unsigned tid;
  unsigned vertexAttrib, texcoordAttrib;
  unsigned mvpUniform, diffuseUniform, colorUniform;
};

/////////////////////////////////////////////////////////////////////////

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

void Font::render( const char* text,
                   float x, float y,
                   float w, float h,
                   const Color& color ) {

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  glDisable( GL_CULL_FACE );
  glDisable( GL_DEPTH_TEST );
  glDepthMask( GL_FALSE );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glUseProgram( impl->program );

  Matrix4 m;
  m.makeOrthographic( 0, w, h, 0, -1, 1 );
  glUniformMatrix4fv( impl->mvpUniform,
                      1,
                      GL_FALSE,
                      m.elements );//( float * )GFX_get_modelview_projection_matrix() );

  Vector4 rgba(color.r, color.g, color.b, 1.f);
  glUniform1i(  impl->diffuseUniform, 0 );
  glUniform4fv( impl->colorUniform, 1, rgba.xyzw );

  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, impl->tid );
  glEnableVertexAttribArray( impl->vertexAttrib );
  glEnableVertexAttribArray( impl->texcoordAttrib );

  while( *text ) {

    if( *text >= impl->firstCharacter &&
        *text <= ( impl->firstCharacter + impl->countCharacter ) ) {

      Vector2 vert[ 4 ];
      Vector2 uv[ 4 ];

      const auto& bakedchar = impl->characterData[( *text - impl->firstCharacter )];

      int round_x = STBTT_ifloor( x + bakedchar.xoff );
      int round_y = STBTT_ifloor( y - bakedchar.yoff );

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

      vert[ 0 ].x = quad.x1; vert[ 0 ].y = quad.y0;
      uv  [ 0 ].x = quad.s1; uv  [ 0 ].y = quad.t0;

      vert[ 1 ].x = quad.x0; vert[ 1 ].y = quad.y0;
      uv  [ 1 ].x = quad.s0; uv  [ 1 ].y = quad.t0;

      vert[ 2 ].x = quad.x1; vert[ 2 ].y = quad.y1;
      uv  [ 2 ].x = quad.s1; uv  [ 2 ].y = quad.t1;

      vert[ 3 ].x = quad.x0; vert[ 3 ].y = quad.y1;
      uv  [ 3 ].x = quad.s0; uv  [ 3 ].y = quad.t1;

      glVertexAttribPointer( impl->vertexAttrib,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            ( float* )&vert[ 0 ] );

      glVertexAttribPointer( impl->texcoordAttrib,
                            2,
                            GL_FLOAT,
                            GL_FALSE,
                            0,
                            ( float* )&uv[ 0 ] );

      glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
    }

    ++text;
  }

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  glBindTexture( GL_TEXTURE_2D, 0 );
  glEnable( GL_CULL_FACE );
  glEnable( GL_DEPTH_TEST );
  glDepthMask( GL_TRUE );
  glDisable( GL_BLEND );
}

/////////////////////////////////////////////////////////////////////////

Font::Font() : impl( new Impl() ) { }

Font::~Font() { }

bool Font::initialize( const std::string& ttf,
                       float fontSize,
                       int textureWidth,
                       int textureHeight,
                       int firstCharacter,
                       int countCharacter ) {

  detail::compile( GL_VERTEX_SHADER, impl->vertexShader,
          "uniform mat4 MODELVIEWPROJECTIONMATRIX;"
          "attribute vec2 POSITION;"
          "attribute vec2 TEXCOORD0;"
          "varying vec2 texcoord0;"
          "void main( void ) {"
          "  texcoord0 = TEXCOORD0;"
          "  gl_Position = MODELVIEWPROJECTIONMATRIX * vec4( POSITION.x, POSITION.y, 0.0, 1.0 );"
          "}" );

  detail::compile( GL_FRAGMENT_SHADER, impl->fragmentShader,
           "uniform sampler2D DIFFUSE;"
           "uniform vec4 COLOR;"
           "varying vec2 texcoord0;"
           "void main( void ) {"
           "  vec4 color = texture2D( DIFFUSE, texcoord0 );"
           "  color.x = COLOR.x;"
           "  color.y = COLOR.y;"
           "  color.z = COLOR.z;"
           "  color.w *= COLOR.w;"
           "  gl_FragColor = color;"
           "}" );

  if ( !impl->vertexShader || !impl->fragmentShader ) {
    return false;
  }


  impl->program = glCreateProgram();
  glAttachShader( impl->program, impl->vertexShader );
  glAttachShader( impl->program, impl->fragmentShader );
  glLinkProgram( impl->program );

  int status = 0;
  glGetProgramiv( impl->program, GL_LINK_STATUS, &status );
  if( !status ) {
    console().error() << "three::utils::Font: Unable to link text program";
    glDeleteProgram( impl->program );
    impl->program = 0;
  }

  if ( !impl->program )
    return false;

  /////////////////////////////////////////////////////////////////////////

  impl->vertexAttrib   = glGetAttribLocation( impl->program, "POSITION" ),
  impl->texcoordAttrib = glGetAttribLocation( impl->program, "TEXCOORD0" );
  impl->mvpUniform     = glGetUniformLocation( impl->program, "MODELVIEWPROJECTIONMATRIX");
  impl->diffuseUniform = glGetUniformLocation( impl->program, "DIFFUSE" );
  impl->colorUniform   = glGetUniformLocation( impl->program, "COLOR" );

  /////////////////////////////////////////////////////////////////////////

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

  glGenTextures(1, &impl->tid );
  glBindTexture( GL_TEXTURE_2D, impl->tid );
  glTexImage2D( GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                textureWidth,
                textureHeight,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                texels.data() );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  return true;
}

} // namespace utils
} // namespace three

#endif // !THREE_NEW_FONT

#endif // THREE_FONT_IPP