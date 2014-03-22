#ifndef THREE_GL_RENDER_TARGET_H
#define THREE_GL_RENDER_TARGET_H

#include <three/common.h>

#include <three/renderers/gl_render_target_buffer.h>

namespace three {

struct TargetDesc {

  explicit TargetDesc( enums::Wrapping wrapS     = enums::ClampToEdgeWrapping,
                       enums::Wrapping wrapT     = enums::ClampToEdgeWrapping,
                       enums::Filter magFilter   = enums::LinearFilter,
                       enums::Filter minFilter   = enums::LinearMipMapLinearFilter,
                       enums::PixelFormat format = enums::RGBAFormat,
                       enums::DataType dataType  = enums::UnsignedByteType,
                       float anisotropy          = 1,
                       bool depthBuffer          = true,
                       bool stencilBuffer        = true )
    : wrapS( wrapS ),
      wrapT( wrapT ),
      magFilter( magFilter ),
      minFilter( minFilter ),
      format( format ),
      dataType( dataType ),
      anisotropy( anisotropy ),
      depthBuffer( depthBuffer ),
      stencilBuffer( stencilBuffer ) { }

  enums::Mapping mapping;
  enums::Wrapping wrapS, wrapT;
  enums::Filter magFilter, minFilter;
  enums::PixelFormat format;
  enums::DataType dataType;
  float anisotropy;
  bool depthBuffer;
  bool stencilBuffer;
};

class GLRenderTarget : public GLRenderTargetBuffer {
public:

  typedef std::shared_ptr<GLRenderTarget> Ptr;

  static Ptr create( int width, int height, const TargetDesc& desc ) {
    return make_shared<GLRenderTarget>( width, height, desc );
  }

  virtual enums::TextureType type() const {
    return enums::GLRenderTarget;
  }

  int width, height;

  enums::Wrapping wrapS, wrapT;

  enums::Filter magFilter, minFilter;

  enums::PixelFormat format;

  enums::DataType dataType;

  float anisotropy;
  int activeCubeFace;

  Vector2 offset;
  Vector2 repeat;

  bool depthBuffer, stencilBuffer;
  bool generateMipmaps;

  std::function<void( void )> onUpdate;

  Ptr clone( ) const {

    auto target = create( width, height, TargetDesc() );

    target->width = width;
    target->height = height;

    target->wrapS = wrapS;
    target->wrapT = wrapT;

    target->magFilter = magFilter;
    target->minFilter = minFilter;

    target->format = format;
    target->dataType = dataType;

    target->anisotropy = anisotropy;
    target->activeCubeFace = activeCubeFace;

    target->offset.copy( offset );
    target->repeat.copy( repeat );

    target->depthBuffer = depthBuffer;
    target->stencilBuffer = stencilBuffer;

    target->generateMipmaps = generateMipmaps;

    return target;

  }

protected:

  GLRenderTarget();

  GLRenderTarget( int width, int height, const TargetDesc& desc )
    : width( width ),
      height( height ),
      wrapS( desc.wrapS ),
      wrapT( desc.wrapT ),
      magFilter( desc.magFilter ),
      minFilter( desc.minFilter ),
      format( desc.format ),
      dataType( desc.dataType ),
      anisotropy( desc.anisotropy ),
      activeCubeFace( -1 ),
      offset( 0, 0 ),
      repeat( 1, 1 ),
      depthBuffer( desc.depthBuffer ),
      stencilBuffer( desc.stencilBuffer ),
      generateMipmaps( true ) { }

};

} // namespace three

#endif // THREE_GL_RENDER_TARGET_H