#ifndef THREE_GL_RENDER_TARGET_BUFFER_H
#define THREE_GL_RENDER_TARGET_BUFFER_H

#include <three/utils/noncopyable.h>

namespace three {

struct GLRenderTargetBuffer : NonCopyable {

  typedef unsigned GLBuffer;

  GLBuffer __glTexture;
  std::vector<GLBuffer> __glFramebuffer;
  std::vector<GLBuffer> __glRenderbuffer;

  mutable float __oldAnisotropy;

  GLRenderTargetBuffer()
    : __glTexture( 0 ),
      __oldAnisotropy( -1 ) { }

};

} // namespace three

#endif // THREE_GL_RENDER_TARGET_BUFFER_H