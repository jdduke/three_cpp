#include <three/extras/image_utils.h>

#include <three/console.h>
#include <three/extras/utils/impl/stb_image.h>

#include <vector>

namespace three {

namespace detail {

struct MipMap {
  std::vector<unsigned char> data;
  int width, height;
};

struct DDS {
  DDS() : width( 0 ), height( 0 ), format( 0 ), mipmapCount( 1 ) { }
  std::vector<MipMap> mipmaps;
  int width;
  int height;
  int format;
  int mipmapCount;
};

template <int a, int b, int c, int d>
struct FourCC {
  static const unsigned int value = (((((d << 8) | c) << 8) | b) << 8) | a;
};

inline DDS parseDDS( const std::vector<unsigned char>& buffer, bool loadMipmaps ) {

  DDS dds;

  // Adapted from @toji's DDS utils
  //  https://github.com/toji/webgl-texture-utils/blob/master/texture-util/dds.js

  // All values and structures referenced from:
  // http://msdn.microsoft.com/en-us/library/bb943991.aspx/

  enum { DDS_MAGIC = 0x20534444 };

  enum { DDSD_CAPS = 0x1,
         DDSD_HEIGHT = 0x2,
         DDSD_WIDTH = 0x4,
         DDSD_PITCH = 0x8,
         DDSD_PIXELFORMAT = 0x1000,
         DDSD_MIPMAPCOUNT = 0x20000,
         DDSD_LINEARSIZE = 0x80000,
         DDSD_DEPTH = 0x800000
       };

  enum { DDSCAPS_COMPLEX = 0x8,
         DDSCAPS_MIPMAP = 0x400000,
         DDSCAPS_TEXTURE = 0x1000
       };

  enum { DDSCAPS2_CUBEMAP = 0x200,
         DDSCAPS2_CUBEMAP_POSITIVEX = 0x400,
         DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800,
         DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000,
         DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000,
         DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000,
         DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
         DDSCAPS2_VOLUME = 0x200000
       };

  enum { DDPF_ALPHAPIXELS = 0x1,
         DDPF_ALPHA = 0x2,
         DDPF_FOURCC = 0x4,
         DDPF_RGB = 0x40,
         DDPF_YUV = 0x200,
         DDPF_LUMINANCE = 0x20000
       };

  auto int32ToFourCC = []( int value ) -> std::string {
    std::string s;
    s += ( ( char )( value & 0xff ) );
    s += ( ( char )( ( value >> 8 ) & 0xff ) );
    s += ( ( char )( ( value >> 16 ) & 0xff ) );
    s += ( ( char )( ( value >> 24 ) & 0xff ) );
    return s;
  };

  const auto FOURCC_DXT1 = FourCC<'D','X','T','1'>::value;
  const auto FOURCC_DXT3 = FourCC<'D','X','T','3'>::value;
  const auto FOURCC_DXT5 = FourCC<'D','X','T','5'>::value;

  const auto headerLengthInt = 31; // The header length in 32 bit ints

  // Offsets into the header array

  enum {
    off_magic = 0,
    off_size = 1,
    off_flags = 2,
    off_height = 3,
    off_width = 4,
    off_mipmapCount = 7,
    off_pfFlags = 20,
    off_pfFourCC = 21
  };

  // Parse header

  std::vector<int> header( (const int*)buffer.data(),
                           (const int*)buffer.data() + headerLengthInt );

  if ( header[ off_magic ] != DDS_MAGIC ) {
    console().error( "ImageUtils.parseDDS(): Invalid magic number in DDS header" );
    return dds;
  }

  if ( (header[ off_pfFlags ] & DDPF_FOURCC) != DDPF_FOURCC ) {
    console().error( "ImageUtils.parseDDS(): Unsupported format, must contain a FourCC code" );
    return dds;
  }

  int blockBytes;

  const auto fourCC = header[ off_pfFourCC ];

  switch ( fourCC ) {

  case FOURCC_DXT1:
    blockBytes = 8;
    dds.format = enums::RGB_S3TC_DXT1_Format;
    break;

  case FOURCC_DXT3:
    blockBytes = 16;
    dds.format = enums::RGBA_S3TC_DXT3_Format;
    break;

  case FOURCC_DXT5:
    blockBytes = 16;
    dds.format = enums::RGBA_S3TC_DXT5_Format;
    break;

  default:

    console().error() << "ImageUtils.parseDDS(): Unsupported FourCC code: " << int32ToFourCC( fourCC );
    return dds;
  };

  dds.mipmapCount = 1;
  if ( header[ off_flags ] & DDSD_MIPMAPCOUNT && loadMipmaps ) {
    dds.mipmapCount = Math::max( 1, header[ off_mipmapCount ] );
  }

  dds.width = header[ off_width ];
  dds.height = header[ off_height ];

  auto dataOffset = header[ off_size ] + 4;

  // Extract mipmaps buffers

  auto width = dds.width;
  auto height = dds.height;

  for ( int i = 0; i < dds.mipmapCount; i ++ ) {

    const auto dataLength = Math::max( 4, width ) / 4 * Math::max( 4, height ) / 4 * blockBytes;
    auto byteArray = std::vector<unsigned char>(buffer.data() + dataOffset, buffer.data() + dataOffset + dataLength );

    MipMap mipmap = { byteArray, width, height };
    dds.mipmaps.push_back( mipmap );

    dataOffset += dataLength;

    width = Math::max(   width / 2, 1 );
    height = Math::max( height / 2, 1 );

  }

  return dds;

}

//#endif // TODO_LOAD_COMPRESSED_TEXTURE

#ifdef TODO_NORMAL_MAP

inline getNormalMap:
function( image, depth ) {

  // Adapted from http://www.paulbrunt.co.uk/lab/heightnormal/

  var cross = function( a, b ) {
    return [ a[ 1 ] * b[ 2 ] - a[ 2 ] * b[ 1 ], a[ 2 ] * b[ 0 ] - a[ 0 ] * b[ 2 ], a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ] ];
  }

  var subtract = function( a, b ) {
    return [ a[ 0 ] - b[ 0 ], a[ 1 ] - b[ 1 ], a[ 2 ] - b[ 2 ] ];
  }

  var normalize = function( a ) {
    var l = Math::sqrt( a[ 0 ] * a[ 0 ] + a[ 1 ] * a[ 1 ] + a[ 2 ] * a[ 2 ] );
    return [ a[ 0 ] / l, a[ 1 ] / l, a[ 2 ] / l ];
  }

  depth = depth | 1;

  var width = image.width;
  var height = image.height;

  var canvas = document.createElement( 'canvas' );
  canvas.width = width;
  canvas.height = height;

  var context = canvas.getContext( '2d' );
  context.drawImage( image, 0, 0 );

  var data = context.getImageData( 0, 0, width, height ).data;
  var imageData = context.createImageData( width, height );
  var output = imageData.data;

  for ( var x = 0; x < width; x ++ ) {

    for ( var y = 0; y < height; y ++ ) {

      var ly = y - 1 < 0 ? 0 : y - 1;
      var uy = y + 1 > height - 1 ? height - 1 : y + 1;
      var lx = x - 1 < 0 ? 0 : x - 1;
      var ux = x + 1 > width - 1 ? width - 1 : x + 1;

      var points = [];
      var origin = [ 0, 0, data[( y * width + x ) * 4 ] / 255 * depth ];
      points.push( [ - 1, 0, data[( y * width + lx ) * 4 ] / 255 * depth ] );
      points.push( [ - 1, - 1, data[( ly * width + lx ) * 4 ] / 255 * depth ] );
      points.push( [ 0, - 1, data[( ly * width + x ) * 4 ] / 255 * depth ] );
      points.push( [  1, - 1, data[( ly * width + ux ) * 4 ] / 255 * depth ] );
      points.push( [ 1, 0, data[( y * width + ux ) * 4 ] / 255 * depth ] );
      points.push( [ 1, 1, data[( uy * width + ux ) * 4 ] / 255 * depth ] );
      points.push( [ 0, 1, data[( uy * width + x ) * 4 ] / 255 * depth ] );
      points.push( [ - 1, 1, data[( uy * width + lx ) * 4 ] / 255 * depth ] );

      var normals = [];
      var num_points = points.length;

      for ( var i = 0; i < num_points; i ++ ) {

        var v1 = points[ i ];
        var v2 = points[( i + 1 ) % num_points ];
        v1 = subtract( v1, origin );
        v2 = subtract( v2, origin );
        normals.push( normalize( cross( v1, v2 ) ) );

      }

      var normal = [ 0, 0, 0 ];

      for ( var i = 0; i < normals.length; i ++ ) {

        normal[ 0 ] += normals[ i ][ 0 ];
        normal[ 1 ] += normals[ i ][ 1 ];
        normal[ 2 ] += normals[ i ][ 2 ];

      }

      normal[ 0 ] /= normals.length;
      normal[ 1 ] /= normals.length;
      normal[ 2 ] /= normals.length;

      var idx = ( y * width + x ) * 4;

      output[ idx ] = ( ( normal[ 0 ] + 1.0 ) / 2.0 * 255 ) | 0;
      output[ idx + 1 ] = ( ( normal[ 1 ] + 1.0 ) / 2.0 * 255 ) | 0;
      output[ idx + 2 ] = ( normal[ 2 ] * 255 ) | 0;
      output[ idx + 3 ] = 255;

    }

  }

  context.putImageData( imageData, 0, 0 );

  return canvas;

}

#endif // TODO_NORMAL_MAP

} // namespace detail

Texture::Ptr ImageUtils::loadTexture( const std::string& url ) {
  //,enums::Mapping mapping /*= enums::UVMapping*/ ) {

  typedef std::unique_ptr<unsigned char, std::function<void(unsigned char*)>> stbi_ptr;

  int w, h, n;
  stbi_ptr data( stbi_load( url.c_str(), &w, &h, &n, 0), []( unsigned char* data ) {
    if ( data ) stbi_image_free( data );
  } );

  if ( !data ) {
    console().error() << "three::ImageUtils::loadTexture: Error loading " << url;
    return Texture::Ptr();
  }

  return Texture::create(
           TextureDesc( Image(data.get(), w * h * n, w, h),
                        n == 3 ? enums::RGBFormat : enums::RGBAFormat )
         );

}

#ifdef TODO_LOAD_COMPRESSED_TEXTURE

Texture::Ptr ImageUtils::loadCompressedTexture( const std::string& url,
    enums::Mapping mapping /*= enums::UVMapping*/ ) {

  var texture = new enums::CompressedTexture();
  texture.mapping = mapping;

  var request = new XMLHttpRequest();

  request.onload = function() {

    var buffer = request.response;
    var dds = enums::ImageUtils.parseDDS( buffer, true );

    texture.format = dds.format;

    texture.mipmaps = dds.mipmaps;
    texture.image.width = dds.width;
    texture.image.height = dds.height;

    // gl.generateMipmap fails for compressed textures
    // mipmaps must be embedded in the DDS file
    // or texture filters must not use mipmapping

    texture.generateMipmaps = false;

    texture.needsUpdate = true;

    if ( onLoad ) onLoad( texture );

  }

  request.onerror = onError;

  request.open( 'GET', url, true );
  request.responseType = "arraybuffer";
  request.send( null );

  return texture;

}

#endif // TODO_LOAD_COMPRESSED_TEXTURE

Texture::Ptr ImageUtils::generateDataTexture( int width, int height, Color color ) {

  const auto size = width * height;
  if ( size <= 0 ) {
    console().error("three::ImageUtils::generateDataTexture: Texture must have positive dimensions");
    return Texture::Ptr();
  }

  std::vector<unsigned char> data( 3 * size );

  const auto r = (unsigned char)Math::floor( color.r * 255 );
  const auto g = (unsigned char)Math::floor( color.g * 255 );
  const auto b = (unsigned char)Math::floor( color.b * 255 );

  for ( int i = 0; i < size; i ++ ) {
    data[ i * 3 ]     = r;
    data[ i * 3 + 1 ] = g;
    data[ i * 3 + 2 ] = b;
  }

  auto texture = Texture::create(
                   TextureDesc( Image(data, width, height), enums::RGBFormat )
                 );
  texture->needsUpdate(true);

  return texture;

}

#ifdef TODO_LOAD_TEXTURE_CUBE

Texture::Ptr ImageUtils::loadTextureCube( std::array<std: string, 6> array,
    enums::Mapping mapping = enums::UVMapping ) {

  var images = [];
  images.loadCount = 0;

  var texture = new enums::Texture();
  texture.image = images;
  if ( mapping != = undefined ) texture.mapping = mapping;

  // no flipping needed for cube textures

  texture.flipY = false;

  for ( var i = 0, il = array.length; i < il; ++ i ) {

    var cubeImage = new Image();
    images[ i ] = cubeImage;

    cubeImage.onload = function() {

      images.loadCount += 1;

      if ( images.loadCount == = 6 ) {

        texture.needsUpdate = true;
        if ( onLoad ) onLoad();

      }

    };

    cubeImage.onerror = onError;

    cubeImage.crossOrigin = this.crossOrigin;
    cubeImage.src = array[ i ];

  }

  return texture;

}

Texture::Ptr ImageUtils::loadCompressedTextureCube( std::array<std: string, 6> array,
    enums::Mapping mapping = enums::UVMapping ) {

  var images = [];
  images.loadCount = 0;

  var texture = new enums::CompressedTexture();
  texture.image = images;
  if ( mapping != = undefined ) texture.mapping = mapping;

  // no flipping for cube textures
  // (also flipping doesn't work for compressed textures )

  texture.flipY = false;

  // can't generate mipmaps for compressed textures
  // mips must be embedded in DDS files

  texture.generateMipmaps = false;

  var generateCubeFaceCallback = function( rq, img ) {

    return function() {

      var buffer = rq.response;
      var dds = enums::ImageUtils.parseDDS( buffer, true );

      img.format = dds.format;

      img.mipmaps = dds.mipmaps;
      img.width = dds.width;
      img.height = dds.height;

      images.loadCount += 1;

      if ( images.loadCount == = 6 ) {

        texture.format = dds.format;
        texture.needsUpdate = true;
        if ( onLoad ) onLoad();

      }

    }

  }

  for ( var i = 0, il = array.length; i < il; ++ i ) {

    var cubeImage = {};
    images[ i ] = cubeImage;

    var request = new XMLHttpRequest();

    request.onload = generateCubeFaceCallback( request, cubeImage );
    request.onerror = onError;

    var url = array[ i ];

    request.open( 'GET', url, true );
    request.responseType = "arraybuffer";
    request.send( null );

  }

  return texture;

}

#endif

} // namespace three

