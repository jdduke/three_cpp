#ifndef THREE_TEXTURE_HPP
#define THREE_TEXTURE_HPP

#include <three/common.hpp>
#include <three/utils.hpp>

#include <three/core/vector2.hpp>

#include <three/textures/texture_buffer.hpp>

#include <functional>

namespace three {

struct Image {
  Image() : width( 0 ), height ( 0 ), __glTextureCube( 0 ) { }
  Image( unsigned char* buffer, int bufferLength, int width, int height )
    : data( buffer, buffer + bufferLength ), width( width ), height( height ), __glTextureCube( 0 ) { }
  Image( std::vector<unsigned char> data, int width, int height )
    : data( std::move( data ) ), width( width ), height( height ), __glTextureCube( 0 ) { }
  bool valid() const { return data.size() > 0 && width > 0 && height > 0; }
  std::vector<unsigned char> data;
  int width, height;
  unsigned __glTextureCube;
};

struct TextureDesc {
  explicit TextureDesc( Image image,
                        THREE::PixelFormat format = THREE::RGBAFormat,
                        THREE::Mapping mapping    = THREE::UVMapping,
                        THREE::Wrapping wrapS     = THREE::ClampToEdgeWrapping,
                        THREE::Wrapping wrapT     = THREE::ClampToEdgeWrapping,
                        THREE::Filter magFilter   = THREE::LinearFilter,
                        THREE::Filter minFilter   = THREE::LinearMipMapLinearFilter,
                        THREE::DataType dataType  = THREE::UnsignedByteType,
                        float anisotropy          = 1 )
    : image( std::move( image ) ),
      mapping( mapping ),
      wrapS( wrapS ),
      wrapT( wrapT ),
      magFilter( magFilter ),
      minFilter( minFilter ),
      format( format ),
      dataType( dataType ),
      anisotropy( anisotropy ) { }

  Image image;
  THREE::Mapping mapping;
  THREE::Wrapping wrapS, wrapT;
  THREE::Filter magFilter, minFilter;
  THREE::PixelFormat format;
  THREE::DataType dataType;
  float anisotropy;
};

class Texture : public TextureBuffer {
public:

  typedef std::shared_ptr<Texture> Ptr;

  static Ptr create( const TextureDesc& desc ) { return three::make_shared<Texture>( desc ); }

  virtual THREE::TextureType type() const { return THREE::Texture; }

  /////////////////////////////////////////////////////////////////////////

  int id;

  std::vector<Image> image;

  THREE::Mapping mapping;

  THREE::Wrapping wrapS, wrapT;

  THREE::Filter magFilter, minFilter;

  THREE::PixelFormat format;

  THREE::DataType dataType;

  float anisotropy;

  Vector2 offset;
  Vector2 repeat;

  bool generateMipmaps;
  bool premultiplyAlpha;
  bool flipY;

  mutable bool needsUpdate;

  std::function<void( void )> onUpdate;

  /////////////////////////////////////////////////////////////////////////

  Texture::Ptr clone( ) const {

    auto texture = create( TextureDesc( image[0] ) );

    texture->image = image;

    texture->mapping = mapping;

    texture->wrapS = wrapS;
    texture->wrapT = wrapT;

    texture->magFilter = magFilter;
    texture->minFilter = minFilter;

    texture->format = format;
    texture->dataType = dataType;

    texture->anisotropy = anisotropy;

    texture->offset.copy( offset );
    texture->repeat.copy( repeat );

    texture->generateMipmaps = generateMipmaps;
    texture->premultiplyAlpha = premultiplyAlpha;
    texture->flipY = flipY;

    return texture;

  }

protected:

  Texture( const TextureDesc& desc )
    : id( TextureCount()++ ),
      image( 1, desc.image ),
      mapping( desc.mapping ),
      wrapS( desc.wrapS ),
      wrapT( desc.wrapT ),
      magFilter( desc.magFilter ),
      minFilter( desc.minFilter ),
      format( desc.format ),
      dataType( desc.dataType ),
      anisotropy( desc.anisotropy ),
      offset( 0, 0 ),
      repeat( 1, 1 ),
      generateMipmaps( true ),
      premultiplyAlpha( false ),
      flipY( true ),
      needsUpdate( true ) { }

private:

  static int& TextureCount() {
    static int sTextureCount = 0;
    return sTextureCount;
  }

};

} // namespace three

#endif // THREE_TEXTURE_HPP