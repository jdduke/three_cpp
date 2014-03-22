#ifndef enums_TEXTURE_H
#define enums_TEXTURE_H

#include <three/common.h>
#include <three/constants.h>
#include <three/utils/memory.h>
#include <three/math/vector2.h>
#include <three/textures/texture_buffer.h>

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
                        enums::PixelFormat format = enums::RGBAFormat,
                        enums::Mapping mapping    = enums::UVMapping,
                        enums::Wrapping wrapS     = enums::ClampToEdgeWrapping,
                        enums::Wrapping wrapT     = enums::ClampToEdgeWrapping,
                        enums::Filter magFilter   = enums::LinearFilter,
                        enums::Filter minFilter   = enums::LinearMipMapLinearFilter,
                        enums::DataType dataType  = enums::UnsignedByteType,
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
  enums::Mapping mapping;
  enums::Wrapping wrapS, wrapT;
  enums::Filter magFilter, minFilter;
  enums::PixelFormat format;
  enums::DataType dataType;
  float anisotropy;
};

class Texture : public TextureBuffer {

public:

  typedef std::shared_ptr<Texture> Ptr;

  static Ptr create( const TextureDesc& desc ) { 

    return three::make_shared<Texture>( desc ); 

  }

  const int id;

  const std::string uuid;

  std::string name;

  std::vector<Image> image;

  enums::Mapping mapping;

  enums::Wrapping wrapS, wrapT;

  enums::Filter magFilter, minFilter;

  enums::PixelFormat format;

  enums::DataType dataType;

  float anisotropy;

  Vector2 offset;
  Vector2 repeat;

  bool generateMipmaps;
  bool premultiplyAlpha;
  bool flipY;

  std::function<void( void )> onUpdate;

  virtual enums::TextureType type() const { 

    return enums::Texture; 

  }

  inline bool needsUpdate() const {

    return _needsUpdate;

  }

  inline void needsUpdate( bool value ) {

    if(value) {

      update();

    }

    _needsUpdate = value;

  }

  THREE_TODO("dispatcher")
  inline void update() const {

    //this.dispatchEvent( { type: 'update' } );

  }

  THREE_TODO("dispatcher")
  inline void dispose() const {

    //this.dispatchEvent( { type: 'dispose' } );

  }

  THREE_REVIEW("Why a vector with Image?")
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
      uuid( Math::generateUUID() ),
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
      _needsUpdate( true ) { }

private:

  static int& TextureCount() {

    static int sTextureCount = 0;

    return sTextureCount;

  }

  mutable bool _needsUpdate;

};

} // namespace three

#endif // enums_TEXTURE_H