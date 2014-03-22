#ifndef THREE_GL_RENDERER
#define THREE_GL_RENDERER

#include <three/common.h>

#include <three/gl.h>

#include <three/math/frustum.h>
#include <three/math/vector3.h>
#include <three/math/matrix4.h>
#include <three/core/interfaces.h>

#include <three/scenes/scene.h>
#include <three/materials/program.h>
#include <three/textures/texture.h>

#include <three/renderers/gl_render_target.h>

#ifndef TEXTURE_MAX_ANISOTROPY_EXT
#define TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#endif

namespace three {

typedef std::vector<Scene::GLObject> RenderList;
typedef std::vector<Object3D*>       RenderListDirect;
typedef std::vector<Light*>          Lights;
typedef std::vector<std::string>     Identifiers;

struct ProgramParameters;
struct RendererParameters;

class GLRenderer {
public:

  typedef std::shared_ptr<GLRenderer> Ptr;

  static Ptr create( const RendererParameters& parameters );

public:

  void* context;

  // clearing

  bool autoClear;
  bool autoClearColor;
  bool autoClearDepth;
  bool autoClearStencil;

  // scene graph

  bool sortObjects;

  bool autoUpdateObjects;
  bool autoUpdateScene;

  // physically based shading

  bool gammaInput;
  bool gammaOutput;
  bool physicallyBasedShading;

  // shadow map

  bool shadowMapEnabled;
  bool shadowMapAutoUpdate;
  bool shadowMapSoft;
  bool shadowMapCullFrontFaces;
  bool shadowMapDebug;
  bool shadowMapCascade;

  // morphs

  int maxMorphTargets;
  int maxMorphNormals;

  // flags

  bool autoScaleCubemaps;

  std::vector<std::shared_ptr<IPlugin>> renderPluginsPre;
  std::vector<std::shared_ptr<IPlugin>> renderPluginsPost;

public:

  void* getContext() { return _gl; }
  bool supportsVertexTextures() const { return _supportsVertexTextures; }
  float getMaxAnisotropy() const { return _maxAnisotropy; }
  int width() const { return _width; }
  int height() const { return _height; }

  void setSize( int width, int height );
  void setViewport( int x = 0, int y = 0, int width = -1, int height = -1 );
  void setScissor( int x, int y, int width, int height );
  void enableScissorTest( bool enable );

  // Clearing
  void setClearColorHex( int hex, float alpha );
  void setClearColor( Color color, float alpha );

  Color getClearColor() const { return _clearColor; }
  float getClearAlpha() const { return _clearAlpha; }

  void clear( bool color = true, bool depth = true, bool stencil = true );
  void clearTarget( const GLRenderTarget::Ptr& renderTarget, bool color = true, bool depth = true, bool stencil = true );

  // Plugins
  void addPostPlugin(const IPlugin::Ptr& plugin );
  void addPrePlugin( const IPlugin::Ptr& plugin );

  // Deallocation
  void deallocateObject( Object3D& object );
  void deallocateTexture( Texture& texture );
  void deallocateRenderTarget( GLRenderTarget& renderTarget );
  void deallocateMaterial( Material& material );

  // Rendering
  void render( Scene& scene, Camera& camera, const GLRenderTarget::Ptr& renderTarget = GLRenderTarget::Ptr(), bool forceClear = false );
  void updateShadowMap( const Scene& scene, const Camera& camera );
  void resetStates();

private:

  // Internal functions

  struct InternalLights;
  struct LightCount { int directional, point, spot, hemi; };

  // Buffer allocation
  void createParticleBuffers( Geometry& geometry );
  void createLineBuffers( Geometry& geometry );
  void createRibbonBuffers( Geometry& geometry );
  void createMeshBuffers( GeometryGroup& geometryGroup );

  // Buffer deallocation
  void deleteParticleBuffers( Geometry& geometry );
  void deleteLineBuffers( Geometry& geometry );
  void deleteRibbonBuffers( Geometry& geometry );
  void deleteMeshBuffers( GeometryGroup& geometryGroup );

  // Buffer initialization
  void initCustomAttributes( Geometry& geometry, Object3D& object );
  void initParticleBuffers( Geometry& geometry, Object3D& object );
  void initLineBuffers( Geometry& geometry, Object3D& object );
  void initRibbonBuffers( Geometry& geometry );
  void initMeshBuffers( GeometryGroup& geometryGroup, Mesh& object );

  Material* getBufferMaterial( Object3D& object, GeometryGroup* geometryGroup );
  bool materialNeedsSmoothNormals( const Material* material );
  enums::Shading bufferGuessNormalType( const Material* material );
  enums::Colors bufferGuessVertexColorType( const Material* material );
  bool bufferGuessUVType( const Material* material );

  //

  void initDirectBuffers( Geometry& geometry );

  // Buffer setting
  void setParticleBuffers( Geometry& geometry, int hint, Object3D& object );
  void setLineBuffers( Geometry& geometry, int hint );
  void setRibbonBuffers( Geometry& geometry, int hint );
  void setMeshBuffers( GeometryGroup& geometryGroup, Object3D& object, int hint, bool dispose, Material* material );
  void setDirectBuffers( Geometry& geometry, int hint, bool dispose );

  // Buffer rendering
  void renderBuffer( Camera& camera, Lights& lights, IFog* fog, Material& material, GeometryGroup& geometryGroup, Object3D& object );
  void renderBufferImmediate( Object3D& object, Program& program, Material& material );
  void renderBufferDirect( Camera& camera, Lights& lights, IFog* fog, Material& material, BufferGeometry& geometry, Object3D& object );

  // Sorting
  void setupMorphTargets( Material& material, GeometryGroup& geometryGroup, Object3D& object );

  // Rendering
  void renderPlugins( std::vector<IPlugin::Ptr>& plugins, Scene& scene, Camera& camera );
  void renderObjects( RenderList& renderList, bool reverse, enums::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial = nullptr );
  void renderObjectsImmediate( RenderList& renderList, enums::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial = nullptr );
  void renderImmediateObject( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object );
  void unrollImmediateBufferMaterial( Scene::GLObject& globject );
  void unrollBufferMaterial( Scene::GLObject& globject );

  // Geometry splitting
  void sortFacesByMaterial( Geometry& geometry );

  // Objects refresh
  void initGLObjects( Scene& scene );

  // Objects adding
  void addObject( Object3D& object, Scene& scene );

  // Objects updates
  void updateObject( Object3D& object );

  // Objects updates - custom attributes check
  bool areCustomAttributesDirty( const Material& material );
  void clearCustomAttributes( Material& material );

  // Objects removal
  void removeObject( Object3D& object, Scene& scene );
  void removeInstances( RenderList& objlist, Object3D& object );
  void removeInstancesDirect( RenderListDirect& objlist, Object3D& object );

  // Materials
  void initMaterial( Material& material, Lights& lights, IFog* fog, Object3D& object );
  void setMaterialShaders( Material& material, const Shader& shaders );
  Program& setProgram( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object );

  // Uniforms (refresh uniforms objects)
  void refreshUniformsCommon( Uniforms& uniforms, Material& material );
  void refreshUniformsLine( Uniforms& uniforms, Material& material );
  void refreshUniformsParticle( Uniforms& uniforms, Material& material );
  void refreshUniformsFog( Uniforms& uniforms, IFog& fog );
  void refreshUniformsPhong( Uniforms& uniforms, Material& material );
  void refreshUniformsLambert( Uniforms& uniforms, Material& material );
  void refreshUniformsLights( Uniforms& uniforms, InternalLights& lights );
  void refreshUniformsShadow( Uniforms& uniforms, Lights& lights );

  // Uniforms (load to GPU)
  void loadUniformsMatrices( UniformLocations& uniforms, Object3D& object );
   int getTextureUnit();
  void loadUniformsGeneric( Program& program, UniformsList& uniforms, bool warnOnNotFound );
  void setupMatrices( Object3D& object, Camera& camera );
  void setColorGamma( std::vector<float>& array, size_t offset, const Color& color, float intensitySq );
  void setColorLinear( std::vector<float>& array, size_t offset, const Color& color, float intensity );
  void setupLights( Program& program, Lights& lights );


  // GL state setting
  void setFaceCulling( enums::Side cullFace = enums::NoSide, enums::Dir frontFace = enums::CCW );
  void setMaterialFaces( Material& material );
  void setDepthTest( bool depthTest );
  void setDepthWrite( bool depthWrite );
  void setLineWidth( float width );
  void setPolygonOffset( bool polygonoffset, float factor, float units );
  void setBlending( enums::Blending blending,
                    enums::BlendEquation blendEquation = enums::AddEquation,
                    enums::BlendFactor blendSrc = enums::OneFactor,
                    enums::BlendFactor blendDst = enums::OneFactor );

  // Shaders
  Program::Ptr buildProgram( const std::string& shaderID,
                                        const std::string& fragmentShader,
                                        const std::string& vertexShader,
                                        const Uniforms& uniforms,
                                        const Attributes& attributes,
                                        ProgramParameters& parameters );

  // Shader parameters cache
  static void cacheUniformLocations( Program& program, const Identifiers& identifiers );
  static void cacheAttributeLocations( Program& program, const Identifiers& identifiers );
  static std::string addLineNumbers( const std::string& string );
  Buffer getShader( enums::ShaderType type, const std::string& source );


  // Textures
  template < typename TextureType >
  void setTextureParameters( int textureType, const TextureType& texture, bool isImagePowerOfTwo ) {

    if ( isImagePowerOfTwo ) {

      glTexParameteri( textureType, GL_TEXTURE_WRAP_S, paramThreeToGL( texture.wrapS ) );
      glTexParameteri( textureType, GL_TEXTURE_WRAP_T, paramThreeToGL( texture.wrapT ) );

      glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, paramThreeToGL( texture.magFilter ) );
      glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, paramThreeToGL( texture.minFilter ) );

    } else {

      glTexParameteri( textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
      glTexParameteri( textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

      glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, filterFallback( texture.magFilter ) );
      glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, filterFallback( texture.minFilter ) );

    }

    if ( _glExtensionTextureFilterAnisotropic && texture.dataType != enums::FloatType ) {
      if ( texture.anisotropy > 1 || texture.__oldAnisotropy ) {
        glTexParameterf( textureType, TEXTURE_MAX_ANISOTROPY_EXT, Math::min( texture.anisotropy, _maxAnisotropy ) );
        texture.__oldAnisotropy = texture.anisotropy;
      }
    }

  }

  void setTexture( Texture& texture, int slot );
  static Image& clampToMaxSize( Image& image, int maxSize );
  void setCubeTexture( Texture& texture, int slot );
  void setCubeTextureDynamic( Texture& texture, int slot );


  // Render targets
  void setupFrameBuffer( Buffer framebuffer, GLRenderTarget& renderTarget, GLenum textureTarget );
  void setupRenderBuffer( Buffer renderbuffer, GLRenderTarget& renderTarget );
  void setRenderTarget( const GLRenderTarget::Ptr& renderTarget );
  void updateRenderTargetMipmap( GLRenderTarget& renderTarget );


  // Fallback filters for non-power-of-2 textures
  static int filterFallback( int f );

  // Map enums::cpp constants to WebGL constants
  static int paramThreeToGL( int p );

  // Allocations
  LightCount allocateLights( Lights& lights );
  int allocateShadows( Lights& lights );
  int allocateBones( Object3D& object );

protected:

  GLRenderer( const RendererParameters& parameters );

  void initialize();
  void initGL();
  void setDefaultGLState();

private:

  int _width, _height;
  bool _vsync;
  enums::PrecisionType _precision;
  bool _alpha;
  bool _premultipliedAlpha;
  bool _antialias;
  bool _stencil;
  bool _preserveDrawingBuffer;
  Color _clearColor;
  float _clearAlpha;
  int _maxLights;

  // info

  struct Info {

    struct Memory {
      Memory() : programs( 0 ), geometries( 0 ), textures( 0 ) { }
      int programs;
      int geometries;
      int textures;
    } memory;

    struct Render {
      Render() : calls( 0 ), vertices( 0 ), faces( 0 ), points( 0 ) { }
      int calls;
      int vertices;
      int faces;
      int points;
    } render;

  } _info;

  // internal properties

  struct ProgramInfo {
    ProgramInfo( const Program::Ptr& program, std::string code, int usedTimes )
      : program( program ), code( code ), usedTimes( usedTimes ) { }
    ProgramInfo()
      : program( 0 ), usedTimes( 0 ) { }

    Program::Ptr program;
    std::string code;
    int usedTimes;
  };
  std::vector<ProgramInfo> _programs;
  int _programs_counter;

  // internal state cache

  Program* _currentProgram;
  Buffer _currentFramebuffer;
  int _currentMaterialId;
  int _currentGeometryGroupHash;
  Camera* _currentCamera;
  int _geometryGroupCounter;
  int _usedTextureUnits;

  // GL state cache

  int _oldDoubleSided;
  int _oldFlipSided;

  int _oldBlending;

  int _oldBlendEquation;
  int _oldBlendSrc;
  int _oldBlendDst;

  int _oldDepthTest;
  int _oldDepthWrite;

  int _oldPolygonOffset;
  float _oldPolygonOffsetFactor;
  float _oldPolygonOffsetUnits;

  float _oldLineWidth;

  int _viewportX;
  int _viewportY;
  int _viewportWidth;
  int _viewportHeight;
  int _currentWidth;
  int _currentHeight;

  Frustum _frustum;

  // camera matrices cache
  Matrix4 _projScreenMatrix;
  Matrix4 _projScreenMatrixPS;

  Vector3 _vector3;

  // light arrays cache
  Vector3 _direction;
  bool _lightsNeedUpdate;
  struct InternalLights {

    std::vector<float> ambient;

    struct Directional {
      int length;
      std::vector<float> colors;
      std::vector<float> positions;
    } directional;

    struct Point {
      int length;
      std::vector<float> colors;
      std::vector<float> positions;
      std::vector<float> distances;
    } point;

    struct Spot {
      int length;
      std::vector<float> colors;
      std::vector<float> positions;
      std::vector<float> distances;
      std::vector<float> directions;
      std::vector<float> angles;
      std::vector<float> exponents;
    } spot;

    struct Hemi {
      int length;
      std::vector<float> skyColors, groundColors;
      std::vector<float> positions;
    } hemi;

  } _lights;

  void* _gl;

  bool _glExtensionTextureFloat;
  bool _glExtensionStandardDerivatives;
  bool _glExtensionTextureFilterAnisotropic;

  // GPU capabilities

  int _maxTextures;
  int _maxVertexTextures;
  int _maxTextureSize;
  int _maxCubemapSize;

  float _maxAnisotropy;

  bool _supportsVertexTextures;
  bool _supportsBoneTextures;

  /*
  // default plugins (order is important)

  shadowMapPlugin = new enums::ShadowMapPlugin();
  addPrePlugin( shadowMapPlugin );

  addPostPlugin( new enums::SpritePlugin() );
  addPostPlugin( new enums::LensFlarePlugin() );
  */

private:



}; // class GLRenderer

} // namespace three

#endif // THREE_GL_RENDERER
