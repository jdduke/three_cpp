#include <three/renderers/gl_renderer.h>

#include <three/common.h>
#include <three/console.h>
#include <three/gl.h>

#include <three/cameras/camera.h>

#include <three/math/vector2.h>
#include <three/math/vector3.h>
#include <three/math/vector4.h>
#include <three/math/color.h>

#include <three/core/interfaces.h>
#include <three/core/buffer_geometry.h>
#include <three/core/event_dispatcher.h>
#include <three/core/geometry.h>
#include <three/core/geometry_group.h>
#include <three/core/face.h>

#include <three/lights/spot_light.h>
#include <three/lights/hemisphere_light.h>

#include <three/materials/material.h>
#include <three/materials/program.h>
#include <three/materials/mesh_face_material.h>

#include <three/objects/line.h>
#include <three/objects/mesh.h>
#include <three/objects/particle_system.h>

#include <three/renderers/gl_render_target.h>
#include <three/renderers/gl/gl_shader.h>
#include <three/renderers/shaders/shader_lib.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/renderables/renderable_object.h>

#include <three/scenes/scene.h>
#include <three/scenes/fog.h>
#include <three/scenes/fog_exp2.h>

#include <three/textures/texture.h>

#include <three/utils/hash.h>
#include <three/utils/conversion.h>
#include <three/utils/template.h>

#ifndef NDEBUG
#define GL_CALL(a) (a); _gl.Error(__FILE__, __LINE__)
#else
#define GL_CALL(a) (a)
#endif

namespace three {

struct NumericalSort {
    template < typename T, typename U >
    inline bool operator()( const std::pair<T, U>& a, const std::pair<T, U>& b ) {
        return a.second > b.second;
    }
};

struct ProgramParameters {
  bool map, envMap, lightMap, bumpMap, normalMap, specularMap;
  THREE::Colors vertexColors;
  IFog* fog;
  bool useFog;
  bool fogExp;
  bool sizeAttenuation;
  bool skinning;
  int maxBones;
  bool useVertexTexture;
  //int boneTextureWidth;
  //int boneTextureHeight;
  bool morphTargets;
  bool morphNormals;
  int maxMorphTargets;
  int maxMorphNormals;
  int maxDirLights;
  int maxPointLights;
  int maxSpotLights;
  int maxHemiLights;
  int maxShadows;
  bool shadowMapEnabled;
  THREE::ShadowTypes shadowMapType;
  bool shadowMapDebug;
  bool shadowMapCascade;

  float alphaTest;
  bool metal;
  bool perPixel;
  bool wrapAround;
  bool doubleSided;
  bool flipSided;
};

GLRenderer::Ptr GLRenderer::create( const RendererParameters& parameters,
                                    const GLInterface& gl ) {
  auto renderer = make_shared<GLRenderer>( parameters, gl );
  renderer->initialize();
  return renderer;
}

GLRenderer::GLRenderer( const RendererParameters& parameters, const GLInterface& gl )
  : devicePixelRatio( 1 ),
    autoClear( true ),
    autoClearColor( true ),
    autoClearDepth( true ),
    autoClearStencil( true ),
    sortObjects( true ),
    autoUpdateObjects( true ),
    autoUpdateScene( true ),
    gammaInput( false ),
    gammaOutput( false ),
    physicallyBasedShading( false ),
    shadowMapEnabled( false ),
    shadowMapAutoUpdate( true ),
    shadowMapType( THREE::PCFShadowMap ),
    shadowMapCullFrontFace( THREE::CullFaceFront ),
    shadowMapDebug( false ),
    shadowMapCascade( false ),
    maxMorphTargets( 8 ),
    maxMorphNormals( 4 ),
    autoScaleCubemaps( true ),
    _width( parameters.width ),
    _height( parameters.height ),
    _precision( parameters.precision ),
    _alpha( parameters.alpha ),
    _premultipliedAlpha( parameters.premultipliedAlpha ),
    _antialias( parameters.antialias ),
    _stencil( parameters.stencil ),
    _preserveDrawingBuffer( parameters.preserveDrawingBuffer ),
    _clearColor( parameters.clearColor ),
    _clearAlpha( parameters.clearAlpha ),
    _maxLights( parameters.maxLights ),
    _programs_counter( 0 ),
    _currentProgram( 0 ),
    _currentFramebuffer( 0 ),
    _currentMaterialId( -1 ),
    _currentGeometryGroupHash( -1 ),
    _currentCamera( nullptr ),
    _geometryGroupCounter( 0 ),
    _usedTextureUnits( 0 ),
    _oldDoubleSided( -1 ),
    _oldFlipSided( -1 ),
    _oldBlending( -1 ),
    _oldBlendEquation( -1 ),
    _oldBlendSrc( -1 ),
    _oldBlendDst( -1 ),
    _oldDepthTest( -1 ),
    _oldDepthWrite( -1 ),
    _oldPolygonOffset( 0 ),
    _oldPolygonOffsetFactor( 0 ),
    _oldPolygonOffsetUnits( 0 ),
    _oldLineWidth( 0 ),
    _viewportX( 0 ),
    _viewportY( 0 ),
    _viewportWidth( 0 ),
    _viewportHeight( 0 ),
    _currentWidth( 0 ),
    _currentHeight( 0 ),
    _lightsNeedUpdate( true ),
    _gl( gl ) {
  console().log() << "GLRenderer created";
}

/*
// default plugins (order is important)

shadowMapPlugin = new THREE::ShadowMapPlugin();
addPrePlugin( shadowMapPlugin );

addPostPlugin( new THREE::SpritePlugin() );
addPostPlugin( new THREE::LensFlarePlugin() );
*/

void GLRenderer::initialize() {

  console().log() << "GLRenderer initializing";

  initGL();

  setDefaultGLState();

  // GPU capabilities

  _maxTextures       = _gl.GetParameteri( GL_MAX_TEXTURE_IMAGE_UNITS );
  _maxVertexTextures = _gl.GetParameteri( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ),
  _maxTextureSize    = _gl.GetParameteri( GL_MAX_TEXTURE_SIZE ),
  _maxCubemapSize    = _gl.GetParameteri( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
  _maxAnisotropy = _glExtensionTextureFilterAnisotropic ? _gl.GetTexParameterf( TEXTURE_MAX_ANISOTROPY_EXT ) : 0.f;
  _supportsVertexTextures = ( _maxVertexTextures > 0 );
  _supportsBoneTextures = _supportsVertexTextures && _glExtensionTextureFloat;

  console().log() << "THREE::GLRenderer initialized";

}

void GLRenderer::initGL() {

  THREE_ASSERT( _gl.validate() );

  THREE_REVIEW("Have client indicate whether these are supported.")
  /*
  _glExtensionTextureFloat = glewIsExtensionSupported( "ARB_texture_float" ) != 0 ? true : false;
  _glExtensionStandardDerivatives = glewIsExtensionSupported( "OES_standard_derivatives" ) != 0 ? true : false;
  _glExtensionTextureFilterAnisotropic = glewIsExtensionSupported( "EXT_texture_filter_anisotropic" ) != 0 ? true : false;

  if ( ! _glExtensionTextureFloat ) {
    console().log( "THREE::GLRenderer: Float textures not supported." );
  }

  if ( ! _glExtensionStandardDerivatives ) {
    console().log( "THREE::GLRenderer: Standard derivatives not supported." );
  }

  if ( ! _glExtensionTextureFilterAnisotropic ) {
    console().log( "THREE::GLRenderer: Anisotropic texture filtering not supported." );
  }

  */
  _glExtensionTextureFloat = false;
  _glExtensionTextureFloatLinear = false;
  _glExtensionStandardDerivatives = false;
  _glExtensionTextureFilterAnisotropic = false;
  _glExtensionCompressedTextureS3TC = false;
}

void GLRenderer::setDefaultGLState() {

  _gl.ClearColor( 0, 0, 0, 1 );
  _gl.ClearDepth( 1 );
  _gl.ClearStencil( 0 );

  _gl.Enable( GL_DEPTH_TEST );
  _gl.DepthFunc( GL_LEQUAL );

  _gl.FrontFace( GL_CCW );
  _gl.CullFace( GL_BACK );
  _gl.Enable( GL_CULL_FACE );

  _gl.Enable( GL_BLEND );
  _gl.BlendEquation( GL_FUNC_ADD );
  _gl.BlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  THREE_REVIEW("EA: Viewport values are 0/-1 here. Why?")
  //_gl.Viewport( _viewportX, _viewportY, _viewportWidth, _viewportHeight );

  _gl.ClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );

}

void GLRenderer::setSize( int width, int height ) {

  _width = (int)(width * devicePixelRatio);
  _height = (int)(height * devicePixelRatio);

  setViewport( 0, 0, _width, _height );
}

void GLRenderer::setViewport( int x /*= 0*/, int y /*= 0*/, int width /*= -1*/, int height /*= -1*/ ) {
  _viewportX = x;
  _viewportY = y;

  _viewportWidth  = width  != -1 ? width  : _width;
  _viewportHeight = height != -1 ? height : _height;

  _gl.Viewport( _viewportX, _viewportY, _viewportWidth, _viewportHeight );
}

void GLRenderer::setScissor( int x, int y, int width, int height ) {
  _gl.Scissor( x, y, width, height );
}

void GLRenderer::enableScissorTest( bool enable ) {
  enable ? _gl.Enable( GL_SCISSOR_TEST ) : _gl.Disable( GL_SCISSOR_TEST );
}

// Clearing

void GLRenderer::setClearColor( Color color, float alpha ) {
  _clearColor.copy( color );
  _clearAlpha = alpha;

  _gl.ClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );
}

void GLRenderer::clear( bool color /*= true*/, bool depth /*= true*/, bool stencil /*= true*/ ) {
  int bits = 0;

  if ( color )   bits |= GL_COLOR_BUFFER_BIT;
  if ( depth )   bits |= GL_DEPTH_BUFFER_BIT;
  if ( stencil ) bits |= GL_STENCIL_BUFFER_BIT;

  _gl.Clear( bits );
}

void GLRenderer::clearTarget( const GLRenderTarget::Ptr& renderTarget, bool color /*= true*/, bool depth /*= true*/, bool stencil /*= true*/ ) {

  setRenderTarget( renderTarget );
  clear( color, depth, stencil );

}

// Plugins

void GLRenderer::addPostPlugin( const IPlugin::Ptr& plugin ) {

  plugin->init( *this );
  renderPluginsPost.push_back( plugin );

}

void GLRenderer::addPrePlugin( const IPlugin::Ptr& plugin ) {

  plugin->init( *this );
  renderPluginsPre.push_back( plugin );

}

// Deallocation

// void GLRenderer::deallocateObject( Object3D& object ) {

//   if ( ! object.glData.__glInit ) return;

//   object.glData.clear();

//   if ( !object.geometry ) {
//     console().warn( "Object3D contains no geometry" );
//     return;
//   }

//   auto& geometry = *object.geometry;

//   if ( object.type() == THREE::Mesh ) {
//     for ( auto& geometryGroup : geometry.geometryGroups ) {
//       deleteMeshBuffers( *geometryGroup.second );
//     }
//   } else if ( object.type() == THREE::Ribbon ) {
//     deleteRibbonBuffers( geometry );
//   } else if ( object.type() == THREE::Line ) {
//     deleteLineBuffers( geometry );
//   } else if ( object.type() == THREE::ParticleSystem ) {
//     deleteParticleBuffers( geometry );
//   }

// }

// Rendering

void GLRenderer::updateShadowMap( const Scene& scene, const Camera& camera ) {

  _currentProgram = 0;
  _oldBlending = -1;
  _oldDepthTest = -1;
  _oldDepthWrite = -1;
  _currentGeometryGroupHash = -1;
  _currentMaterialId = -1;
  _lightsNeedUpdate = true;
  _oldDoubleSided = -1;
  _oldFlipSided = -1;

  // TODO
  //shadowMapPlugin.update( scene, camera );

}

// Internal functions

// Buffer allocation

void GLRenderer::createParticleBuffers( Geometry& geometry ) {

  geometry.__glVertexBuffer = _gl.CreateBuffer();
  geometry.__glColorBuffer = _gl.CreateBuffer();

  _info.memory.geometries ++;

}

void GLRenderer::createLineBuffers( Geometry& geometry ) {

  geometry.__glVertexBuffer = _gl.CreateBuffer();
  geometry.__glColorBuffer  = _gl.CreateBuffer();
  geometry.__glLineDistanceBuffer  = _gl.CreateBuffer();

  _info.memory.geometries ++;

}

// void GLRenderer::createRibbonBuffers( Geometry& geometry ) {

//   geometry.__glVertexBuffer = _gl.CreateBuffer();
//   geometry.__glColorBuffer  = _gl.CreateBuffer();

//   _info.memory.geometries ++;

// }

void GLRenderer::createMeshBuffers( GeometryGroup& geometryGroup ) {

  geometryGroup.__glVertexBuffer  = _gl.CreateBuffer();
  geometryGroup.__glNormalBuffer  = _gl.CreateBuffer();
  geometryGroup.__glTangentBuffer = _gl.CreateBuffer();
  geometryGroup.__glColorBuffer   = _gl.CreateBuffer();
  geometryGroup.__glUVBuffer      = _gl.CreateBuffer();
  geometryGroup.__glUV2Buffer     = _gl.CreateBuffer();

  geometryGroup.__glSkinIndicesBuffer = _gl.CreateBuffer();
  geometryGroup.__glSkinWeightsBuffer = _gl.CreateBuffer();

  geometryGroup.__glFaceBuffer = _gl.CreateBuffer();
  geometryGroup.__glLineBuffer = _gl.CreateBuffer();

  if ( geometryGroup.numMorphTargets > 0 ) {

    geometryGroup.__glMorphTargetsBuffers.clear();

    for ( int m = 0, ml = geometryGroup.numMorphTargets; m < ml; m ++ ) {
      geometryGroup.__glMorphTargetsBuffers.push_back( _gl.CreateBuffer() );
    }

  }

  if ( geometryGroup.numMorphNormals > 0 ) {

    geometryGroup.__glMorphNormalsBuffers.clear();

    for ( int m = 0, ml = geometryGroup.numMorphNormals; m < ml; m ++ ) {
      geometryGroup.__glMorphNormalsBuffers.push_back( _gl.CreateBuffer() );
    }

  }

  _info.memory.geometries ++;

}

// Events

void GLRenderer::onGeometryDispose( const Event& event ) {
  console().warn("TODO onGeometryDispose");
}

void GLRenderer::onTextureDispose( const Event& event ) {
  console().warn("TODO onTextureDispose");
}

void GLRenderer::onRenderTargetDispose( const Event& event ) {
  console().warn("TODO onRenderTargetDispose");
}

void GLRenderer::onMaterialDispose( const Event& event ) {
  console().warn("TODO onMaterialDispose");
}

// Buffer deallocation

void GLRenderer::deleteBuffers( GeometryBuffer& geometry ) {

  _gl.DeleteBuffer( geometry.__glVertexBuffer );
  _gl.DeleteBuffer( geometry.__glNormalBuffer );
  _gl.DeleteBuffer( geometry.__glTangentBuffer );
  _gl.DeleteBuffer( geometry.__glColorBuffer );
  _gl.DeleteBuffer( geometry.__glUVBuffer );
  _gl.DeleteBuffer( geometry.__glUV2Buffer );

  _gl.DeleteBuffer( geometry.__glSkinIndicesBuffer );
  _gl.DeleteBuffer( geometry.__glSkinWeightsBuffer );

  _gl.DeleteBuffer( geometry.__glFaceBuffer );
  _gl.DeleteBuffer( geometry.__glLineBuffer );

  _gl.DeleteBuffer( geometry.__glLineDistanceBuffer );

  // custom attributes

  for ( auto& attribute : geometry.__glCustomAttributesList ) {
    _gl.DeleteBuffer( attribute->buffer );
  }

  _info.memory.geometries --;

}

void GLRenderer::deallocateGeometry( Geometry& geometry ) {

  geometry.__glInit = false;

  if ( geometry.type() == THREE::BufferGeometry ) {

    auto& attributes = geometry.attributes;

     for ( auto& namedAttribute : attributes ) {

      //auto& a = namedAttribute.first;
      auto& attribute = namedAttribute.second;

      _gl.DeleteBuffer( attribute.buffer );

    }

    _info.memory.geometries --;

  } else {

    if ( geometry.geometryGroups.size() ) {

      for ( auto& g : geometry.geometryGroups ) {

        auto& geometryGroup = g.second;

        if ( geometryGroup->morphTargets.size() ) {

          for ( size_t m = 0, ml = geometryGroup->morphTargets.size(); m < ml; m ++ ) {

            _gl.DeleteBuffer( geometryGroup->__glMorphTargetsBuffers[ m ] );

          }

        }

        if ( geometryGroup->morphNormals.size() ) {

          for ( size_t m = 0, ml = geometryGroup->morphNormals.size(); m < ml; m ++ ) {

            _gl.DeleteBuffer( geometryGroup->__glMorphNormalsBuffers[ m ] );

          }

        }

        deleteBuffers( *geometryGroup );

      }

    } else {

      deleteBuffers( geometry );

    }

  }

};


void GLRenderer::deallocateTexture( Texture& texture ) {

  // REVIEW Correctness
  _gl.DeleteTexture( texture.__glTextureCube );

  if ( ! texture.__glInit ) return;

  texture.__glInit = false;
  _gl.DeleteTexture( texture.__glTexture );

  _info.memory.textures --;

}


void GLRenderer::deallocateRenderTarget( GLRenderTarget& renderTarget ) {

  if ( ! renderTarget.__glTexture ) return;

  _gl.DeleteTexture( renderTarget.__glTexture );

  for ( auto& frameBuffer : renderTarget.__glFramebuffer ) {
    _gl.DeleteFramebuffer( frameBuffer );
  }

  renderTarget.__glFramebuffer.clear();

  for ( auto& renderBuffer : renderTarget.__glRenderbuffer ) {
    _gl.DeleteRenderbuffer( renderBuffer );
  }

  renderTarget.__glRenderbuffer.clear();

}


void GLRenderer::deallocateMaterial( Material& material ) {

  auto program = material.program;

  if ( ! program ) return;

  // only deallocate GL program if this was the last use of shared program
  // assumed there is only single copy of any program in the _programs list
  // (that's how it's constructed)

  auto programCmp = [&]( const ProgramInfo& programInfo ) {
    return programInfo.program == program;
  };

  auto programIt = std::find_if( _programs.begin(), _programs.end(), programCmp );

  if ( programIt == _programs.end() )
    return;

  if ( --programIt->usedTimes == 0 ) {

    _gl.DeleteProgram( program->program );
    _info.memory.programs --;
    _programs.erase( std::remove_if(_programs.begin(), _programs.end(), programCmp) );

  }

}

// Buffer initialization

void GLRenderer::initCustomAttributes( Geometry& geometry, Object3D& object ) {

  const auto nvertices = geometry.vertices.size();

  if ( !object.material ) {
    console().warn( "Object contains no material" );
    return;
  }

  auto& material = *object.material;

  //if ( material.attributes.size() > 0 )
  {

    geometry.__glCustomAttributesList.clear();

    for ( auto& namedAttribute : material.attributes ) {

      auto& a = namedAttribute.first;
      auto& attribute = namedAttribute.second;

      if ( !attribute.__glInitialized || attribute.createUniqueBuffers ) {

        attribute.__glInitialized = true;

        auto size = 1;      // "f" and "i"

        if      ( attribute.type == THREE::v2 ) size = 2;
        else if ( attribute.type == THREE::v3 ) size = 3;
        else if ( attribute.type == THREE::v4 ) size = 4;
        else if ( attribute.type == THREE::c  ) size = 3;

        attribute.size = size;

        attribute.array.resize( nvertices * size );

        attribute.buffer = _gl.CreateBuffer();
        attribute.belongsToAttribute = a;

        attribute.needsUpdate = true;

      }

      geometry.__glCustomAttributesList.emplace_back( &attribute );

    }

  }

}

void GLRenderer::initParticleBuffers( Geometry& geometry, Object3D& object ) {

  auto nvertices = ( int )geometry.vertices.size();

  geometry.__vertexArray.resize( nvertices * 3 );
  geometry.__colorArray.resize( nvertices * 3 );

  geometry.__sortArray.clear();

  geometry.__glParticleCount = nvertices;

  initCustomAttributes( geometry, object );

}

void GLRenderer::initLineBuffers( Geometry& geometry, Object3D& object ) {

  auto nvertices = geometry.vertices.size();

  geometry.__vertexArray.resize( nvertices * 3 );
  geometry.__colorArray.resize( nvertices * 3 );
  geometry.__lineDistanceArray.resize( nvertices );

  geometry.__glLineCount = ( int )nvertices;

  initCustomAttributes( geometry, object );

}

void GLRenderer::initMeshBuffers( GeometryGroup& geometryGroup, Mesh& object ) {

  auto& geometry = *object.geometry;
  auto& faces3 = geometryGroup.faces3;

  auto nvertices = ( int )faces3.size() * 3;
  auto ntris     = ( int )faces3.size() * 1;
  auto nlines    = ( int )faces3.size() * 3;

  auto material = getBufferMaterial( object, &geometryGroup );

  auto uvType = bufferGuessUVType( material );
  auto normalType = bufferGuessNormalType( material );
  auto vertexColorType = bufferGuessVertexColorType( material );

  //console().log( "uvType", uvType, "normalType", normalType, "vertexColorType", vertexColorType, object, geometryGroup, material );

  geometryGroup.__vertexArray.resize( nvertices * 3 );

  if ( normalType ) {
    geometryGroup.__normalArray.resize( nvertices * 3 );
  }

  if ( geometry.hasTangents ) {
    geometryGroup.__tangentArray.resize( nvertices * 4 );
  }

  if ( vertexColorType ) {
    geometryGroup.__colorArray.resize( nvertices * 3 );
  }

  if ( uvType ) {
    if ( geometry.faceVertexUvs.size() > 0 ) {
      geometryGroup.__uvArray.resize( nvertices * 2 );
    }

    if ( geometry.faceVertexUvs.size() > 1 ) {
      geometryGroup.__uv2Array.resize( nvertices * 2 );
    }

  }

  if ( geometry.skinWeights.size() && geometry.skinIndices.size() ) {

    geometryGroup.__skinIndexArray.resize( nvertices * 4 );
    geometryGroup.__skinWeightArray.resize( nvertices * 4 );

  }

  geometryGroup.__faceArray.resize( ntris * 3 );
  geometryGroup.__lineArray.resize( nlines * 2 );

  if ( geometryGroup.numMorphTargets ) {

    geometryGroup.__morphTargetsArrays.clear();

    for ( int m = 0, ml = geometryGroup.numMorphTargets; m < ml; m ++ ) {

      geometryGroup.__morphTargetsArrays.push_back( std::vector<float>( nvertices * 3 ) );

    }

  }

  if ( geometryGroup.numMorphNormals ) {

    geometryGroup.__morphNormalsArrays.clear();

    for ( int m = 0, ml = geometryGroup.numMorphNormals; m < ml; m ++ ) {
      geometryGroup.__morphNormalsArrays.push_back( std::vector<float>( nvertices * 3 ) );
    }

  }

  geometryGroup.__glFaceCount = ntris * 3;
  geometryGroup.__glLineCount = nlines * 2;


  // custom attributes

  if ( material ) {

    geometryGroup.__glCustomAttributesList.clear();

    for ( auto& a : material->attributes ) {

      // Do a shallow copy of the attribute object soe different geometryGroup chunks use different
      // attribute buffers which are correctly indexed in the setMeshBuffers function

      auto& originalAttribute = a.second;

      GeometryBuffer::AttributePtr attribute( new Attribute(originalAttribute) );

      /*for ( auto& property : originalAttribute ) {

          attribute[ property ] = originalAttribute[ property ];

      }*/

      if ( !attribute->__glInitialized || attribute->createUniqueBuffers ) {

        attribute->__glInitialized = true;

        auto size = 1;      // "f" and "i"

        if ( attribute->type == THREE::v2 ) size = 2;
        else if ( attribute->type == THREE::v3 ) size = 3;
        else if ( attribute->type == THREE::v4 ) size = 4;
        else if ( attribute->type == THREE::c ) size = 3;

        attribute->size = size;

        attribute->array.resize( nvertices * size );

        attribute->buffer = _gl.CreateBuffer();
        attribute->belongsToAttribute = a.first;

        originalAttribute.needsUpdate = true;
        attribute->__original = &originalAttribute;

      }

      geometryGroup.__glCustomAttributesList.push_back( std::move(attribute) );

    }

  }

  geometryGroup.__inittedArrays = true;

}

Material* GLRenderer::getBufferMaterial( Object3D& object, GeometryBuffer* geometryBuffer ) {

  auto material = object.material.get();

  if( material ) {

    if ( geometryBuffer && material->type() == THREE::MeshFaceMaterial) {

      auto geometry = object.geometry.get();

      if( geometry && geometryBuffer->materialIndex.valid() ) {

        auto meshFaceMaterial = static_cast<MeshFaceMaterial*>(material);

        if( meshFaceMaterial ) {
          return meshFaceMaterial->materials[ geometryBuffer->materialIndex.value ].get();
        }

      }

    }

  }

  return material;
}

bool GLRenderer::materialNeedsSmoothNormals( const Material* material ) {
  return material && material->shading == THREE::SmoothShading;
}

THREE::Shading GLRenderer::bufferGuessNormalType( const Material* material ) {

  // only MeshBasicMaterial and MeshDepthMaterial don't need normals

  if ( material &&
       (( material->type() == THREE::MeshBasicMaterial && !material->envMap ) ||
        ( material->type() == THREE::MeshDepthMaterial )) ) {
    return THREE::NoShading;
  }

  if ( materialNeedsSmoothNormals( material ) ) {
    return THREE::SmoothShading;
  } else {
    return THREE::FlatShading;
  }

}

THREE::Colors GLRenderer::bufferGuessVertexColorType( const Material* material ) {
  if ( material ) {
    return material->vertexColors;
  }
  return THREE::NoColors;
}

bool GLRenderer::bufferGuessUVType( const Material* material ) {
  // material must use some texture to require uvs
  if ( material && ( material->map || material->lightMap || material->bumpMap || material->specularMap || material->type() == THREE::ShaderMaterial ) ) {
    return true;
  }
  return false;
}

//

void GLRenderer::initDirectBuffers( Geometry& geometry ) {

  for ( auto& a : geometry.attributes ) {

    auto type = a.first == AttributeKey::index() ? GL_ELEMENT_ARRAY_BUFFER
                : GL_ARRAY_BUFFER;

    auto& attribute = a.second;
    attribute.buffer = _gl.CreateBuffer();
    attribute.numItems = (int)attribute.array.size();

    _gl.BindAndBuffer( type, attribute.buffer, attribute.array, GL_STATIC_DRAW );

  }

}

// Buffer setting

void GLRenderer::setParticleBuffers( Geometry& geometry, int hint, Object3D& object ) {

  auto& vertices = geometry.vertices;
  const auto vl = ( int )vertices.size();

  auto& colors = geometry.colors;
  const auto cl = ( int )colors.size();

  auto& vertexArray = geometry.__vertexArray;
  auto& colorArray = geometry.__colorArray;

  auto& sortArray = geometry.__sortArray;

  auto dirtyVertices = geometry.verticesNeedUpdate;
  //auto dirtyElements = geometry.elementsNeedUpdate;
  auto dirtyColors = geometry.colorsNeedUpdate;

  auto& customAttributes = geometry.__glCustomAttributesList;

  Vector3 _vector3;
  int offset = 0;

  if ( object.sortParticles ) {

    _projScreenMatrixPS.copy( _projScreenMatrix );
    _projScreenMatrixPS.multiply( object.matrixWorld );
    sortArray.resize( vl );

    for ( int v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ v ];

      _vector3.copy( vertex );
      _projScreenMatrixPS.multiplyVector3( _vector3 );

      // push_back ?
      sortArray[ v ] = std::make_pair( _vector3.z, v );

    }

    typedef std::pair<float, int> SortPair;

    std::sort( sortArray.begin(),
              sortArray.end(),
              []( const SortPair & a, const SortPair & b ) {
                  return a.first > b.first;
              }  );

    for ( int v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ sortArray[v].second ];

      offset = v * 3;

      vertexArray[ offset ]     = vertex.x;
      vertexArray[ offset + 1 ] = vertex.y;
      vertexArray[ offset + 2 ] = vertex.z;

    }

    for ( int c = 0; c < cl; c ++ ) {

      offset = c * 3;

      const auto& color = colors[ sortArray[c].second ];

      colorArray[ offset ]     = color.r;
      colorArray[ offset + 1 ] = color.g;
      colorArray[ offset + 2 ] = color.b;

    }

    for ( int i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

      auto& customAttribute = *customAttributes[ i ];

      if ( !( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) ) continue;

      if ( customAttribute.size == 1 ) {
        fillFromAny<float>( customAttribute.value, sortArray, customAttribute.array );
      } else if ( customAttribute.size == 2 ) {
        fillFromAny<Vector2>( customAttribute.value, sortArray, customAttribute.array );
      } else if ( customAttribute.size == 3 ) {
        if ( customAttribute.type == THREE::c ) {
          fillFromAny<Color>( customAttribute.value, sortArray, customAttribute.array );
        } else {
          fillFromAny<Vector3>( customAttribute.value, sortArray, customAttribute.array );
        }
      } else if ( customAttribute.size == 4 ) {
        fillFromAny<Vector4>( customAttribute.value, sortArray, customAttribute.array );
      }

    }

  } else {

    if ( dirtyVertices ) {

      for ( int v = 0; v < vl; v ++ ) {

        const auto& vertex = vertices[ v ];

        offset = v * 3;

        vertexArray[ offset ]     = vertex.x;
        vertexArray[ offset + 1 ] = vertex.y;
        vertexArray[ offset + 2 ] = vertex.z;

      }

    }

    if ( dirtyColors ) {

      for ( int c = 0; c < cl; c ++ ) {

        const auto& color = colors[ c ];

        offset = c * 3;

        colorArray[ offset ]     = color.r;
        colorArray[ offset + 1 ] = color.g;
        colorArray[ offset + 2 ] = color.b;

      }

    }

    for ( int i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

      auto& customAttribute = *customAttributes[ i ];

      if ( customAttribute.needsUpdate &&
           ( customAttribute.boundTo.empty() ||
             customAttribute.boundTo == "vertices" ) ) {

        if ( customAttribute.size == 1 ) {
          fillFromAny<float>( customAttribute.value, customAttribute.array );
        } else if ( customAttribute.size == 2 ) {
          fillFromAny<Vector2>( customAttribute.value, customAttribute.array );
        } else if ( customAttribute.size == 3 ) {
          if ( customAttribute.type == THREE::c ) {
            fillFromAny<Color>( customAttribute.value, customAttribute.array );
          } else {
            fillFromAny<Vector3>( customAttribute.value, customAttribute.array );
          }
        } else if ( customAttribute.size == 4 ) {
          fillFromAny<Vector4>( customAttribute.value, customAttribute.array );
        } else {
          console().error("Invalid attribute size");
        }

      }

    }

  }

  if ( vl > 0 && ( dirtyVertices || object.sortParticles ) ) {
    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometry.__glVertexBuffer, vertexArray, hint );
  }

  if ( cl > 0 && ( dirtyColors || object.sortParticles ) ) {
    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometry.__glColorBuffer, colorArray, hint );
  }

  for ( int i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

    auto& customAttribute = *customAttributes[ i ];

    if ( customAttribute.needsUpdate || object.sortParticles ) {
      _gl.BindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );
    }

  }

}


void GLRenderer::setLineBuffers( Geometry& geometry, int hint ) {

  const auto& vertices = geometry.vertices;
  const auto& colors = geometry.colors;
  const auto& lineDistances = geometry.lineDistances;

  const auto vl = ( int )vertices.size();
  const auto cl = ( int )colors.size();
  const auto dl = ( int )lineDistances.size();

  auto& vertexArray = geometry.__vertexArray;
  auto& colorArray = geometry.__colorArray;
  auto& lineDistanceArray = geometry.__lineDistanceArray;

  auto dirtyVertices = geometry.verticesNeedUpdate;
  auto dirtyColors = geometry.colorsNeedUpdate;
  auto dirtylineDistances = geometry.lineDistancesNeedUpdate;

  auto& customAttributes = geometry.__glCustomAttributesList;

  int offset = 0;

  if ( dirtyVertices ) {

    for ( int v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ v ];

      offset = v * 3;

      vertexArray[ offset ]     = vertex.x;
      vertexArray[ offset + 1 ] = vertex.y;
      vertexArray[ offset + 2 ] = vertex.z;

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometry.__glVertexBuffer, vertexArray, hint );

  }

  if ( dirtyColors ) {

    for ( int c = 0; c < cl; c ++ ) {

      const auto& color = colors[ c ];

      offset = c * 3;

      colorArray[ offset ]     = color.r;
      colorArray[ offset + 1 ] = color.g;
      colorArray[ offset + 2 ] = color.b;

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometry.__glColorBuffer, colorArray, hint );

  }

  if ( dirtylineDistances ) {

      for ( int d = 0; d < dl; d ++ ) {

        lineDistanceArray[ d ] = lineDistances[ d ];

      }

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometry.__glLineDistanceBuffer, lineDistanceArray, hint );

    }

  for ( int i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

    auto& customAttribute = *customAttributes[ i ];

    if ( customAttribute.needsUpdate &&
         ( customAttribute.boundTo.empty() ||
           customAttribute.boundTo == "vertices" ) ) {

      if ( customAttribute.size == 1 ) {
        fillFromAny<float>( customAttribute.value, customAttribute.array );
      } else if ( customAttribute.size == 2 ) {
        fillFromAny<Vector2>( customAttribute.value, customAttribute.array );
      } else if ( customAttribute.size == 3 ) {
        if ( customAttribute.type == THREE::c ) {
          fillFromAny<Color>( customAttribute.value, customAttribute.array );
        } else {
          fillFromAny<Vector3>( customAttribute.value, customAttribute.array );
        }
      } else if ( customAttribute.size == 4 ) {
        fillFromAny<Vector4>( customAttribute.value, customAttribute.array );
      }

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );

    }

  }

}


void GLRenderer::setMeshBuffers( GeometryGroup& geometryGroup, Object3D& object, int hint, bool dispose, Material* material ) {

  if ( ! geometryGroup.__inittedArrays ) {
    return;
  }

  const auto normalType      = bufferGuessNormalType( material );
  const auto vertexColorType = bufferGuessVertexColorType( material );
  const auto uvType          = bufferGuessUVType( material );

  const auto needsSmoothNormals = ( normalType == THREE::SmoothShading );

  Color c1, c2, c3, c4;

  int vertexIndex        = 0,
      offset             = 0,
      offset_uv          = 0,
      offset_uv2         = 0,
      offset_face        = 0,
      offset_normal      = 0,
      offset_tangent     = 0,
      offset_line        = 0,
      offset_color       = 0,
      offset_skin        = 0,
      offset_morphTarget = 0,
      offset_custom      = 0;

  auto& vertexArray  = geometryGroup.__vertexArray;
  auto& uvArray      = geometryGroup.__uvArray;
  auto& uv2Array     = geometryGroup.__uv2Array;
  auto& normalArray  = geometryGroup.__normalArray;
  auto& tangentArray = geometryGroup.__tangentArray;
  auto& colorArray   = geometryGroup.__colorArray;

  auto& skinIndexArray   = geometryGroup.__skinIndexArray;
  auto& skinWeightArray  = geometryGroup.__skinWeightArray;

  auto& morphTargetsArrays = geometryGroup.__morphTargetsArrays;
  auto& morphNormalsArrays = geometryGroup.__morphNormalsArrays;

  auto& customAttributes = geometryGroup.__glCustomAttributesList;

  auto& faceArray = geometryGroup.__faceArray;
  auto& lineArray = geometryGroup.__lineArray;

  Geometry& geometry = *object.geometry;

  const bool dirtyVertices     = geometry.verticesNeedUpdate,
             dirtyElements     = geometry.elementsNeedUpdate,
             dirtyUvs          = geometry.uvsNeedUpdate,
             dirtyNormals      = geometry.normalsNeedUpdate,
             dirtyTangents     = geometry.tangentsNeedUpdate,
             dirtyColors       = geometry.colorsNeedUpdate,
             dirtyMorphTargets = geometry.morphTargetsNeedUpdate;

  auto& vertices     = geometry.vertices;
  auto& chunk_faces3 = geometryGroup.faces3;
  auto& obj_faces    = geometry.faces;

  auto& obj_uvs  = geometry.faceVertexUvs[ 0 ];
  auto& obj_uvs2 = geometry.faceVertexUvs[ 1 ];

  //auto& obj_colors = geometry.colors;

  auto& obj_skinIndices   = geometry.skinIndices;
  auto& obj_skinWeights   = geometry.skinWeights;

  auto& morphTargets = geometry.morphTargets;
  auto& morphNormals = geometry.morphNormals;

  if ( dirtyVertices ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      const auto& v1 = vertices[ face.a ];
      const auto& v2 = vertices[ face.b ];
      const auto& v3 = vertices[ face.c ];

      vertexArray[ offset ]     = v1.x;
      vertexArray[ offset + 1 ] = v1.y;
      vertexArray[ offset + 2 ] = v1.z;

      vertexArray[ offset + 3 ] = v2.x;
      vertexArray[ offset + 4 ] = v2.y;
      vertexArray[ offset + 5 ] = v2.z;

      vertexArray[ offset + 6 ] = v3.x;
      vertexArray[ offset + 7 ] = v3.y;
      vertexArray[ offset + 8 ] = v3.z;

      offset += 9;

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer, vertexArray, hint );

  }

  if ( dirtyMorphTargets ) {

    for ( size_t vk = 0, vkl = morphTargets.size(); vk < vkl; vk ++ ) {

      offset_morphTarget = 0;

      for ( const auto& chf : chunk_faces3 ) {

        const auto& face = obj_faces[ chf ];

        // morph positions

        const auto& v1 = morphTargets[ vk ].vertices[ face.a ];
        const auto& v2 = morphTargets[ vk ].vertices[ face.b ];
        const auto& v3 = morphTargets[ vk ].vertices[ face.c ];

        auto& vka = morphTargetsArrays[ vk ];

        vka[ offset_morphTarget ]     = v1.x;
        vka[ offset_morphTarget + 1 ] = v1.y;
        vka[ offset_morphTarget + 2 ] = v1.z;

        vka[ offset_morphTarget + 3 ] = v2.x;
        vka[ offset_morphTarget + 4 ] = v2.y;
        vka[ offset_morphTarget + 5 ] = v2.z;

        vka[ offset_morphTarget + 6 ] = v3.x;
        vka[ offset_morphTarget + 7 ] = v3.y;
        vka[ offset_morphTarget + 8 ] = v3.z;

        // morph normals

        if ( material && material->morphNormals ) {

          Vector3 n1, n2, n3;

          if ( needsSmoothNormals ) {

            // TODO: FIgure out where the vertexNormals array comes from
            const auto& faceVertexNormals = morphNormals[ vk ].vertexNormals[ chf ];

            n1 = faceVertexNormals.a;
            n2 = faceVertexNormals.b;
            n3 = faceVertexNormals.c;

          } else {

            // TODO: FIgure out where the faceNormals array comes from
            n1 = morphNormals[ vk ].faceNormals[ chf ];
            n2 = n1;
            n3 = n1;

          }

          auto& nka = morphNormalsArrays[ vk ];

          nka[ offset_morphTarget ]     = n1.x;
          nka[ offset_morphTarget + 1 ] = n1.y;
          nka[ offset_morphTarget + 2 ] = n1.z;

          nka[ offset_morphTarget + 3 ] = n2.x;
          nka[ offset_morphTarget + 4 ] = n2.y;
          nka[ offset_morphTarget + 5 ] = n2.z;

          nka[ offset_morphTarget + 6 ] = n3.x;
          nka[ offset_morphTarget + 7 ] = n3.y;
          nka[ offset_morphTarget + 8 ] = n3.z;

        }

        //

        offset_morphTarget += 9;

      }

      _gl.BindAndBuffer( GL_ARRAY_BUFFER,
                       geometryGroup.__glMorphTargetsBuffers[ vk ],
                       morphTargetsArrays[ vk ], hint );

      if ( material && material->morphNormals ) {

        _gl.BindAndBuffer( GL_ARRAY_BUFFER,
                         geometryGroup.__glMorphNormalsBuffers[ vk ],
                         morphNormalsArrays[ vk ], hint );

      }

    }

  }

  if ( obj_skinWeights.size() ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      // weights

      const auto& sw1 = obj_skinWeights[ face.a ];
      const auto& sw2 = obj_skinWeights[ face.b ];
      const auto& sw3 = obj_skinWeights[ face.c ];

      skinWeightArray[ offset_skin ]     = sw1.x;
      skinWeightArray[ offset_skin + 1 ] = sw1.y;
      skinWeightArray[ offset_skin + 2 ] = sw1.z;
      skinWeightArray[ offset_skin + 3 ] = sw1.w;

      skinWeightArray[ offset_skin + 4 ] = sw2.x;
      skinWeightArray[ offset_skin + 5 ] = sw2.y;
      skinWeightArray[ offset_skin + 6 ] = sw2.z;
      skinWeightArray[ offset_skin + 7 ] = sw2.w;

      skinWeightArray[ offset_skin + 8 ]  = sw3.x;
      skinWeightArray[ offset_skin + 9 ]  = sw3.y;
      skinWeightArray[ offset_skin + 10 ] = sw3.z;
      skinWeightArray[ offset_skin + 11 ] = sw3.w;

      // indices

      const auto& si1 = obj_skinIndices[ face.a ];
      const auto& si2 = obj_skinIndices[ face.b ];
      const auto& si3 = obj_skinIndices[ face.c ];

      skinIndexArray[ offset_skin ]     = si1.x;
      skinIndexArray[ offset_skin + 1 ] = si1.y;
      skinIndexArray[ offset_skin + 2 ] = si1.z;
      skinIndexArray[ offset_skin + 3 ] = si1.w;

      skinIndexArray[ offset_skin + 4 ] = si2.x;
      skinIndexArray[ offset_skin + 5 ] = si2.y;
      skinIndexArray[ offset_skin + 6 ] = si2.z;
      skinIndexArray[ offset_skin + 7 ] = si2.w;

      skinIndexArray[ offset_skin + 8 ]  = si3.x;
      skinIndexArray[ offset_skin + 9 ]  = si3.y;
      skinIndexArray[ offset_skin + 10 ] = si3.z;
      skinIndexArray[ offset_skin + 11 ] = si3.w;

      offset_skin += 12;

    }

    if ( offset_skin > 0 ) {

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinIndicesBuffer, skinIndexArray, hint );
      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinWeightsBuffer, skinWeightArray, hint );

    }

  }

  if ( dirtyColors && vertexColorType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      const auto& vertexColors = face.vertexColors;
      const auto& faceColor = face.color;

      if ( face.size() == 3 && vertexColorType == THREE::VertexColors ) {

        c1 = vertexColors[ 0 ];
        c2 = vertexColors[ 1 ];
        c3 = vertexColors[ 2 ];

      } else {

        c1 = faceColor;
        c2 = faceColor;
        c3 = faceColor;

      }

      colorArray[ offset_color ]     = c1.r;
      colorArray[ offset_color + 1 ] = c1.g;
      colorArray[ offset_color + 2 ] = c1.b;

      colorArray[ offset_color + 3 ] = c2.r;
      colorArray[ offset_color + 4 ] = c2.g;
      colorArray[ offset_color + 5 ] = c2.b;

      colorArray[ offset_color + 6 ] = c3.r;
      colorArray[ offset_color + 7 ] = c3.g;
      colorArray[ offset_color + 8 ] = c3.b;

      offset_color += 9;

    }

    if ( offset_color > 0 ) {

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glColorBuffer, colorArray, hint );

    }

  }

  if ( dirtyTangents && geometry.hasTangents ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      const auto& vertexTangents = face.vertexTangents;

      const auto& t1 = vertexTangents[ 0 ];
      const auto& t2 = vertexTangents[ 1 ];
      const auto& t3 = vertexTangents[ 2 ];

      tangentArray[ offset_tangent ]     = t1.x;
      tangentArray[ offset_tangent + 1 ] = t1.y;
      tangentArray[ offset_tangent + 2 ] = t1.z;
      tangentArray[ offset_tangent + 3 ] = t1.w;

      tangentArray[ offset_tangent + 4 ] = t2.x;
      tangentArray[ offset_tangent + 5 ] = t2.y;
      tangentArray[ offset_tangent + 6 ] = t2.z;
      tangentArray[ offset_tangent + 7 ] = t2.w;

      tangentArray[ offset_tangent + 8 ]  = t3.x;
      tangentArray[ offset_tangent + 9 ]  = t3.y;
      tangentArray[ offset_tangent + 10 ] = t3.z;
      tangentArray[ offset_tangent + 11 ] = t3.w;

      offset_tangent += 12;

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glTangentBuffer, tangentArray, hint );

  }

  int i = 0;

  if ( dirtyNormals && normalType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      const auto& vertexNormals = face.vertexNormals;
      const auto& faceNormal = face.normal;

      if ( face.size() == 3 && needsSmoothNormals ) {

        for ( i = 0; i < 3; i ++ ) {

          const auto& vn = vertexNormals[ i ];

          normalArray[ offset_normal ]     = vn.x;
          normalArray[ offset_normal + 1 ] = vn.y;
          normalArray[ offset_normal + 2 ] = vn.z;

          offset_normal += 3;

        }

      } else {

        for ( i = 0; i < 3; i ++ ) {

          normalArray[ offset_normal ]     = faceNormal.x;
          normalArray[ offset_normal + 1 ] = faceNormal.y;
          normalArray[ offset_normal + 2 ] = faceNormal.z;

          offset_normal += 3;

        }

      }

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glNormalBuffer, normalArray, hint );

  }

  if ( dirtyUvs && obj_uvs.size() > 0 && uvType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& uv = obj_uvs[ fi ];

      // TODO:?
      //if ( uv == undefined ) continue;

      for ( i = 0; i < 3; i ++ ) {

        const auto& uvi = uv[ i ];

        uvArray[ offset_uv ]     = uvi.x;
        uvArray[ offset_uv + 1 ] = uvi.y;

        offset_uv += 2;

      }

    }

    if ( offset_uv > 0 ) {

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUVBuffer, uvArray, hint );

    }

  }

  if ( dirtyUvs && obj_uvs2.size() != 0 && uvType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& uv2 = obj_uvs2[ fi ];

      // TODO:?
      //if ( uv2 == undefined ) continue;

      for ( i = 0; i < 3; i ++ ) {

        const auto& uv2i = uv2[ i ];

        uv2Array[ offset_uv2 ]     = uv2i.x;
        uv2Array[ offset_uv2 + 1 ] = uv2i.y;

        offset_uv2 += 2;

      }

    }

    if ( offset_uv2 > 0 ) {

      _gl.BindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUV2Buffer, uv2Array, hint );

    }

  }

  if ( dirtyElements ) {

    for ( size_t f = 0; f < chunk_faces3.size(); ++ f ) {

      faceArray[ offset_face ]     = vertexIndex;
      faceArray[ offset_face + 1 ] = vertexIndex + 1;
      faceArray[ offset_face + 2 ] = vertexIndex + 2;

      offset_face += 3;

      lineArray[ offset_line ]     = vertexIndex;
      lineArray[ offset_line + 1 ] = vertexIndex + 1;

      lineArray[ offset_line + 2 ] = vertexIndex;
      lineArray[ offset_line + 3 ] = vertexIndex + 2;

      lineArray[ offset_line + 4 ] = vertexIndex + 1;
      lineArray[ offset_line + 5 ] = vertexIndex + 2;

      offset_line += 6;

      vertexIndex += 3;

    }

    _gl.BindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glFaceBuffer, faceArray, hint );
    _gl.BindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glLineBuffer, lineArray, hint );

  }

  for ( auto& customAttributePtr : customAttributes ) {

    auto& customAttribute = *customAttributePtr;

    if ( customAttribute.__original && ( ! customAttribute.__original->needsUpdate ) ) continue;

    offset_custom = 0;

    if ( customAttribute.size == 1 ) {

      const auto& values = customAttribute.value.cast<std::vector<float>>();

      if ( customAttribute.boundTo.size() == 0 || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          customAttribute.array[ offset_custom ]     = values[ face.a ];
          customAttribute.array[ offset_custom + 1 ] = values[ face.b ];
          customAttribute.array[ offset_custom + 2 ] = values[ face.c ];

          offset_custom += 3;

        }


      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = values[ fi ];

          customAttribute.array[ offset_custom ]     = value;
          customAttribute.array[ offset_custom + 1 ] = value;
          customAttribute.array[ offset_custom + 2 ] = value;

          offset_custom += 3;

        }

      }

    } else if ( customAttribute.size == 2 ) {

      // TODO: Determine the proper data type here...
      const auto& values = customAttribute.value.cast<std::vector<Vector2>>();

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = values[ face.a ];
          const auto& v2 = values[ face.b ];
          const auto& v3 = values[ face.c ];

          customAttribute.array[ offset_custom ]     = v1.x;
          customAttribute.array[ offset_custom + 1 ] = v1.y;

          customAttribute.array[ offset_custom + 2 ] = v2.x;
          customAttribute.array[ offset_custom + 3 ] = v2.y;

          customAttribute.array[ offset_custom + 4 ] = v3.x;
          customAttribute.array[ offset_custom + 5 ] = v3.y;

          offset_custom += 6;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = values[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;

          customAttribute.array[ offset_custom ]     = v1.x;
          customAttribute.array[ offset_custom + 1 ] = v1.y;

          customAttribute.array[ offset_custom + 2 ] = v2.x;
          customAttribute.array[ offset_custom + 3 ] = v2.y;

          customAttribute.array[ offset_custom + 4 ] = v3.x;
          customAttribute.array[ offset_custom + 5 ] = v3.y;

          offset_custom += 6;

        }

      }

    } else if ( customAttribute.size == 3 ) {

      // TODO: Support colors! THREE::c
      const auto& values = customAttribute.value.cast<std::vector<Vector3>>();

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = values[ face.a ];
          const auto& v2 = values[ face.b ];
          const auto& v3 = values[ face.c ];

          customAttribute.array[ offset_custom ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1 ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2 ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3 ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4 ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5 ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6 ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7 ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8 ] = v3[ 2 ];

          offset_custom += 9;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = values[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;

          customAttribute.array[ offset_custom ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1 ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2 ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3 ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4 ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5 ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6 ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7 ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8 ] = v3[ 2 ];

          offset_custom += 9;

        }

      } else if ( customAttribute.boundTo == "faceVertices" ) {

        const auto& face_values = customAttribute.value.cast<std::vector<std::array<Vector3, 4>>>();

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = face_values[ fi ];

          const auto& v1 = value[ 0 ];
          const auto& v2 = value[ 1 ];
          const auto& v3 = value[ 2 ];

          customAttribute.array[ offset_custom ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1 ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2 ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3 ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4 ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5 ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6 ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7 ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8 ] = v3[ 2 ];

          offset_custom += 9;

        }

      }

    } else if ( customAttribute.size == 4 ) {

      const auto& values = customAttribute.value.cast<std::vector<Vector4>>();

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = values[ face.a ];
          const auto& v2 = values[ face.b ];
          const auto& v3 = values[ face.c ];

          customAttribute.array[ offset_custom  ]     = v1.x;
          customAttribute.array[ offset_custom + 1  ] = v1.y;
          customAttribute.array[ offset_custom + 2  ] = v1.z;
          customAttribute.array[ offset_custom + 3  ] = v1.w;

          customAttribute.array[ offset_custom + 4  ] = v2.x;
          customAttribute.array[ offset_custom + 5  ] = v2.y;
          customAttribute.array[ offset_custom + 6  ] = v2.z;
          customAttribute.array[ offset_custom + 7  ] = v2.w;

          customAttribute.array[ offset_custom + 8  ] = v3.x;
          customAttribute.array[ offset_custom + 9  ] = v3.y;
          customAttribute.array[ offset_custom + 10 ] = v3.z;
          customAttribute.array[ offset_custom + 11 ] = v3.w;

          offset_custom += 12;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = values[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;

          customAttribute.array[ offset_custom  ]     = v1.x;
          customAttribute.array[ offset_custom + 1  ] = v1.y;
          customAttribute.array[ offset_custom + 2  ] = v1.z;
          customAttribute.array[ offset_custom + 3  ] = v1.w;

          customAttribute.array[ offset_custom + 4  ] = v2.x;
          customAttribute.array[ offset_custom + 5  ] = v2.y;
          customAttribute.array[ offset_custom + 6  ] = v2.z;
          customAttribute.array[ offset_custom + 7  ] = v2.w;

          customAttribute.array[ offset_custom + 8  ] = v3.x;
          customAttribute.array[ offset_custom + 9  ] = v3.y;
          customAttribute.array[ offset_custom + 10 ] = v3.z;
          customAttribute.array[ offset_custom + 11 ] = v3.w;

          offset_custom += 12;

        }

      } else if ( customAttribute.boundTo == "faceVertices" ) {

        const auto& face_values = customAttribute.value.cast<std::vector<std::array<Vector4, 4>>>();

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = face_values[ fi ];

          const auto& v1 = value[ 0 ];
          const auto& v2 = value[ 1 ];
          const auto& v3 = value[ 2 ];

          customAttribute.array[ offset_custom  ]     = v1.x;
          customAttribute.array[ offset_custom + 1  ] = v1.y;
          customAttribute.array[ offset_custom + 2  ] = v1.z;
          customAttribute.array[ offset_custom + 3  ] = v1.w;

          customAttribute.array[ offset_custom + 4  ] = v2.x;
          customAttribute.array[ offset_custom + 5  ] = v2.y;
          customAttribute.array[ offset_custom + 6  ] = v2.z;
          customAttribute.array[ offset_custom + 7  ] = v2.w;

          customAttribute.array[ offset_custom + 8  ] = v3.x;
          customAttribute.array[ offset_custom + 9  ] = v3.y;
          customAttribute.array[ offset_custom + 10 ] = v3.z;
          customAttribute.array[ offset_custom + 11 ] = v3.w;

          offset_custom += 12;

        }

      }

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );

  }

  if ( dispose ) {

    geometryGroup.dispose();

  }

}

void GLRenderer::setDirectBuffers( Geometry& geometry, int hint, bool dispose ) {

  auto& attributes = geometry.attributes;

  for ( auto& a : attributes ) {

    const auto& attributeName = a.first;
    auto& attributeItem = a.second;

    if ( attributeItem.needsUpdate ) {

      if( attributeName == AttributeKey::index() ) {
        _gl.BindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, attributeItem.buffer, attributeItem.array, hint );
      } else {
        _gl.BindAndBuffer( GL_ARRAY_BUFFER, attributeItem.buffer, attributeItem.array, hint );
      }

      attributeItem.needsUpdate = false;
    }

    if ( dispose && !attributeItem.dynamic ) {

      attributeItem.array.clear();

    }

  }

}

// Buffer rendering

void GLRenderer::renderBufferImmediate( Object3D& object, Program& program, Material& material ) {

  if ( object.glImmediateData.hasPositions && ! object.glImmediateData.__glVertexBuffer ) object.glImmediateData.__glVertexBuffer = _gl.CreateBuffer();
  if ( object.glImmediateData.hasNormals   && ! object.glImmediateData.__glNormalBuffer ) object.glImmediateData.__glNormalBuffer = _gl.CreateBuffer();
  if ( object.glImmediateData.hasUvs       && ! object.glImmediateData.__glUvBuffer )     object.glImmediateData.__glUvBuffer     = _gl.CreateBuffer();
  if ( object.glImmediateData.hasColors    && ! object.glImmediateData.__glColorBuffer )  object.glImmediateData.__glColorBuffer  = _gl.CreateBuffer();

  if ( object.glImmediateData.hasPositions ) {

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glVertexBuffer, object.glImmediateData.positionArray, GL_DYNAMIC_DRAW );
    _gl.EnableVertexAttribArray( program.attributes[AttributeKey::position()] );
    _gl.VertexAttribPointer( program.attributes[AttributeKey::position()], 3, GL_FLOAT, false, 0, 0 );

  }

  if ( object.glImmediateData.hasNormals ) {

    if ( material.shading == THREE::FlatShading ) {

      auto& normalArray = object.glImmediateData.normalArray;

      for ( int i = 0, il = object.glImmediateData.count; i < il; i += 9 ) {

        const auto nax  = normalArray[ i ];
        const auto nay  = normalArray[ i + 1 ];
        const auto naz  = normalArray[ i + 2 ];

        const auto nbx  = normalArray[ i + 3 ];
        const auto nby  = normalArray[ i + 4 ];
        const auto nbz  = normalArray[ i + 5 ];

        const auto ncx  = normalArray[ i + 6 ];
        const auto ncy  = normalArray[ i + 7 ];
        const auto ncz  = normalArray[ i + 8 ];

        const auto nx = ( nax + nbx + ncx ) / 3;
        const auto ny = ( nay + nby + ncy ) / 3;
        const auto nz = ( naz + nbz + ncz ) / 3;

        normalArray[ i ]     = nx;
        normalArray[ i + 1 ] = ny;
        normalArray[ i + 2 ] = nz;

        normalArray[ i + 3 ] = nx;
        normalArray[ i + 4 ] = ny;
        normalArray[ i + 5 ] = nz;

        normalArray[ i + 6 ] = nx;
        normalArray[ i + 7 ] = ny;
        normalArray[ i + 8 ] = nz;

      }

    }

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glNormalBuffer, object.glImmediateData.normalArray, GL_DYNAMIC_DRAW );
    _gl.EnableVertexAttribArray( program.attributes[AttributeKey::normal()] );
    _gl.VertexAttribPointer( program.attributes[AttributeKey::normal()], 3, GL_FLOAT, false, 0, 0 );

  }

  if ( object.glImmediateData.hasUvs && material.map ) {

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glUvBuffer, object.glImmediateData.uvArray, GL_DYNAMIC_DRAW );
    _gl.EnableVertexAttribArray( program.attributes[AttributeKey::uv()] );
    _gl.VertexAttribPointer( program.attributes[AttributeKey::uv()], 2, GL_FLOAT, false, 0, 0 );

  }

  if ( object.glImmediateData.hasColors && material.vertexColors != THREE::NoColors ) {

    _gl.BindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glColorBuffer, object.glImmediateData.colorArray, GL_DYNAMIC_DRAW );
    _gl.EnableVertexAttribArray( program.attributes[AttributeKey::color()] );
    _gl.VertexAttribPointer( program.attributes[AttributeKey::color()], 3, GL_FLOAT, false, 0, 0 );

  }

  _gl.DrawArrays( GL_TRIANGLES, 0, object.glImmediateData.count );

  object.glImmediateData.count = 0;

}

void GLRenderer::renderBufferDirect( Camera& camera, Lights& lights, IFog* fog, Material& material, BufferGeometry& geometry, Object3D& object ) {

  if ( material.visible == false ) return;

  auto& program = setProgram( camera, lights, fog, material, object );

  auto& programAttributes = program.attributes;
  auto& geometryAttributes = geometry.attributes;

  auto updateBuffers = false;
  auto wireframeBit = material.wireframe ? 1 : 0;
  auto geometryHash = ( geometry.id * 0xffffff ) + ( program.id * 2 ) + wireframeBit;

  if ( geometryHash != _currentGeometryGroupHash ) {

    _currentGeometryGroupHash = geometryHash;
    updateBuffers = true;

  }

  if ( updateBuffers ) {

      disableAttributes();

  }

  // render mesh

  if ( object.type() == THREE::Mesh ) {

    if( geometry.attributes.contains( AttributeKey::index() ) ) {

      const auto& offsets = geometry.offsets;

      // if there is more than 1 chunk
      // must set attribute pointers to use new offsets for each chunk
      // even if geometry and materials didn't change

      if ( offsets.size() > 1 ) updateBuffers = true;

      for ( size_t i = 0, il = offsets.size(); i < il; ++ i ) {

        const auto startIndex = offsets[ i ].index;

        if ( updateBuffers ) {

          for ( auto& namedAttribute : programAttributes ) {

            auto& attrKey = namedAttribute.first;
            auto& attributePointer = namedAttribute.second;

            if((int)attributePointer >= 0 && geometry.attributes.contains( attrKey ) ) {

                auto& attributeItem = geometryAttributes[ attrKey ];

                const auto attributeItemSize = attributeItem.itemSize;

                _gl.BindBuffer( GL_ARRAY_BUFFER, attributeItem.buffer );
                enableAttribute( attributePointer );
                _gl.VertexAttribPointer( attributePointer, attributeItemSize, GL_FLOAT, false, 0, toOffset( startIndex * attributeItemSize * 4 ) );

            } else if ( material.defaultAttributeValues.find( attrKey ) != material.defaultAttributeValues.end() ) {

              if ( material.defaultAttributeValues[ attrKey ].size() == 2 ) {

                _gl.VertexAttrib2fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

              } else if ( material.defaultAttributeValues[ attrKey ].size() == 3 ) {

                _gl.VertexAttrib3fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

              }

            }

          }

          // indices

          const auto& index = geometry.attributes[ AttributeKey::index() ];

          _gl.BindBuffer( GL_ELEMENT_ARRAY_BUFFER, index.buffer );

        }

        // render indexed triangles

        _gl.DrawElements( GL_TRIANGLES, offsets[ i ].count, GL_UNSIGNED_SHORT, toOffset( offsets[ i ].start * 2 ) ); // 2 bytes per Uint16

        _info.render.calls ++;
        _info.render.vertices += offsets[ i ].count; // not really true, here vertices can be shared
        _info.render.faces += offsets[ i ].count / 3;

      }

    // non-indexed triangles

    } else {

        if ( updateBuffers ) {

          for ( auto& namedAttribute : programAttributes ) {

            if ( namedAttribute.first == AttributeKey::index()) continue;

            auto& attrKey = namedAttribute.first;
            auto& attributePointer = namedAttribute.second;

            if((int)attributePointer >= 0 && geometry.attributes.contains( attrKey ) ) {
              auto& attributeItem = geometryAttributes[ attrKey ];

                const auto attributeItemSize = attributeItem.itemSize;

                _gl.BindBuffer( GL_ARRAY_BUFFER, attributeItem.buffer );
                enableAttribute( attributePointer );
                _gl.VertexAttribPointer( attributePointer, attributeItemSize, GL_FLOAT, false, 0, 0 );

            }
            else if ( material.defaultAttributeValues.find( attrKey ) != material.defaultAttributeValues.end() ) {

              if ( material.defaultAttributeValues[ attrKey ].size() == 2 ) {

                _gl.VertexAttrib2fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

              } else if ( material.defaultAttributeValues[ attrKey ].size() == 3 ) {

                _gl.VertexAttrib3fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

              }

            }

          }
        }

        // render non-indexed triangles

        const auto& position = geometry.attributes[ AttributeKey::position() ];

        _gl.DrawArrays(GL_TRIANGLES, 0, position.numItems / 3);

        _info.render.calls ++;
        _info.render.vertices += position.numItems / 3;
        _info.render.faces += position.numItems / 3 / 3;

    }

    // render particles

  } else if ( object.type() == THREE::ParticleSystem ) {

    if ( updateBuffers ) {

      for ( auto& namedAttribute : programAttributes ) {

        auto& attrKey = namedAttribute.first;
        auto& attributePointer = namedAttribute.second;

        if((int)attributePointer >= 0 && geometry.attributes.contains( attrKey ) ) {
          auto& attributeItem = geometryAttributes[ attrKey ];

            const auto attributeItemSize = attributeItem.itemSize;

            _gl.BindBuffer( GL_ARRAY_BUFFER, attributeItem.buffer );
            enableAttribute( attributePointer );
            _gl.VertexAttribPointer( attributePointer, attributeItemSize, GL_FLOAT, false, 0, 0 );

        }
        else if ( material.defaultAttributeValues.find( attrKey ) != material.defaultAttributeValues.end() ) {

          if ( material.defaultAttributeValues[ attrKey ].size() == 2 ) {

            _gl.VertexAttrib2fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

          } else if ( material.defaultAttributeValues[ attrKey ].size() == 3 ) {

            _gl.VertexAttrib3fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

          }

        }

      }
    }

    const auto& position = geometry.attributes[ AttributeKey::position() ];

    _gl.DrawArrays(GL_POINTS, 0, position.numItems / 3);

    _info.render.calls ++;
    _info.render.points += position.numItems / 3;

  } // ParticleSystem
  else if ( object.type() == THREE::Line ) {

    if ( updateBuffers ) {

      for ( auto& namedAttribute : programAttributes ) {

        auto& attrKey = namedAttribute.first;
        auto& attributePointer = namedAttribute.second;

        if((int)attributePointer >= 0 && geometry.attributes.contains( attrKey ) ) {
          auto& attributeItem = geometryAttributes[ attrKey ];

            const auto attributeItemSize = attributeItem.itemSize;

            _gl.BindBuffer( GL_ARRAY_BUFFER, attributeItem.buffer );
            enableAttribute( attributePointer );
            _gl.VertexAttribPointer( attributePointer, attributeItemSize, GL_FLOAT, false, 0, 0 );


        } else if ( material.defaultAttributeValues.find( attrKey ) != material.defaultAttributeValues.end() ) {

          if ( material.defaultAttributeValues[ attrKey ].size() == 2 ) {

            _gl.VertexAttrib2fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

          } else if ( material.defaultAttributeValues[ attrKey ].size() == 3 ) {

            _gl.VertexAttrib3fv( attributePointer, &material.defaultAttributeValues[ attrKey ][0] );

          }

        }

      }

      const auto& position = geometry.attributes[ AttributeKey::position() ];

      const auto primitives = ( static_cast<Line&>(object).lineType == THREE::LineStrip ) ? GL_LINE_STRIP : GL_LINES;

      _gl.DrawArrays(primitives, 0, position.numItems / 3);

      _info.render.calls ++;
      _info.render.points += position.numItems;

    }
  }

}

void GLRenderer::renderBuffer( Camera& camera, Lights& lights, IFog* fog, Material& material, GeometryGroup& geometryGroup, Object3D& object ) {

  if ( material.visible == false ) return;

  auto& program = setProgram( camera, lights, fog, material, object );

  auto& attributes = program.attributes;

  auto updateBuffers = false;
  auto wireframeBit = material.wireframe ? 1 : 0;
  auto geometryGroupHash = ( geometryGroup.id * 0xffffff ) + ( program.id * 2 ) + wireframeBit;

  if ( geometryGroupHash != _currentGeometryGroupHash ) {

    _currentGeometryGroupHash = geometryGroupHash;
    updateBuffers = true;

  }

  if( updateBuffers ) {
    disableAttributes();
  }

  // vertices

  if ( !material.morphTargets && attributes[AttributeKey::position()].valid() ) {

    if ( updateBuffers ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer );
      enableAttribute( attributes[AttributeKey::position()] );
      _gl.VertexAttribPointer( attributes[AttributeKey::position()], 3, GL_FLOAT, false, 0, 0 );

    }

  } else {

    if ( object.morphTargetBase != -1 ) {

      setupMorphTargets( material, geometryGroup, object );

    }

  }


  if ( updateBuffers ) {

    // custom attributes

    // Use the per-geometryGroup custom attribute arrays which are setup in initMeshBuffers

    for ( auto& attribute : geometryGroup.__glCustomAttributesList ) {

      auto attributeIt = attributes.find( attribute->belongsToAttribute );
      if ( attributeIt != attributes.end() ) {

        _gl.BindBuffer( GL_ARRAY_BUFFER, attribute->buffer );
        enableAttribute( attributeIt->second );
        _gl.VertexAttribPointer( attributeIt->second, attribute->size, GL_FLOAT, false, 0, 0 );

      }

    }

    int index = -1;

    // colors

    if ( (index = attributes[AttributeKey::color()]) >= 0 ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glColorBuffer );
      enableAttribute( attributes[AttributeKey::color()] );
      _gl.VertexAttribPointer( index, 3, GL_FLOAT, false, 0, 0 );

    } else {

      _gl.VertexAttrib3fv( attributes[AttributeKey::color()], &material.defaultAttributeValues[ AttributeKey::color() ][0] );

    }

    // normals

    if ( attributes[AttributeKey::normal()].valid() ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glNormalBuffer );
      enableAttribute( attributes[AttributeKey::normal()] );
      _gl.VertexAttribPointer( attributes[AttributeKey::normal()], 3, GL_FLOAT, false, 0, 0 );

    }

    // tangents

    if ( attributes[AttributeKey::tangent()].valid() ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glTangentBuffer );
      enableAttribute( attributes[AttributeKey::tangent()] );
      _gl.VertexAttribPointer( attributes[AttributeKey::tangent()], 4, GL_FLOAT, false, 0, 0 );

    }

    // uvs

    if ( attributes[AttributeKey::uv()].valid() ) {

      if ( object.geometry->faceVertexUvs.size() > 0 ) {

        _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUVBuffer );
        enableAttribute( attributes[AttributeKey::uv()] );
        _gl.VertexAttribPointer( attributes[AttributeKey::uv()], 2, GL_FLOAT, false, 0, 0 );

        _gl.EnableVertexAttribArray( attributes[AttributeKey::uv()] );

      } else {

        _gl.VertexAttrib3fv( attributes[AttributeKey::uv()], &material.defaultAttributeValues[ AttributeKey::uv() ][0] );

      }

    }

    if ( attributes[AttributeKey::uv2()].valid() ) {

      if ( object.geometry->faceVertexUvs.size() > 1 ) {

        _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUV2Buffer );
        _gl.VertexAttribPointer( attributes[AttributeKey::uv2()], 2, GL_FLOAT, false, 0, 0 );

        _gl.EnableVertexAttribArray( attributes[AttributeKey::uv2()] );

      } else {

        _gl.DisableVertexAttribArray( attributes[AttributeKey::uv2()] );

      }

    }

    if ( material.skinning &&
         attributes[AttributeKey::skinIndex()].valid() && attributes[AttributeKey::skinWeight()].valid() ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinIndicesBuffer );
      enableAttribute( attributes[AttributeKey::skinIndex()] );
      _gl.VertexAttribPointer( attributes[AttributeKey::skinIndex()], 4, GL_FLOAT, false, 0, 0 );

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinWeightsBuffer );
      enableAttribute( attributes[AttributeKey::skinWeight()] );
      _gl.VertexAttribPointer( attributes[AttributeKey::skinWeight()], 4, GL_FLOAT, false, 0, 0 );

    }

      int lineDistance =attributes[AttributeKey::lineDistance()];

    if ( lineDistance >= 0 ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glLineDistanceBuffer );
      enableAttribute( lineDistance );
      _gl.VertexAttribPointer( lineDistance, 1, GL_FLOAT, false, 0, 0 );

    }

  }

  // render mesh

  if ( object.type() == THREE::Mesh ) {

    // wireframe

    if ( material.wireframe ) {

      setLineWidth( material.wireframeLinewidth );

      if ( updateBuffers ) _gl.BindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glLineBuffer );
      _gl.DrawElements( GL_LINES, geometryGroup.__glLineCount, GL_UNSIGNED_SHORT, 0 );

      // triangles

    } else {

      if ( updateBuffers ) _gl.BindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glFaceBuffer );
      _gl.DrawElements( GL_TRIANGLES, geometryGroup.__glFaceCount, GL_UNSIGNED_SHORT, 0 );

    }

    _info.render.calls ++;
    _info.render.vertices += geometryGroup.__glFaceCount;
    _info.render.faces += geometryGroup.__glFaceCount / 3;

    // render lines

  } else if ( object.type() == THREE::Line ) {

    const auto primitives = ( static_cast<Line&>(object).lineType == THREE::LineStrip ) ? GL_LINE_STRIP : GL_LINES;

    setLineWidth( material.linewidth );

    _gl.DrawArrays( primitives, 0, geometryGroup.__glLineCount );

    _info.render.calls ++;


    // render particles

  } else if ( object.type() == THREE::ParticleSystem ) {

#ifndef THREE_GLES
    // TODO(jdd): Check usage with core profile.
    _gl.Enable(GL_VERTEX_PROGRAM_POINT_SIZE);
    //_gl.Enable(GL_POINT_SMOOTH);
    THREE_REVIEW("EA: Without GL_POINT_SPRITE enabled, the particles aren't rendered, due to gl_PointCoord pointing to 0s")
    _gl.Enable(GL_POINT_SPRITE);
    //glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
#endif

    _gl.DrawArrays( GL_POINTS, 0, geometryGroup.__glParticleCount );

    _info.render.calls ++;
    _info.render.points += geometryGroup.__glParticleCount;

  }

}

void GLRenderer::enableAttribute( int attributeId ) {

  _gl.EnableVertexAttribArray( attributeId );
  _enabledAttributes[ attributeId ] = true;

}

void GLRenderer::disableAttributes() {

  for ( auto& attribute : _enabledAttributes ) {

    _gl.DisableVertexAttribArray( attribute.first );

    _enabledAttributes[ attribute.first ] = false;

  }

}

void GLRenderer::setupMorphTargets( Material& material, GeometryGroup& geometryGroup, Object3D& object ) {

#ifndef TODO_MORPH_TARGETS

  console().warn( "GLRenderer::setupMorphTargets: Not implemented" );

#else

  // set base

  auto& attributes = material.program.attributes;

  if ( object.morphTargetBase != -1 ) {

    _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ object.morphTargetBase ] );
    _gl.VertexAttribPointer( attributes[AttributeKey::position()], 3, GL_FLOAT, false, 0, 0 );

  } else if ( attributes[AttributeKey::position()].valid() ) {

    _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer );
    _gl.VertexAttribPointer( attributes[AttributeKey::position()], 3, GL_FLOAT, false, 0, 0 );

  }

  if ( object.morphTargetForcedOrder.size() ) {

    // set forced order

    auto m = 0;
    auto& order = object.morphTargetForcedOrder;
    auto& influences = object.glData.morphTargetInfluences;

    while ( m < material.numSupportedMorphTargets && m < order.size() ) {

      _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ order[ m ] ] );
      _gl.VertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

      if ( material.morphNormals ) {

        _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphNormalsBuffers[ order[ m ] ] );
        _gl.VertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

      }

      object.glData.__glMorphTargetInfluences[ m ] = influences[ order[ m ] ];

      m ++;
    }

  } else {

    // find the most influencing

    typedef std::pair<int, int> IndexedInfluence;
    std::vector<IndexedInfluence> activeInfluenceIndices;
    auto& influences = object.morphTargetInfluences;

    for ( size_t i = 0, il = influences.size(); i < il; i ++ ) {

      auto& influence = influences[ i ];

      if ( influence > 0 ) {

        activeInfluenceIndices.push_back( IndexedInfluence( i, influence ) );

      }

    }

    if ( activeInfluenceIndices.size() > material.numSupportedMorphTargets ) {

      std::sort( activeInfluenceIndices.begin(),
                 activeInfluenceIndices.end(),
                 NumericalSort() );
      activeInfluenceIndices.resize( material.numSupportedMorphTargets );

    } else if ( activeInfluenceIndices.size() > material.numSupportedMorphNormals ) {

      std::sort( activeInfluenceIndices.begin(),
                 activeInfluenceIndices.end(),
                 NumericalSort() );

    } else if ( activeInfluenceIndices.size() == 0 ) {

      activeInfluenceIndices.push_back( IndexedInfluence( 0, 0 ) );

    }

    auto influenceIndex, m = 0;

    while ( m < material.numSupportedMorphTargets ) {

      if ( activeInfluenceIndices[ m ] ) {

        influenceIndex = activeInfluenceIndices[ m ][ 0 ];

        _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ influenceIndex ] );

        _gl.VertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

        if ( material.morphNormals ) {

          _gl.BindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphNormalsBuffers[ influenceIndex ] );
          _gl.VertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

        }

        object.__glMorphTargetInfluences[ m ] = influences[ influenceIndex ];

      } else {

        _gl.VertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

        if ( material.morphNormals ) {

          _gl.VertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

        }

        object.__glMorphTargetInfluences[ m ] = 0;

      }

      m ++;

    }

  }

  // load updated influences uniform

  if ( material.program.uniforms.morphTargetInfluences.size() > 0 ) {

    _gl.Uniform1fv( material.program.uniforms.morphTargetInfluences, object.__glMorphTargetInfluences );

  }

#endif // TODO_MORPH_TARGETS

}

// Rendering

void GLRenderer::render( Scene& scene, Camera& camera, const GLRenderTarget::Ptr& renderTarget /*= GLRenderTarget::Ptr()*/, bool forceClear /*= false*/ ) {

  auto& lights = scene.__lights;
  auto  fog = scene.fog.get();

  // reset caching for this frame

  _currentMaterialId = -1;
  _lightsNeedUpdate = true;

  // update scene graph

  if ( scene.autoUpdate ) scene.updateMatrixWorld();

  // update camera matrices and frustum

  if ( ! camera.parent ) camera.updateMatrixWorld();

  camera.matrixWorldInverse.getInverse( camera.matrixWorld );

  camera.matrixWorldInverse.flattenToArray( camera._viewMatrixArray );
  camera.projectionMatrix.flattenToArray( camera._projectionMatrixArray );

  _projScreenMatrix.multiplyMatrices( camera.projectionMatrix, camera.matrixWorldInverse );
  _frustum.setFromMatrix( _projScreenMatrix );

  // update WebGL objects

  if ( autoUpdateObjects ) initGLObjects( scene );


  // custom render plugins (pre pass)

  renderPlugins( renderPluginsPre, scene, camera );

  //

  _info.render.calls = 0;
  _info.render.vertices = 0;
  _info.render.faces = 0;
  _info.render.points = 0;

  setRenderTarget( renderTarget );

  if ( autoClear || forceClear ) {
    clear( autoClearColor, autoClearDepth, autoClearStencil );
  }

  // set matrices for regular objects (frustum culled)

  auto& renderList = scene.__glObjects;

  for ( size_t i = 0; i < renderList.size(); ++i) {

    auto& glObject = renderList[i];
    auto& object = *glObject.object;

    glObject.id = static_cast<int>( i );
    glObject.render = false;

    if ( object.visible ) {

      if ( !( object.type() == THREE::Mesh || object.type() == THREE::ParticleSystem ) ||
           !( object.frustumCulled ) || _frustum.intersectsObject( object ) ) {

        setupMatrices( object, camera );
        unrollBufferMaterial( glObject );
        glObject.render = true;

        if ( sortObjects ) {

          if ( object.renderDepth ) {
            glObject.z = object.renderDepth;
          } else {
            _vector3.copy( object.matrixWorld.getPosition() );
            _vector3.applyProjection( _projScreenMatrix );
            glObject.z = _vector3.z;
          }

        }

      }

    }

  }

  if ( sortObjects ) {
    std::sort( renderList.begin(), renderList.end(), PainterSortStable() );
  }

  // set matrices for immediate objects

  auto& immediateList = scene.__glObjectsImmediate;

  for ( auto& glObject : immediateList ) {

    auto& object = *glObject.object;

    if ( object.visible ) {

      if ( object.matrixAutoUpdate ) {
        object.matrixWorld.flattenToArray( object.glData._modelMatrixArray );
      }

      setupMatrices( object, camera );
      unrollImmediateBufferMaterial( glObject );
    }
  }

  if ( scene.overrideMaterial ) {

    auto& material = *scene.overrideMaterial;

    setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst );
    setDepthTest( material.depthTest );
    setDepthWrite( material.depthWrite );
    setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );

    renderObjects( scene.__glObjects, false, THREE::Override, camera, lights, fog, true, &material );
    renderObjectsImmediate( scene.__glObjectsImmediate, THREE::Override, camera, lights, fog, false, &material );

  } else {

    // opaque pass (front-to-back order)

    setBlending( THREE::NoBlending );

    renderObjects( scene.__glObjects, true, THREE::Opaque, camera, lights, fog, false );
    renderObjectsImmediate( scene.__glObjectsImmediate, THREE::Opaque, camera, lights, fog, false );

    // transparent pass (back-to-front order)

    renderObjects( scene.__glObjects, false, THREE::Transparent, camera, lights, fog, true );
    renderObjectsImmediate( scene.__glObjectsImmediate, THREE::Transparent, camera, lights, fog, true );

  }

  // custom render plugins (post pass)

  renderPlugins( renderPluginsPost, scene, camera );

  // Generate mipmap if we're using any kind of mipmap filtering

  if ( renderTarget &&
       renderTarget->generateMipmaps &&
       renderTarget->minFilter != THREE::NearestFilter &&
       renderTarget->minFilter != THREE::LinearFilter ) {

    updateRenderTargetMipmap( *renderTarget );

  }

  // Ensure depth buffer writing is enabled so it can be cleared on next render

  setDepthTest( true );
  setDepthWrite( true );

  // _gl.Finish();

}

void GLRenderer::renderPlugins( std::vector<IPlugin::Ptr>& plugins, Scene& scene, Camera& camera ) {

  for ( auto& plugin : plugins ) {

    // reset state for plugin (to start from clean slate)

    resetStates();

    plugin->render( scene, camera, _currentWidth, _currentHeight );

    // reset state after plugin (anything could have changed)
    resetStates();

  }

}

void GLRenderer::renderObjects( RenderList& renderList, bool reverse, THREE::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial /*= nullptr*/ ) {

  int start, end, delta;

  if ( reverse ) {

    start = ( int )renderList.size() - 1;
    end = -1;
    delta = -1;

  } else {

    start = 0;
    end = ( int )renderList.size();
    delta = 1;
  }

  for ( auto i = start; i != end; i += delta ) {

    auto& glObject = renderList[ i ];

    if ( glObject.render ) {

      auto& object = *glObject.object;
      auto& buffer = *glObject.buffer;

      Material* material = nullptr;

      if ( overrideMaterial ) {

        material = overrideMaterial;

      } else {

        material = materialType == THREE::Opaque ? glObject.opaque : glObject.transparent;

        if ( ! material ) continue;

          if ( useBlending ) {
             setBlending( material->blending, material->blendEquation, material->blendSrc, material->blendDst );
          }

        setDepthTest( material->depthTest );
        setDepthWrite( material->depthWrite );
        setPolygonOffset( material->polygonOffset, material->polygonOffsetFactor, material->polygonOffsetUnits );
      }

      setMaterialFaces( *material );

      if ( buffer.type() == THREE::BufferGeometry ) {
        renderBufferDirect( camera, lights, fog, *material, static_cast<BufferGeometry&>( buffer ), object );
      } else {
        renderBuffer( camera, lights, fog, *material, static_cast<GeometryGroup&>( buffer ), object );
      }

    }

  }

}

void GLRenderer::renderObjectsImmediate( RenderList& renderList, THREE::RenderType materialType, Camera& camera, Lights& lights, IFog* fog, bool useBlending, Material* overrideMaterial /*= nullptr*/ ) {

  for ( auto& glObject : renderList ) {

    auto& object = *glObject.object;

    if ( object.visible ) {

      Material* material = nullptr;

      if ( overrideMaterial ) {

        material = overrideMaterial;

      } else {

        material = materialType == THREE::Opaque ? glObject.opaque : glObject.transparent;

        if ( ! material ) continue;

        if ( useBlending ) setBlending( material->blending, material->blendEquation, material->blendSrc, material->blendDst );

        setDepthTest( material->depthTest );
        setDepthWrite( material->depthWrite );
        setPolygonOffset( material->polygonOffset, material->polygonOffsetFactor, material->polygonOffsetUnits );

      }

      renderImmediateObject( camera, lights, fog, *material, object );

    }

  }

}

void GLRenderer::renderImmediateObject( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object ) {

  auto& program = setProgram( camera, lights, fog, material, object );

  _currentGeometryGroupHash = -1;

  setMaterialFaces( material );

  if ( object.immediateRenderCallback ) {
    object.immediateRenderCallback( &program, &_gl, &_frustum );
  } else {
    object.render( [this, &program, &material]( Object3D & object ) {
      renderBufferImmediate( object, program, material );
    } );
  }

}

void GLRenderer::unrollImmediateBufferMaterial( Scene::GLObject& globject ) {

  auto& object = *globject.object;

  if ( !object.material )
    return;

  auto& material = *object.material;

  if ( material.transparent ) {

    globject.transparent = &material;
    globject.opaque = nullptr;

  } else {

    globject.opaque = &material;
    globject.transparent = nullptr;

  }

}

void GLRenderer::unrollBufferMaterial( Scene::GLObject& globject ) {

  if ( !globject.object || !globject.buffer )
    return;

  auto& object = *globject.object;

  if ( !object.material )
    return;

  auto& buffer = *globject.buffer;

  auto& meshMaterial = object.material;

  if ( meshMaterial->type() == THREE::MeshFaceMaterial ) {

    const auto materialIndex = buffer.materialIndex;

    if ( materialIndex.valid() ) {

        const auto& material = std::static_pointer_cast<MeshFaceMaterial>(meshMaterial)->materials[ materialIndex.value ];

      if ( material->transparent ) {

        globject.transparent = material.get();
        globject.opaque = nullptr;

      } else {

        globject.opaque = material.get();
        globject.transparent = nullptr;

      }

    }

  } else {

    auto& material = meshMaterial;

    if ( material->transparent ) {

      globject.transparent = material.get();
      globject.opaque = nullptr;

    } else {

      globject.opaque = material.get();
      globject.transparent = nullptr;

    }

  }

}

// Geometry splitting

void GLRenderer::sortFacesByMaterial( Geometry& geometry, Material& material ) {

  // 0: index, 1: count
  typedef std::pair<int, int> MaterialHashValue;

  std::unordered_map<int, MaterialHashValue> hash_map;

  const auto numMorphTargets = ( int )geometry.morphTargets.size();
  const auto numMorphNormals = ( int )geometry.morphNormals.size();

  const auto useFaceMaterial = material.type() == THREE::MeshFaceMaterial;

  geometry.geometryGroups.clear();

  for ( int f = 0, fl = ( int )geometry.faces.size(); f < fl; ++f ) {

    const auto& face = geometry.faces[ f ];

    const auto materialIndex = useFaceMaterial ? face.materialIndex : 0;
    const auto materialHash = materialIndex;

    if ( hash_map.count( materialHash ) == 0 ) {
      hash_map[ materialHash ] = MaterialHashValue( materialHash, 0 );
    }

    auto groupHash = toString( hash_map[ materialHash ] );

    if ( geometry.geometryGroups.count( groupHash ) == 0 ) {
      geometry.geometryGroups.insert( std::make_pair( groupHash, GeometryGroup::create( materialIndex, numMorphTargets, numMorphNormals ) ) );
    }

    auto geometryGroup = geometry.geometryGroups[ groupHash ];

    const auto vertices = 3;

    if ( geometryGroup->vertices + vertices > 65535 ) {

      hash_map[ materialHash ].second += 1;
      groupHash = toString( hash_map[ materialHash ] );

      if ( geometry.geometryGroups.count( groupHash ) == 0 ) {
        geometryGroup = GeometryGroup::create( materialIndex, numMorphTargets, numMorphNormals );
        geometry.geometryGroups.insert( std::make_pair( groupHash, geometryGroup ) );
      }

    }

    THREE_ASSERT( face.type() == THREE::Face3 );
    geometryGroup->faces3.push_back( f );

    geometryGroup->vertices += vertices;

  }

  geometry.geometryGroupsList.clear();

  for ( auto& geometryGroup : geometry.geometryGroups ) {
    geometryGroup.second->id = _geometryGroupCounter ++;
    geometry.geometryGroupsList.push_back( geometryGroup.second.get() );
  }

}

// Objects refresh

void GLRenderer::initGLObjects( Scene& scene ) {

  /*scene.__glObjects.clear();
  scene.__glObjectsImmediate.clear();
  scene.__glSprites.clear();
  scene.__glFlares.clear();*/

  while ( scene.__objectsAdded.size() ) {
    addObject( *scene.__objectsAdded[ 0 ], scene );
    scene.__objectsAdded.erase( scene.__objectsAdded.begin() );
  }

  while ( scene.__objectsRemoved.size() ) {
    removeObject( *scene.__objectsRemoved[ 0 ], scene );
    scene.__objectsRemoved.erase( scene.__objectsRemoved.begin() );
  }

  // update must be called after objects adding / removal

  for ( auto& glObject : scene.__glObjects ) {

    //TODO(ea): Do we need the hack here?

    updateObject( *glObject.object );
  }

}

// Objects adding

static inline void addBuffer( RenderList& objlist, GeometryBuffer& buffer, Object3D& object ) {
  objlist.emplace_back( Scene::GLObject(
                          0,
                          &buffer,
                          &object,
                          false,
                          nullptr,
                          nullptr
                        ) );
}

static inline void addBufferImmediate( RenderList& objlist, Object3D& object ) {
  objlist.emplace_back( Scene::GLObject(
                          0,
                          nullptr,
                          &object,
                          false,
                          nullptr,
                          nullptr
                        ) );
}

void GLRenderer::addObject( Object3D& object, Scene& scene ) {

  Geometry& geometry = *object.geometry;

  if ( ! object.glData.__glInit ) {

    object.glData.__glInit = true;

    geometry.__glInit = true;

    object.geometry->addEventListener( "dispose", std::bind(&GLRenderer::onGeometryDispose, this, std::placeholders::_1 ) );
      
    if ( geometry.type() == THREE::BufferGeometry ) {

        initDirectBuffers( geometry );

    } else if ( object.type() == THREE::Mesh ) {

      if ( geometry.geometryGroups.empty() ) {
        sortFacesByMaterial( geometry, *static_cast<Mesh&>( object ).material );
      }

      // create separate VBOs per geometry chunk

      for ( auto& geometryGroup : geometry.geometryGroups ) {

        // initialise VBO on the first access

        if ( ! geometryGroup.second->__glVertexBuffer ) {

          createMeshBuffers( *geometryGroup.second );
          initMeshBuffers( *geometryGroup.second, static_cast<Mesh&>( object ) );

          geometry.verticesNeedUpdate     = true;
          geometry.morphTargetsNeedUpdate = true;
          geometry.elementsNeedUpdate     = true;
          geometry.uvsNeedUpdate          = true;
          geometry.normalsNeedUpdate      = true;
          geometry.tangentsNeedUpdate     = true;
          geometry.colorsNeedUpdate       = true;

        }

      }

    } else if ( object.type() == THREE::Line ) {

      if ( ! geometry.__glVertexBuffer ) {

        createLineBuffers( geometry );
        initLineBuffers( geometry, object );

        geometry.verticesNeedUpdate = true;
        geometry.colorsNeedUpdate = true;
        geometry.lineDistancesNeedUpdate = true;

      }

    } else if ( object.type() == THREE::ParticleSystem ) {

      if ( ! geometry.__glVertexBuffer ) {

        createParticleBuffers( geometry );
        initParticleBuffers( geometry, object );

        geometry.verticesNeedUpdate = true;
        geometry.colorsNeedUpdate = true;

      }

    }

  }

  if ( ! object.glData.__glActive ) {

    if ( object.type() == THREE::Mesh ) {

      if ( geometry.type() == THREE::BufferGeometry ) {

        addBuffer( scene.__glObjects, geometry, object );

      } else {

        for ( auto& geometryGroup : geometry.geometryGroups ) {

          addBuffer( scene.__glObjects, *geometryGroup.second, object );

        }

      }

    } else if ( object.type() == THREE::Line ||
                object.type() == THREE::ParticleSystem ) {

      addBuffer( scene.__glObjects, geometry, object );

    } else if ( object.type() == THREE::ImmediateRenderObject || object.immediateRenderCallback ) {

      addBufferImmediate( scene.__glObjectsImmediate, object );

    } else if ( object.type() == THREE::Sprite ) {

      scene.__glSprites.push_back( &object );

    } else if ( object.type() == THREE::LensFlare ) {

      scene.__glFlares.push_back( &object );

    }

    object.glData.__glActive = true;

  }

}

// Objects updates

void GLRenderer::updateObject( Object3D& object ) {

  if ( !object.geometry )
    return;

  auto& geometry = *object.geometry;

  Material* material = nullptr;
  //GeometryGroup* geometryGroup = nullptr;

  if ( geometry.type() == THREE::BufferGeometry ) {
    setDirectBuffers( geometry, GL_DYNAMIC_DRAW, !geometry.dynamic );
  }
  else if ( object.type() == THREE::Mesh ) {

    // check all geometry groups

    for ( auto& geometryGroup : geometry.geometryGroupsList ) {

      material = getBufferMaterial( object, geometryGroup );

      if ( !material ) continue;

      const auto customAttributesDirty = areCustomAttributesDirty( *material );

      if ( geometry.verticesNeedUpdate  || geometry.morphTargetsNeedUpdate ||
           geometry.uvsNeedUpdate       || geometry.normalsNeedUpdate      ||
           geometry.colorsNeedUpdate    || geometry.tangentsNeedUpdate     ||
           geometry.elementsNeedUpdate  || customAttributesDirty ) {

        setMeshBuffers( *geometryGroup, object, GL_DYNAMIC_DRAW, !geometry.dynamic, material );

      }

      clearCustomAttributes( *material );

    }

    geometry.verticesNeedUpdate     = false;
    geometry.morphTargetsNeedUpdate = false;
    geometry.elementsNeedUpdate     = false;
    geometry.uvsNeedUpdate          = false;
    geometry.normalsNeedUpdate      = false;
    geometry.colorsNeedUpdate       = false;
    geometry.tangentsNeedUpdate     = false;

    geometry.buffersNeedUpdate      = false;

    if( material->attributes.size() ) clearCustomAttributes( *material );


  } else if ( object.type() == THREE::Line ) {

    auto material = getBufferMaterial( object, &geometry );

    if ( !material ) return;

    const auto customAttributesDirty = areCustomAttributesDirty( *material );

    if ( geometry.verticesNeedUpdate ||  geometry.colorsNeedUpdate || geometry.lineDistancesNeedUpdate || customAttributesDirty ) {
      setLineBuffers( geometry, GL_DYNAMIC_DRAW );
    }

    geometry.verticesNeedUpdate = false;
    geometry.colorsNeedUpdate = false;
    geometry.lineDistancesNeedUpdate = false;

    clearCustomAttributes( *material );

  } else if ( object.type() == THREE::ParticleSystem ) {

    auto material = getBufferMaterial( object, &geometry );

    if ( !material ) return;

    const auto customAttributesDirty = areCustomAttributesDirty( *material );

    if ( geometry.verticesNeedUpdate || geometry.colorsNeedUpdate || object.sortParticles || customAttributesDirty ) {
      setParticleBuffers( geometry, GL_DYNAMIC_DRAW, object );
    }

    geometry.verticesNeedUpdate = false;
    geometry.colorsNeedUpdate = false;

    clearCustomAttributes( *material );

  }

}

// Objects updates - custom attributes check

bool GLRenderer::areCustomAttributesDirty( const Material& material ) {

  for ( const auto& attribute : material.attributes ) {
    if ( attribute.second.needsUpdate ) return true;
  }

  return false;

}

void GLRenderer::clearCustomAttributes( Material& material ) {

  for ( auto& attribute : material.attributes ) {
    attribute.second.needsUpdate = false;
  }

}

// Objects removal

void GLRenderer::removeObject( Object3D& object, Scene& scene ) {

  if ( object.type() == THREE::Mesh  ||
       object.type() == THREE::ParticleSystem ||
       object.type() == THREE::Line ) {
    removeInstances( scene.__glObjects, object );
  } else if ( object.type() == THREE::Sprite ) {
    removeInstancesDirect( scene.__glSprites, object );
  } else if ( object.type() == THREE::LensFlare ) {
    removeInstancesDirect( scene.__glFlares, object );
  } else if ( object.type() == THREE::ImmediateRenderObject || object.immediateRenderCallback ) {
    removeInstances( scene.__glObjectsImmediate, object );
  }

  object.glData.__glActive = false;

}

void GLRenderer::removeInstances( RenderList& objlist, Object3D& object ) {

  for ( auto o = (int)objlist.size() - 1; o >= 0; o -- ) {
    if ( objlist[ o ].object == &object ) {
      objlist.erase( objlist.begin() + o );
    }
  }

}

void GLRenderer::removeInstancesDirect( RenderListDirect& objlist, Object3D& object ) {

  for ( auto o = (int)objlist.size() - 1; o >= 0; o -- ) {
    if ( objlist[ o ] == &object ) {
      objlist.erase( objlist.begin() + o );
    }
  }

}

// Materials

void GLRenderer::initMaterial( Material& material, Lights& lights, IFog* fog, Object3D& object ) {

  material.addEventListener( "dispose", std::bind(&GLRenderer::onMaterialDispose, this, std::placeholders::_1) );

  std::string shaderID;

  switch ( material.type() ) {
  case THREE::MeshDepthMaterial:
    setMaterialShaders( material, ShaderLib::depth() );
    break;
  case THREE::MeshNormalMaterial:
    setMaterialShaders( material, ShaderLib::normal() );
    break;
  case THREE::MeshBasicMaterial:
    setMaterialShaders( material, ShaderLib::basic() );
    break;
  case THREE::MeshLambertMaterial:
    setMaterialShaders( material, ShaderLib::lambert() );
    break;
  case THREE::MeshPhongMaterial:
    setMaterialShaders( material, ShaderLib::phong() );
    break;
  case THREE::LineBasicMaterial:
    setMaterialShaders( material, ShaderLib::basic() );
    break;
  case THREE::LineDashedMaterial:
    setMaterialShaders( material, ShaderLib::dashed() );
    break;
  case THREE::ParticleSystemMaterial:
    setMaterialShaders( material, ShaderLib::particleBasic() );
    break;
  case THREE::ShaderMaterial:
    break;
  default:
    console().warn( "GLRenderer::initMaterial: Unknown material type" );
    break;
  };

  // heuristics to create shader parameters according to lights in the scene
  // (not to blow over maxLights budget)

  const auto maxLightCount = allocateLights( lights );
  const auto maxShadows = allocateShadows( lights );
  const auto maxBones = allocateBones( object );

  auto parameters = ProgramParameters();
  parameters.map = !!material.map;
  parameters.envMap = !!material.envMap;
  parameters.bumpMap = !!material.bumpMap;
  parameters.normalMap = !!material.normalMap;
  parameters.specularMap = !!material.specularMap;
  parameters.vertexColors = material.vertexColors;
  parameters.fog = fog;
  parameters.useFog = !!material.fog;
  parameters.fogExp = fog ? fog->type() == THREE::FogExp2 : false;
  parameters.sizeAttenuation = material.sizeAttenuation;
  parameters.skinning = material.skinning;
  parameters.maxBones = maxBones;
  parameters.useVertexTexture = _supportsBoneTextures && object.useVertexTexture;
  parameters.morphTargets = material.morphTargets;
  parameters.morphNormals = material.morphNormals;
  parameters.maxMorphTargets = maxMorphTargets;
  parameters.maxMorphNormals = maxMorphNormals;
  parameters.maxDirLights = maxLightCount.directional;
  parameters.maxPointLights = maxLightCount.point;
  parameters.maxSpotLights = maxLightCount.spot;
  parameters.maxHemiLights = maxLightCount.hemi;
  parameters.maxShadows = maxShadows;
  parameters.shadowMapEnabled = shadowMapEnabled && object.receiveShadow;
  parameters.shadowMapType = shadowMapType;
  parameters.shadowMapDebug = shadowMapDebug;
  parameters.shadowMapCascade = shadowMapCascade;
  parameters.alphaTest = material.alphaTest;
  parameters.metal = material.metal;
  parameters.perPixel = material.perPixel;
  parameters.wrapAround = material.wrapAround;
  parameters.doubleSided = material.side == THREE::DoubleSide;
  parameters.flipSided = material.side == THREE::BackSide;

  material.program = buildProgram( shaderID,
                                   material.fragmentShader,
                                   material.vertexShader,
                                   material.uniforms,
                                   material.attributes,
                                   material.defines,
                                   parameters );

  if ( !material.program ) {
    console().error() << "Aborting material initialization";
    return;
  }

  auto& attributes = material.program->attributes;

  // if ( attributes[AttributeKey::position()].valid() )
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::position()] );

  // if ( attributes[AttributeKey::color()].valid() )
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::color()] );

  // if ( attributes[AttributeKey::normal()].valid() )
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::normal()] );

  // if ( attributes[AttributeKey::tangent()].valid() )
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::tangent()] );

  // if ( material.skinning &&
  //      attributes[AttributeKey::skinVertexA()].valid() && attributes[AttributeKey::skinVertexB()].valid() &&
  //      attributes[AttributeKey::skinIndex()].valid() && attributes[AttributeKey::skinWeight()].valid() ) {

  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::skinVertexA()] );
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::skinVertexB()] );
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::skinIndex()] );
  //   _gl.EnableVertexAttribArray( attributes[AttributeKey::skinWeight()] );

  // }

  // for ( const auto& a : material.attributes ) {
  //   auto attributeIt = attributes.find( a.first );
  //   if ( attributeIt != attributes.end() && attributeIt->second.valid() ) {
  //     _gl.EnableVertexAttribArray( attributeIt->second );
  //   }
  // }

  if ( material.morphTargets ) {

    material.numSupportedMorphTargets = 0;

    std::string id, base = "morphTarget";

    for ( int i = 0; i < maxMorphTargets; i ++ ) {
      id = toString( base, i );
      if ( attributes[ id ].valid() ) {
        _gl.EnableVertexAttribArray( attributes[ id ] );
        material.numSupportedMorphTargets ++;
      }
    }

  }

  if ( material.morphNormals ) {

    material.numSupportedMorphNormals = 0;

    const std::string base = "morphNormal";

    for ( int i = 0; i < maxMorphNormals; i ++ ) {
      auto id = toString( base, i );
      if ( attributes[ id ].valid() ) {
        _gl.EnableVertexAttribArray( attributes[ id ] );
        material.numSupportedMorphNormals ++;
      }
    }

  }

  material.uniformsList.clear();

  for ( auto& u : material.uniforms ) {
    material.uniformsList.emplace_back( &u.second, u.first );
  }

}

void GLRenderer::setMaterialShaders( Material& material, const Shader& shaders ) {
  material.uniforms = shaders.uniforms;
  material.vertexShader = shaders.vertexShader;
  material.fragmentShader = shaders.fragmentShader;
}

Program& GLRenderer::setProgram( Camera& camera, Lights& lights, IFog* fog, Material& material, Object3D& object ) {

  _usedTextureUnits = 0;

  if ( material.needsUpdate ) {
    if ( material.program ) {
      deallocateMaterial( material );
    }
    initMaterial( material, lights, fog, object );
    material.needsUpdate = false;
  }

  if ( material.morphTargets ) {
    object.glData.__glMorphTargetInfluences.resize( maxMorphTargets );
  }

  auto refreshMaterial = false;

  auto& program    = *material.program;
  auto& p_uniforms = program.uniforms;
  auto& m_uniforms = material.uniforms;

  if ( &program != _currentProgram ) {
    _gl.UseProgram( program.program );
    _currentProgram = &program;
    refreshMaterial = true;
  }

  if ( material.id != _currentMaterialId ) {
    _currentMaterialId = material.id;
    refreshMaterial = true;
  }

  if ( refreshMaterial || &camera != _currentCamera ) {
    _gl.UniformMatrix4fv( p_uniforms[UniformKey::projectionMatrix()], 1, false, camera._projectionMatrixArray.data() );
    if ( &camera != _currentCamera ) _currentCamera = &camera;
  }

  // skinning uniforms must be set even if material didn't change
  // auto-setting of texture unit for bone texture must go before other textures
  // not sure why, but otherwise weird things happen

   if ( material.skinning ) {
    if ( _supportsBoneTextures && object.useVertexTexture ) {
      const auto boneTextureLocation = uniformLocation( p_uniforms, "boneTexture" );
      if ( validUniformLocation( boneTextureLocation ) ) {
        auto textureUnit = getTextureUnit();
        _gl.Uniform1i( boneTextureLocation, textureUnit );
        setTexture( *object.boneTexture, textureUnit );
      }

      const auto boneTextureWidthLocation = uniformLocation( p_uniforms, "boneTextureWidth" );
      if ( validUniformLocation( boneTextureWidthLocation ) ) {
        _gl.Uniform1i( boneTextureWidthLocation, object.boneTextureWidth );
      }

      const auto boneTextureHeightLocation = uniformLocation( p_uniforms, "boneTextureHeight" );
      if ( validUniformLocation( boneTextureHeightLocation ) ) {
        _gl.Uniform1i( boneTextureHeightLocation, object.boneTextureHeight );
      }
    } else {
      const auto boneMatricesLocation = uniformLocation( p_uniforms, "boneGlobalMatrices" );
      if ( validUniformLocation( boneMatricesLocation ) ) {
        _gl.UniformMatrix4fv( boneMatricesLocation,
                            ( int )object.boneMatrices.size(),
                            false,
                            reinterpret_cast<const float*>( &object.boneMatrices[0] ) );
      }
    }
  }

  if ( refreshMaterial ) {
    // refresh uniforms common to several materials
    if ( fog && material.fog ) {
      refreshUniformsFog( m_uniforms, *fog );
    }

    if ( material.type() == THREE::MeshPhongMaterial ||
         material.type() == THREE::MeshLambertMaterial ||
         material.lights ) {

      if ( _lightsNeedUpdate ) {
        setupLights( program, lights );
        _lightsNeedUpdate = false;
      }

      refreshUniformsLights( m_uniforms, _lights );
    }

    if ( material.type() == THREE::MeshBasicMaterial ||
         material.type() == THREE::MeshLambertMaterial ||
         material.type() == THREE::MeshPhongMaterial ) {
      refreshUniformsCommon( m_uniforms, material );
    }

    // refresh single material specific uniforms

    if ( material.type() == THREE::LineBasicMaterial ) {
      refreshUniformsLine( m_uniforms, material );
    } else if ( material.type() == THREE::LineDashedMaterial ) {
      refreshUniformsLine( m_uniforms, material );
      refreshUniformsDash( m_uniforms, material );
    } else if ( material.type() == THREE::ParticleSystemMaterial ) {
      refreshUniformsParticle( m_uniforms, material );
    } else if ( material.type() == THREE::MeshPhongMaterial ) {
      refreshUniformsPhong( m_uniforms, material );
    } else if ( material.type() == THREE::MeshLambertMaterial ) {
      refreshUniformsLambert( m_uniforms, material );
    } else if ( material.type() == THREE::MeshDepthMaterial ) {
      m_uniforms[UniformKey::mNear()].value = camera.near;
      m_uniforms[UniformKey::mFar()].value = camera.far;
      m_uniforms[UniformKey::opacity()].value = material.opacity;
    } else if ( material.type() == THREE::MeshNormalMaterial ) {
      m_uniforms[UniformKey::opacity()].value = material.opacity;
    }

    if ( object.receiveShadow && ! material.shadowPass ) {
      refreshUniformsShadow( m_uniforms, lights );
    }

    // load common uniforms

    const bool warnOnNotFound = material.type() == THREE::ShaderMaterial;
    loadUniformsGeneric( program, material.uniformsList, warnOnNotFound );

    // load material specific uniforms
    // (shader material also gets them for the sake of genericity)

    if ( material.type() == THREE::ShaderMaterial ||
         material.type() == THREE::MeshPhongMaterial ||
         material.envMap ) {

      const auto cameraPositionLocation = uniformLocation( p_uniforms, "cameraPosition" );
      if ( validUniformLocation( cameraPositionLocation ) ) {
        _vector3.setFromMatrixPosition( camera.matrixWorld );
        _gl.Uniform3f( cameraPositionLocation, _vector3.x, _vector3.y, _vector3.z );
      }

    }

    if ( material.type() == THREE::MeshPhongMaterial ||
         material.type() == THREE::MeshLambertMaterial ||
         material.type() == THREE::ShaderMaterial ||
         material.skinning ) {

      const auto viewMatrixLocation = uniformLocation( p_uniforms, "viewMatrix" );
      if ( validUniformLocation( viewMatrixLocation ) ) {
        _gl.UniformMatrix4fv( viewMatrixLocation, 1, false, camera._viewMatrixArray.data() );
      }

    }
  }

  loadUniformsMatrices( p_uniforms, object );

  const auto modelMatrixLocation = uniformLocation( p_uniforms, "modelMatrix" );
  if ( validUniformLocation( modelMatrixLocation ) ) {
    _gl.UniformMatrix4fv( modelMatrixLocation, 1, false, object.matrixWorld.elements );
  }

  return program;

}


// Uniforms (refresh uniforms objects)

void GLRenderer::refreshUniformsCommon( Uniforms& uniforms, Material& material ) {

  uniforms[UniformKey::opacity()].value = material.opacity;

  if ( gammaInput ) {
    uniforms[UniformKey::diffuse()].value = Color().copyGammaToLinear( material.color );
  } else {
    uniforms[UniformKey::diffuse()].value = material.color;
  }

  uniforms[UniformKey::map()].value         = material.map.get();
  uniforms[UniformKey::lightMap()].value    = material.lightMap.get();
  uniforms[UniformKey::specularMap()].value = material.specularMap.get();

  if ( material.bumpMap ) {
    uniforms[UniformKey::bumpMap()].value   = material.bumpMap.get();
    uniforms[UniformKey::bumpScale()].value = material.bumpScale;
  }

  if ( material.normalMap ) {
    uniforms[UniformKey::normalMap()].value   = material.normalMap.get();
    uniforms[UniformKey::normalScale()].value = material.normalScale;
  }

  // uv repeat and offset setting priorities
  //  1. color map
  //  2. specular map
  //  3. bump map
  //  4. normal map

  Texture* uvScaleMap = nullptr;

  if ( material.map ) {
    uvScaleMap = material.map.get();
  } else if ( material.specularMap ) {
    uvScaleMap = material.specularMap.get();
  } else if ( material.bumpMap ) {
    uvScaleMap = material.bumpMap.get();
  } else if ( material.normalMap ) {
    uvScaleMap = material.normalMap.get();
  }

  if ( uvScaleMap ) {
    const auto& offset = uvScaleMap->offset;
    const auto& repeat = uvScaleMap->repeat;

    uniforms[UniformKey::offsetRepeat()].value = Vector4( offset.x, offset.y, repeat.x, repeat.y );
  }

  uniforms[UniformKey::envMap()].value     = material.envMap.get();
  uniforms[UniformKey::flipEnvMap()].value = ( material.envMap && material.envMap->type() == THREE::GLRenderTargetCube ) ? 1 : -1;

  if ( gammaInput ) {
    //uniforms.reflectivity.value = material.reflectivity * material.reflectivity;
    uniforms[UniformKey::reflectivity()].value = material.reflectivity;
  } else {
    uniforms[UniformKey::reflectivity()].value = material.reflectivity;
  }

  uniforms[UniformKey::refractionRatio()].value = material.refractionRatio;
  uniforms[UniformKey::combine()].value         = ( int )material.combine;
  uniforms[UniformKey::useRefract()].value      = material.envMap && material.envMap->mapping == THREE::CubeRefractionMapping;

}

void GLRenderer::refreshUniformsLine( Uniforms& uniforms, Material& material ) {

  uniforms[UniformKey::diffuse()].value = material.color;
  uniforms[UniformKey::opacity()].value = material.opacity;

}

void GLRenderer::refreshUniformsDash( Uniforms& uniforms, Material& material ) {

  uniforms[UniformKey::dashSize()].value = material.dashSize;
  uniforms[UniformKey::totalSize()].value = material.dashSize + material.gapSize;
  uniforms[UniformKey::scale()].value = material.scale;

}

void GLRenderer::refreshUniformsParticle( Uniforms& uniforms, Material& material ) {

  uniforms[UniformKey::psColor()].value = material.color;
  uniforms[UniformKey::opacity()].value = material.opacity;
  uniforms[UniformKey::size()].value    = material.size;
  uniforms[UniformKey::scale()].value   = _height / 2.0f; // TODO: Cache

  uniforms[UniformKey::map()].value = material.map.get();

}

void GLRenderer::refreshUniformsFog( Uniforms& uniforms, IFog& fog ) {

  if ( fog.type() == THREE::Fog ) {

    auto& f = static_cast<Fog&>(fog);
    uniforms[UniformKey::fogColor()].value = f.color;
    uniforms[UniformKey::fogNear()].value  = f.near;
    uniforms[UniformKey::fogFar()].value   = f.far;

  } else if ( fog.type() == THREE::FogExp2 ) {

    auto& f = static_cast<FogExp2&>(fog);
    uniforms[UniformKey::fogColor()].value   = f.color;
    uniforms[UniformKey::fogDensity()].value = f.density;

  }

}

void GLRenderer::refreshUniformsPhong( Uniforms& uniforms, Material& material ) {

  uniforms[UniformKey::shininess()].value = material.shininess;

  if ( gammaInput ) {
    uniforms[UniformKey::ambient()].value  = Color().copyGammaToLinear( material.ambient );
    uniforms[UniformKey::emissive()].value = Color().copyGammaToLinear( material.emissive );
    uniforms[UniformKey::specular()].value = Color().copyGammaToLinear( material.specular );
  } else {
    uniforms[UniformKey::ambient()].value  = material.ambient;
    uniforms[UniformKey::emissive()].value = material.emissive;
    uniforms[UniformKey::specular()].value = material.specular;
  }

  if ( material.wrapAround ) {
    uniforms[UniformKey::wrapRGB()].value = material.wrapRGB;
  }

}

void GLRenderer::refreshUniformsLambert( Uniforms& uniforms, Material& material ) {

  if ( gammaInput ) {
    uniforms[UniformKey::ambient()].value  = Color().copyGammaToLinear( material.ambient );
    uniforms[UniformKey::emissive()].value = Color().copyGammaToLinear( material.emissive );
  } else {
    uniforms[UniformKey::ambient()].value  = material.ambient;
    uniforms[UniformKey::emissive()].value = material.emissive;
  }

  if ( material.wrapAround ) {
    uniforms[UniformKey::wrapRGB()].value = material.wrapRGB;
  }

}

void GLRenderer::refreshUniformsLights( Uniforms& uniforms, InternalLights& lights ) {

  uniforms[UniformKey::ambientLightColor()].value         = lights.ambient;

  uniforms[UniformKey::directionalLightColor()].value     = lights.directional.colors;
  uniforms[UniformKey::directionalLightDirection()].value = lights.directional.positions;

  uniforms[UniformKey::pointLightColor()].value           = lights.point.colors;
  uniforms[UniformKey::pointLightPosition()].value        = lights.point.positions;
  uniforms[UniformKey::pointLightDistance()].value        = lights.point.distances;

  uniforms[UniformKey::spotLightColor()].value            = lights.spot.colors;
  uniforms[UniformKey::spotLightPosition()].value         = lights.spot.positions;
  uniforms[UniformKey::spotLightDistance()].value         = lights.spot.distances;
  uniforms[UniformKey::spotLightDirection()].value        = lights.spot.directions;
  uniforms[UniformKey::spotLightAngleCos()].value         = lights.spot.anglesCos;
  uniforms[UniformKey::spotLightExponent()].value         = lights.spot.exponents;

  uniforms[UniformKey::hemisphereLightSkyColor()].value   = lights.hemi.skyColors;
  uniforms[UniformKey::hemisphereLightGroundColor()].value = lights.hemi.groundColors;
  uniforms[UniformKey::hemisphereLightDirection()].value   = lights.hemi.positions;

}

void GLRenderer::refreshUniformsShadow( Uniforms& uniforms, Lights& lights ) {

#ifndef TODO_UNIFORMS_SHADOW

  console().warn( "GLRenderer::refreshUniformsShadow: Not implemented" );

#else
  if ( contains( uniforms, "shadowMatrix" ) ) {

    int j = 0;

    for ( auto i = 0, il = lights.size(); i < il; i ++ ) {

      auto light = *lights[ i ];

      if ( ! light.castShadow ) continue;

      if ( light.type() == THREE::SpotLight || ( light.type() == THREE::DirectionalLight && ! light.shadowCascade ) ) {

        uniforms[UniformKey::shadowMap()].value[ j ]   = light.shadowMap;
        uniforms[UniformKey::shadowMapSize()].value[ j ] = light.shadowMapSize;

        uniforms[UniformKey::shadowMatrix()].value[ j ] = light.shadowMatrix;

        uniforms[UniformKey::shadowDarkness()].value[ j ] = light.shadowDarkness;
        uniforms[UniformKey::shadowBias()].value[ j ]     = light.shadowBias;

        j ++;

      }

    }

  }
#endif

}

// Uniforms (load to GPU)

void GLRenderer::loadUniformsMatrices( UniformLocations& uniforms, Object3D& object ) {

  _gl.UniformMatrix4fv( uniforms[UniformKey::modelViewMatrix()], 1, false, object.glData._modelViewMatrix.elements );
  const auto normalMatrixLocation = uniformLocation( uniforms, "normalMatrix" );
  if ( validUniformLocation( normalMatrixLocation ) ) {
    _gl.UniformMatrix3fv( normalMatrixLocation, 1, false, object.glData._normalMatrix.elements );
  }

}

int GLRenderer::getTextureUnit() {

  auto textureUnit = _usedTextureUnits;
  if ( textureUnit >= _maxTextures ) {
    console().warn() << "Trying to use " << textureUnit << " texture units while this GPU supports only " << _maxTextures;
  }
  _usedTextureUnits += 1;

  return textureUnit;

}

void GLRenderer::loadUniformsGeneric( Program& program, UniformsList& uniforms, bool warnIfNotFound ) {

  for ( const auto& uniformAndKey: uniforms ) {//( size_t j = 0, jl = uniforms.size(); j < jl; j ++ ) {

    const auto& location = program.uniforms[ uniformAndKey.second ];

    if ( !location.valid() ) {
      if ( warnIfNotFound )
        console().warn() << "three::GLRenderer::loadUniformsGeneric: Expected uniform \""
                         << uniformAndKey.second
                         << "\" location does not exist";
      continue;
    }

    auto& uniform = *uniformAndKey.first;

    uniform.load( _gl, location );

    THREE_REVIEW("EA: What about the rest of the uniforms?")

    if ( uniform.type == THREE::t ) { // single THREE::Texture (2d or cube)

      const auto& texture = uniform.value.cast<Texture*>();
      const auto  textureUnit = getTextureUnit();

      _gl.Uniform1i( location, textureUnit );

      if ( !texture ) continue;

      if ( texture->image.size() == 6 ) {
        setCubeTexture( *texture, textureUnit );
      } else if ( texture->type() == THREE::GLRenderTargetCube ) {
        setCubeTextureDynamic( *texture, textureUnit );
      } else {
        setTexture( *texture, textureUnit );
      }

    } else if ( uniform.type == THREE::tv ) {

      const auto& textures = uniform.value.cast<std::vector<Texture*>>();

      std::vector<int> textureUnits;
      for ( size_t i = 0; i < textures.size(); ++i ) {
        textureUnits.push_back( getTextureUnit() );
      }

      _gl.Uniform1iv( location, (int)textureUnits.size(), textureUnits.data() );

      for ( size_t i = 0; i < textures.size(); ++i ) {

        const auto& texture = textures[ i ];
        const auto textureUnit = textureUnits[ i ];

        if ( !texture ) continue;

        setTexture( *texture, textureUnit );
      }

    }

  }

}

void GLRenderer::setupMatrices( Object3D& object, Camera& camera ) {

  object.glData._modelViewMatrix.multiplyMatrices( camera.matrixWorldInverse, object.matrixWorld );
  object.glData._normalMatrix.getNormalMatrix( object.glData._modelViewMatrix );

}

void GLRenderer::setColorGamma( std::vector<float>& array, size_t offset, const Color& color, float intensitySq ) {

  array[ offset ]     = color.r * color.r * intensitySq;
  array[ offset + 1 ] = color.g * color.g * intensitySq;
  array[ offset + 2 ] = color.b * color.b * intensitySq;

}

void GLRenderer::setColorLinear( std::vector<float>& array, size_t offset, const Color& color, float intensity ) {

  array[ offset ]     = color.r * intensity;
  array[ offset + 1 ] = color.g * intensity;
  array[ offset + 2 ] = color.b * intensity;

}

void GLRenderer::setupLights( Program& program, Lights& lights ) {

  auto& zlights = _lights;

  auto& dirColors     = zlights.directional.colors;
  auto& dirPositions  = zlights.directional.positions;

  auto& pointColors     = zlights.point.colors;
  auto& pointPositions  = zlights.point.positions;
  auto& pointDistances  = zlights.point.distances;

  auto& spotColors     = zlights.spot.colors;
  auto& spotPositions  = zlights.spot.positions;
  auto& spotDistances  = zlights.spot.distances;
  auto& spotDirections = zlights.spot.directions;
  auto& spotAnglesCos  = zlights.spot.anglesCos;
  auto& spotExponents  = zlights.spot.exponents;

  auto& hemiSkyColors    = zlights.hemi.skyColors;
  auto& hemiGroundColors = zlights.hemi.groundColors;
  auto& hemiPositions    = zlights.hemi.positions;

  int dirLength = 0, pointLength = 0, spotLength = 0, hemiLength = 0;
  int dirOffset = 0, pointOffset = 0, spotOffset = 0, hemiOffset = 0;
  int dirCount = 0, pointCount = 0, spotCount = 0, hemiCount = 0;

  float r  = 0, g = 0, b = 0;

  for ( size_t l = 0, ll = lights.size(); l < ll; l ++ ) {

    auto& light = *lights[ l ];

    if ( light.onlyShadow ) continue;

    const auto& color    = light.color;
    const auto intensity = light.intensity;
    const auto distance  = light.distance;

    if ( light.type() == THREE::AmbientLight ) {

      if ( ! light.visible ) continue;

      if ( gammaInput ) {

        r += color.r * color.r;
        g += color.g * color.g;
        b += color.b * color.b;

      } else {

        r += color.r;
        g += color.g;
        b += color.b;

      }

    } else if ( light.type() == THREE::DirectionalLight ) {

      dirCount += 1;

      if ( ! light.visible ) continue;

      _direction.setFromMatrixPosition( light.matrixWorld );
      _vector3.setFromMatrixPosition( light.target->matrixWorld );
      _direction.sub( _vector3 );
      _direction.normalize();

      // skip lights with undefined direction
      // these create troubles in OpenGL (making pixel black)

      if(_direction.x == 0 && _direction.y == 0 && _direction.z == 0 ) continue;

      dirOffset = dirLength * 3;

      grow( dirColors, dirOffset + 3 );
      grow( dirPositions, dirOffset + 3 );

      dirPositions[ dirOffset ]     = _direction.x;
      dirPositions[ dirOffset + 1 ] = _direction.y;
      dirPositions[ dirOffset + 2 ] = _direction.z;

      if ( gammaInput ) {

        setColorGamma( dirColors, dirOffset, color, intensity * intensity );

      } else {

        setColorLinear( dirColors, dirOffset, color, intensity );

      }

      dirLength += 1;

    } else if ( light.type() == THREE::PointLight ) {

      pointCount += 1;

      if ( ! light.visible ) continue;

      pointOffset = pointLength * 3;

      grow( pointColors, pointOffset + 3 );
      grow( pointPositions, pointOffset + 3 );
      grow( pointDistances, pointLength + 1 );

      if ( gammaInput ) {

        setColorGamma( pointColors, pointOffset, color, intensity * intensity );

      } else {

        setColorLinear( pointColors, pointOffset, color, intensity );

      }

      _vector3.setFromMatrixPosition( light.matrixWorld );

      pointPositions[ pointOffset ]     = _vector3.x;
      pointPositions[ pointOffset + 1 ] = _vector3.y;
      pointPositions[ pointOffset + 2 ] = _vector3.z;

      pointDistances[ pointLength ] = distance;

      pointLength += 1;

    } else if ( light.type() == THREE::SpotLight ) {

      spotCount += 1;

      if ( ! light.visible ) continue;

      auto& spotLight = static_cast<SpotLight&>( light );

      spotOffset = spotLength * 3;

      grow( spotColors, spotOffset + 3 );
      grow( spotPositions, spotOffset + 3 );
      grow( spotDistances, spotLength + 1 );

      if ( gammaInput ) {

        setColorGamma( spotColors, spotOffset, color, intensity * intensity );

      } else {

        setColorLinear( spotColors, spotOffset, color, intensity );

      }

      _vector3.setFromMatrixPosition( spotLight.matrixWorld );

      spotPositions[ spotOffset ]     = _vector3.x;
      spotPositions[ spotOffset + 1 ] = _vector3.y;
      spotPositions[ spotOffset + 2 ] = _vector3.z;

      spotDistances[ spotLength ] = distance;

      _direction.copy( _vector3 );
      _vector3.setFromMatrixPosition( spotLight.target->matrixWorld );
      _direction.sub( _vector3 );
      _direction.normalize();

      spotDirections[ spotOffset ]     = _direction.x;
      spotDirections[ spotOffset + 1 ] = _direction.y;
      spotDirections[ spotOffset + 2 ] = _direction.z;

      spotAnglesCos[ spotLength ] = Math::cos( spotLight.angle );
      spotExponents[ spotLength ] = spotLight.exponent;

      spotLength += 1;

    } else if ( light.type() == THREE::HemisphereLight ) {

      hemiCount += 1;

      if ( ! light.visible ) continue;

      auto& hemiLight = static_cast<HemisphereLight&>( light );

      const auto& skyColor = hemiLight.color;
      const auto& groundColor = hemiLight.groundColor;

      hemiOffset = hemiLength * 3;

      grow( hemiPositions, hemiOffset + 3 );
      grow( hemiGroundColors, hemiOffset + 3 );
      grow( hemiSkyColors, hemiOffset + 3 );

      hemiPositions[ hemiOffset ]     = _direction.x;
      hemiPositions[ hemiOffset + 1 ] = _direction.y;
      hemiPositions[ hemiOffset + 2 ] = _direction.z;

      if ( gammaInput ) {

        auto intensitySq = intensity * intensity;

        setColorGamma( hemiSkyColors, hemiOffset, skyColor, intensitySq );
        setColorGamma( hemiGroundColors, hemiOffset, groundColor, intensitySq );

      } else {

        setColorLinear( hemiSkyColors, hemiOffset, skyColor, intensity );
        setColorLinear( hemiGroundColors, hemiOffset, groundColor, intensity );

      }

      hemiLength += 1;

    }

  }

  // 0 eventual remains from removed lights
  // (this is to avoid if in shader)

  for ( size_t l = dirLength * 3, ll = dirColors.size(); l < ll; l ++ ) dirColors[ l ] = 0.0;
  for ( size_t l = pointLength * 3, ll = pointColors.size(); l < ll; l ++ ) pointColors[ l ] = 0.0;
  for ( size_t l = spotLength * 3, ll = spotColors.size(); l < ll; l ++ ) spotColors[ l ] = 0.0;
  for ( size_t l = hemiLength * 3, ll = hemiSkyColors.size(); l < ll; l ++ ) hemiSkyColors[ l ] = 0.0;
  for ( size_t l = hemiLength * 3, ll = hemiGroundColors.size(); l < ll; l ++ ) hemiGroundColors[ l ] = 0.0;

  zlights.directional.length = dirLength;
  zlights.point.length = pointLength;
  zlights.spot.length = spotLength;
  zlights.hemi.length = hemiLength;

  grow( zlights.ambient, 3 );
  zlights.ambient[ 0 ] = r;
  zlights.ambient[ 1 ] = g;
  zlights.ambient[ 2 ] = b;

}


// GL state setting

void GLRenderer::setFaceCulling( THREE::CullFace cullFace, THREE::FrontFaceDirection frontFaceDirection ) {

  if ( cullFace == THREE::CullFaceNone ) {

    _gl.Disable( GL_CULL_FACE );

  } else {

    if ( frontFaceDirection == THREE::FrontFaceDirectionCW ) {
      _gl.FrontFace( GL_CW );
    } else {
      _gl.FrontFace( GL_CCW );
    }

    if ( cullFace == THREE::CullFaceBack ) {
      _gl.CullFace( GL_BACK );
    } else if ( cullFace == THREE::CullFaceFront ) {
      _gl.CullFace( GL_FRONT );
    } else {
      _gl.CullFace( GL_FRONT_AND_BACK );
    }

    _gl.Enable( GL_CULL_FACE );

  }

}

void GLRenderer::setMaterialFaces( Material& material ) {

  auto doubleSided = toInt( material.side == THREE::DoubleSide );
  auto flipSided = toInt( material.side == THREE::BackSide );

  if ( _oldDoubleSided != doubleSided ) {

    if ( doubleSided ) {
      _gl.Disable( GL_CULL_FACE );
    } else {
      _gl.Enable( GL_CULL_FACE );
    }

    _oldDoubleSided = doubleSided;

  }

  if ( _oldFlipSided != flipSided ) {

    if ( flipSided ) {
      _gl.FrontFace( GL_CW );
    } else {
      _gl.FrontFace( GL_CCW );
    }

    _oldFlipSided = flipSided;

  }

}

void GLRenderer::setDepthTest( bool depthTest ) {

  if ( _oldDepthTest != toInt( depthTest ) ) {

    if ( depthTest ) {
      _gl.Enable( GL_DEPTH_TEST );
    } else {
      _gl.Disable( GL_DEPTH_TEST );
    }

    _oldDepthTest = toInt( depthTest );

  }

}

void GLRenderer::setDepthWrite( bool depthWrite ) {

  if ( _oldDepthWrite != toInt( depthWrite ) ) {
    _gl.DepthMask( depthWrite );
    _oldDepthWrite = toInt( depthWrite );
  }

}

void GLRenderer::setLineWidth( float width ) {

  if ( width != _oldLineWidth ) {
    _gl.LineWidth( width );
    _oldLineWidth = width;
  }

}

void GLRenderer::setPolygonOffset( bool polygonoffset, float factor, float units ) {

  if ( _oldPolygonOffset != toInt( polygonoffset ) ) {

    if ( polygonoffset ) {
      _gl.Enable( GL_POLYGON_OFFSET_FILL );
    } else {
      _gl.Disable( GL_POLYGON_OFFSET_FILL );
    }

    _oldPolygonOffset = toInt( polygonoffset );

  }

  if ( polygonoffset && ( _oldPolygonOffsetFactor != factor || _oldPolygonOffsetUnits != units ) ) {
    _gl.PolygonOffset( factor, units );
    _oldPolygonOffsetFactor = factor;
    _oldPolygonOffsetUnits = units;
  }

}

void GLRenderer::setBlending( THREE::Blending blending,
                              THREE::BlendEquation blendEquation /*= THREE::AddEquation*/,
                              THREE::BlendFactor blendSrc /*= THREE::OneFactor*/,
                              THREE::BlendFactor blendDst /*= THREE::OneFactor*/ ) {

  if ( blending != _oldBlending ) {

    if ( blending == THREE::NoBlending ) {

      _gl.Disable( GL_BLEND );

    } else if ( blending == THREE::AdditiveBlending ) {

      _gl.Enable( GL_BLEND );
      _gl.BlendEquation( GL_FUNC_ADD );
      _gl.BlendFunc( GL_SRC_ALPHA, GL_ONE );

    } else if ( blending == THREE::SubtractiveBlending ) {

      // TODO: Find blendFuncSeparate() combination
      _gl.Enable( GL_BLEND );
      _gl.BlendEquation( GL_FUNC_ADD );
      _gl.BlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_COLOR );

    } else if ( blending == THREE::MultiplyBlending ) {

      // TODO: Find blendFuncSeparate() combination
      _gl.Enable( GL_BLEND );
      _gl.BlendEquation( GL_FUNC_ADD );
      _gl.BlendFunc( GL_ZERO, GL_SRC_COLOR );

    } else if ( blending == THREE::CustomBlending ) {

      _gl.Enable( GL_BLEND );

    } else {

      _gl.Enable( GL_BLEND );
      _gl.BlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
      _gl.BlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

    }

    _oldBlending = blending;

  }

  if ( blending == THREE::CustomBlending ) {

    if ( blendEquation != _oldBlendEquation ) {
      _gl.BlendEquation( paramThreeToGL( blendEquation ) );
      _oldBlendEquation = blendEquation;
    }

    if ( blendSrc != _oldBlendSrc || blendDst != _oldBlendDst ) {
      _gl.BlendFunc( paramThreeToGL( blendSrc ), paramThreeToGL( blendDst ) );
      _oldBlendSrc = blendSrc;
      _oldBlendDst = blendDst;
    }

  } else {

    _oldBlendEquation = -1;
    _oldBlendSrc = -1;
    _oldBlendDst = -1;

  }

}

void GLRenderer::resetStates() {
  _currentProgram = 0;
  _currentCamera = 0;

  _oldBlending = -1;
  _oldDepthTest = -1;
  _oldDepthWrite = -1;
  _oldDoubleSided = -1;
  _oldFlipSided = -1;
  _currentGeometryGroupHash = -1;
  _currentMaterialId = -1;

  _lightsNeedUpdate = true;
}

// Defines
std::string GLRenderer::generateDefines( const Material::Defines& defines ) const {
  std::stringstream chunks;

  for( auto& d : defines ) {
    chunks << "#define " << d.first << " " << d.second << std::endl;
  }

  return chunks.str();
}

// Shaders

Program::Ptr GLRenderer::buildProgram( const std::string& shaderID,
                                       const std::string& fragmentShader,
                                       const std::string& vertexShader,
                                       const Uniforms& uniforms,
                                       const Attributes& attributes,
                                       const Material::Defines& defines,
                                       ProgramParameters& parameters/*,
                                       const std::string& index0AttributeName*/) {


  // Generate code
  std::hash<std::string> hash;
  std::stringstream chunks;

  if ( !shaderID.empty() ) {
    chunks << shaderID;
  } else {
    chunks << hash( fragmentShader );
    chunks << hash( vertexShader );
  }

  chunks << jenkins_hash( &defines );

  chunks << jenkins_hash( &parameters );

  auto code = chunks.str();

  // Check if code has been already compiled

  for ( auto& programInfo : _programs ) {
    if ( programInfo.code == code ) {
      console().log( "Code already compiled." /*: \n\n" + code*/ );
      programInfo.usedTimes ++;
      return programInfo.program;
    }
  }

  auto shadowMapTypeDefine = "SHADOWMAP_TYPE_BASIC";

  if ( parameters.shadowMapType == THREE::PCFShadowMap ) {

      shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF";

  } else if ( parameters.shadowMapType == THREE::PCFSoftShadowMap ) {

      shadowMapTypeDefine = "SHADOWMAP_TYPE_PCF_SOFT";

  }

  //console().log( "building new program " );

  auto customDefines = generateDefines( defines );

  auto glProgram = GL_CALL( _gl.CreateProgram() );

  auto prefix_vertex = [this, &parameters, &customDefines, &shadowMapTypeDefine]() -> std::string {
    std::stringstream ss;

#if defined(THREE_GLES)
    ss << "precision " << _precision << " float;" << std::endl;
    ss << "precision " << _precision << " int;" << std::endl;
#endif

    ss << customDefines;

    if ( _supportsVertexTextures ) ss << "#define VERTEX_TEXTURES" << std::endl;

    if ( gammaInput )             ss << "#define GAMMA_INPUT" << std::endl;
    if ( gammaOutput )            ss << "#define GAMMA_OUTPUT" << std::endl;
    THREE_REVIEW("EA: Obsolete?")
    //if ( physicallyBasedShading ) ss << "#define PHYSICALLY_BASED_SHADING" << std::endl;

    ss << "#define MAX_DIR_LIGHTS "   << parameters.maxDirLights << std::endl <<
    "#define MAX_POINT_LIGHTS " << parameters.maxPointLights << std::endl <<
    "#define MAX_SPOT_LIGHTS "  << parameters.maxSpotLights << std::endl <<
    "#define MAX_HEMI_LIGHTS "  << parameters.maxHemiLights << std::endl <<

    "#define MAX_SHADOWS "      << parameters.maxShadows << std::endl <<

    "#define MAX_BONES "        << parameters.maxBones << std::endl;

    if ( parameters.map )          ss << "#define USE_MAP" << std::endl;
    if ( parameters.envMap )       ss << "#define USE_ENVMAP" << std::endl;
    if ( parameters.lightMap )     ss << "#define USE_LIGHTMAP" << std::endl;
    if ( parameters.bumpMap )      ss << "#define USE_BUMPMAP" << std::endl;
    if ( parameters.normalMap )    ss << "#define USE_NORMALMAP" << std::endl;
    if ( parameters.specularMap )  ss << "#define USE_SPECULARMAP" << std::endl;
    if ( parameters.vertexColors ) ss << "#define USE_COLOR" << std::endl;

    if ( parameters.skinning )          ss << "#define USE_SKINNING" << std::endl;
    if ( parameters.useVertexTexture )  ss << "#define BONE_TEXTURE" << std::endl;
    THREE_REVIEW("EA: Obsolete?")
    //if ( parameters.boneTextureWidth )  ss << "#define N_BONE_PIXEL_X " << parameters.boneTextureWidth << std::endl;
    //if ( parameters.boneTextureHeight ) ss << "#define N_BONE_PIXEL_Y " << parameters.boneTextureHeight << std::endl;

    if ( parameters.morphTargets ) ss << "#define USE_MORPHTARGETS" << std::endl;
    if ( parameters.morphNormals ) ss << "#define USE_MORPHNORMALS" << std::endl;
    if ( parameters.perPixel )     ss << "#define PHONG_PER_PIXEL" << std::endl;
    if ( parameters.wrapAround )   ss << "#define WRAP_AROUND" << std::endl;
    if ( parameters.doubleSided )  ss << "#define DOUBLE_SIDED" << std::endl;
    if ( parameters.flipSided )    ss << "#define FLIP_SIDED" << std::endl;

    if ( parameters.shadowMapEnabled ) ss << "#define USE_SHADOWMAP" << std::endl;
    if ( parameters.shadowMapType )    ss << "#define " << shadowMapTypeDefine << std::endl;
    if ( parameters.shadowMapDebug )   ss << "#define SHADOWMAP_DEBUG" << std::endl;
    if ( parameters.shadowMapCascade ) ss << "#define SHADOWMAP_CASCADE" << std::endl;

    if ( parameters.sizeAttenuation ) ss << "#define USE_SIZEATTENUATION" << std::endl;

    ss <<

    "uniform mat4 modelMatrix;" << std::endl <<
    "uniform mat4 modelViewMatrix;" << std::endl <<
    "uniform mat4 projectionMatrix;" << std::endl <<
    "uniform mat4 viewMatrix;" << std::endl <<
    "uniform mat3 normalMatrix;" << std::endl <<
    "uniform vec3 cameraPosition;" << std::endl <<

    "attribute vec3 position;" << std::endl <<
    "attribute vec3 normal;" << std::endl <<
    "attribute vec2 uv;" << std::endl <<
    "attribute vec2 uv2;" << std::endl <<

    "#ifdef USE_COLOR" << std::endl <<

    "attribute vec3 color;" << std::endl <<

    "#endif" << std::endl <<

    "#ifdef USE_MORPHTARGETS" << std::endl <<

    "attribute vec3 morphTarget0;" << std::endl <<
    "attribute vec3 morphTarget1;" << std::endl <<
    "attribute vec3 morphTarget2;" << std::endl <<
    "attribute vec3 morphTarget3;" << std::endl <<

    "#ifdef USE_MORPHNORMALS" << std::endl <<

    "attribute vec3 morphNormal0;" << std::endl <<
    "attribute vec3 morphNormal1;" << std::endl <<
    "attribute vec3 morphNormal2;" << std::endl <<
    "attribute vec3 morphNormal3;" << std::endl <<

    "#else" << std::endl <<

    "attribute vec3 morphTarget4;" << std::endl <<
    "attribute vec3 morphTarget5;" << std::endl <<
    "attribute vec3 morphTarget6;" << std::endl <<
    "attribute vec3 morphTarget7;" << std::endl <<

    "#endif" << std::endl <<

    "#endif" << std::endl <<

    "#ifdef USE_SKINNING" << std::endl <<

    //"attribute vec4 skinVertexA;" << std::endl <<
    //"attribute vec4 skinVertexB;" << std::endl <<
    "attribute vec4 skinIndex;" << std::endl <<
    "attribute vec4 skinWeight;" << std::endl <<

    "#endif" << std::endl;

    return ss.str();

  }();

  auto prefix_fragment = [this, &parameters, &customDefines, &shadowMapTypeDefine]() -> std::string {
    std::stringstream ss;

#if defined(THREE_GLES)
    ss << "precision " << _precision << " float;" << std::endl;
    ss << "precision " << _precision << " int;" << std::endl;
#elif defined(__APPLE__)
    ss << "#version 120" << std::endl;
#else
    ss << "#version 140" << std::endl;
#endif

    ss << customDefines;

    if ( parameters.bumpMap || parameters.normalMap ) ss << "#extension GL_OES_standard_derivatives : enable" << std::endl;

    ss << "#define MAX_DIR_LIGHTS "   << parameters.maxDirLights << std::endl <<
    "#define MAX_POINT_LIGHTS " << parameters.maxPointLights << std::endl <<
    "#define MAX_SPOT_LIGHTS "  << parameters.maxSpotLights << std::endl <<
    "#define MAX_HEMI_LIGHTS "  << parameters.maxHemiLights << std::endl <<
    "#define MAX_SHADOWS "      << parameters.maxShadows << std::endl;

    if ( parameters.alphaTest ) ss << "#define ALPHATEST " << parameters.alphaTest << std::endl;

    if ( gammaInput )             ss << "#define GAMMA_INPUT" << std::endl;
    if ( gammaOutput )            ss << "#define GAMMA_OUTPUT" << std::endl;
    if ( physicallyBasedShading ) ss << "#define PHYSICALLY_BASED_SHADING" << std::endl;

    if ( parameters.useFog && parameters.fog != nullptr ) ss << "#define USE_FOG" << std::endl;
    if ( parameters.useFog && parameters.fogExp )        ss << "#define FOG_EXP2" << std::endl;

    if ( parameters.map )          ss << "#define USE_MAP" <<  std::endl;
    if ( parameters.envMap )       ss << "#define USE_ENVMAP" <<  std::endl;
    if ( parameters.lightMap )     ss << "#define USE_LIGHTMAP" <<  std::endl;
    if ( parameters.bumpMap )      ss << "#define USE_BUMPMAP" <<  std::endl;
    if ( parameters.normalMap )    ss << "#define USE_NORMALMAP" <<  std::endl;
    if ( parameters.specularMap )  ss << "#define USE_SPECULARMAP" <<  std::endl;
    if ( parameters.vertexColors ) ss << "#define USE_COLOR" <<  std::endl;

    if ( parameters.metal )       ss << "#define METAL" <<  std::endl;
    if ( parameters.perPixel )    ss << "#define PHONG_PER_PIXEL" <<  std::endl;
    if ( parameters.wrapAround )  ss << "#define WRAP_AROUND" <<  std::endl;
    if ( parameters.doubleSided ) ss << "#define DOUBLE_SIDED" <<  std::endl;
    if ( parameters.flipSided )   ss << "#define FLIP_SIDED" <<  std::endl;

    if ( parameters.shadowMapEnabled ) ss << "#define USE_SHADOWMAP" <<  std::endl;
    if ( parameters.shadowMapType )    ss << "#define " << shadowMapTypeDefine <<  std::endl;
    if ( parameters.shadowMapDebug )   ss << "#define SHADOWMAP_DEBUG" <<  std::endl;
    if ( parameters.shadowMapCascade ) ss << "#define SHADOWMAP_CASCADE" <<  std::endl;

    ss << "uniform mat4 viewMatrix;" << std::endl <<
    "uniform vec3 cameraPosition;" << std::endl;

    return ss.str();

  }();

  auto glVertexShader   = getShader( THREE::ShaderVertex,   prefix_vertex   + vertexShader );
  auto glFragmentShader = getShader( THREE::ShaderFragment, prefix_fragment + fragmentShader );

  GL_CALL( _gl.AttachShader( glProgram, glVertexShader ) );
  GL_CALL( _gl.AttachShader( glProgram, glFragmentShader ) );

  //Force a particular attribute to index 0.
  // because potentially expensive emulation is done by browser if attribute 0 is disabled.
  //And, color, for example is often automatically bound to index 0 so disabling it
  //if ( index0AttributeName.length() ) {
  //  _gl.BindAttribLocation( glProgram, 0, index0AttributeName );
  //}

  GL_CALL( _gl.LinkProgram( glProgram ) );

  if ( GL_TRUE != _gl.GetProgramParameter( glProgram, GL_LINK_STATUS ) ) {
    int loglen;
    char logbuffer[1000];
    _gl.GetProgramInfoLog( glProgram, sizeof( logbuffer ), &loglen, logbuffer );
    console().error( logbuffer );
    //console.error( "Program Info Log: " + _gl.getProgramInfoLog( program ) );
    //console().error() << addLineNumbers( source );
    _gl.DeleteProgram( glProgram );
    glProgram = 0;
  }

  // clean up

  _gl.DeleteShader( glFragmentShader );
  glFragmentShader = 0;
  _gl.DeleteShader( glVertexShader );
  glVertexShader = 0;

  if ( !glProgram ) {

    return Program::Ptr();

  }

  //console().log() << prefix_fragment + fragmentShader;
  //console().log() << prefix_vertex   + vertexShader;

  auto program = Program::create( glProgram, _programs_counter++ );

  {
    // cache uniform locations

    std::array<std::string, 7> identifiersArray = {

      "viewMatrix",
      "modelViewMatrix",
      "projectionMatrix",
      "normalMatrix",
      "modelMatrix",
      "cameraPosition",
      "morphTargetInfluences"

    };

    Identifiers identifiers( identifiersArray.begin(), identifiersArray.end() );

    if ( parameters.useVertexTexture ) {
      identifiers.push_back( "boneTexture" );
      identifiers.push_back( "boneTextureWidth" );
      identifiers.push_back( "boneTextureHeight" );
    } else {
      identifiers.push_back( "boneGlobalMatrices" );
    }

    for ( const auto& u : uniforms ) {
      identifiers.push_back( u.first );
    }

    cacheUniformLocations( *program, identifiers );

  }

  {
    // cache attributes locations

    std::array<std::string, 10> identifiersArray = {

      AttributeKey::position(), AttributeKey::normal(),
      AttributeKey::uv(), AttributeKey::uv2(),
      AttributeKey::tangent(),
      AttributeKey::color(),
      AttributeKey::skinIndex(), AttributeKey::skinWeight(),
      AttributeKey::lineDistance()

    };

    Identifiers identifiers( identifiersArray.begin(), identifiersArray.end() );

    for ( int i = 0; i < parameters.maxMorphTargets; i ++ ) {

      std::stringstream ss;
      ss << "morphTarget" << i;
      identifiers.push_back( ss.str() );

    }

    for ( int i = 0; i < parameters.maxMorphNormals; i ++ ) {

      std::stringstream ss;
      ss << "morphNormal" << i;
      identifiers.push_back( ss.str() );

    }

    for ( const auto& a : attributes ) {
      identifiers.push_back( a.first );
    }

    cacheAttributeLocations( *program, identifiers );

  }

  _programs.push_back( ProgramInfo( program, code, 1 ) );

  _info.memory.programs = ( int )_programs.size();

  return program;

}

// Shader parameters cache

void GLRenderer::cacheUniformLocations( Program& program, const Identifiers& identifiers ) {
  for ( const auto& id : identifiers ) {
    program.uniforms[ id ] = GL_CALL( _gl.GetUniformLocation( program.program, id.c_str() ) );
  }
}

void GLRenderer::cacheAttributeLocations( Program& program, const Identifiers& identifiers ) {
  for ( const auto& id : identifiers ) {
    program.attributes[ id ] = GL_CALL( _gl.GetAttribLocation( program.program, id.c_str() ) );
  }
}

std::string GLRenderer::addLineNumbers( const std::string& string ) {

  std::stringstream ss;
  std::istringstream iss( string );
  std::string line;
  int i = 1;

  while ( std::getline( iss, line ) ) {
    ss << i++ << ": " << line << std::endl;
  }

  return ss.str();

}

Buffer GLRenderer::getShader( THREE::ShaderType type, const std::string& source ) {

  Buffer shader = 0;

  if ( type == THREE::ShaderFragment ) {
    shader = GL_CALL( _gl.CreateShader( GL_FRAGMENT_SHADER ) );
  } else if ( type == THREE::ShaderVertex ) {
    shader = GL_CALL( _gl.CreateShader( GL_VERTEX_SHADER ) );
  }

  const char* source_str = source.c_str();
  GL_CALL( _gl.ShaderSource( shader, 1, &source_str, nullptr ) );
  GL_CALL( _gl.CompileShader( shader ) );

  if ( GL_TRUE != _gl.GetShaderParameter( shader, GL_COMPILE_STATUS ) ) {
    int loglen;
    char logbuffer[1000];
    _gl.GetShaderInfoLog( shader, sizeof( logbuffer ), &loglen, logbuffer );
    console().error( logbuffer );
    console().error() << addLineNumbers( source );
    return 0;
  }

  return shader;

}


// Textures

void GLRenderer::setTexture( Texture& texture, int slot ) {

  if ( texture.needsUpdate() ) {

    if ( ! texture.__glInit ) {

      texture.__glInit = true;
        
      texture.addEventListener( "dispose", std::bind(&GLRenderer::onTextureDispose, this, std::placeholders::_1) );

      texture.__glTexture = _gl.CreateTexture();

      _info.memory.textures ++;

    }

    _gl.ActiveTexture( GL_TEXTURE0 + slot );
    _gl.BindTexture( GL_TEXTURE_2D, texture.__glTexture );

#ifdef TODO_glUnPACK
    _gl.PixelStorei( GL_UNPACK_FLIP_Y_WEBGL, texture.flipY );
    _gl.PixelStorei( GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, texture.premultiplyAlpha );
    _gl.PixelStorei( GL_UNPACK_ALIGNMENT, texture.unpackAlignment );
#endif

    const auto& image = texture.image[0];
    const auto isImagePowerOfTwo = Math::isPowerOfTwo( image.width ) && Math::isPowerOfTwo( image.height );
    const auto glFormat = paramThreeToGL( texture.format );
    const auto glType = paramThreeToGL( texture.dataType );

    setTextureParameters( GL_TEXTURE_2D, texture, isImagePowerOfTwo );

    // TODO(ea): Update setTexture to r65+
    //if ( texture.type() == THREE::DataTexture ) {

    _gl.TexImage2D( GL_TEXTURE_2D, 0, glFormat, image.width, image.height, 0, glFormat, glType, image.data.data() );

    //} else {

    //    _gl.TexImage2D( GL_TEXTURE_2D, 0, glFormat, glFormat, glType, texture.image );

    //}

    if ( texture.generateMipmaps && isImagePowerOfTwo )
      _gl.GenerateMipmap( GL_TEXTURE_2D );

    texture.needsUpdate(false);

    if ( texture.onUpdate ) texture.onUpdate();

  } else {

    _gl.ActiveTexture( GL_TEXTURE0 + slot );
    _gl.BindTexture( GL_TEXTURE_2D, texture.__glTexture );

  }

}

Image& GLRenderer::clampToMaxSize( Image& image, int maxSize ) {

#ifdef TODO_IMAGE_SCALING
  if ( image.width <= maxSize && image.height <= maxSize ) {

    return image;

  }

  // Warning: Scaling through the canvas will only work with images that use
  // premultiplied alpha.

  auto maxDimension = Math::max( image.width, image.height );
  auto newWidth = Math::floor( image.width * maxSize / maxDimension );
  auto newHeight = Math::floor( image.height * maxSize / maxDimension );

  auto canvas = document.createElement( 'canvas' );
  canvas.width = newWidth;
  canvas.height = newHeight;

  auto ctx = canvas.getContext( "2d" );
  ctx.drawImage( image, 0, 0, image.width, image.height, 0, 0, newWidth, newHeight );

  return canvas;

#else

  return image;

#endif

}

void GLRenderer::setCubeTexture( Texture& texture, int slot ) {

  if ( texture.image.size() == 6 ) {

    if ( texture.needsUpdate() ) {

      if ( ! texture.__glTextureCube ) {
          
        texture.addEventListener( "dispose", std::bind(&GLRenderer::onTextureDispose, this, std::placeholders::_1) );

        texture.__glTextureCube = _gl.CreateTexture();

        _info.memory.textures ++;
      }

      _gl.ActiveTexture( GL_TEXTURE0 + slot );
      _gl.BindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTextureCube );

#ifdef TODO_IMAGE_SCALING
      _gl.PixelStorei( GL_UNPACK_FLIP_Y, texture.flipY );

      std::array<Image*, 6> cubeImage = [];
      for ( auto i = 0; i < 6; i ++ ) {

        if ( _autoScaleCubemaps ) {

          cubeImage[ i ] = &clampToMaxSize( texture.image[ i ], _maxCubemapSize );

        } else {

          cubeImage[ i ] = &texture.image[ i ];

        }

      }
#else
      auto& cubeImage = texture.image;
#endif

      const auto& image = cubeImage[ 0 ];
      const auto isImagePowerOfTwo = Math::isPowerOfTwo( image.width ) && Math::isPowerOfTwo( image.height );
      const auto glFormat = paramThreeToGL( texture.format );
      const auto glType = paramThreeToGL( texture.dataType );

      setTextureParameters( GL_TEXTURE_CUBE_MAP, texture, isImagePowerOfTwo );

      for ( auto i = 0; i < 6; i ++ ) {
        //glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, glFormat, glType, cubeImage[ i ] );
        _gl.TexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, cubeImage[ 0 ].width, cubeImage[ 0 ].height, 0, glFormat, glType, cubeImage[ i ].data.data() );
      }

      if ( texture.generateMipmaps && isImagePowerOfTwo ) {
        _gl.GenerateMipmap( GL_TEXTURE_CUBE_MAP );
      }

      texture.needsUpdate(false);

      if ( texture.onUpdate ) texture.onUpdate();

    } else {
      _gl.ActiveTexture( GL_TEXTURE0 + slot );
      _gl.BindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTextureCube );
    }

  }

}

void GLRenderer::setCubeTextureDynamic( Texture& texture, int slot ) {

  _gl.ActiveTexture( GL_TEXTURE0 + slot );
  _gl.BindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTexture );

}


// Render targets

void GLRenderer::setupFrameBuffer( Buffer framebuffer, GLRenderTarget& renderTarget, GLenum textureTarget ) {

  _gl.BindFramebuffer( GL_FRAMEBUFFER, framebuffer );
  _gl.FramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, renderTarget.__glTexture, 0 );

}

void GLRenderer::setupRenderBuffer( Buffer renderbuffer, GLRenderTarget& renderTarget ) {

  _gl.BindRenderbuffer( GL_RENDERBUFFER, renderbuffer );

  if ( renderTarget.depthBuffer && ! renderTarget.stencilBuffer ) {

    _gl.RenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTarget.width, renderTarget.height );
    _gl.FramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );

    /* For some reason this is not working. Defaulting to RGBA4.
    } else if( ! renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

        _gl.RenderbufferStorage( GLrenderBuffer, GL_STENCIL_INDEX8, renderTarget.width, renderTarget.height );
        _gl.FramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GLrenderBuffer, renderbuffer );
    */
  } else if ( renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

#if !defined(THREE_GLES)
    // TODO: Enable with GLES
    _gl.RenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, renderTarget.width, renderTarget.height );
    _gl.FramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );
#endif

  } else {

    _gl.RenderbufferStorage( GL_RENDERBUFFER, GL_RGBA4, renderTarget.width, renderTarget.height );

  }

}

void GLRenderer::setRenderTarget( const GLRenderTarget::Ptr& renderTarget ) {

  auto isCube = false;// TODO: ( renderTarget.type() == THREE::WebglRenderTargetCube );

  if ( renderTarget && renderTarget->__glFramebuffer.size() == 0 ) {
      
    renderTarget->addEventListener( "dispose", std::bind(&GLRenderer::onRenderTargetDispose, this, std::placeholders::_1) );

    renderTarget->__glTexture = _gl.CreateTexture();

    _info.memory.textures ++;

    // Setup texture, create render and frame buffers

    const auto isTargetPowerOfTwo = Math::isPowerOfTwo( renderTarget->width ) && Math::isPowerOfTwo( renderTarget->height );
    const auto glFormat = paramThreeToGL( renderTarget->format );
    const auto glType = paramThreeToGL( renderTarget->dataType );

    if ( isCube ) {

      renderTarget->__glFramebuffer.resize( 6 );
      renderTarget->__glRenderbuffer.resize( 6 );

      _gl.BindTexture( GL_TEXTURE_CUBE_MAP, renderTarget->__glTexture );
      setTextureParameters( GL_TEXTURE_CUBE_MAP, *renderTarget, isTargetPowerOfTwo );

      for ( auto i = 0; i < 6; i ++ ) {

        renderTarget->__glFramebuffer[ i ] = _gl.CreateFramebuffer();
        renderTarget->__glRenderbuffer[ i ] = _gl.CreateRenderbuffer();

        _gl.TexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, renderTarget->width, renderTarget->height, 0, glFormat, glType, 0 );

        setupFrameBuffer( renderTarget->__glFramebuffer[ i ], *renderTarget, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
        setupRenderBuffer( renderTarget->__glRenderbuffer[ i ], *renderTarget );

      }

      if ( isTargetPowerOfTwo ) _gl.GenerateMipmap( GL_TEXTURE_CUBE_MAP );

    } else {

      renderTarget->__glFramebuffer.resize( 1 );
      renderTarget->__glRenderbuffer.resize( 1 );

      renderTarget->__glFramebuffer[ 0 ] = _gl.CreateFramebuffer();

      if(renderTarget->shareDepthFrom) {
        renderTarget->__glRenderbuffer = renderTarget->shareDepthFrom->__glRenderbuffer;
      } else {
        renderTarget->__glRenderbuffer[ 0 ] = _gl.CreateRenderbuffer();
      }

      _gl.BindTexture( GL_TEXTURE_2D, renderTarget->__glTexture );
      setTextureParameters( GL_TEXTURE_2D, *renderTarget, isTargetPowerOfTwo );

      _gl.TexImage2D( GL_TEXTURE_2D, 0, glFormat, renderTarget->width, renderTarget->height, 0, glFormat, glType, 0 );

      setupFrameBuffer( renderTarget->__glFramebuffer[ 0 ], *renderTarget, GL_TEXTURE_2D );


      if ( renderTarget->shareDepthFrom ) {

        if ( renderTarget->depthBuffer && ! renderTarget->stencilBuffer ) {

          _gl.FramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderTarget->__glRenderbuffer[0] );

        } else if ( renderTarget->depthBuffer && renderTarget->stencilBuffer ) {

          _gl.FramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderTarget->__glRenderbuffer[0] );

        }

      } else {
        setupRenderBuffer( renderTarget->__glRenderbuffer[ 0 ], *renderTarget );
      }

      if ( isTargetPowerOfTwo ) _gl.GenerateMipmap( GL_TEXTURE_2D );

    }

    // Release everything

    if ( isCube ) {
      _gl.BindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    } else {
      _gl.BindTexture( GL_TEXTURE_2D, 0 );
    }

    _gl.BindRenderbuffer( GL_RENDERBUFFER, 0 );
    _gl.BindFramebuffer( GL_FRAMEBUFFER, 0 );

  }

  Buffer framebuffer = 0;
  int width = 0, height = 0, vx = 0, vy = 0;

  if ( renderTarget ) {

    if ( isCube ) {
      framebuffer = renderTarget->__glFramebuffer[ renderTarget->activeCubeFace ];
    } else {
      framebuffer = renderTarget->__glFramebuffer[ 0 ];
    }

    width = renderTarget->width;
    height = renderTarget->height;

    vx = 0;
    vy = 0;

  } else {

    framebuffer = 0;

    width = _viewportWidth;
    height = _viewportHeight;

    vx = _viewportX;
    vy = _viewportY;

  }

  if ( framebuffer != _currentFramebuffer ) {

    _gl.BindFramebuffer( GL_FRAMEBUFFER, framebuffer );
    _gl.Viewport( vx, vy, width, height );

    _currentFramebuffer = framebuffer;

  }

  _currentWidth = width;
  _currentHeight = height;

}

void GLRenderer::updateRenderTargetMipmap( GLRenderTarget& renderTarget ) {

  if ( renderTarget.type() == THREE::GLRenderTargetCube ) {

    _gl.BindTexture( GL_TEXTURE_CUBE_MAP, renderTarget.__glTexture );
    _gl.GenerateMipmap( GL_TEXTURE_CUBE_MAP );
    _gl.BindTexture( GL_TEXTURE_CUBE_MAP, 0 );

  } else {

    _gl.BindTexture( GL_TEXTURE_2D, renderTarget.__glTexture );
    _gl.GenerateMipmap( GL_TEXTURE_2D );
    _gl.BindTexture( GL_TEXTURE_2D, 0 );

  }

}

// Fallback filters for non-power-of-2 textures

int GLRenderer::filterFallback( int f ) {
  if ( f == THREE::NearestFilter || f == THREE::NearestMipMapNearestFilter || f == THREE::NearestMipMapLinearFilter ) {
    return GL_NEAREST;
  }
  return GL_LINEAR;
}

// Map THREE::cpp constants to WebGL constants

int GLRenderer::paramThreeToGL( int p ) {

  switch ( p ) {

  case THREE::RepeatWrapping:
    return GL_REPEAT;
  case THREE::ClampToEdgeWrapping:
    return GL_CLAMP_TO_EDGE;
  case THREE::MirroredRepeatWrapping:
    return GL_MIRRORED_REPEAT;

  case THREE::NearestFilter:
    return GL_NEAREST;
  case THREE::NearestMipMapNearestFilter:
    return GL_NEAREST_MIPMAP_NEAREST;
  case THREE::NearestMipMapLinearFilter:
    return GL_NEAREST_MIPMAP_LINEAR;

  case THREE::LinearFilter:
    return GL_LINEAR;
  case THREE::LinearMipMapNearestFilter:
    return GL_LINEAR_MIPMAP_NEAREST;
  case THREE::LinearMipMapLinearFilter:
    return GL_LINEAR_MIPMAP_LINEAR;

  case THREE::UnsignedByteType:
    return GL_UNSIGNED_BYTE;
  case THREE::UnsignedShort4444Type:
    return GL_UNSIGNED_SHORT_4_4_4_4;
  case THREE::UnsignedShort5551Type:
    return GL_UNSIGNED_SHORT_5_5_5_1;
  case THREE::UnsignedShort565Type:
    return GL_UNSIGNED_SHORT_5_6_5;

  case THREE::ByteType:
    return GL_BYTE;
  case THREE::ShortType:
    return GL_SHORT;
  case THREE::UnsignedShortType:
    return GL_UNSIGNED_SHORT;
  case THREE::IntType:
    return GL_INT;
  case THREE::UnsignedIntType:
    return GL_UNSIGNED_INT;
  case THREE::FloatType:
    return GL_FLOAT;

  case THREE::AlphaFormat:
    return GL_ALPHA;
  case THREE::RGBFormat:
    return GL_RGB;
  case THREE::RGBAFormat:
    return GL_RGBA;
#if !defined(THREE_GLES)
  case THREE::BGRFormat:
    return GL_BGR;
  case THREE::BGRAFormat:
    return GL_BGRA;
#endif

  case THREE::AddEquation:
    return GL_FUNC_ADD;
  case THREE::SubtractEquation:
    return GL_FUNC_SUBTRACT;
  case THREE::ReverseSubtractEquation:
    return GL_FUNC_REVERSE_SUBTRACT;

  case THREE::ZeroFactor:
    return GL_ZERO;
  case THREE::OneFactor:
    return GL_ONE;
  case THREE::SrcColorFactor:
    return GL_SRC_COLOR;
  case THREE::OneMinusSrcColorFactor:
    return GL_ONE_MINUS_SRC_COLOR;
  case THREE::SrcAlphaFactor:
    return GL_SRC_ALPHA;
  case THREE::OneMinusSrcAlphaFactor:
    return GL_ONE_MINUS_SRC_ALPHA;
  case THREE::DstAlphaFactor:
    return GL_DST_ALPHA;
  case THREE::OneMinusDstAlphaFactor:
    return GL_ONE_MINUS_DST_ALPHA;

  case THREE::DstColorFactor:
    return GL_DST_COLOR;
  case THREE::OneMinusDstColorFactor:
    return GL_ONE_MINUS_DST_COLOR;
  case THREE::SrcAlphaSaturateFactor:
    return GL_SRC_ALPHA_SATURATE;

  /*
    TODO S3TC/Compressed textures
  */

  default:
    return 0;

  }
}

// Allocations

int GLRenderer::allocateBones( Object3D& object ) {

  if ( _supportsBoneTextures && object.useVertexTexture ) {

    return 1024;

  } else {

    // default for when object is not specified
    // ( for example when prebuilding shader
    //   to be used with multiple objects )
    //
    //  - leave some extra space for other uniforms
    //  - limit here is ANGLE's 254 max uniform vectors
    //    (up to 54 should be safe)

    auto nVertexUniforms = 254;//glGetParameteri( GL_MAX_VERTEX_UNIFORM_VECTORS );
    auto nVertexMatrices = ( int )Math::floor( ( ( float )nVertexUniforms - 20 ) / 4 );

    auto maxBones = nVertexMatrices;

    if ( object.type() == THREE::SkinnedMesh ) {

      maxBones = Math::min( ( int )object.bones.size(), maxBones );

      if ( maxBones < ( int )object.bones.size() ) {
        console().warn() << "WebGLRenderer: too many bones - "
                         << object.bones.size() <<
                         ", this GPU supports just " << maxBones << " (try OpenGL instead of ANGLE)";
      }

    }

    return maxBones;

  }

}

GLRenderer::LightCount GLRenderer::allocateLights( Lights& lights ) {

  int dirLights = 0,
      pointLights  = 0,
      spotLights = 0,
      hemiLights = 0;

  for ( const auto& light : lights ) {
    if ( light->onlyShadow ) continue;

    if ( light->type() == THREE::DirectionalLight ) dirLights ++;
    if ( light->type() == THREE::PointLight ) pointLights ++;
    if ( light->type() == THREE::SpotLight ) spotLights ++;
    if ( light->type() == THREE::SpotLight ) hemiLights ++;
  }

  LightCount lightCount;
  lightCount.directional = dirLights;
  lightCount.point = pointLights;
  lightCount.spot = spotLights;
  lightCount.hemi = hemiLights;

  return lightCount;

}

int GLRenderer::allocateShadows( Lights& lights ) {

  int maxShadows = 0;

  for ( const auto& light : lights ) {
    if ( ! light->castShadow ) continue;

    if ( light->type() == THREE::SpotLight ) maxShadows ++;
    if ( light->type() == THREE::DirectionalLight && ! light->shadowCascade ) maxShadows ++;
  }

  return maxShadows;

}

} // namespace three
