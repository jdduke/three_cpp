#ifndef THREE_TEXTURE_BUFFER_HPP
#define THREE_TEXTURE_BUFFER_HPP

#include <three/utils/noncopyable.hpp>

namespace three {

// TODO: Make this a member of Texture, rather than a parent...
struct TextureBuffer : NonCopyable {

  typedef unsigned GLBuffer;

  mutable bool __glInit;
  mutable GLBuffer __glTexture;
  mutable GLBuffer __glTextureCube;
  mutable float __oldAnisotropy;

  TextureBuffer()
    : __glInit( false ),
      __glTexture( 0 ),
      __glTextureCube( 0 ),
      __oldAnisotropy( -1 ) { }

};

}

#endif // THREE_TEXTURE_BUFFER_HPP