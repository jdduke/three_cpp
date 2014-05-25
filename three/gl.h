#ifndef THREE_GL_H
#define THREE_GL_H

#if defined(THREE_GLES)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#define GL_GLEXT_PROTOTYPES
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <three/deps/GL/glcorearb.h>
#undef GL_GLEXT_PROTOTYPES
#endif

#include <three/common.h>

namespace three {

struct THREE_DECL GLInterface {
public:
  template <typename PtrType> class THREE_DECL GLPtr {
  public:
    GLPtr() : _ptr( nullptr ) {}
    GLPtr operator=(PtrType ptr) { _ptr = ptr; return *this; }
    operator PtrType() const { return _ptr; }
  private:
    PtrType _ptr;
  };

  bool validate() const;

#if defined(THREE_GLES)
#define GL_FUNC_DECL_IMPL(GL_FUNC, FUNC) GLPtr<decltype((GL_FUNC))> FUNC;
#define GL_FUNC_DECL(PFUNC, FUNC) GL_FUNC_DECL_IMPL(gl ## FUNC, FUNC)
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) GL_FUNC_DECL_IMPL(gl ## FUNC, FUNC)
#else
#define GL_FUNC_DECL(PFUNC, FUNC) GLPtr<PFUNC> FUNC;
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) GLPtr<PFUNC> FUNC;
#endif
#include "three/gl_functions.h"
#undef GL_FUNC_DECL
#undef GL_FUNC_EXT_DECL

#if defined(THREE_GLES)
  void ClearDepth(GLfloat d) { ClearDepthf(d); }
#endif
};

struct THREE_DECL GLInterfaceWrapper : public GLInterface {
  GLInterfaceWrapper(const GLInterface& gl);

  int Error( const char* file, int line ) const;
  GLuint CreateBuffer() const;
  void DeleteBuffer( GLuint& buffer ) const;
  GLuint CreateTexture() const;
  void DeleteTexture( GLuint& texture ) const;
  GLuint CreateFramebuffer() const;
  void DeleteFramebuffer( GLuint& buffer ) const;
  GLuint CreateRenderbuffer() const;
  void DeleteRenderbuffer( GLuint& buffer ) const;
  GLint GetParameteri( GLenum pname ) const;
  GLfloat GetParameterf( GLenum pname ) const;
  GLint GetTexParameteri( GLenum pname ) const;
  GLfloat GetTexParameterf( GLenum pname ) const;
  GLint GetProgramParameter( GLuint program, GLenum pname ) const;
  GLint GetShaderParameter( GLuint program, GLenum pname ) const;

  template < typename C >
  inline void BindAndBuffer( GLenum target, unsigned buffer, const C& container, GLenum usage ) const {
    BindBuffer( target, buffer );
    BufferData( target, container.size() * sizeof( container[0] ), container.data(), usage );
  }
};

} // namespace three

#endif // THREE_GL_H
