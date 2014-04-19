#ifndef THREE_GL_H
#define THREE_GL_H

#define GL_GLEXT_PROTOTYPES
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

  GLPtr<PFNGLACTIVETEXTUREPROC> ActiveTexture;
  GLPtr<PFNGLATTACHSHADERPROC> AttachShader;
  GLPtr<PFNGLBINDATTRIBLOCATIONPROC> BindAttribLocation;
  GLPtr<PFNGLBINDBUFFERPROC> BindBuffer;
  GLPtr<PFNGLBINDFRAMEBUFFERPROC> BindFramebuffer;
  GLPtr<PFNGLBLENDEQUATIONPROC> BlendEquation;
  GLPtr<PFNGLBINDRENDERBUFFERPROC> BindRenderbuffer;
  GLPtr<PFNGLBINDTEXTUREPROC> BindTexture;
  GLPtr<PFNGLBLENDEQUATIONSEPARATEPROC> BlendEquationSeparate;
  GLPtr<PFNGLBLENDFUNCPROC> BlendFunc;
  GLPtr<PFNGLBLENDFUNCSEPARATEPROC> BlendFuncSeparate;
  GLPtr<PFNGLBUFFERDATAPROC> BufferData;
  GLPtr<PFNGLBUFFERSUBDATAPROC> BufferSubData;
  GLPtr<PFNGLCLEARPROC> Clear;
  GLPtr<PFNGLCLEARCOLORPROC> ClearColor;
  GLPtr<PFNGLCLEARDEPTHPROC> ClearDepth;
  GLPtr<PFNGLCLEARSTENCILPROC> ClearStencil;
  GLPtr<PFNGLCOLORMASKPROC> ColorMask;
  GLPtr<PFNGLCOMPILESHADERPROC> CompileShader;
  GLPtr<PFNGLCREATEPROGRAMPROC> CreateProgram;
  GLPtr<PFNGLCREATESHADERPROC> CreateShader;
  GLPtr<PFNGLCULLFACEPROC> CullFace;
  GLPtr<PFNGLDELETEBUFFERSPROC> DeleteBuffers;
  GLPtr<PFNGLDELETEFRAMEBUFFERSPROC> DeleteFramebuffers;
  GLPtr<PFNGLDELETEPROGRAMPROC> DeleteProgram;
  GLPtr<PFNGLDELETERENDERBUFFERSPROC> DeleteRenderbuffers;
  GLPtr<PFNGLDELETESHADERPROC> DeleteShader;
  GLPtr<PFNGLDELETETEXTURESPROC> DeleteTextures;
  GLPtr<PFNGLDEPTHFUNCPROC> DepthFunc;
  GLPtr<PFNGLDEPTHMASKPROC> DepthMask;
  GLPtr<PFNGLDISABLEPROC> Disable;
  GLPtr<PFNGLDISABLEVERTEXATTRIBARRAYPROC> DisableVertexAttribArray;
  GLPtr<PFNGLDRAWARRAYSPROC> DrawArrays;
  GLPtr<PFNGLDRAWELEMENTSPROC> DrawElements;
  GLPtr<PFNGLENABLEPROC> Enable;
  GLPtr<PFNGLENABLEVERTEXATTRIBARRAYPROC> EnableVertexAttribArray;
  GLPtr<PFNGLFINISHPROC> Finish;
  GLPtr<PFNGLFLUSHPROC> Flush;
  GLPtr<PFNGLFRAMEBUFFERRENDERBUFFERPROC> FramebufferRenderbuffer;
  GLPtr<PFNGLFRAMEBUFFERTEXTURE2DPROC> FramebufferTexture2D;
  GLPtr<PFNGLFRONTFACEPROC> FrontFace;
  GLPtr<PFNGLGENBUFFERSPROC> GenBuffers;
  GLPtr<PFNGLGENERATEMIPMAPPROC> GenerateMipmap;
  GLPtr<PFNGLGENFRAMEBUFFERSPROC> GenFramebuffers;
  GLPtr<PFNGLGENRENDERBUFFERSPROC> GenRenderbuffers;
  GLPtr<PFNGLGENTEXTURESPROC> GenTextures;
  GLPtr<PFNGLGETATTRIBLOCATIONPROC> GetAttribLocation;
  GLPtr<PFNGLGETBUFFERPARAMETERIVPROC> GetBufferParameteriv;
  GLPtr<PFNGLGETERRORPROC> GetError;
  GLPtr<PFNGLGETFLOATVPROC> GetFloatv;
  GLPtr<PFNGLGETINTEGERVPROC> GetIntegerv;
  GLPtr<PFNGLGETPROGRAMINFOLOGPROC> GetProgramInfoLog;
  GLPtr<PFNGLGETPROGRAMIVPROC> GetProgramiv;
  GLPtr<PFNGLGETSHADERINFOLOGPROC> GetShaderInfoLog;
  GLPtr<PFNGLGETSHADERIVPROC> GetShaderiv;
  GLPtr<PFNGLGETSTRINGPROC> GetString;
  GLPtr<PFNGLGETTEXPARAMETERFVPROC> GetTexParameterfv;
  GLPtr<PFNGLGETTEXPARAMETERIVPROC> GetTexParameteriv;
  GLPtr<PFNGLGETUNIFORMLOCATIONPROC> GetUniformLocation;
  GLPtr<PFNGLLINEWIDTHPROC> LineWidth;
  GLPtr<PFNGLLINKPROGRAMPROC> LinkProgram;
  GLPtr<PFNGLPOLYGONOFFSETPROC> PolygonOffset;
  GLPtr<PFNGLRENDERBUFFERSTORAGEPROC> RenderbufferStorage;
  GLPtr<PFNGLSCISSORPROC> Scissor;
  GLPtr<PFNGLSHADERSOURCEPROC> ShaderSource;
  GLPtr<PFNGLSTENCILFUNCPROC> StencilFunc;
  GLPtr<PFNGLSTENCILOPPROC> StencilOp;
  GLPtr<PFNGLSTENCILMASKPROC> StencilMask;
  GLPtr<PFNGLTEXIMAGE2DPROC> TexImage2D;
  GLPtr<PFNGLTEXPARAMETERFPROC> TexParameterf;
  GLPtr<PFNGLTEXPARAMETERFVPROC> TexParameterfv;
  GLPtr<PFNGLTEXPARAMETERIPROC> TexParameteri;
  GLPtr<PFNGLTEXPARAMETERIVPROC> TexParameteriv;
  GLPtr<PFNGLTEXSUBIMAGE2DPROC> TexSubImage2D;
  GLPtr<PFNGLUNIFORM1FPROC> Uniform1f;
  GLPtr<PFNGLUNIFORM1IPROC> Uniform1i;
  GLPtr<PFNGLUNIFORM1FVPROC> Uniform1fv;
  GLPtr<PFNGLUNIFORM1IVPROC> Uniform1iv;
  GLPtr<PFNGLUNIFORM2FPROC> Uniform2f;
  GLPtr<PFNGLUNIFORM2IPROC> Uniform2i;
  GLPtr<PFNGLUNIFORM2FVPROC> Uniform2fv;
  GLPtr<PFNGLUNIFORM2IVPROC> Uniform2iv;
  GLPtr<PFNGLUNIFORM3FPROC> Uniform3f;
  GLPtr<PFNGLUNIFORM3IPROC> Uniform3i;
  GLPtr<PFNGLUNIFORM3FVPROC> Uniform3fv;
  GLPtr<PFNGLUNIFORM3IVPROC> Uniform3iv;
  GLPtr<PFNGLUNIFORM4FPROC> Uniform4f;
  GLPtr<PFNGLUNIFORM4IPROC> Uniform4i;
  GLPtr<PFNGLUNIFORM4FVPROC> Uniform4fv;
  GLPtr<PFNGLUNIFORM4IVPROC> Uniform4iv;
  GLPtr<PFNGLUNIFORMMATRIX2FVPROC> UniformMatrix2fv;
  GLPtr<PFNGLUNIFORMMATRIX3FVPROC> UniformMatrix3fv;
  GLPtr<PFNGLUNIFORMMATRIX4FVPROC> UniformMatrix4fv;
  GLPtr<PFNGLUSEPROGRAMPROC> UseProgram;
  GLPtr<PFNGLVERTEXATTRIBPOINTERPROC> VertexAttribPointer;
  GLPtr<PFNGLVIEWPORTPROC> Viewport;
};

struct GLInterfaceWrapper : public GLInterface {
  GLInterfaceWrapper(const GLInterface& gl);

  int Error( const char* file, int line );
  GLuint CreateBuffer();
  void DeleteBuffer( GLuint& buffer );
  GLuint CreateTexture();
  void DeleteTexture( GLuint& texture );
  GLuint CreateFramebuffer();
  void DeleteFramebuffer( GLuint& buffer );
  GLuint CreateRenderbuffer();
  void DeleteRenderbuffer( GLuint& buffer );
  GLint GetParameteri( GLenum pname );
  GLfloat GetParameterf( GLenum pname );
  GLint GetTexParameteri( GLenum pname );
  GLfloat GetTexParameterf( GLenum pname );
  GLint GetProgramParameter( GLuint program, GLenum pname );
  GLint GetShaderParameter( GLuint program, GLenum pname );

  template < typename C >
  inline void BindAndBuffer( GLenum target, unsigned buffer, const C& container, GLenum usage ) {
    BindBuffer( target, buffer );
    BufferData( target, container.size() * sizeof( container[0] ), container.data(), usage );
  }
};

} // namespace three

#endif // THREE_GL_H
