#ifndef THREE_GL_H
#define THREE_GL_H

#define GL_GLEXT_PROTOTYPES
#define NOMINMAX
#include <three/deps/GL/glcorearb.h>
#undef GL_GLEXT_PROTOTYPES

namespace three {

struct GLInterface {
public:
  template <typename PtrType> class GLPtr {
  public:
    GLPtr() : _ptr( nullptr ) {}
    GLPtr operator=(PtrType ptr) { _ptr = ptr; return *this; }
    operator PtrType() const { return _ptr; }
  private:
    PtrType _ptr;
  };

  bool validate() const;

#define GL_FUNC_DECL(PFUNC, FUNC) GLPtr<PFUNC> FUNC;
#define GL_FUNC_EXT_DECL(PFUNC, FUNC) GLPtr<PFUNC> FUNC;
#include "three/gl_functions.h"
#undef GL_FUNC_DECL
#undef GL_FUNC_EXT_DECL

};

struct GLInterfaceWrapper : public GLInterface {
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
