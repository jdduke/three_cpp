#ifndef THREE_GL_RENDER_TARGET_BUFFER_HPP
#define THREE_GL_RENDER_TARGET_BUFFER_HPP

#include <three/extras/noncopyable.hpp>

namespace three {

struct GLRenderTargetBuffer : NonCopyable {

    typedef unsigned GLBuffer;

    GLBuffer __glTexture;
    std::vector<GLBuffer> __glFramebuffer;
    std::vector<GLBuffer> __glRenderbuffer;

    GLRenderTargetBuffer() 
        : __glTexture ( 0 ) { }

};

} // namespace three

#endif // THREE_GL_RENDER_TARGET_BUFFER_HPP