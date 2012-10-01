#ifndef THREE_GL_RENDERER
#define THREE_GL_RENDERER

#include <three/common.hpp>

#include <three/gl.hpp>

#include <three/core/frustum.hpp>
#include <three/core/vector3.hpp>
#include <three/core/matrix4.hpp>
#include <three/core/interfaces.hpp>

#include <three/scenes/scene.hpp>
#include <three/materials/program.hpp>
#include <three/textures/texture.hpp>

#include <three/renderers/gl_render_target.hpp>

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

  THREE_DECL static Ptr create( const RendererParameters& parameters );

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

  THREE_DECL void setSize( int width, int height );
  THREE_DECL void setViewport( int x = 0, int y = 0, int width = -1, int height = -1 );
  THREE_DECL void setScissor( int x, int y, int width, int height );
  THREE_DECL void enableScissorTest( bool enable );

  // Clearing
  THREE_DECL void setClearColorHex( int hex, float alpha );
  THREE_DECL void setClearColor( Color color, float alpha );

  Color getClearColor() const { return _clearColor; }
  float getClearAlpha() const { return _clearAlpha; }

  THREE_DECL void clear( bool color = true, bool depth = true, bool stencil = true );
  THREE_DECL void clearTarget( GLRenderTarget::Ptr renderTarget, bool color = true, bool depth = true, bool stencil = true );

  // Plugins
  THREE_DECL void addPostPlugin( IPlugin::Ptr plugin );
  THREE_DECL void addPrePlugin( IPlugin::Ptr plugin );

  // Deallocation
  THREE_DECL void deallocateObject( Object3D& object );
  THREE_DECL void deallocateTexture( Texture& texture );
  THREE_DECL void deallocateRenderTarget( GLRenderTarget& renderTarget );
  THREE_DECL void deallocateMaterial( Material& material );

  // Rendering
  THREE_DECL void render( Scene& scene, Camera& camera, GLRenderTarget::Ptr renderTarget = GLRenderTarget::Ptr(), bool forceClear = false );
  THREE_DECL void updateShadowMap( const Scene& scene, const Camera& camera );
  THREE_DECL void resetStates();

private:

  // Internal functions

  struct InternalLights;
  struct LightCount { int directional, point, spot, hemi; };

  // Buffer allocation
  THREE_DECL void createParticleBuffers( Geometry& geometry );
  THREE_DECL void createLineBuffers( Geometry& geometry );
  THREE_DECL void createRibbonBuffers( Geometry& geometry );
  THREE_DECL void createMeshBuffers( GeometryGroup& geometryGroup );

  // Buffer deallocation
  THREE_DECL void deleteParticleBuffers( Geometry& geometry );
  THREE_DECL void deleteLineBuffers( Geometry& geometry );
  THREE_DECL void deleteRibbonBuffers( Geometry& geometry );
  THREE_DECL void deleteMeshBuffers( GeometryGroup& geometryGroup );

  // Buffer initialization
  THREE_DECL void initCustomAttributes( Geometry& geometry, Object3D& object );
  THREE_DECL void initParticleBuffers( Geometry& geometry, Object3D& object );
  THREE_DECL void initLineBuffers( Geometry& geometry, Object3D& object );
  THREE_DECL void initRibbonBuffers( Geometry& geometry );
  THREE_DECL void initMeshBuffers( GeometryGroup& geometryGroup, Mesh& object );

  THREE_DECL Material* getBufferMaterial( Object3D& object, GeometryGroup* geometryGroup );
  THREE_DECL bool materialNeedsSmoothNormals( const Material* material );
  THREE_DECL THREE::Shading bufferGuessNormalType( const Material* material );
  THREE_DECL THREE::Colors bufferGuessVertexColorType( const Material* material );
  THREE_DECL bool bufferGuessUVType( const Material* material );

  //

  THREE_DECL void initDirectBuffers( Geometry& geometry );

  // Buffer setting
  THREE_DECL void setParticleBuffers( Geometry& geometry, int hint, Object3D& object );
  THREE_DECL void setLineBuffers( Geometry& geometry, int hint );
  THREE_DECL void setRibbonBuffers( Geometry& geometry, int hint );
  THREE_DECL void setMeshBuffers( GeometryGroup& geometryGroup, Object3D& object, int hint, bool dispose, Material* material );
  THREE_DECL void setDirectBuffers( Geometry& geometry, int hint, bool dispose );

  // Buffer rendering
  THREE_DECL void renderBuffer( Camera& camera, Lights& lights, IFog* fog, Material& material, GeometryGroup& geometryGroup, Object3D& object );
  THREE_DECL void renderBufferImmediate( Object3D& object, Program& program, Material& material );
  THREE_DECL void renderBufferDirect( Camera& camera, Lights& lights, IFog* fog, Material& material, Geometry& geometry, Object3D& object );

  // Sorting
  THREE_DECL void setupMorphTargets( Material& material, GeometryGroup& geometryGroup, Object3D& object );

  // Rendering
  THREE_DECL void renderPlugins( std::vector<IPlugin::Ptr>& plugins, Scene& scene, Camera& camera );
  THREE_DECL void renderObjects( RenderList& renderList, bool reverse, THREE::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial = nullptr );
  THREE_DECL void renderObjectsImmediate( RenderList& renderList, THREE::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial = nullptr );
  THREE_DECL void renderImmediateObject( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object );
  THREE_DECL void unrollImmediateBufferMaterial( Scene::GLObject& globject );
  THREE_DECL void unrollBufferMaterial( Scene::GLObject& globject );

  // Geometry splitting
  THREE_DECL void sortFacesByMaterial( Geometry& geometry );

  // Objects refresh
  THREE_DECL void initGLObjects( Scene& scene );

  // Objects adding
  THREE_DECL void addObject( Object3D& object, Scene& scene );

  // Objects updates
  THREE_DECL void updateObject( Object3D& object );

  // Objects updates - custom attributes check
  bool areCustomAttributesDirty( const Material& material );
  THREE_DECL void clearCustomAttributes( Material& material );

  // Objects removal
  THREE_DECL void removeObject( Object3D& object, Scene& scene );
  THREE_DECL void removeInstances( RenderList& objlist, Object3D& object );
  THREE_DECL void removeInstancesDirect( RenderListDirect& objlist, Object3D& object );
  
  // Materials
  THREE_DECL void initMaterial( Material& material, Lights& lights, IFog* fog, Object3D& object );
  THREE_DECL void setMaterialShaders( Material& material, const Shader& shaders );
  Program& setProgram( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object );
  
  // Uniforms (refresh uniforms objects)
  THREE_DECL void refreshUniformsCommon( Uniforms& uniforms, Material& material );
  THREE_DECL void refreshUniformsLine( Uniforms& uniforms, Material& material );
  THREE_DECL void refreshUniformsParticle( Uniforms& uniforms, Material& material );
  THREE_DECL void refreshUniformsFog( Uniforms& uniforms, IFog& fog );
  THREE_DECL void refreshUniformsPhong( Uniforms& uniforms, Material& material );
  THREE_DECL void refreshUniformsLambert( Uniforms& uniforms, Material& material );
  THREE_DECL void refreshUniformsLights( Uniforms& uniforms, InternalLights& lights );
  THREE_DECL void refreshUniformsShadow( Uniforms& uniforms, Lights& lights );
  
  // Uniforms (load to GPU)
  THREE_DECL void loadUniformsMatrices( UniformsIndices& uniforms, Object3D& object );
  THREE_DECL void loadUniformsGeneric( Program& program, UniformsList& uniforms, bool warnOnNotFound );
  THREE_DECL void setupMatrices( Object3D& object, Camera& camera );
  THREE_DECL void setColorGamma( std::vector<float>& array, size_t offset, const Color& color, float intensitySq );
  THREE_DECL void setColorLinear( std::vector<float>& array, size_t offset, const Color& color, float intensity );
  THREE_DECL void setupLights( Program& program, Lights& lights );


  // GL state setting
  THREE_DECL void setFaceCulling( THREE::Side cullFace = THREE::NoSide, THREE::Dir frontFace = THREE::CCW );
  THREE_DECL void setMaterialFaces( Material& material );
  THREE_DECL void setDepthTest( bool depthTest );
  THREE_DECL void setDepthWrite( bool depthWrite );
  THREE_DECL void setLineWidth( float width );
  THREE_DECL void setPolygonOffset( bool polygonoffset, float factor, float units );
  THREE_DECL void setBlending( THREE::Blending blending,
                    THREE::BlendEquation blendEquation = THREE::AddEquation,
                    THREE::BlendFactor blendSrc = THREE::OneFactor,
                    THREE::BlendFactor blendDst = THREE::OneFactor );

  // Shaders
  THREE_DECL Program::Ptr buildProgram( const std::string& shaderID,
                                        const std::string& fragmentShader,
                                        const std::string& vertexShader,
                                        const Uniforms& uniforms,
                                        const Attributes& attributes,
                                        ProgramParameters& parameters );

  // Shader parameters cache
  THREE_DECL static void cacheUniformLocations( Program& program, const Identifiers& identifiers );
  THREE_DECL static void cacheAttributeLocations( Program& program, const Identifiers& identifiers );
  THREE_DECL static std::string addLineNumbers( const std::string& string );
  THREE_DECL Buffer getShader( THREE::ShaderType type, const std::string& source );


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

    if ( _glExtensionTextureFilterAnisotropic && texture.dataType != THREE::FloatType ) {
      if ( texture.anisotropy > 1 || texture.__oldAnisotropy ) {
        glTexParameterf( textureType, TEXTURE_MAX_ANISOTROPY_EXT, Math::min( texture.anisotropy, _maxAnisotropy ) );
        texture.__oldAnisotropy = texture.anisotropy;
      }
    }

  }

  THREE_DECL void setTexture( Texture& texture, int slot );
  THREE_DECL static Image& clampToMaxSize( Image& image, int maxSize );
  THREE_DECL void setCubeTexture( Texture& texture, int slot );
  THREE_DECL void setCubeTextureDynamic( Texture& texture, int slot );


  // Render targets
  THREE_DECL void setupFrameBuffer( Buffer framebuffer, GLRenderTarget& renderTarget, GLenum textureTarget );
  THREE_DECL void setupRenderBuffer( Buffer renderbuffer, GLRenderTarget& renderTarget );
  THREE_DECL void setRenderTarget( GLRenderTarget::Ptr renderTarget );
  THREE_DECL void updateRenderTargetMipmap( GLRenderTarget& renderTarget );


  // Fallback filters for non-power-of-2 textures
  THREE_DECL static int filterFallback( int f );

  // Map THREE::cpp constants to WebGL constants
  THREE_DECL static int paramThreeToGL( int p );

  // Allocations
  THREE_DECL LightCount allocateLights( Lights& lights );
  THREE_DECL int allocateShadows( Lights& lights );
  THREE_DECL int allocateBones( Object3D& object );

protected:

  THREE_DECL GLRenderer( const RendererParameters& parameters );

  THREE_DECL void initialize();
  THREE_DECL void initGL();
  THREE_DECL void setDefaultGLState();

private:

  int _width, _height;
  bool _vsync;
  THREE::PrecisionType _precision;
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
    ProgramInfo( Program::Ptr program, std::string code, int usedTimes )
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

  int _maxVertexTextures;
  int _maxTextureSize;
  int _maxCubemapSize;

  float _maxAnisotropy;

  bool _supportsVertexTextures;
  bool _supportsBoneTextures;

  /*
  // default plugins (order is important)

  shadowMapPlugin = new THREE::ShadowMapPlugin();
  addPrePlugin( shadowMapPlugin );

  addPostPlugin( new THREE::SpritePlugin() );
  addPostPlugin( new THREE::LensFlarePlugin() );
  */

private:



}; // class GLRenderer

} // namespace three

#if defined(THREE_HEADER_ONLY)
# include <three/renderers/impl/gl_renderer.ipp>
#endif // defined(THREE_HEADER_ONLY)

#endif // THREE_GL_RENDERER