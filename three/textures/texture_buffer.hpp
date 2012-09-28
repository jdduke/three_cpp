#ifndef THREE_TEXTURE_BUFFER_HPP
#define THREE_TEXTURE_BUFFER_HPP

#include <three/utils/noncopyable.hpp>

namespace three {

struct TextureBuffer : NonCopyable {

  typedef unsigned GLBuffer;

  GLBuffer __glTexture;
  GLBuffer __glTextureCube;

  bool __glInit;

  mutable float __oldAnisotropy;

  TextureBuffer()
    : __glTexture( 0 ),
      __glTextureCube( 0 ),
      __glInit( false ),
      __oldAnisotropy( -1 ) { }

};

}

#endif // THREE_TEXTURE_BUFFER_HPP