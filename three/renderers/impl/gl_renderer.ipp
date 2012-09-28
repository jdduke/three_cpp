#ifndef THREE_GL_RENDERER_IPP
#define THREE_GL_RENDERER_IPP

#include <three/renderers/gl_renderer.hpp>

#include <three/common.hpp>

#include <three/gl.hpp>

#include <three/cameras/camera.hpp>

#include <three/core/frustum.hpp>
#include <three/core/interfaces.hpp>
#include <three/core/geometry.hpp>
#include <three/core/geometry_group.hpp>

#include <three/lights/spot_light.hpp>
#include <three/lights/hemisphere_light.hpp>

#include <three/materials/program.hpp>

#include <three/renderers/gl_render_target.hpp>
#include <three/renderers/gl_shaders.hpp>
#include <three/renderers/renderables/renderable_object.hpp>

#include <three/scenes/scene.hpp>
#include <three/scenes/fog.hpp>
#include <three/scenes/fog_exp2.hpp>

#include <three/textures/texture.hpp>

namespace three {

struct ProgramParameters {
  bool map, envMap, lightMap, bumpMap, specularMap;
  THREE::Colors vertexColors;
  IFog* fog;
  bool useFog;
  bool sizeAttenuation;
  bool skinning;
  int maxBones;
  bool useVertexTexture;
  int boneTextureWidth;
  int boneTextureHeight;
  bool morphTargets;
  bool morphNormals;
  int maxMorphTargets;
  int maxMorphNormals;
  int maxDirLights;
  int maxPointLights;
  int maxSpotLights;
  int maxShadows;
  bool shadowMapEnabled;
  bool shadowMapSoft;
  bool shadowMapDebug;
  bool shadowMapCascade;

  float alphaTest;
  bool metal;
  bool perPixel;
  bool wrapAround;
  bool doubleSided;
};

GLRenderer::Ptr GLRenderer::create( const GLRenderer::Parameters& parameters /*= Parameters()*/ ) {
  auto renderer = make_shared<GLRenderer>( parameters );
  renderer->initialize();
  return renderer;
}

GLRenderer::GLRenderer( const Parameters& parameters )
  : _width( parameters.width ),
    _height( parameters.height ),
    _vsync ( parameters.vsync ),
    _precision( parameters.precision ),
    _alpha( parameters.alpha ),
    _premultipliedAlpha( parameters.premultipliedAlpha ),
    _antialias( parameters.antialias ),
    _stencil( parameters.stencil ),
    _preserveDrawingBuffer( parameters.preserveDrawingBuffer ),
    _clearColor( parameters.clearColor ),
    _clearAlpha( parameters.clearAlpha ),
    _maxLights( parameters.maxLights ),
    context( nullptr ),
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
    shadowMapSoft( true ),
    shadowMapCullFrontFaces( true ),
    shadowMapDebug( false ),
    shadowMapCascade( false ),
    maxMorphTargets( 8 ),
    maxMorphNormals( 4 ),
    autoScaleCubemaps( true ),
    _programs_counter( 0 ),
    _currentProgram( 0 ),
    _currentFramebuffer( 0 ),
    _currentMaterialId( -1 ),
    _currentGeometryGroupHash( -1 ),
    _currentCamera( nullptr ),
    _geometryGroupCounter( 0 ),
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
    _lightsNeedUpdate( true ) {
  console().log() << "THREE::GLRenderer created";
}

/*
// default plugins (order is important)

shadowMapPlugin = new THREE::ShadowMapPlugin();
addPrePlugin( shadowMapPlugin );

addPostPlugin( new THREE::SpritePlugin() );
addPostPlugin( new THREE::LensFlarePlugin() );
*/

void GLRenderer::initialize() {

  console().log() << "THREE::GLRenderer initializing";

  initGL();

  setDefaultGLState();

  //context = _gl;

  // GPU capabilities

  _maxVertexTextures = glGetParameteri( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ),
  _maxTextureSize    = glGetParameteri( GL_MAX_TEXTURE_SIZE ),
  _maxCubemapSize    = glGetParameteri( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
  _maxAnisotropy = _glExtensionTextureFilterAnisotropic ? glGetTexParameterf( TEXTURE_MAX_ANISOTROPY_EXT ) : 0.f;
  _supportsVertexTextures = ( _maxVertexTextures > 0 );
  _supportsBoneTextures = _supportsVertexTextures && _glExtensionTextureFloat;

  console().log() << "THREE::GLRenderer initialized";

}

void GLRenderer::initGL() {

  // TODO: Force client to initialize opengl
  if ( !_vsync )
    glEnableVSync( false );

  /*
  if ( glload::LoadFunctions() == glload::LS_LOAD_FAILED ) {
      console().error( "Error loading OpenGL functions" );
  }*/

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

}

void GLRenderer::setDefaultGLState() {

  glClearColor( 0, 0, 0, 1 );
  glClearDepth( 1 );
  glClearStencil( 0 );

  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );

  glFrontFace( GL_CCW );
  glCullFace( GL_BACK );
  glEnable( GL_CULL_FACE );

  glEnable( GL_BLEND );
  glBlendEquation( GL_FUNC_ADD );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );

}

/////////////////////////////////////////////////////////////////////////

void GLRenderer::setSize( int width, int height ) {
  // TODO: Implement
  _width = width;
  _height = height;
  setViewport( 0, 0, _width, _height );
}

void GLRenderer::setViewport( int x /*= 0*/, int y /*= 0*/, int width /*= -1*/, int height /*= -1*/ ) {
  _viewportX = x;
  _viewportY = y;

  _viewportWidth  = width  != -1 ? width  : _width;
  _viewportHeight = height != -1 ? height : _height;

  glViewport( _viewportX, _viewportY, _viewportWidth, _viewportHeight );
}

void GLRenderer::setScissor( int x, int y, int width, int height ) {
  glScissor( x, y, width, height );
}

void GLRenderer::enableScissorTest( bool enable ) {
  enable ? glEnable( GL_SCISSOR_TEST ) : glDisable( GL_SCISSOR_TEST );
}

// Clearing

void GLRenderer::setClearColorHex( int hex, float alpha ) {
  _clearColor.setHex( hex );
  _clearAlpha = alpha;

  glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );
}

void GLRenderer::setClearColor( Color color, float alpha ) {
  _clearColor.copy( color );
  _clearAlpha = alpha;

  glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );
}

void GLRenderer::clear( bool color /*= true*/, bool depth /*= true*/, bool stencil /*= true*/ ) {
  int bits = 0;

  if ( color )   bits |= GL_COLOR_BUFFER_BIT;
  if ( depth )   bits |= GL_DEPTH_BUFFER_BIT;
  if ( stencil ) bits |= GL_STENCIL_BUFFER_BIT;

  glClear( bits );
}

void GLRenderer::clearTarget( GLRenderTarget::Ptr renderTarget, bool color /*= true*/, bool depth /*= true*/, bool stencil /*= true*/ ) {

  setRenderTarget( renderTarget );
  clear( color, depth, stencil );

}

// Plugins

void GLRenderer::addPostPlugin( IPlugin::Ptr plugin ) {

  plugin->init( *this );
  renderPluginsPost.push_back( plugin );

}

void GLRenderer::addPrePlugin( IPlugin::Ptr plugin ) {

  plugin->init( *this );
  renderPluginsPre.push_back( plugin );

}

// Deallocation

void GLRenderer::deallocateObject( Object3D::Ptr object ) {

  if ( !object || ! object->glData.__glInit ) return;

  object->glData.clear();

  if ( !object->geometry ) {
    console().warn( "Object3D contains no geometry" );
    return;
  }

  auto& geometry = *object->geometry;

  if ( object->type() == THREE::Mesh ) {
    for ( auto& geometryGroup : geometry.geometryGroups ) {
      deleteMeshBuffers( geometryGroup.second );
    }
  } else if ( object->type() == THREE::Ribbon ) {
    deleteRibbonBuffers( geometry );
  } else if ( object->type() == THREE::Line ) {
    deleteLineBuffers( geometry );
  } else if ( object->type() == THREE::ParticleSystem ) {
    deleteParticleBuffers( geometry );
  }

}

void GLRenderer::deallocateTexture( Texture::Ptr texture ) {

  if ( !texture || ! texture->__glInit ) return;

  texture->__glInit = false;
  glDeleteTexture( texture->__glTexture );

  _info.memory.textures --;

}


void GLRenderer::deallocateRenderTarget( GLRenderTarget::Ptr renderTarget ) {

  if ( !renderTarget || ! renderTarget->__glTexture ) return;

  glDeleteTexture( renderTarget->__glTexture );

  for ( auto& frameBuffer : renderTarget->__glFramebuffer ) {
    glDeleteFramebuffer( frameBuffer );
  }

  renderTarget->__glFramebuffer.clear();

  for ( auto& renderBuffer : renderTarget->__glRenderbuffer ) {
    glDeleteRenderbuffer( renderBuffer );
  }

  renderTarget->__glRenderbuffer.clear();

}


void GLRenderer::deallocateMaterial( Material& material ) {

  auto program = material.program;

  if ( ! program ) return;


  // only deallocate GL program if this was the last use of shared program
  // assumed there is only single copy of any program in the _programs list
  // (that's how it's constructed)

  auto deleteProgram = false;

  for ( auto& programInfo : _programs ) {

    if ( programInfo.program == program ) {
      programInfo.usedTimes--;
      if ( programInfo.usedTimes == 0 ) {
        deleteProgram = true;
      }

      break;
    }

  }

  if ( deleteProgram ) {

    decltype( _programs ) newPrograms;

    for ( auto& programInfo : _programs ) {
      if ( programInfo.program != program ) {
        newPrograms.push_back( programInfo );
      }
    }

    _programs = std::move( newPrograms );
    glDeleteProgram( program->program );
    _info.memory.programs --;

  }

}

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

  // TODO:
  //shadowMapPlugin.update( scene, camera );

}

// Internal functions

// Buffer allocation

void GLRenderer::createParticleBuffers( Geometry& geometry ) {

  geometry.__glVertexBuffer = glCreateBuffer();
  geometry.__glColorBuffer = glCreateBuffer();

  _info.memory.geometries ++;

}

void GLRenderer::createLineBuffers( Geometry& geometry ) {

  geometry.__glVertexBuffer = glCreateBuffer();
  geometry.__glColorBuffer  = glCreateBuffer();

  _info.memory.geometries ++;

}

void GLRenderer::createRibbonBuffers( Geometry& geometry ) {

  geometry.__glVertexBuffer = glCreateBuffer();
  geometry.__glColorBuffer  = glCreateBuffer();

  _info.memory.geometries ++;

}

void GLRenderer::createMeshBuffers( GeometryGroup& geometryGroup ) {

  geometryGroup.__glVertexBuffer  = glCreateBuffer();
  geometryGroup.__glNormalBuffer  = glCreateBuffer();
  geometryGroup.__glTangentBuffer = glCreateBuffer();
  geometryGroup.__glColorBuffer   = glCreateBuffer();
  geometryGroup.__glUVBuffer      = glCreateBuffer();
  geometryGroup.__glUV2Buffer     = glCreateBuffer();

  geometryGroup.__glSkinVertexABuffer = glCreateBuffer();
  geometryGroup.__glSkinVertexBBuffer = glCreateBuffer();
  geometryGroup.__glSkinIndicesBuffer = glCreateBuffer();
  geometryGroup.__glSkinWeightsBuffer = glCreateBuffer();

  geometryGroup.__glFaceBuffer = glCreateBuffer();
  geometryGroup.__glLineBuffer = glCreateBuffer();

  if ( geometryGroup.numMorphTargets > 0 ) {

    geometryGroup.__glMorphTargetsBuffers.clear();

    for ( int m = 0, ml = geometryGroup.numMorphTargets; m < ml; m ++ ) {
      geometryGroup.__glMorphTargetsBuffers.push_back( glCreateBuffer() );
    }

  }

  if ( geometryGroup.numMorphNormals > 0 ) {

    geometryGroup.__glMorphNormalsBuffers.clear();

    for ( int m = 0, ml = geometryGroup.numMorphNormals; m < ml; m ++ ) {
      geometryGroup.__glMorphNormalsBuffers.push_back( glCreateBuffer() );
    }

  }

  _info.memory.geometries ++;

}

// Buffer deallocation

void GLRenderer::deleteParticleBuffers( Geometry& geometry ) {

  glDeleteBuffer( geometry.__glVertexBuffer );
  glDeleteBuffer( geometry.__glColorBuffer );

  _info.memory.geometries --;

}

void GLRenderer::deleteLineBuffers( Geometry& geometry ) {

  glDeleteBuffer( geometry.__glVertexBuffer );
  glDeleteBuffer( geometry.__glColorBuffer );

  _info.memory.geometries --;

}

void GLRenderer::deleteRibbonBuffers( Geometry& geometry ) {

  glDeleteBuffer( geometry.__glVertexBuffer );
  glDeleteBuffer( geometry.__glColorBuffer );

  _info.memory.geometries --;

}

void GLRenderer::deleteMeshBuffers( GeometryGroup& geometryGroup ) {

  glDeleteBuffer( geometryGroup.__glVertexBuffer );
  glDeleteBuffer( geometryGroup.__glNormalBuffer );
  glDeleteBuffer( geometryGroup.__glTangentBuffer );
  glDeleteBuffer( geometryGroup.__glColorBuffer );
  glDeleteBuffer( geometryGroup.__glUVBuffer );
  glDeleteBuffer( geometryGroup.__glUV2Buffer );

  glDeleteBuffer( geometryGroup.__glSkinVertexABuffer );
  glDeleteBuffer( geometryGroup.__glSkinVertexBBuffer );
  glDeleteBuffer( geometryGroup.__glSkinIndicesBuffer );
  glDeleteBuffer( geometryGroup.__glSkinWeightsBuffer );

  glDeleteBuffer( geometryGroup.__glFaceBuffer );
  glDeleteBuffer( geometryGroup.__glLineBuffer );

  if ( geometryGroup.numMorphTargets > 0 ) {
    for ( int m = 0, ml = geometryGroup.numMorphTargets; m < ml; m ++ ) {
      glDeleteBuffer( geometryGroup.__glMorphTargetsBuffers[ m ] );
    }
  }

  if ( geometryGroup.numMorphNormals > 0 ) {
    for ( int m = 0, ml = geometryGroup.numMorphNormals; m < ml; m ++ ) {
      glDeleteBuffer( geometryGroup.__glMorphNormalsBuffers[ m ] );
    }
  }

  for ( auto& attribute : geometryGroup.__glCustomAttributesList ) {
    glDeleteBuffer( attribute.buffer );
  }

  _info.memory.geometries --;

}


// Buffer initialization

void GLRenderer::initCustomAttributes( Geometry& geometry, Object3D& object ) {

  const auto nvertices = geometry.vertices.size();

  if ( !object.material ) {
    console().warn( "Object contains no material" );
    return;
  }

  auto& material = *object.material;

  if ( material.customAttributes.size() > 0 ) {

    geometry.__glCustomAttributesList.clear();

    for ( auto& namedAttribute : material.customAttributes ) {

      auto& a = namedAttribute.first;
      auto& attribute = namedAttribute.second;

      if ( !attribute.__glInitialized || attribute.createUniqueBuffers ) {

        attribute.__glInitialized = true;

        auto size = 1;      // "f" and "i"

        if ( attribute.type == THREE::V2 ) size = 2;
        else if ( attribute.type == THREE::V3 ) size = 3;
        else if ( attribute.type == THREE::V4 ) size = 4;
        else if ( attribute.type == THREE::C ) size = 3;

        attribute.size = size;

        attribute.array.resize( nvertices * size );

        attribute.buffer = glCreateBuffer();
        attribute.belongsToAttribute = a;

        attribute.needsUpdate = true;

      }

      geometry.__glCustomAttributesList.push_back( attribute );

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

  geometry.__glLineCount = ( int )nvertices;

  initCustomAttributes( geometry, object );

}

void GLRenderer::initRibbonBuffers( Geometry& geometry ) {

  auto nvertices = ( int )geometry.vertices.size();

  geometry.__vertexArray.resize( nvertices * 3 );
  geometry.__colorArray.resize( nvertices * 3 );

  geometry.__glVertexCount = nvertices;

}

void GLRenderer::initMeshBuffers( GeometryGroup& geometryGroup, Mesh& object ) {

  auto& geometry = *object.geometry;
  auto& faces3 = geometryGroup.faces3;
  auto& faces4 = geometryGroup.faces4;

  auto nvertices = ( int )faces3.size() * 3 + ( int )faces4.size() * 4;
  auto ntris     = ( int )faces3.size() * 1 + ( int )faces4.size() * 2;
  auto nlines    = ( int )faces3.size() * 3 + ( int )faces4.size() * 4;

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

    if ( geometry.faceUvs.size() > 0 || geometry.faceVertexUvs.size() > 0 ) {
      geometryGroup.__uvArray.resize( nvertices * 2 );
    }

    if ( geometry.faceUvs.size() > 1 || geometry.faceVertexUvs.size() > 1 ) {
      geometryGroup.__uv2Array.resize( nvertices * 2 );
    }

  }

  if ( geometry.skinWeights.size() && geometry.skinIndices.size() ) {

    geometryGroup.__skinVertexAArray.resize( nvertices * 4 );
    geometryGroup.__skinVertexBArray.resize( nvertices * 4 );
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

  if ( material && material->customAttributes.size() > 0 ) {

    geometryGroup.__glCustomAttributesList.clear();

    for ( auto& a : material->customAttributes ) {

      // Do a shallow copy of the attribute object soe different geometryGroup chunks use different
      // attribute buffers which are correctly indexed in the setMeshBuffers function

      auto& originalAttribute = a.second;

      CustomAttribute attribute( originalAttribute );

      /*for ( auto& property : originalAttribute ) {

          attribute[ property ] = originalAttribute[ property ];

      }*/

      if ( !attribute.__glInitialized || attribute.createUniqueBuffers ) {

        attribute.__glInitialized = true;

        auto size = 1;      // "f" and "i"

        if ( attribute.type == THREE::V2 ) size = 2;
        else if ( attribute.type == THREE::V3 ) size = 3;
        else if ( attribute.type == THREE::V4 ) size = 4;
        else if ( attribute.type == THREE::C ) size = 3;

        attribute.size = size;

        attribute.array.resize( nvertices * size );

        attribute.buffer = glCreateBuffer();
        attribute.belongsToAttribute = a.first;

        originalAttribute.needsUpdate = true;
        attribute.__original = &originalAttribute;

      }

      geometryGroup.__glCustomAttributesList.push_back( attribute );

    }

  }

  geometryGroup.__inittedArrays = true;

}

Material* GLRenderer::getBufferMaterial( Object3D& object, GeometryGroup* geometryGroup ) {

  auto material = object.material.get();
  auto geometry = object.geometry.get();

  if ( material && !( material->type() == THREE::MeshFaceMaterial ) ) {
    return material;
  } else if ( geometry && geometryGroup && geometryGroup->materialIndex >= 0 ) {
    return geometry->materials[ geometryGroup->materialIndex ].get();
  }

  return nullptr;

}

bool GLRenderer::materialNeedsSmoothNormals( const Material* material ) {
  return material && material->shading == THREE::SmoothShading;
}

THREE::Shading GLRenderer::bufferGuessNormalType( const Material* material ) {

  // only MeshBasicMaterial and MeshDepthMaterial don't need normals

  //if ( material && ( material->type() == THREE::MeshBasicMaterial && !material->envMap ) || material->type() == THREE::MeshDepthMaterial ) {
  if ( material && ( ( material->type() == THREE::MeshBasicMaterial ) || material->type() == THREE::MeshDepthMaterial ) ) {
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

  //if ( material && ( material->map || material->lightMap || material->bumpMap || material->specularMap || material->type() == THREE::ShaderMaterial ) ) {
  if ( material && material->type() == THREE::ShaderMaterial ) {
    return true;
  }

  return false;

}

//

void GLRenderer::initDirectBuffers( Geometry& geometry ) {

  for ( auto& a : geometry.attributes ) {

    auto type = a.first == "index" ? GL_ELEMENT_ARRAY_BUFFER
                : GL_ARRAY_BUFFER;

    auto& attribute = a.second;
    attribute.buffer = glCreateBuffer();

    glBindAndBuffer( type, attribute.buffer, attribute.array, GL_STATIC_DRAW );

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
  auto dirtyElements = geometry.elementsNeedUpdate;
  auto dirtyColors = geometry.colorsNeedUpdate;

  auto& customAttributes = geometry.__glCustomAttributesList;

  Vector3 _vector3;
  int offset = 0;

  if ( object.sortParticles ) {

    _projScreenMatrixPS.copy( _projScreenMatrix );
    _projScreenMatrixPS.multiplySelf( object.matrixWorld );

    for ( int v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ v ];

      _vector3.copy( vertex.position );
      _projScreenMatrixPS.multiplyVector3( _vector3 );

      // push_back ?
      sortArray[ v ] = std::make_pair( _vector3.z, v );

    }

    typedef std::pair<float, int> SortPair;

    std::sort( sortArray.begin(),
               sortArray.end(),
    []( const SortPair & a, const SortPair & b ) {
      return a.first > b.first;
    } );

    for ( int v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ sortArray[v].second ].position;

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

      auto& customAttribute = customAttributes[ i ];

      if ( !( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) ) continue;

      offset = 0;

      const auto cal = customAttribute.value.size();

      if ( customAttribute.size == 1 ) {

        for ( size_t ca = 0; ca < cal; ca ++ ) {

          const auto& index = sortArray[ ca ].second;

          //customAttribute.array[ ca ] = customAttribute.value[ index ];
          customAttribute.array[ ca ] = customAttribute.value[ index ][ 0 ];

        }

      } else if ( customAttribute.size == 2 ) {

        for ( size_t ca = 0; ca < cal; ca ++ ) {

          const auto& index = sortArray[ ca ].second;

          const auto& value = customAttribute.value[ index ];

          customAttribute.array[ offset ]     = value[ 0 ];
          customAttribute.array[ offset + 1 ] = value[ 1 ];

          offset += 2;

        }

      } else if ( customAttribute.size == 3 ) {

        if ( customAttribute.type == THREE::C ) {

          for ( size_t ca = 0; ca < cal; ca ++ ) {

            const auto index = sortArray[ ca ].second;

            const auto& value = customAttribute.value[ index ];

            customAttribute.array[ offset ]     = value[ 0 ];
            customAttribute.array[ offset + 1 ] = value[ 1 ];
            customAttribute.array[ offset + 2 ] = value[ 2 ];

            offset += 3;

          }

        } else {

          for ( size_t ca = 0; ca < cal; ca ++ ) {

            const auto index = sortArray[ ca ].second;

            const auto& value = customAttribute.value[ index ];

            customAttribute.array[ offset ]     = value[ 0 ];
            customAttribute.array[ offset + 1 ] = value[ 1 ];
            customAttribute.array[ offset + 2 ] = value[ 2 ];

            offset += 3;

          }

        }

      } else if ( customAttribute.size == 4 ) {

        for ( size_t ca = 0; ca < cal; ca ++ ) {

          const auto index = sortArray[ ca ].second;

          const auto& value = customAttribute.value[ index ];

          customAttribute.array[ offset ]      = value[ 0 ];
          customAttribute.array[ offset + 1  ] = value[ 1 ];
          customAttribute.array[ offset + 2  ] = value[ 2 ];
          customAttribute.array[ offset + 3  ] = value[ 3 ];

          offset += 4;

        }

      }

    }

  } else {

    if ( dirtyVertices ) {

      for ( int v = 0; v < vl; v ++ ) {

        const auto& vertex = vertices[ v ].position;

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

      auto& customAttribute = customAttributes[ i ];

      if ( customAttribute.needsUpdate &&
           ( customAttribute.boundTo.empty() ||
             customAttribute.boundTo == "vertices" ) ) {

        const auto cal = customAttribute.value.size();

        offset = 0;

        if ( customAttribute.size == 1 ) {

          for ( size_t ca = 0; ca < cal; ca ++ ) {

            //customAttribute.array[ ca ] = customAttribute.value[ ca ];
            customAttribute.array[ ca ] = customAttribute.value[ ca ][ 0 ];

          }

        } else if ( customAttribute.size == 2 ) {

          for ( size_t ca = 0; ca < cal; ca ++ ) {

            const auto& value = customAttribute.value[ ca ];

            customAttribute.array[ offset ]     = value[ 0 ];
            customAttribute.array[ offset + 1 ] = value[ 1 ];

            offset += 2;

          }

        } else if ( customAttribute.size == 3 ) {

          if ( customAttribute.type == THREE::C ) {

            for ( size_t ca = 0; ca < cal; ca ++ ) {

              const auto& value = customAttribute.value[ ca ];

              customAttribute.array[ offset ]     = value[ 0 ];
              customAttribute.array[ offset + 1 ] = value[ 1 ];
              customAttribute.array[ offset + 2 ] = value[ 2 ];

              offset += 3;

            }

          } else {

            for ( size_t ca = 0; ca < cal; ca ++ ) {

              const auto& value = customAttribute.value[ ca ];

              customAttribute.array[ offset ]     = value[ 0 ];
              customAttribute.array[ offset + 1 ] = value[ 1 ];
              customAttribute.array[ offset + 2 ] = value[ 2 ];

              offset += 3;

            }

          }

        } else if ( customAttribute.size == 4 ) {

          for ( size_t ca = 0; ca < cal; ca ++ ) {

            const auto& value = customAttribute.value[ ca ];

            customAttribute.array[ offset ]      = value[ 0 ];
            customAttribute.array[ offset + 1  ] = value[ 1 ];
            customAttribute.array[ offset + 2  ] = value[ 2 ];
            customAttribute.array[ offset + 3  ] = value[ 3 ];

            offset += 4;

          }

        }

      }

    }

  }

  if ( dirtyVertices || object.sortParticles ) {
    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glVertexBuffer, vertexArray, hint );
  }

  if ( dirtyColors || object.sortParticles ) {
    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glColorBuffer, colorArray, hint );
  }

  for ( int i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

    auto& customAttribute = customAttributes[ i ];

    if ( customAttribute.needsUpdate || object.sortParticles ) {
      glBindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );
    }

  }

}


void GLRenderer::setLineBuffers( Geometry& geometry, int hint ) {

  const auto& vertices = geometry.vertices;
  const auto& colors = geometry.colors;
  const auto vl = ( int )vertices.size();
  const auto cl = ( int )colors.size();

  auto& vertexArray = geometry.__vertexArray;
  auto& colorArray = geometry.__colorArray;

  auto dirtyVertices = geometry.verticesNeedUpdate;
  auto dirtyColors = geometry.colorsNeedUpdate;

  auto& customAttributes = geometry.__glCustomAttributesList;

  int i, il, ca, cal, v, c, offset;

  if ( dirtyVertices ) {

    for ( v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ v ].position;

      offset = v * 3;

      vertexArray[ offset ]     = vertex.x;
      vertexArray[ offset + 1 ] = vertex.y;
      vertexArray[ offset + 2 ] = vertex.z;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glVertexBuffer, vertexArray, hint );

  }

  if ( dirtyColors ) {

    for ( c = 0; c < cl; c ++ ) {

      const auto& color = colors[ c ];

      offset = c * 3;

      colorArray[ offset ]     = color.r;
      colorArray[ offset + 1 ] = color.g;
      colorArray[ offset + 2 ] = color.b;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glColorBuffer, colorArray, hint );

  }

  for ( i = 0, il = ( int )customAttributes.size(); i < il; i ++ ) {

    auto& customAttribute = customAttributes[ i ];

    if ( customAttribute.needsUpdate &&
         ( customAttribute.boundTo.empty() ||
           customAttribute.boundTo == "vertices" ) ) {

      offset = 0;

      cal = ( int )customAttribute.value.size();

      if ( customAttribute.size == 1 ) {

        for ( ca = 0; ca < cal; ca ++ ) {

          customAttribute.array[ ca ] = customAttribute.value[ ca ][ 0 ];

        }

      } else if ( customAttribute.size == 2 ) {

        for ( ca = 0; ca < cal; ca ++ ) {

          const auto& value = customAttribute.value[ ca ];

          customAttribute.array[ offset ]     = value[ 0 ];

          offset += 2;

        }

      } else if ( customAttribute.size == 3 ) {

        if ( customAttribute.type == THREE::C ) {

          for ( ca = 0; ca < cal; ca ++ ) {

            const auto& value = customAttribute.value[ ca ];

            customAttribute.array[ offset ]     = value[ 0 ];
            customAttribute.array[ offset + 1 ] = value[ 1 ];
            customAttribute.array[ offset + 2 ] = value[ 2 ];

            offset += 3;

          }

        } else {

          for ( ca = 0; ca < cal; ca ++ ) {

            const auto& value = customAttribute.value[ ca ];

            customAttribute.array[ offset ]     = value[ 0 ];
            customAttribute.array[ offset + 1 ] = value[ 1 ];
            customAttribute.array[ offset + 2 ] = value[ 2 ];

            offset += 3;

          }

        }

      } else if ( customAttribute.size == 4 ) {

        for ( ca = 0; ca < cal; ca ++ ) {

          const auto& value = customAttribute.value[ ca ];

          customAttribute.array[ offset ]      = value[ 0 ];
          customAttribute.array[ offset + 1  ] = value[ 1 ];
          customAttribute.array[ offset + 2  ] = value[ 2 ];
          customAttribute.array[ offset + 3  ] = value[ 3 ];

          offset += 4;

        }

      }

      glBindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );

    }

  }

}


void GLRenderer::setRibbonBuffers( Geometry& geometry, int hint ) {

  int v, c, offset;

  const auto& vertices = geometry.vertices;
  const auto& colors = geometry.colors;
  const auto vl = ( int )vertices.size();
  const auto cl = ( int )colors.size();

  auto& vertexArray = geometry.__vertexArray;
  auto& colorArray = geometry.__colorArray;

  const auto dirtyVertices = geometry.verticesNeedUpdate;
  const auto dirtyColors = geometry.colorsNeedUpdate;

  if ( dirtyVertices ) {

    for ( v = 0; v < vl; v ++ ) {

      const auto& vertex = vertices[ v ].position;

      offset = v * 3;

      vertexArray[ offset ]     = vertex.x;
      vertexArray[ offset + 1 ] = vertex.y;
      vertexArray[ offset + 2 ] = vertex.z;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glVertexBuffer, vertexArray, hint );

  }

  if ( dirtyColors ) {

    for ( c = 0; c < cl; c ++ ) {

      const auto& color = colors[ c ];

      offset = c * 3;

      colorArray[ offset ]     = color.r;
      colorArray[ offset + 1 ] = color.g;
      colorArray[ offset + 2 ] = color.b;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometry.__glColorBuffer, colorArray, hint );

  }

}

void GLRenderer::setMeshBuffers( GeometryGroup& geometryGroup, Object3D& object, int hint, bool dispose, Material* material ) {

  if ( ! geometryGroup.__inittedArrays ) {

    // console().log( object );
    return;

  }

  const auto normalType = bufferGuessNormalType( material );
  const auto vertexColorType = bufferGuessVertexColorType( material );
  const auto uvType = bufferGuessUVType( material );
  const auto needsSmoothNormals = ( normalType == THREE::SmoothShading );

  Color c1, c2, c3, c4;

  int vertexIndex = 0,
      offset = 0,
      offset_uv = 0,
      offset_uv2 = 0,
      offset_face = 0,
      offset_normal = 0,
      offset_tangent = 0,
      offset_line = 0,
      offset_color = 0,
      offset_skin = 0,
      offset_morphTarget = 0,
      offset_custom = 0,
      offset_customSrc = 0;

  auto& vertexArray = geometryGroup.__vertexArray;
  auto& uvArray = geometryGroup.__uvArray;
  auto& uv2Array = geometryGroup.__uv2Array;
  auto& normalArray = geometryGroup.__normalArray;
  auto& tangentArray = geometryGroup.__tangentArray;
  auto& colorArray = geometryGroup.__colorArray;

  auto& skinVertexAArray = geometryGroup.__skinVertexAArray;
  auto& skinVertexBArray = geometryGroup.__skinVertexBArray;
  auto& skinIndexArray = geometryGroup.__skinIndexArray;
  auto& skinWeightArray = geometryGroup.__skinWeightArray;

  auto& morphTargetsArrays = geometryGroup.__morphTargetsArrays;
  auto& morphNormalsArrays = geometryGroup.__morphNormalsArrays;

  auto& customAttributes = geometryGroup.__glCustomAttributesList;

  auto& faceArray = geometryGroup.__faceArray;
  auto& lineArray = geometryGroup.__lineArray;

  Geometry& geometry = *object.geometry;

  const bool dirtyVertices = geometry.verticesNeedUpdate,
             dirtyElements = geometry.elementsNeedUpdate,
             dirtyUvs = geometry.uvsNeedUpdate,
             dirtyNormals = geometry.normalsNeedUpdate,
             dirtyTangents = geometry.tangentsNeedUpdate,
             dirtyColors = geometry.colorsNeedUpdate,
             dirtyMorphTargets = geometry.morphTargetsNeedUpdate;

  auto& vertices = geometry.vertices;
  auto& chunk_faces3 = geometryGroup.faces3;
  auto& chunk_faces4 = geometryGroup.faces4;
  auto& obj_faces = geometry.faces;

  auto& obj_uvs  = geometry.faceVertexUvs[ 0 ];
  auto& obj_uvs2 = geometry.faceVertexUvs[ 1 ];

  auto& obj_colors = geometry.colors;

  auto& obj_skinVerticesA = geometry.skinVerticesA;
  auto& obj_skinVerticesB = geometry.skinVerticesB;
  auto& obj_skinIndices = geometry.skinIndices;
  auto& obj_skinWeights = geometry.skinWeights;

  auto& morphTargets = geometry.morphTargets;
  auto& morphNormals = geometry.morphNormals;

  if ( dirtyVertices ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

      const auto& v1 = vertices[ face.a ].position;
      const auto& v2 = vertices[ face.b ].position;
      const auto& v3 = vertices[ face.c ].position;

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

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];

      const auto& v1 = vertices[ face.a ].position;
      const auto& v2 = vertices[ face.b ].position;
      const auto& v3 = vertices[ face.c ].position;
      const auto& v4 = vertices[ face.d ].position;

      vertexArray[ offset ]     = v1.x;
      vertexArray[ offset + 1 ] = v1.y;
      vertexArray[ offset + 2 ] = v1.z;

      vertexArray[ offset + 3 ] = v2.x;
      vertexArray[ offset + 4 ] = v2.y;
      vertexArray[ offset + 5 ] = v2.z;

      vertexArray[ offset + 6 ] = v3.x;
      vertexArray[ offset + 7 ] = v3.y;
      vertexArray[ offset + 8 ] = v3.z;

      vertexArray[ offset + 9 ]  = v4.x;
      vertexArray[ offset + 10 ] = v4.y;
      vertexArray[ offset + 11 ] = v4.z;

      offset += 12;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer, vertexArray, hint );

  }

  if ( dirtyMorphTargets ) {

    for ( size_t vk = 0, vkl = morphTargets.size(); vk < vkl; vk ++ ) {

      offset_morphTarget = 0;

      for ( const auto& chf : chunk_faces3 ) {

        const auto& face = obj_faces[ chf ];

        // morph positions

        const auto& v1 = morphTargets[ vk ].vertices[ face.a ].position;
        const auto& v2 = morphTargets[ vk ].vertices[ face.b ].position;
        const auto& v3 = morphTargets[ vk ].vertices[ face.c ].position;

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
            const auto& faceVertexNormals = morphNormals[ vk ].vertexNormals;//[ chf ];

            n1 = faceVertexNormals[0];
            n2 = faceVertexNormals[1];
            n3 = faceVertexNormals[2];

          } else {

            // TODO: FIgure out where the faceNormals array comes from
            //n1 = morphNormals[ vk ].faceNormals[ chf ];
            n1 = morphNormals[ vk ].normal;//faceNormals[ chf ];
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

      for ( const auto& chf : chunk_faces4 ) {

        const auto& face = obj_faces[ chf ];

        // morph positions

        const auto& v1 = morphTargets[ vk ].vertices[ face.a ];
        const auto& v2 = morphTargets[ vk ].vertices[ face.b ];
        const auto& v3 = morphTargets[ vk ].vertices[ face.c ];
        const auto& v4 = morphTargets[ vk ].vertices[ face.d ];

        auto& vka = morphTargetsArrays[ vk ];

        vka[ offset_morphTarget ]     = v1.position.x;
        vka[ offset_morphTarget + 1 ] = v1.position.y;
        vka[ offset_morphTarget + 2 ] = v1.position.z;

        vka[ offset_morphTarget + 3 ] = v2.position.x;
        vka[ offset_morphTarget + 4 ] = v2.position.y;
        vka[ offset_morphTarget + 5 ] = v2.position.z;

        vka[ offset_morphTarget + 6 ] = v3.position.x;
        vka[ offset_morphTarget + 7 ] = v3.position.y;
        vka[ offset_morphTarget + 8 ] = v3.position.z;

        vka[ offset_morphTarget + 9 ]  = v4.position.x;
        vka[ offset_morphTarget + 10 ] = v4.position.y;
        vka[ offset_morphTarget + 11 ] = v4.position.z;

        // morph normals

        if ( material && material->morphNormals ) {

          Vector3 n1, n2, n3, n4;

          if ( needsSmoothNormals ) {

            // TODO: Figure out if vector needed
            //const auto& faceVertexNormals = morphNormals[ vk ].vertexNormals[ chf ];
            const auto& faceVertexNormals = morphNormals[ vk ].vertexNormals;//[ chf ];

            n1 = faceVertexNormals[0];
            n2 = faceVertexNormals[1];
            n3 = faceVertexNormals[2];
            n4 = faceVertexNormals[3];

          } else {

            // TODO: Figure out if vector needed
            //n1 = morphNormals[ vk ].faceNormals[ chf ];
            n1 = morphNormals[ vk ].normal;//faceNormals[ chf ];
            n2 = n1;
            n3 = n1;
            n4 = n1;

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

          nka[ offset_morphTarget + 9 ]  = n4.x;
          nka[ offset_morphTarget + 10 ] = n4.y;
          nka[ offset_morphTarget + 11 ] = n4.z;

        }

        //

        offset_morphTarget += 12;

      }

      glBindAndBuffer( GL_ARRAY_BUFFER,
                       geometryGroup.__glMorphTargetsBuffers[ vk ],
                       morphTargetsArrays[ vk ], hint );

      if ( material && material->morphNormals ) {

        glBindAndBuffer( GL_ARRAY_BUFFER,
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

      // vertices A

      const auto& sa1 = obj_skinVerticesA[ face.a ];
      const auto& sa2 = obj_skinVerticesA[ face.b ];
      const auto& sa3 = obj_skinVerticesA[ face.c ];

      skinVertexAArray[ offset_skin ]     = sa1.x;
      skinVertexAArray[ offset_skin + 1 ] = sa1.y;
      skinVertexAArray[ offset_skin + 2 ] = sa1.z;
      skinVertexAArray[ offset_skin + 3 ] = 1; // pad for faster vertex shader

      skinVertexAArray[ offset_skin + 4 ] = sa2.x;
      skinVertexAArray[ offset_skin + 5 ] = sa2.y;
      skinVertexAArray[ offset_skin + 6 ] = sa2.z;
      skinVertexAArray[ offset_skin + 7 ] = 1;

      skinVertexAArray[ offset_skin + 8 ]  = sa3.x;
      skinVertexAArray[ offset_skin + 9 ]  = sa3.y;
      skinVertexAArray[ offset_skin + 10 ] = sa3.z;
      skinVertexAArray[ offset_skin + 11 ] = 1;

      // vertices B

      const auto& sb1 = obj_skinVerticesB[ face.a ];
      const auto& sb2 = obj_skinVerticesB[ face.b ];
      const auto& sb3 = obj_skinVerticesB[ face.c ];

      skinVertexBArray[ offset_skin ]     = sb1.x;
      skinVertexBArray[ offset_skin + 1 ] = sb1.y;
      skinVertexBArray[ offset_skin + 2 ] = sb1.z;
      skinVertexBArray[ offset_skin + 3 ] = 1; // pad for faster vertex shader

      skinVertexBArray[ offset_skin + 4 ] = sb2.x;
      skinVertexBArray[ offset_skin + 5 ] = sb2.y;
      skinVertexBArray[ offset_skin + 6 ] = sb2.z;
      skinVertexBArray[ offset_skin + 7 ] = 1;

      skinVertexBArray[ offset_skin + 8 ]  = sb3.x;
      skinVertexBArray[ offset_skin + 9 ]  = sb3.y;
      skinVertexBArray[ offset_skin + 10 ] = sb3.z;
      skinVertexBArray[ offset_skin + 11 ] = 1;

      offset_skin += 12;

    }

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];

      // weights

      const auto& sw1 = obj_skinWeights[ face.a ];
      const auto& sw2 = obj_skinWeights[ face.b ];
      const auto& sw3 = obj_skinWeights[ face.c ];
      const auto& sw4 = obj_skinWeights[ face.d ];

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

      skinWeightArray[ offset_skin + 12 ] = sw4.x;
      skinWeightArray[ offset_skin + 13 ] = sw4.y;
      skinWeightArray[ offset_skin + 14 ] = sw4.z;
      skinWeightArray[ offset_skin + 15 ] = sw4.w;

      // indices

      const auto& si1 = obj_skinIndices[ face.a ];
      const auto& si2 = obj_skinIndices[ face.b ];
      const auto& si3 = obj_skinIndices[ face.c ];
      const auto& si4 = obj_skinIndices[ face.d ];

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

      skinIndexArray[ offset_skin + 12 ] = si4.x;
      skinIndexArray[ offset_skin + 13 ] = si4.y;
      skinIndexArray[ offset_skin + 14 ] = si4.z;
      skinIndexArray[ offset_skin + 15 ] = si4.w;

      // vertices A

      const auto& sa1 = obj_skinVerticesA[ face.a ];
      const auto& sa2 = obj_skinVerticesA[ face.b ];
      const auto& sa3 = obj_skinVerticesA[ face.c ];
      const auto& sa4 = obj_skinVerticesA[ face.d ];

      skinVertexAArray[ offset_skin ]     = sa1.x;
      skinVertexAArray[ offset_skin + 1 ] = sa1.y;
      skinVertexAArray[ offset_skin + 2 ] = sa1.z;
      skinVertexAArray[ offset_skin + 3 ] = 1; // pad for faster vertex shader

      skinVertexAArray[ offset_skin + 4 ] = sa2.x;
      skinVertexAArray[ offset_skin + 5 ] = sa2.y;
      skinVertexAArray[ offset_skin + 6 ] = sa2.z;
      skinVertexAArray[ offset_skin + 7 ] = 1;

      skinVertexAArray[ offset_skin + 8 ]  = sa3.x;
      skinVertexAArray[ offset_skin + 9 ]  = sa3.y;
      skinVertexAArray[ offset_skin + 10 ] = sa3.z;
      skinVertexAArray[ offset_skin + 11 ] = 1;

      skinVertexAArray[ offset_skin + 12 ] = sa4.x;
      skinVertexAArray[ offset_skin + 13 ] = sa4.y;
      skinVertexAArray[ offset_skin + 14 ] = sa4.z;
      skinVertexAArray[ offset_skin + 15 ] = 1;

      // vertices B

      const auto& sb1 = obj_skinVerticesB[ face.a ];
      const auto& sb2 = obj_skinVerticesB[ face.b ];
      const auto& sb3 = obj_skinVerticesB[ face.c ];
      const auto& sb4 = obj_skinVerticesB[ face.d ];

      skinVertexBArray[ offset_skin ]     = sb1.x;
      skinVertexBArray[ offset_skin + 1 ] = sb1.y;
      skinVertexBArray[ offset_skin + 2 ] = sb1.z;
      skinVertexBArray[ offset_skin + 3 ] = 1; // pad for faster vertex shader

      skinVertexBArray[ offset_skin + 4 ] = sb2.x;
      skinVertexBArray[ offset_skin + 5 ] = sb2.y;
      skinVertexBArray[ offset_skin + 6 ] = sb2.z;
      skinVertexBArray[ offset_skin + 7 ] = 1;

      skinVertexBArray[ offset_skin + 8 ]  = sb3.x;
      skinVertexBArray[ offset_skin + 9 ]  = sb3.y;
      skinVertexBArray[ offset_skin + 10 ] = sb3.z;
      skinVertexBArray[ offset_skin + 11 ] = 1;

      skinVertexBArray[ offset_skin + 12 ] = sb4.x;
      skinVertexBArray[ offset_skin + 13 ] = sb4.y;
      skinVertexBArray[ offset_skin + 14 ] = sb4.z;
      skinVertexBArray[ offset_skin + 15 ] = 1;

      offset_skin += 16;

    }

    if ( offset_skin > 0 ) {

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinVertexABuffer, skinVertexAArray, hint );

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinVertexBBuffer, skinVertexBArray, hint );

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinIndicesBuffer, skinIndexArray, hint );

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinWeightsBuffer, skinWeightArray, hint );

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

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];

      const auto& vertexColors = face.vertexColors;
      const auto& faceColor = face.color;

      if ( face.size() == 4 && vertexColorType == THREE::VertexColors ) {

        c1 = vertexColors[ 0 ];
        c2 = vertexColors[ 1 ];
        c3 = vertexColors[ 2 ];
        c4 = vertexColors[ 3 ];

      } else {

        c1 = faceColor;
        c2 = faceColor;
        c3 = faceColor;
        c4 = faceColor;

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

      colorArray[ offset_color + 9 ]  = c4.r;
      colorArray[ offset_color + 10 ] = c4.g;
      colorArray[ offset_color + 11 ] = c4.b;

      offset_color += 12;

    }

    if ( offset_color > 0 ) {

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glColorBuffer, colorArray, hint );

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

    for ( const auto& chunk_face4 : chunk_faces4 ) {

      const auto& face = obj_faces[ chunk_face4 ];

      const auto& vertexTangents = face.vertexTangents;

      const auto& t1 = vertexTangents[ 0 ];
      const auto& t2 = vertexTangents[ 1 ];
      const auto& t3 = vertexTangents[ 2 ];
      const auto& t4 = vertexTangents[ 3 ];

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

      tangentArray[ offset_tangent + 12 ] = t4.x;
      tangentArray[ offset_tangent + 13 ] = t4.y;
      tangentArray[ offset_tangent + 14 ] = t4.z;
      tangentArray[ offset_tangent + 15 ] = t4.w;

      offset_tangent += 16;

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glTangentBuffer, tangentArray, hint );

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

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];

      const auto& vertexNormals = face.vertexNormals;
      const auto& faceNormal = face.normal;

      if ( face.size() == 4 && needsSmoothNormals ) {

        for ( i = 0; i < 4; i ++ ) {

          const auto& vn = vertexNormals[ i ];

          normalArray[ offset_normal ]     = vn.x;
          normalArray[ offset_normal + 1 ] = vn.y;
          normalArray[ offset_normal + 2 ] = vn.z;

          offset_normal += 3;

        }

      } else {

        for ( i = 0; i < 4; i ++ ) {

          normalArray[ offset_normal ]     = faceNormal.x;
          normalArray[ offset_normal + 1 ] = faceNormal.y;
          normalArray[ offset_normal + 2 ] = faceNormal.z;

          offset_normal += 3;

        }

      }

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glNormalBuffer, normalArray, hint );

  }

  if ( dirtyUvs && obj_uvs.size() > 0 && uvType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];
      const auto& uv = obj_uvs[ fi ];

      // TODO:?
      //if ( uv == undefined ) continue;

      for ( i = 0; i < 3; i ++ ) {

        const auto& uvi = uv[ i ];

        uvArray[ offset_uv ]     = uvi.u;
        uvArray[ offset_uv + 1 ] = uvi.v;

        offset_uv += 2;

      }

    }

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];
      const auto& uv = obj_uvs[ fi ];

      // TODO:
      //if ( uv == undefined ) continue;

      for ( i = 0; i < 4; i ++ ) {

        const auto& uvi = uv[ i ];

        uvArray[ offset_uv ]     = uvi.u;
        uvArray[ offset_uv + 1 ] = uvi.v;

        offset_uv += 2;

      }

    }

    if ( offset_uv > 0 ) {

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUVBuffer, uvArray, hint );

    }

  }

  if ( dirtyUvs && obj_uvs2.size() > 0 && uvType ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];
      const auto& uv2 = obj_uvs2[ fi ];

      // TODO:?
      //if ( uv2 == undefined ) continue;

      for ( i = 0; i < 3; i ++ ) {

        const auto& uv2i = uv2[ i ];

        uv2Array[ offset_uv2 ]     = uv2i.u;
        uv2Array[ offset_uv2 + 1 ] = uv2i.v;

        offset_uv2 += 2;

      }

    }

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];
      const auto& uv2 = obj_uvs2[ fi ];

      // TODO:?
      //if ( uv2 == undefined ) continue;

      for ( i = 0; i < 4; i ++ ) {

        const auto& uv2i = uv2[ i ];

        uv2Array[ offset_uv2 ]     = uv2i.u;
        uv2Array[ offset_uv2 + 1 ] = uv2i.v;

        offset_uv2 += 2;

      }

    }

    if ( offset_uv2 > 0 ) {

      glBindAndBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUV2Buffer, uv2Array, hint );

    }

  }

  if ( dirtyElements ) {

    for ( const auto& fi : chunk_faces3 ) {

      const auto& face = obj_faces[ fi ];

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

    for ( const auto& fi : chunk_faces4 ) {

      const auto& face = obj_faces[ fi ];

      faceArray[ offset_face ]     = vertexIndex;
      faceArray[ offset_face + 1 ] = vertexIndex + 1;
      faceArray[ offset_face + 2 ] = vertexIndex + 3;

      faceArray[ offset_face + 3 ] = vertexIndex + 1;
      faceArray[ offset_face + 4 ] = vertexIndex + 2;
      faceArray[ offset_face + 5 ] = vertexIndex + 3;

      offset_face += 6;

      lineArray[ offset_line ]     = vertexIndex;
      lineArray[ offset_line + 1 ] = vertexIndex + 1;

      lineArray[ offset_line + 2 ] = vertexIndex;
      lineArray[ offset_line + 3 ] = vertexIndex + 3;

      lineArray[ offset_line + 4 ] = vertexIndex + 1;
      lineArray[ offset_line + 5 ] = vertexIndex + 2;

      lineArray[ offset_line + 6 ] = vertexIndex + 2;
      lineArray[ offset_line + 7 ] = vertexIndex + 3;

      offset_line += 8;

      vertexIndex += 4;

    }

    glBindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glFaceBuffer, faceArray, hint );
    glBindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glLineBuffer, lineArray, hint );

  }

  for ( auto& customAttribute : customAttributes ) {

    if ( customAttribute.__original && ( ! customAttribute.__original->needsUpdate ) ) continue;

    offset_custom = 0;
    offset_customSrc = 0;

    if ( customAttribute.size == 1 ) {

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          customAttribute.array[ offset_custom ]     = customAttribute.value[ face.a ].x;
          customAttribute.array[ offset_custom + 1 ] = customAttribute.value[ face.b ].x;
          customAttribute.array[ offset_custom + 2 ] = customAttribute.value[ face.c ].x;

          offset_custom += 3;

        }

        for ( const auto& fi : chunk_faces4 ) {

          const auto& face = obj_faces[ fi ];

          customAttribute.array[ offset_custom ]     = customAttribute.value[ face.a ].x;
          customAttribute.array[ offset_custom + 1 ] = customAttribute.value[ face.b ].x;
          customAttribute.array[ offset_custom + 2 ] = customAttribute.value[ face.c ].x;
          customAttribute.array[ offset_custom + 3 ] = customAttribute.value[ face.d ].x;

          offset_custom += 4;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

          customAttribute.array[ offset_custom ]     = value.x;
          customAttribute.array[ offset_custom + 1 ] = value.x;
          customAttribute.array[ offset_custom + 2 ] = value.x;

          offset_custom += 3;

        }

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          customAttribute.array[ offset_custom ]     = value.x;
          customAttribute.array[ offset_custom + 1 ] = value.x;
          customAttribute.array[ offset_custom + 2 ] = value.x;
          customAttribute.array[ offset_custom + 3 ] = value.x;

          offset_custom += 4;

        }

      }

    } else if ( customAttribute.size == 2 ) {

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];

          customAttribute.array[ offset_custom ]     = v1.x;
          customAttribute.array[ offset_custom + 1 ] = v1.y;

          customAttribute.array[ offset_custom + 2 ] = v2.x;
          customAttribute.array[ offset_custom + 3 ] = v2.y;

          customAttribute.array[ offset_custom + 4 ] = v3.x;
          customAttribute.array[ offset_custom + 5 ] = v3.y;

          offset_custom += 6;

        }

        for ( const auto& fi : chunk_faces4 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];
          const auto& v4 = customAttribute.value[ face.d ];

          customAttribute.array[ offset_custom ]     = v1.x;
          customAttribute.array[ offset_custom + 1 ] = v1.y;

          customAttribute.array[ offset_custom + 2 ] = v2.x;
          customAttribute.array[ offset_custom + 3 ] = v2.y;

          customAttribute.array[ offset_custom + 4 ] = v3.x;
          customAttribute.array[ offset_custom + 5 ] = v3.y;

          customAttribute.array[ offset_custom + 6 ] = v4.x;
          customAttribute.array[ offset_custom + 7 ] = v4.y;

          offset_custom += 8;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;
          const auto& v4 = value;

          customAttribute.array[ offset_custom ]     = v1.x;
          customAttribute.array[ offset_custom + 1 ] = v1.y;

          customAttribute.array[ offset_custom + 2 ] = v2.x;
          customAttribute.array[ offset_custom + 3 ] = v2.y;

          customAttribute.array[ offset_custom + 4 ] = v3.x;
          customAttribute.array[ offset_custom + 5 ] = v3.y;

          customAttribute.array[ offset_custom + 6 ] = v4.x;
          customAttribute.array[ offset_custom + 7 ] = v4.y;

          offset_custom += 8;

        }

      }

    } else if ( customAttribute.size == 3 ) {

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];
          const auto& v4 = customAttribute.value[ face.d ];

          customAttribute.array[ offset_custom  ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1  ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2  ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3  ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4  ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5  ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6  ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7  ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8  ] = v3[ 2 ];

          customAttribute.array[ offset_custom + 9  ] = v4[ 0 ];
          customAttribute.array[ offset_custom + 10 ] = v4[ 1 ];
          customAttribute.array[ offset_custom + 11 ] = v4[ 2 ];

          offset_custom += 12;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;
          const auto& v4 = value;

          customAttribute.array[ offset_custom  ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1  ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2  ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3  ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4  ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5  ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6  ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7  ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8  ] = v3[ 2 ];

          customAttribute.array[ offset_custom + 9  ] = v4[ 0 ];
          customAttribute.array[ offset_custom + 10 ] = v4[ 1 ];
          customAttribute.array[ offset_custom + 11 ] = v4[ 2 ];

          offset_custom += 12;

        }

      }

#ifdef TODO_CHUNK_FACE_VERTICES
      else if ( customAttribute.boundTo == "faceVertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          const auto& v1 = value[ 0 ];
          const auto& v2 = value[ 1 ];
          const auto& v3 = value[ 2 ];
          const auto& v4 = value[ 3 ];

          customAttribute.array[ offset_custom  ]     = v1[ 0 ];
          customAttribute.array[ offset_custom + 1  ] = v1[ 1 ];
          customAttribute.array[ offset_custom + 2  ] = v1[ 2 ];

          customAttribute.array[ offset_custom + 3  ] = v2[ 0 ];
          customAttribute.array[ offset_custom + 4  ] = v2[ 1 ];
          customAttribute.array[ offset_custom + 5  ] = v2[ 2 ];

          customAttribute.array[ offset_custom + 6  ] = v3[ 0 ];
          customAttribute.array[ offset_custom + 7  ] = v3[ 1 ];
          customAttribute.array[ offset_custom + 8  ] = v3[ 2 ];

          customAttribute.array[ offset_custom + 9  ] = v4[ 0 ];
          customAttribute.array[ offset_custom + 10 ] = v4[ 1 ];
          customAttribute.array[ offset_custom + 11 ] = v4[ 2 ];

          offset_custom += 12;

        }
      }
#endif

    } else if ( customAttribute.size == 4 ) {

      if ( customAttribute.boundTo.empty() || customAttribute.boundTo == "vertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& face = obj_faces[ fi ];

          const auto& v1 = customAttribute.value[ face.a ];
          const auto& v2 = customAttribute.value[ face.b ];
          const auto& v3 = customAttribute.value[ face.c ];
          const auto& v4 = customAttribute.value[ face.d ];

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

          customAttribute.array[ offset_custom + 12 ] = v4.x;
          customAttribute.array[ offset_custom + 13 ] = v4.y;
          customAttribute.array[ offset_custom + 14 ] = v4.z;
          customAttribute.array[ offset_custom + 15 ] = v4.w;

          offset_custom += 16;

        }

      } else if ( customAttribute.boundTo == "faces" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          const auto& v1 = value;
          const auto& v2 = value;
          const auto& v3 = value;
          const auto& v4 = value;

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

          customAttribute.array[ offset_custom + 12 ] = v4.x;
          customAttribute.array[ offset_custom + 13 ] = v4.y;
          customAttribute.array[ offset_custom + 14 ] = v4.z;
          customAttribute.array[ offset_custom + 15 ] = v4.w;

          offset_custom += 16;

        }

      }
#ifdef TODO_CHUNK_FACE_VERTICES
      else if ( customAttribute.boundTo == "faceVertices" ) {

        for ( const auto& fi : chunk_faces3 ) {

          const auto& value = customAttribute.value[ fi ];

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

        for ( const auto& fi : chunk_faces4 ) {

          const auto& value = customAttribute.value[ fi ];

          const auto& v1 = value[ 0 ];
          const auto& v2 = value[ 1 ];
          const auto& v3 = value[ 2 ];
          const auto& v4 = value[ 3 ];

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

          customAttribute.array[ offset_custom + 12 ] = v4.x;
          customAttribute.array[ offset_custom + 13 ] = v4.y;
          customAttribute.array[ offset_custom + 14 ] = v4.z;
          customAttribute.array[ offset_custom + 15 ] = v4.w;

          offset_custom += 16;

        }

      }

#endif // TODO_CHUNK_FACE_VERTICES

    }

    glBindAndBuffer( GL_ARRAY_BUFFER, customAttribute.buffer, customAttribute.array, hint );

  }

  if ( dispose ) {

    geometryGroup.dispose();

  }

}


void GLRenderer::setDirectBuffers( Geometry& geometry, int hint, bool dispose ) {

  auto& attributes = geometry.attributes;

  if ( geometry.elementsNeedUpdate && contains( attributes, "index" ) ) {

    auto& index = attributes[ "index" ];
    glBindAndBuffer( GL_ELEMENT_ARRAY_BUFFER, index.buffer, index.array, hint );

  }

  if ( geometry.verticesNeedUpdate && contains( attributes, "position" ) ) {

    auto& position = attributes[ "position" ];
    glBindAndBuffer( GL_ARRAY_BUFFER, position.buffer, position.array, hint );

  }

  if ( geometry.normalsNeedUpdate && contains( attributes, "normals" ) ) {

    auto& normal   = attributes[ "normal" ];
    glBindAndBuffer( GL_ARRAY_BUFFER, normal.buffer, normal.array, hint );

  }

  if ( geometry.uvsNeedUpdate && contains( attributes, "uv" ) ) {

    auto& uv       = attributes[ "uv" ];
    glBindAndBuffer( GL_ARRAY_BUFFER, uv.buffer, uv.array, hint );

  }

  if ( geometry.colorsNeedUpdate && contains( attributes, "color" ) ) {

    auto& color    = attributes[ "color" ];
    glBindAndBuffer( GL_ARRAY_BUFFER, color.buffer, color.array, hint );

  }

  if ( geometry.tangentsNeedUpdate && contains( attributes, "tangent" ) ) {

    auto& tangent  = attributes[ "tangent" ];
    glBindAndBuffer( GL_ARRAY_BUFFER, tangent.buffer, tangent.array, hint );

  }

  if ( dispose ) {

for ( auto& attribute : geometry.attributes ) {

      attribute.second.array.clear();

    }

  }

}

// Buffer rendering


void GLRenderer::renderBufferImmediate( Object3D& object, Program& program, Material& material ) {

  if ( object.glImmediateData.hasPositions && ! object.glImmediateData.__glVertexBuffer ) object.glImmediateData.__glVertexBuffer = glCreateBuffer();
  if ( object.glImmediateData.hasNormals   && ! object.glImmediateData.__glNormalBuffer ) object.glImmediateData.__glNormalBuffer = glCreateBuffer();
  if ( object.glImmediateData.hasUvs       && ! object.glImmediateData.__glUvBuffer )     object.glImmediateData.__glUvBuffer     = glCreateBuffer();
  if ( object.glImmediateData.hasColors    && ! object.glImmediateData.__glColorBuffer )  object.glImmediateData.__glColorBuffer  = glCreateBuffer();

  if ( object.glImmediateData.hasPositions ) {

    glBindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glVertexBuffer, object.glImmediateData.positionArray, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( program.attributes["position"] );
    glVertexAttribPointer( program.attributes["position"], 3, GL_FLOAT, false, 0, 0 );

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

    glBindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glNormalBuffer, object.glImmediateData.normalArray, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( program.attributes["normal"] );
    glVertexAttribPointer( program.attributes["normal"], 3, GL_FLOAT, false, 0, 0 );

  }

  if ( object.glImmediateData.hasUvs && material.map ) {

    glBindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glUvBuffer, object.glImmediateData.uvArray, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( program.attributes["uv"] );
    glVertexAttribPointer( program.attributes["uv"], 2, GL_FLOAT, false, 0, 0 );

  }

  if ( object.glImmediateData.hasColors && material.vertexColors != THREE::NoColors ) {

    glBindAndBuffer( GL_ARRAY_BUFFER, object.glImmediateData.__glColorBuffer, object.glImmediateData.colorArray, GL_DYNAMIC_DRAW );
    glEnableVertexAttribArray( program.attributes["color"] );
    glVertexAttribPointer( program.attributes["color"], 3, GL_FLOAT, false, 0, 0 );

  }

  glDrawArrays( GL_TRIANGLES, 0, object.glImmediateData.count );

  object.glImmediateData.count = 0;

}

void GLRenderer::renderBufferDirect( Camera& camera, Lights& lights, IFog* fog, Material& material, Geometry& geometry, Object3D& object ) {

  if ( material.visible == false ) return;

  auto& program = setProgram( camera, lights, fog, material, object );

  auto& attributes = program.attributes;

  auto updateBuffers = false;
  auto wireframeBit = material.wireframe ? 1 : 0;
  auto geometryHash = ( geometry.id * 0xffffff ) + ( program.id * 2 ) + wireframeBit;

  if ( geometryHash != _currentGeometryGroupHash ) {

    _currentGeometryGroupHash = geometryHash;
    updateBuffers = true;

  }

  // render mesh

  if ( object.type() == THREE::Mesh ) {

    const auto& offsets = geometry.offsets;

    // if there is more than 1 chunk
    // must set attribute pointers to use new offsets for each chunk
    // even if geometry and materials didn't change

    if ( offsets.size() > 1 ) updateBuffers = true;

    for ( size_t i = 0, il = offsets.size(); i < il; ++ i ) {

      const auto startIndex = offsets[ i ].index;

      if ( updateBuffers ) {

        // vertices

        auto& position = geometry.attributes[ "position" ];
        const auto positionSize = position.itemSize;

        glBindBuffer( GL_ARRAY_BUFFER, position.buffer );
        glVertexAttribPointer( attributes["position"], positionSize, GL_FLOAT, false, 0, toOffset( startIndex * positionSize * 4 ) ); // 4 bytes per Float32

        // normals

        if ( attributes["normal"] >= 0 && contains( geometry.attributes, "normal" ) ) {

          auto& normal = geometry.attributes[ "normal" ];
          auto normalSize = normal.itemSize;

          glBindBuffer( GL_ARRAY_BUFFER, normal.buffer );
          glVertexAttribPointer( attributes["normal"], normalSize, GL_FLOAT, false, 0, toOffset( startIndex * normalSize * 4 ) );

        }

        // uvs

        if ( attributes["uv"] >= 0 && contains( geometry.attributes, "uv" ) ) {

          const auto& uv = geometry.attributes[ "uv" ];

          if ( uv.buffer ) {

            const auto uvSize = uv.itemSize;

            glBindBuffer( GL_ARRAY_BUFFER, uv.buffer );
            glVertexAttribPointer( attributes["uv"], uvSize, GL_FLOAT, false, 0, toOffset( startIndex * uvSize * 4 ) );

            glEnableVertexAttribArray( attributes["uv"] );

          } else {

            glDisableVertexAttribArray( attributes["uv"] );

          }

        }

        // colors

        if ( attributes["color"] >= 0 && contains( geometry.attributes, "color" ) ) {

          const auto& color = geometry.attributes[ "color" ];
          const auto colorSize = color.itemSize;

          glBindBuffer( GL_ARRAY_BUFFER, color.buffer );
          glVertexAttribPointer( attributes["color"], colorSize, GL_FLOAT, false, 0, toOffset( startIndex * colorSize * 4 ) );

        }

        // tangents

        if ( attributes["tangent"] >= 0 && contains( geometry.attributes, "tangent" ) ) {

          const auto& tangent = geometry.attributes[ "tangent" ];
          const auto tangentSize = tangent.itemSize;

          glBindBuffer( GL_ARRAY_BUFFER, tangent.buffer );
          glVertexAttribPointer( attributes["tangent"], tangentSize, GL_FLOAT, false, 0, toOffset( startIndex * tangentSize * 4 ) );

        }

        // indices

        const auto& index = geometry.attributes[ "index" ];

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index.buffer );

      }

      // render indexed triangles

      glDrawElements( GL_TRIANGLES, offsets[ i ].count, GL_UNSIGNED_SHORT, toOffset( offsets[ i ].start * 2 ) ); // 2 bytes per Uint16

      _info.render.calls ++;
      _info.render.vertices += offsets[ i ].count; // not really true, here vertices can be shared
      _info.render.faces += offsets[ i ].count / 3;

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

  // vertices

  if ( !material.morphTargets && attributes["position"] >= 0 ) {

    if ( updateBuffers ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer );
      glVertexAttribPointer( attributes["position"], 3, GL_FLOAT, false, 0, 0 );

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

      // TODO: Fix this?
      if ( contains( attributes, attribute.belongsToAttribute ) ) {

        glBindBuffer( GL_ARRAY_BUFFER, attribute.buffer );
        glVertexAttribPointer( attributes[ attribute.belongsToAttribute ], attribute.size, GL_FLOAT, false, 0, 0 );

      }

    }


    // colors

    if ( attributes["color"] >= 0 ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glColorBuffer );
      glVertexAttribPointer( attributes["color"], 3, GL_FLOAT, false, 0, 0 );

    }

    // normals

    if ( attributes["normal"] >= 0 ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glNormalBuffer );
      glVertexAttribPointer( attributes["normal"], 3, GL_FLOAT, false, 0, 0 );

    }

    // tangents

    if ( attributes["tangent"] >= 0 ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glTangentBuffer );
      glVertexAttribPointer( attributes["tangent"], 4, GL_FLOAT, false, 0, 0 );

    }

    // uvs

    if ( attributes["uv"] >= 0 ) {

      if ( geometryGroup.__glUVBuffer ) {

        glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUVBuffer );
        glVertexAttribPointer( attributes["uv"], 2, GL_FLOAT, false, 0, 0 );

        glEnableVertexAttribArray( attributes["uv"] );

      } else {

        glDisableVertexAttribArray( attributes["uv"] );

      }

    }

    if ( attributes["uv2"] >= 0 ) {

      if ( geometryGroup.__glUV2Buffer ) {

        glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glUV2Buffer );
        glVertexAttribPointer( attributes["uv2"], 2, GL_FLOAT, false, 0, 0 );

        glEnableVertexAttribArray( attributes["uv2"] );

      } else {

        glDisableVertexAttribArray( attributes["uv2"] );

      }

    }

    if ( material.skinning &&
         attributes["skinVertexA"] >= 0 && attributes["skinVertexB"] >= 0 &&
         attributes["skinIndex"] >= 0 && attributes["skinWeight"] >= 0 ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinVertexABuffer );
      glVertexAttribPointer( attributes["skinVertexA"], 4, GL_FLOAT, false, 0, 0 );

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinVertexBBuffer );
      glVertexAttribPointer( attributes["skinVertexB"], 4, GL_FLOAT, false, 0, 0 );

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinIndicesBuffer );
      glVertexAttribPointer( attributes["skinIndex"], 4, GL_FLOAT, false, 0, 0 );

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glSkinWeightsBuffer );
      glVertexAttribPointer( attributes["skinWeight"], 4, GL_FLOAT, false, 0, 0 );

    }

  }

  // render mesh

  if ( object.type() == THREE::Mesh ) {

    // wireframe

    if ( material.wireframe ) {

      setLineWidth( material.wireframeLinewidth );

      if ( updateBuffers ) glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glLineBuffer );
      glDrawElements( GL_LINES, geometryGroup.__glLineCount, GL_UNSIGNED_SHORT, 0 );

      // triangles

    } else {

      if ( updateBuffers ) glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.__glFaceBuffer );
      glDrawElements( GL_TRIANGLES, geometryGroup.__glFaceCount, GL_UNSIGNED_SHORT, 0 );

    }

    _info.render.calls ++;
    _info.render.vertices += geometryGroup.__glFaceCount;
    _info.render.faces += geometryGroup.__glFaceCount / 3;

    // render lines

  } else if ( object.type() == THREE::Line ) {

#ifdef TODO_LINE_RENDERING
    const auto primitives = ( object.type() == THREE::LineStrip ) ? GL_LINE_STRIP : GL_LINES;
#endif
    const auto primitives = GL_LINES;

    setLineWidth( material.linewidth );

    glDrawArrays( primitives, 0, geometryGroup.__glLineCount );

    _info.render.calls ++;


    // render particles

  } else if ( object.type() == THREE::ParticleSystem ) {

    glDrawArrays( GL_POINTS, 0, geometryGroup.__glParticleCount );

    _info.render.calls ++;
    _info.render.points += geometryGroup.__glParticleCount;

    // render ribbon

  } else if ( object.type() == THREE::Ribbon ) {

    glDrawArrays( GL_TRIANGLE_STRIP, 0, geometryGroup.__glVertexCount );

    _info.render.calls ++;

  }

}

// Sorting

struct NumericalSort {
  template < typename T, typename U >
  bool operator()( const std::pair<T, U>& a, const std::pair<T, U>& b ) {
    return a.second > b.second;
  }
};

void GLRenderer::setupMorphTargets( Material& material, GeometryGroup& geometryGroup, Object3D& object ) {

#ifndef TODO_MORPH_TARGETS

  console().warn( "GLRenderer::setupMorphTargets: Not implemented" );

#else

  // set base

  auto& attributes = material.program.attributes;

  if ( object.morphTargetBase != -1 ) {

    glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ object.morphTargetBase ] );
    glVertexAttribPointer( attributes["position"], 3, GL_FLOAT, false, 0, 0 );

  } else if ( attributes["position" >= 0 ) {

  glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glVertexBuffer );
    glVertexAttribPointer( attributes["position"], 3, GL_FLOAT, false, 0, 0 );

  }

  if ( object.morphTargetForcedOrder.size() ) {

  // set forced order

  auto m = 0;
  auto& order = object.morphTargetForcedOrder;
  auto& influences = object.glData.morphTargetInfluences;

  while ( m < material.numSupportedMorphTargets && m < order.size() ) {

      glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ order[ m ] ] );
      glVertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

      if ( material.morphNormals ) {

        glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphNormalsBuffers[ order[ m ] ] );
        glVertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

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

        glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphTargetsBuffers[ influenceIndex ] );

        glVertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

        if ( material.morphNormals ) {

          glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.__glMorphNormalsBuffers[ influenceIndex ] );
          glVertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

        }

        object.__glMorphTargetInfluences[ m ] = influences[ influenceIndex ];

      } else {

        glVertexAttribPointer( attributes[ "morphTarget" + m ], 3, GL_FLOAT, false, 0, 0 );

        if ( material.morphNormals ) {

          glVertexAttribPointer( attributes[ "morphNormal" + m ], 3, GL_FLOAT, false, 0, 0 );

        }

        object.__glMorphTargetInfluences[ m ] = 0;

      }

      m ++;

    }

  }

  // load updated influences uniform

  if ( material.program.uniforms.morphTargetInfluences.size() > 0 ) {

  glUniform1fv( material.program.uniforms.morphTargetInfluences, object.__glMorphTargetInfluences );

  }

#endif // TODO_MORPH_TARGETS

}

// Rendering

void GLRenderer::render( Scene& scene, Camera& camera, GLRenderTarget::Ptr renderTarget /*= GLRenderTarget::Ptr()*/, bool forceClear /*= false*/ ) {

  auto& lights = scene.__lights;
  auto  fog = scene.fog.get();

  // reset caching for this frame

  _currentMaterialId = -1;
  _lightsNeedUpdate = true;

  // update scene graph

  if ( autoUpdateScene ) scene.updateMatrixWorld();

  // update camera matrices and frustum

  if ( ! camera.parent ) camera.updateMatrixWorld();

  camera.matrixWorldInverse.getInverse( camera.matrixWorld );

  camera.matrixWorldInverse.flattenToArray( camera._viewMatrixArray );
  camera.projectionMatrix.flattenToArray( camera._projectionMatrixArray );

  _projScreenMatrix.multiply( camera.projectionMatrix, camera.matrixWorldInverse );
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

  for ( auto& glObject : renderList ) {

    auto& object = *glObject.object;

    glObject.render = false;

    if ( object.visible ) {

      if ( !( object.type() == THREE::Mesh || object.type() == THREE::ParticleSystem ) || !( object.frustumCulled ) || _frustum.contains( object ) ) {
        //object.matrixWorld.flattenToArray( object._modelMatrixArray );

        setupMatrices( object, camera );

        unrollBufferMaterial( glObject );

        glObject.render = true;

        if ( sortObjects ) {

          if ( object.renderDepth ) {

            glObject.z = object.renderDepth;

          } else {

            _vector3.copy( object.matrixWorld.getPosition() );
            _projScreenMatrix.multiplyVector3( _vector3 );

            glObject.z = _vector3.z;

          }

        }

      }

    }

  }

  if ( sortObjects ) {

    std::sort( renderList.begin(), renderList.end(), PainterSort() );

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

    setBlending( THREE::NormalBlending );

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

  // glFinish();

}

void GLRenderer::renderPlugins( std::vector<IPlugin::Ptr>& plugins, Scene& scene, Camera& camera ) {

  for ( auto& plugin : plugins ) {

    // reset state for plugin (to start from clean slate)

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

    plugin->render( scene, camera, _currentWidth, _currentHeight );

    // reset state after plugin (anything could have changed)

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

  for ( auto& glObject : renderList ) {

    if ( glObject.render ) {

      auto& object = *glObject.object;
      auto& buffer = *glObject.buffer;

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

      setMaterialFaces( *material );

      if ( buffer.type() == THREE::BufferGeometry ) {
        renderBufferDirect( camera, lights, fog, *material, static_cast<Geometry&>( buffer ), object );
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
    object.immediateRenderCallback( &program, _gl, &_frustum );
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

  auto& meshMaterial = *object.material;

  if ( meshMaterial.type() == THREE::MeshFaceMaterial ) {

    const auto materialIndex = buffer.materialIndex;

    if ( materialIndex >= 0 ) {

      auto& material = *object.geometry->materials[ materialIndex ];

      if ( material.transparent ) {

        globject.transparent = &material;
        globject.opaque = nullptr;

      } else {

        globject.opaque = &material;
        globject.transparent = nullptr;

      }

    }

  } else {

    auto& material = meshMaterial;

    if ( material.transparent ) {

      globject.transparent = &material;
      globject.opaque = nullptr;

    } else {

      globject.opaque = &material;
      globject.transparent = nullptr;

    }

  }

}

// Geometry splitting

void GLRenderer::sortFacesByMaterial( Geometry& geometry ) {

  // 0: index, 1: count
  typedef std::pair<int, int> MaterialHashValue;

  std::unordered_map<int, MaterialHashValue> hash_map;

  const auto numMorphTargets = ( int )geometry.morphTargets.size();
  const auto numMorphNormals = ( int )geometry.morphNormals.size();

  geometry.geometryGroups.clear();

  for ( int f = 0, fl = ( int )geometry.faces.size(); f < fl; ++f ) {

    const auto& face = geometry.faces[ f ];

    const auto materialIndex = face.materialIndex;

    const auto materialHash = materialIndex;

    if ( hash_map.count( materialHash ) == 0 ) {

      hash_map[ materialHash ] = MaterialHashValue( materialHash, 0 );

    }

    auto groupHash = toString( hash_map[ materialHash ] );

    if ( geometry.geometryGroups.count( groupHash ) == 0 ) {

      geometry.geometryGroups[ groupHash ] = GeometryGroup( materialIndex, numMorphTargets, numMorphNormals );

    }

    const auto vertices = face.type() == THREE::Face3 ? 3 : 4;

    if ( geometry.geometryGroups[ groupHash ].vertices + vertices > 65535 ) {

      hash_map[ materialHash ].second += 1;
      groupHash = toString( hash_map[ materialHash ] );

      if ( geometry.geometryGroups.count( groupHash ) == 0 ) {

        geometry.geometryGroups[ groupHash ] =  GeometryGroup( materialIndex, numMorphTargets, numMorphNormals );

      }

    }

    if ( face.type() == THREE::Face3 ) {

      geometry.geometryGroups[ groupHash ].faces3.push_back( f );

    } else {

      geometry.geometryGroups[ groupHash ].faces4.push_back( f );

    }

    geometry.geometryGroups[ groupHash ].vertices += vertices;

  }

  geometry.geometryGroupsList.clear();

  for ( auto& geometryGroup : geometry.geometryGroups ) {

    geometryGroup.second.id = _geometryGroupCounter ++;

    geometry.geometryGroupsList.push_back( &geometryGroup.second );

  }

}

// Objects refresh

void GLRenderer::initGLObjects( Scene& scene ) {

  /*
    scene.__glObjects. = [];
    scene.__glObjectsImmediate = [];
    scene.__glSprites = [];
    scene.__glFlares = [];
    */

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

    updateObject( *glObject.object );

  }

}

// Objects adding

static inline void addBuffer( RenderList& objlist, GeometryBuffer& buffer, Object3D& object ) {
  objlist.emplace_back( Scene::GLObject(
                          &buffer,
                          &object,
                          false,
                          nullptr,
                          nullptr
                        ) );
}

static inline void addBufferImmediate( RenderList& objlist, Object3D& object ) {
  objlist.emplace_back( Scene::GLObject(
                          nullptr,
                          &object,
                          false,
                          nullptr,
                          nullptr
                        ) );
}

void GLRenderer::addObject( Object3D& object, Scene& scene ) {

  if ( ! object.glData.__glInit ) {

    object.glData.__glInit = true;

    if ( object.type() == THREE::Mesh ) {

      Material& material = *object.material;
      Geometry& geometry = *object.geometry;

      if ( geometry.type() == THREE::Geometry ) {

        if ( geometry.geometryGroups.empty() ) {

          sortFacesByMaterial( geometry );

        }

        // create separate VBOs per geometry chunk

        for ( auto& geometryGroup : geometry.geometryGroups ) {

          // initialise VBO on the first access

          if ( ! geometryGroup.second.__glVertexBuffer ) {

            createMeshBuffers( geometryGroup.second );
            initMeshBuffers( geometryGroup.second, static_cast<Mesh&>( object ) );

            geometry.verticesNeedUpdate     = true;
            geometry.morphTargetsNeedUpdate = true;
            geometry.elementsNeedUpdate     = true;
            geometry.uvsNeedUpdate          = true;
            geometry.normalsNeedUpdate      = true;
            geometry.tangentsNeedUpdate     = true;
            geometry.colorsNeedUpdate       = true;

          }

        }

      } else if ( geometry.type() == THREE::BufferGeometry ) {

        initDirectBuffers( geometry );

      }

    } else if ( object.type() == THREE::Ribbon ) {

      auto& geometry = *object.geometry;

      if ( ! geometry.__glVertexBuffer ) {

        createRibbonBuffers( geometry );
        initRibbonBuffers( geometry );

        geometry.verticesNeedUpdate = true;
        geometry.colorsNeedUpdate = true;

      }

    } else if ( object.type() == THREE::Line ) {

      auto& geometry = *object.geometry;

      if ( ! geometry.__glVertexBuffer ) {

        createLineBuffers( geometry );
        initLineBuffers( geometry, object );

        geometry.verticesNeedUpdate = true;
        geometry.colorsNeedUpdate = true;

      }

    } else if ( object.type() == THREE::ParticleSystem ) {

      auto& geometry = *object.geometry;

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

      auto& geometry = *object.geometry;

      if ( geometry.type() == THREE::BufferGeometry ) {

        addBuffer( scene.__glObjects, geometry, object );

      } else {

        for ( auto& geometryGroup : geometry.geometryGroups ) {

          addBuffer( scene.__glObjects, geometryGroup.second, object );

        }

      }

    } else if ( object.type() == THREE::Ribbon ||
                object.type() == THREE::Line ||
                object.type() == THREE::ParticleSystem ) {

      auto& geometry = *object.geometry;
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
  GeometryGroup* geometryGroup = nullptr;

  if ( object.type() == THREE::Mesh ) {

    if ( geometry.type() == THREE::BufferGeometry ) {

      if ( geometry.verticesNeedUpdate || geometry.elementsNeedUpdate ||
           geometry.uvsNeedUpdate || geometry.normalsNeedUpdate ||
           geometry.colorsNeedUpdate || geometry.tangentsNeedUpdate ) {

        setDirectBuffers( geometry, GL_DYNAMIC_DRAW, !geometry.dynamic );

      }

      geometry.verticesNeedUpdate = false;
      geometry.elementsNeedUpdate = false;
      geometry.uvsNeedUpdate = false;
      geometry.normalsNeedUpdate = false;
      geometry.colorsNeedUpdate = false;
      geometry.tangentsNeedUpdate = false;

    } else {

      // check all geometry groups

      for ( auto& geometryGroup : geometry.geometryGroupsList ) {

        material = getBufferMaterial( object, geometryGroup );

        if ( !material ) continue;

        const auto customAttributesDirty = material->customAttributes.size() > 0 && areCustomAttributesDirty( *material );

        if ( geometry.verticesNeedUpdate || geometry.morphTargetsNeedUpdate || geometry.elementsNeedUpdate ||
             geometry.uvsNeedUpdate || geometry.normalsNeedUpdate ||
             geometry.colorsNeedUpdate || geometry.tangentsNeedUpdate || customAttributesDirty ) {

          setMeshBuffers( *geometryGroup, object, GL_DYNAMIC_DRAW, !geometry.dynamic, material );

        }

        if ( material->customAttributes.size() > 0 )
          clearCustomAttributes( *material );

      }

      geometry.verticesNeedUpdate = false;
      geometry.morphTargetsNeedUpdate = false;
      geometry.elementsNeedUpdate = false;
      geometry.uvsNeedUpdate = false;
      geometry.normalsNeedUpdate = false;
      geometry.colorsNeedUpdate = false;
      geometry.tangentsNeedUpdate = false;


    }

  } else if ( object.type() == THREE::Ribbon ) {

    if ( geometry.verticesNeedUpdate || geometry.colorsNeedUpdate ) {
      setRibbonBuffers( geometry, GL_DYNAMIC_DRAW );
    }

    geometry.verticesNeedUpdate = false;
    geometry.colorsNeedUpdate = false;

  } else if ( object.type() == THREE::Line ) {

    auto material = getBufferMaterial( object, geometryGroup );

    if ( !material ) return;

    const auto customAttributesDirty = material->customAttributes.size() > 0 && areCustomAttributesDirty( *material );

    if ( geometry.verticesNeedUpdate ||  geometry.colorsNeedUpdate || customAttributesDirty ) {
      setLineBuffers( geometry, GL_DYNAMIC_DRAW );
    }

    geometry.verticesNeedUpdate = false;
    geometry.colorsNeedUpdate = false;

    if ( material->customAttributes.size() > 0 ) clearCustomAttributes( *material );

  } else if ( object.type() == THREE::ParticleSystem ) {

    auto material = getBufferMaterial( object, geometryGroup );

    if ( !material ) return;

    const auto customAttributesDirty = material->customAttributes.size() > 0 && areCustomAttributesDirty( *material );

    if ( geometry.verticesNeedUpdate || geometry.colorsNeedUpdate || object.sortParticles || customAttributesDirty ) {
      setParticleBuffers( geometry, GL_DYNAMIC_DRAW, object );
    }

    geometry.verticesNeedUpdate = false;
    geometry.colorsNeedUpdate = false;

    if ( material->customAttributes.size() > 0 ) clearCustomAttributes( *material );

  }

}

// Objects updates - custom attributes check

bool GLRenderer::areCustomAttributesDirty( const Material& material ) {

  for ( const auto& attribute : material.customAttributes ) {
    if ( attribute.second.needsUpdate ) return true;
  }

  return false;

}

void GLRenderer::clearCustomAttributes( Material& material ) {

  for ( auto& attribute : material.customAttributes ) {
    attribute.second.needsUpdate = false;
  }

}

// Objects removal

void GLRenderer::removeObject( Object3D& object, Scene& scene ) {

  if ( object.type() == THREE::Mesh  ||
       object.type() == THREE::ParticleSystem ||
       object.type() == THREE::Ribbon ||
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
  case THREE::ParticleBasicMaterial:
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

  ProgramParameters parameters = {

    !!material.map,
    !!material.envMap,
    !!material.lightMap,
    !!material.bumpMap,
    !!material.specularMap,

    material.vertexColors,

    fog,
    !!material.fog,

    material.sizeAttenuation,

    material.skinning,
    maxBones,
    //_supportsBoneTextures && object && object.useVertexTexture,
    _supportsBoneTextures && object.useVertexTexture,
    //object && object.boneTextureWidth,
    object.boneTextureWidth,
    //object && object.boneTextureHeight,
    object.boneTextureHeight,

    material.morphTargets,
    material.morphNormals,
    maxMorphTargets,
    maxMorphNormals,

    maxLightCount.directional,
    maxLightCount.point,
    maxLightCount.spot,

    maxShadows,
    shadowMapEnabled && object.receiveShadow,
    shadowMapSoft,
    shadowMapDebug,
    shadowMapCascade,

    material.alphaTest,
    material.metal,
    material.perPixel,
    material.wrapAround,
    material.side == THREE::DoubleSide

  };

  material.program = buildProgram( shaderID,
                                   material.fragmentShader,
                                   material.vertexShader,
                                   material.uniforms,
                                   material.attributes,
                                   parameters );

  auto& attributes = material.program->attributes;

  if ( attributes["position"] >= 0 )
    glEnableVertexAttribArray( attributes["position"] );

  if ( attributes["color"] >= 0 )
    glEnableVertexAttribArray( attributes["color"] );

  if ( attributes["normal"] >= 0 )
    glEnableVertexAttribArray( attributes["normal"] );

  if ( attributes["tangent"] >= 0 )
    glEnableVertexAttribArray( attributes["tangent"] );

  if ( material.skinning &&
       attributes["skinVertexA"] >= 0 && attributes["skinVertexB"] >= 0 &&
       attributes["skinIndex"] >= 0 && attributes["skinWeight"] >= 0 ) {

    glEnableVertexAttribArray( attributes["skinVertexA"] );
    glEnableVertexAttribArray( attributes["skinVertexB"] );
    glEnableVertexAttribArray( attributes["skinIndex"] );
    glEnableVertexAttribArray( attributes["skinWeight"] );

  }

  for ( auto& a : material.attributes ) {
    if ( a.second >= 0 ) {
      glEnableVertexAttribArray( a.second );
    }
  }

  if ( material.morphTargets ) {

    material.numSupportedMorphTargets = 0;

    std::string id, base = "morphTarget";

    for ( int i = 0; i < maxMorphTargets; i ++ ) {
      id = toString( base, i );
      if ( attributes[ id ] >= 0 ) {
        glEnableVertexAttribArray( attributes[ id ] );
        material.numSupportedMorphTargets ++;
      }
    }

  }

  if ( material.morphNormals ) {

    material.numSupportedMorphNormals = 0;

    const std::string base = "morphNormal";

    for ( int i = 0; i < maxMorphNormals; i ++ ) {
      auto id = toString( base, i );
      if ( attributes[ id ] >= 0 ) {
        glEnableVertexAttribArray( attributes[ id ] );
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
    glUseProgram( program.program );
    _currentProgram = &program;
    refreshMaterial = true;
  }

  if ( material.id != _currentMaterialId ) {
    _currentMaterialId = material.id;
    refreshMaterial = true;
  }

  if ( refreshMaterial || &camera != _currentCamera ) {
    glUniformMatrix4fv( p_uniforms["projectionMatrix"], 1, false, camera._projectionMatrixArray.data() );
    if ( &camera != _currentCamera ) _currentCamera = &camera;
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
    } else if ( material.type() == THREE::ParticleBasicMaterial ) {
      refreshUniformsParticle( m_uniforms, material );
    } else if ( material.type() == THREE::MeshPhongMaterial ) {
      refreshUniformsPhong( m_uniforms, material );
    } else if ( material.type() == THREE::MeshLambertMaterial ) {
      refreshUniformsLambert( m_uniforms, material );
    } else if ( material.type() == THREE::MeshDepthMaterial ) {
      m_uniforms["mNear"].value = camera.near;
      m_uniforms["mFar"].value = camera.far;
      m_uniforms["opacity"].value = material.opacity;
    } else if ( material.type() == THREE::MeshNormalMaterial ) {
      m_uniforms["opacity"].value = material.opacity;
    }

    if ( object.receiveShadow && ! material.shadowPass ) {
      refreshUniformsShadow( m_uniforms, lights );
    }

    // load common uniforms

    loadUniformsGeneric( program, material.uniformsList );

    // load material specific uniforms
    // (shader material also gets them for the sake of genericity)

    if ( material.type() == THREE::ShaderMaterial ||
         material.type() == THREE::MeshPhongMaterial ||
         material.envMap ) {

      if ( p_uniforms["cameraPosition"] != 0 ) {
        auto position = camera.matrixWorld.getPosition();
        glUniform3f( p_uniforms["cameraPosition"], position.x, position.y, position.z );
      }

    }

    if ( material.type() == THREE::MeshPhongMaterial ||
         material.type() == THREE::MeshLambertMaterial ||
         material.type() == THREE::ShaderMaterial ||
         material.skinning ) {

      if ( p_uniforms["viewMatrix"] != 0 ) {
        glUniformMatrix4fv( p_uniforms["viewMatrix"], 1, false, camera._viewMatrixArray.data() );
      }

    }
  }

  if ( material.skinning ) {
    if ( _supportsBoneTextures && object.useVertexTexture ) {
      if ( p_uniforms["boneTexture"] != 0  && object.boneTexture ) {
        // shadowMap texture array starts from 6
        // texture unit 12 should leave space for 6 shadowmaps
        auto textureUnit = 12;
        glUniform1i( p_uniforms["boneTexture"], textureUnit );
        setTexture( *object.boneTexture, textureUnit );
      }
    } else {
      if ( p_uniforms["boneGlobalMatrices"] != 0 ) {
        glUniformMatrix4fv( p_uniforms["boneGlobalMatrices"],
                            ( int )object.boneMatrices.size(),
                            false,
                            reinterpret_cast<const float*>( &object.boneMatrices[0] ) );
      }
    }

  }

  loadUniformsMatrices( p_uniforms, object );

  if ( p_uniforms["modelMatrix"] != 0 ) {

    glUniformMatrix4fv( p_uniforms["modelMatrix"], 1, false, object.matrixWorld.elements );

  }

  return program;

}


// Uniforms (refresh uniforms objects)

void GLRenderer::refreshUniformsCommon( Uniforms& uniforms, Material& material ) {

  uniforms["opacity"].value = material.opacity;

  if ( gammaInput ) {

    uniforms["diffuse"].value = Color().copyGammaToLinear( material.color );

  } else {

    uniforms["diffuse"].value = material.color;

  }

  uniforms["map"].texture = material.map.get();
  uniforms["lightMap"].texture = material.lightMap.get();
  uniforms["specularMap"].texture = material.specularMap.get();

  if ( material.bumpMap ) {

    uniforms["bumpMap"].texture = material.bumpMap.get();
    uniforms["bumpScale"].value = material.bumpScale;

  }

  // uv repeat and offset setting priorities
  //  1. color map
  //  2. specular map
  //  3. bump map

  Texture* uvScaleMap = nullptr;

  if ( material.map ) {

    uvScaleMap = material.map.get();

  } else if ( material.specularMap ) {

    uvScaleMap = material.specularMap.get();

  } else if ( material.bumpMap ) {

    uvScaleMap = material.bumpMap.get();

  }

  if ( uvScaleMap ) {

    const auto& offset = uvScaleMap->offset;
    const auto& repeat = uvScaleMap->repeat;

    uniforms["offsetRepeat"].value = Vector4( offset.x, offset.y, repeat.x, repeat.y );

  }

  uniforms["envMap"].texture = material.envMap.get();
  uniforms["flipEnvMap"].value = ( material.envMap && material.envMap->type() == THREE::GLRenderTargetCube ) ? 1 : -1;

  if ( gammaInput ) {

    //uniforms.reflectivity.value = material.reflectivity * material.reflectivity;
    uniforms["reflectivity"].value = material.reflectivity;

  } else {

    uniforms["reflectivity"].value = material.reflectivity;

  }

  uniforms["refractionRatio"].value = material.refractionRatio;
  uniforms["combine"].value         = ( int )material.combine;
  uniforms["useRefract"].value      = material.envMap && material.envMap->mapping == THREE::CubeRefractionMapping;

}

void GLRenderer::refreshUniformsLine( Uniforms& uniforms, Material& material ) {

  uniforms["diffuse"].value = material.color;
  uniforms["opacity"].value = material.opacity;

}

void GLRenderer::refreshUniformsParticle( Uniforms& uniforms, Material& material ) {

  uniforms["psColor"].value = material.color;
  uniforms["opacity"].value = material.opacity;
  uniforms["size"].value    = material.size;
  uniforms["scale"].value   = _height / 2.0f; // TODO: Cache

  uniforms["map"].texture = material.map.get();

}

void GLRenderer::refreshUniformsFog( Uniforms& uniforms, IFog& fog ) {

  uniforms["fogColor"].value = fog.color;

  if ( fog.type() == THREE::Fog ) {

    uniforms["fogNear"].value = static_cast<Fog&>( fog ).near;
    uniforms["fogFar"].value = static_cast<Fog&>( fog ).far;

  } else if ( fog.type() == THREE::FogExp2 ) {

    uniforms["fogDensity"].value = static_cast<FogExp2&>( fog ).density;

  }

}

void GLRenderer::refreshUniformsPhong( Uniforms& uniforms, Material& material ) {

  uniforms["shininess"].value = material.shininess;

  if ( gammaInput ) {

    uniforms["ambient"].value  = Color().copyGammaToLinear( material.ambient );
    uniforms["emissive"].value = Color().copyGammaToLinear( material.emissive );
    uniforms["specular"].value = Color().copyGammaToLinear( material.specular );

  } else {

    uniforms["ambient"].value  = material.ambient;
    uniforms["emissive"].value = material.emissive;
    uniforms["specular"].value = material.specular;

  }

  if ( material.wrapAround ) {

    uniforms["wrapRGB"].value = material.wrapRGB;

  }

}

void GLRenderer::refreshUniformsLambert( Uniforms& uniforms, Material& material ) {

  if ( gammaInput ) {

    uniforms["ambient"].value = Color().copyGammaToLinear( material.ambient );
    uniforms["emissive"].value = Color().copyGammaToLinear( material.emissive );

  } else {

    uniforms["ambient"].value  = material.ambient;
    uniforms["emissive"].value = material.emissive;

  }

  if ( material.wrapAround ) {

    uniforms["wrapRGB"].value = material.wrapRGB;

  }

}

void GLRenderer::refreshUniformsLights( Uniforms& uniforms, InternalLights& lights ) {

  uniforms["ambientLightColor"].value         = lights.ambient;

  uniforms["directionalLightColor"].value     = lights.directional.colors;
  uniforms["directionalLightDirection"].value = lights.directional.positions;

  uniforms["pointLightColor"].value           = lights.point.colors;
  uniforms["pointLightPosition"].value        = lights.point.positions;
  uniforms["pointLightDistance"].value        = lights.point.distances;

  uniforms["spotLightColor"].value            = lights.spot.colors;
  uniforms["spotLightPosition"].value         = lights.spot.positions;
  uniforms["spotLightDistance"].value         = lights.spot.distances;
  uniforms["spotLightDirection"].value        = lights.spot.directions;
  uniforms["spotLightAngle"].value            = lights.spot.angles;
  uniforms["spotLightExponent"].value         = lights.spot.exponents;

  uniforms["hemisphereLightSkyColor"].value   = lights.hemi.skyColors;
  uniforms["hemisphereLightGroundColor"].value = lights.hemi.groundColors;
  uniforms["hemisphereLightPosition"].value   = lights.hemi.positions;

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

        uniforms["shadowMap"].texture[ j ] = light.shadowMap;
        uniforms["shadowMapSize"].value[ j ] = light.shadowMapSize;

        uniforms["shadowMatrix"].value[ j ] = light.shadowMatrix;

        uniforms["shadowDarkness"].value[ j ] = light.shadowDarkness;
        uniforms["shadowBias"].value[ j ] = light.shadowBias;

        j ++;

      }

    }

  }
#endif

}

// Uniforms (load to GPU)

void GLRenderer::loadUniformsMatrices( UniformsIndices& uniforms, Object3D& object ) {

  glUniformMatrix4fv( uniforms["modelViewMatrix"], 1, false, object.glData._modelViewMatrix.elements );
  if ( contains( uniforms, "normalMatrix" ) ) {
    glUniformMatrix3fv( uniforms["normalMatrix"], 1, false, object.glData._normalMatrix.elements );
  }

}

void GLRenderer::loadUniformsGeneric( Program& program, UniformsList& uniforms ) {

  for ( size_t j = 0, jl = uniforms.size(); j < jl; j ++ ) {

    const auto& location = program.uniforms[ uniforms[ j ].second ];

    if ( location <= 0 ) continue;

    auto& uniform = *uniforms[ j ].first;

    uniform.load( location );

    if ( uniform.type == Uniform::t ) { // single THREE::Texture (2d or cube)

      if ( !uniform.texture ) continue;

      auto& texture = *uniform.texture;
      const auto& value = uniform.value.cast<int>();

      if ( texture.image.size() == 6 ) {

        setCubeTexture( texture, value );

      } else if ( texture.type() == THREE::GLRenderTargetCube ) {

        setCubeTextureDynamic( texture, value );

      } else {

        setTexture( texture, value );

      }

    }

  }

}

void GLRenderer::setupMatrices( Object3D& object, Camera& camera ) {

  object.glData._modelViewMatrix.multiply( camera.matrixWorldInverse, object.matrixWorld );
  object.glData._normalMatrix.getInverse( object.glData._modelViewMatrix );
  object.glData._normalMatrix.transpose();

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

  auto& dcolors     = zlights.directional.colors;
  auto& dpositions  = zlights.directional.positions;

  auto& pcolors     = zlights.point.colors;
  auto& ppositions  = zlights.point.positions;
  auto& pdistances  = zlights.point.distances;

  auto& scolors     = zlights.spot.colors;
  auto& spositions  = zlights.spot.positions;
  auto& sdistances  = zlights.spot.distances;
  auto& sdirections = zlights.spot.directions;
  auto& sangles     = zlights.spot.angles;
  auto& sexponents  = zlights.spot.exponents;

  auto& hskyColors    = zlights.hemi.skyColors;
  auto& hgroundColors = zlights.hemi.groundColors;
  auto& hpositions    = zlights.hemi.positions;

  int dlength = 0, plength = 0, slength = 0, hlength = 0;
  int doffset = 0, poffset = 0, soffset = 0, hoffset = 0;

  float r  = 0, g = 0, b = 0;

  for ( size_t l = 0, ll = lights.size(); l < ll; l ++ ) {

    auto& light = *lights[ l ];

    if ( light.onlyShadow || ! light.visible ) continue;

    const auto& color    = light.color;
    const auto intensity = light.intensity;
    const auto distance  = light.distance;

    if ( light.type() == THREE::AmbientLight ) {

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

      const auto doffset = dlength * 3;

      grow( dcolors, doffset + 3 );
      grow( dpositions, doffset + 3 );

      if ( gammaInput ) {

        dcolors[ doffset ]     = color.r * color.r * intensity * intensity;
        dcolors[ doffset + 1 ] = color.g * color.g * intensity * intensity;
        dcolors[ doffset + 2 ] = color.b * color.b * intensity * intensity;

      } else {

        dcolors[ doffset ]     = color.r * intensity;
        dcolors[ doffset + 1 ] = color.g * intensity;
        dcolors[ doffset + 2 ] = color.b * intensity;

      }

      Vector3 _direction = light.matrixWorld.getPosition();
      _direction.subSelf( light.target->matrixWorld.getPosition() );
      _direction.normalize();

      dpositions[ doffset ]     = _direction.x;
      dpositions[ doffset + 1 ] = _direction.y;
      dpositions[ doffset + 2 ] = _direction.z;

      dlength += 1;

    } else if ( light.type() == THREE::PointLight ) {

      poffset = plength * 3;

      grow( pcolors, poffset + 3 );
      grow( ppositions, poffset + 3 );
      grow( pdistances, plength + 1 );

      if ( gammaInput ) {

        pcolors[ poffset ]     = color.r * color.r * intensity * intensity;
        pcolors[ poffset + 1 ] = color.g * color.g * intensity * intensity;
        pcolors[ poffset + 2 ] = color.b * color.b * intensity * intensity;

      } else {

        pcolors[ poffset ]     = color.r * intensity;
        pcolors[ poffset + 1 ] = color.g * intensity;
        pcolors[ poffset + 2 ] = color.b * intensity;

      }

      const auto& position = light.matrixWorld.getPosition();

      ppositions[ poffset ]     = position.x;
      ppositions[ poffset + 1 ] = position.y;
      ppositions[ poffset + 2 ] = position.z;

      pdistances[ plength ] = distance;

      plength += 1;

    } else if ( light.type() == THREE::SpotLight ) {

      auto& slight = static_cast<SpotLight&>( light );

      soffset = slength * 3;

      grow( scolors, soffset + 3 );
      grow( spositions, soffset + 3 );
      grow( sdistances, slength + 1 );

      if ( gammaInput ) {

        scolors[ soffset ]     = color.r * color.r * intensity * intensity;
        scolors[ soffset + 1 ] = color.g * color.g * intensity * intensity;
        scolors[ soffset + 2 ] = color.b * color.b * intensity * intensity;

      } else {

        scolors[ soffset ]     = color.r * intensity;
        scolors[ soffset + 1 ] = color.g * intensity;
        scolors[ soffset + 2 ] = color.b * intensity;

      }

      const auto& position = light.matrixWorld.getPosition();

      spositions[ soffset ]     = position.x;
      spositions[ soffset + 1 ] = position.y;
      spositions[ soffset + 2 ] = position.z;

      sdistances[ slength ] = distance;

      _direction.copy( position );
      _direction.subSelf( light.target->matrixWorld.getPosition() );
      _direction.normalize();

      sdirections[ soffset ]     = _direction.x;
      sdirections[ soffset + 1 ] = _direction.y;
      sdirections[ soffset + 2 ] = _direction.z;

      sangles[ slength ] = Math::cos( slight.angle );
      sexponents[ slength ] = slight.exponent;

      slength += 1;

    } else if ( light.type() == THREE::HemisphereLight ) {

      auto& hlight = static_cast<HemisphereLight&>( light );

      const auto& skyColor = hlight.color;
      const auto& groundColor = hlight.groundColor;

      hoffset = hlength * 3;

      grow( hpositions, hoffset + 3 );
      grow( hgroundColors, hoffset + 3 );
      grow( hskyColors, hoffset + 3 );

      if ( gammaInput ) {

        auto intensitySq = intensity * intensity;

        setColorGamma( hskyColors, hoffset, skyColor, intensitySq );
        setColorGamma( hgroundColors, hoffset, groundColor, intensitySq );

      } else {

        setColorLinear( hskyColors, hoffset, skyColor, intensity );
        setColorLinear( hgroundColors, hoffset, groundColor, intensity );

      }

      const auto& position = light.matrixWorld.getPosition();

      hpositions[ hoffset ]     = position.x;
      hpositions[ hoffset + 1 ] = position.y;
      hpositions[ hoffset + 2 ] = position.z;

      hlength += 1;

    }

  }

  // 0 eventual remains from removed lights
  // (this is to avoid if in shader)

  for ( size_t l = dlength * 3, ll = dcolors.size(); l < ll; l ++ ) dcolors[ l ] = 0.0;
  for ( size_t l = plength * 3, ll = pcolors.size(); l < ll; l ++ ) pcolors[ l ] = 0.0;
  for ( size_t l = slength * 3, ll = scolors.size(); l < ll; l ++ ) scolors[ l ] = 0.0;
  for ( size_t l = hlength * 3, ll = hskyColors.size(); l < ll; l ++ ) hskyColors[ l ] = 0.0;
  for ( size_t l = hlength * 3, ll = hgroundColors.size(); l < ll; l ++ ) hgroundColors[ l ] = 0.0;

  zlights.directional.length = dlength;
  zlights.point.length = plength;
  zlights.spot.length = slength;
  zlights.hemi.length = hlength;

  grow( zlights.ambient, 3 );
  zlights.ambient[ 0 ] = r;
  zlights.ambient[ 1 ] = g;
  zlights.ambient[ 2 ] = b;

}


// GL state setting

void GLRenderer::setFaceCulling( THREE::Side cullFace /*= THREE::NoSide*/, THREE::Dir frontFace /*= THREE::CCW*/ ) {

  if ( cullFace != THREE::NoSide ) {

    if ( frontFace == THREE::CCW ) {
      glFrontFace( GL_CCW );
    } else {
      glFrontFace( GL_CW );
    }

    if ( cullFace == THREE::BackSide ) {
      glCullFace( GL_BACK );
    } else if ( cullFace == THREE::FrontSide ) {
      glCullFace( GL_FRONT );
    } else {
      glCullFace( GL_FRONT_AND_BACK );
    }

    glEnable( GL_CULL_FACE );

  } else {

    glDisable( GL_CULL_FACE );

  }

}

void GLRenderer::setMaterialFaces( Material& material ) {

  auto doubleSided = toInt( material.side == THREE::DoubleSide );
  auto flipSided = toInt( material.side == THREE::BackSide );

  if ( _oldDoubleSided != doubleSided ) {

    if ( doubleSided ) {
      glDisable( GL_CULL_FACE );
    } else {
      glEnable( GL_CULL_FACE );
    }

    _oldDoubleSided = doubleSided;

  }

  if ( _oldFlipSided != flipSided ) {

    if ( flipSided ) {
      glFrontFace( GL_CW );
    } else {
      glFrontFace( GL_CCW );
    }

    _oldFlipSided = flipSided;

  }

}

void GLRenderer::setDepthTest( bool depthTest ) {

  if ( _oldDepthTest != toInt( depthTest ) ) {

    if ( depthTest ) {
      glEnable( GL_DEPTH_TEST );
    } else {
      glDisable( GL_DEPTH_TEST );
    }

    _oldDepthTest = toInt( depthTest );

  }

}

void GLRenderer::setDepthWrite( bool depthWrite ) {

  if ( _oldDepthWrite != toInt( depthWrite ) ) {
    glDepthMask( depthWrite );
    _oldDepthWrite = toInt( depthWrite );
  }

}

void GLRenderer::setLineWidth( float width ) {

  if ( width != _oldLineWidth ) {
    glLineWidth( width );
    _oldLineWidth = width;
  }

}

void GLRenderer::setPolygonOffset( bool polygonoffset, float factor, float units ) {

  if ( _oldPolygonOffset != toInt( polygonoffset ) ) {

    if ( polygonoffset ) {
      glEnable( GL_POLYGON_OFFSET_FILL );
    } else {
      glDisable( GL_POLYGON_OFFSET_FILL );
    }

    _oldPolygonOffset = toInt( polygonoffset );

  }

  if ( polygonoffset && ( _oldPolygonOffsetFactor != factor || _oldPolygonOffsetUnits != units ) ) {
    glPolygonOffset( factor, units );
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

      glDisable( GL_BLEND );

    } else if ( blending == THREE::AdditiveBlending ) {

      glEnable( GL_BLEND );
      glBlendEquation( GL_FUNC_ADD );
      glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    } else if ( blending == THREE::SubtractiveBlending ) {

      // TODO: Find blendFuncSeparate() combination
      glEnable( GL_BLEND );
      glBlendEquation( GL_FUNC_ADD );
      glBlendFunc( GL_ZERO, GL_ONE_MINUS_SRC_COLOR );

    } else if ( blending == THREE::MultiplyBlending ) {

      // TODO: Find blendFuncSeparate() combination
      glEnable( GL_BLEND );
      glBlendEquation( GL_FUNC_ADD );
      glBlendFunc( GL_ZERO, GL_SRC_COLOR );

    } else if ( blending == THREE::CustomBlending ) {

      glEnable( GL_BLEND );

    } else {

      glEnable( GL_BLEND );
      glBlendEquationSeparate( GL_FUNC_ADD, GL_FUNC_ADD );
      glBlendFuncSeparate( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA );

    }

    _oldBlending = blending;

  }

  if ( blending == THREE::CustomBlending ) {

    if ( blendEquation != _oldBlendEquation ) {
      glBlendEquation( paramThreeToGL( blendEquation ) );
      _oldBlendEquation = blendEquation;
    }

    if ( blendSrc != _oldBlendSrc || blendDst != _oldBlendDst ) {
      glBlendFunc( paramThreeToGL( blendSrc ), paramThreeToGL( blendDst ) );
      _oldBlendSrc = blendSrc;
      _oldBlendDst = blendDst;
    }

  } else {

    _oldBlendEquation = -1;
    _oldBlendSrc = -1;
    _oldBlendDst = -1;

  }

}

// Shaders

Program::Ptr GLRenderer::buildProgram( const std::string& shaderID,
                                       const std::string& fragmentShader,
                                       const std::string& vertexShader,
                                       const Uniforms& uniforms,
                                       const Attributes& attributes,
                                       ProgramParameters& parameters ) {


  // Generate code
  std::hash<std::string> hash;
  std::stringstream chunks;

  if ( !shaderID.empty() ) {
    chunks << shaderID;
  } else {
    chunks << hash( fragmentShader );
    chunks << hash( vertexShader );
  }

#ifdef TODO_HASH_PARAMETERS
  for ( const auto& p : parameters ) {
    chunks << p.first;
    chunks << p.second;
  }
#endif

  auto code = chunks.str();

  // Check if code has been already compiled

  for ( auto& programInfo : _programs ) {
    if ( programInfo.code == code ) {
      console().log( "Code already compiled." /*: \n\n" + code*/ );
      programInfo.usedTimes ++;
      return programInfo.program;
    }
  }

  //console().log( "building new program " );

  //

  auto glProgram = GL_CALL( glCreateProgram() );

  auto prefix_vertex = [this, &parameters]() -> std::string {
    std::stringstream ss;

    //ss << "precision " << _precision << " float;" << std::endl;

    if ( _supportsVertexTextures ) ss << "#define VERTEX_TEXTURES" << std::endl;

    if ( gammaInput ) ss << "#define GAMMA_INPUT" << std::endl;
    if ( gammaOutput ) ss << "#define GAMMA_OUTPUT" << std::endl;
    if ( physicallyBasedShading ) ss << "#define PHYSICALLY_BASED_SHADING" << std::endl;

    ss << "#define MAX_DIR_LIGHTS " << parameters.maxDirLights << std::endl <<
    "#define MAX_POINT_LIGHTS " << parameters.maxPointLights << std::endl <<
    "#define MAX_SPOT_LIGHTS " << parameters.maxSpotLights << std::endl <<
    "#define MAX_SHADOWS " << parameters.maxShadows << std::endl <<
    "#define MAX_BONES " << parameters.maxBones << std::endl;

    if ( parameters.map ) ss << "#define USE_MAP" << std::endl;
    if ( parameters.envMap ) ss << "#define USE_ENVMAP" << std::endl;
    if ( parameters.lightMap ) ss << "#define USE_LIGHTMAP" << std::endl;
    if ( parameters.bumpMap ) ss << "#define USE_BUMPMAP" << std::endl;
    if ( parameters.specularMap ) ss << "#define USE_SPECULARMAP" << std::endl;
    if ( parameters.vertexColors ) ss << "#define USE_COLOR" << std::endl;

    if ( parameters.skinning ) ss << "#define USE_SKINNING" << std::endl;
    if ( parameters.useVertexTexture ) ss << "#define BONE_TEXTURE" << std::endl;
    if ( parameters.boneTextureWidth ) ss << "#define N_BONE_PIXEL_X " << parameters.boneTextureWidth << std::endl;
    if ( parameters.boneTextureHeight ) ss << "#define N_BONE_PIXEL_Y " << parameters.boneTextureHeight << std::endl;

    if ( parameters.morphTargets ) ss << "#define USE_MORPHTARGETS" << std::endl;
    if ( parameters.morphNormals ) ss << "#define USE_MORPHNORMALS" << std::endl;
    if ( parameters.perPixel ) ss << "#define PHONG_PER_PIXEL" << std::endl;
    if ( parameters.wrapAround ) ss << "#define WRAP_AROUND" << std::endl;
    if ( parameters.doubleSided ) ss << "#define DOUBLE_SIDED" << std::endl;

    if ( parameters.shadowMapEnabled ) ss << "#define USE_SHADOWMAP" << std::endl;
    if ( parameters.shadowMapSoft ) ss << "#define SHADOWMAP_SOFT" << std::endl;
    if ( parameters.shadowMapDebug ) ss << "#define SHADOWMAP_DEBUG" << std::endl;
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

    "attribute vec4 skinVertexA;" << std::endl <<
    "attribute vec4 skinVertexB;" << std::endl <<
    "attribute vec4 skinIndex;" << std::endl <<
    "attribute vec4 skinWeight;" << std::endl <<

    "#endif" << std::endl;

    return ss.str();

  }();

  auto prefix_fragment = [this, &parameters]() -> std::string {
    std::stringstream ss;

    //ss << "precision " << _precision << " float;" << std::endl;

    if ( parameters.bumpMap ) ss << "#extension GL_OES_standard_derivatives : enable" << std::endl;

    ss << "#define MAX_DIR_LIGHTS " << parameters.maxDirLights << std::endl <<
    "#define MAX_POINT_LIGHTS " << parameters.maxPointLights << std::endl <<
    "#define MAX_SPOT_LIGHTS " << parameters.maxSpotLights << std::endl <<
    "#define MAX_SHADOWS " << parameters.maxShadows << std::endl;

    if ( parameters.alphaTest ) ss << "#define ALPHATEST " << parameters.alphaTest << std::endl;

    if ( gammaInput ) ss << "#define GAMMA_INPUT" << std::endl;
    if ( gammaOutput ) ss << "#define GAMMA_OUTPUT" << std::endl;
    if ( physicallyBasedShading ) ss << "#define PHYSICALLY_BASED_SHADING" << std::endl;

    if ( parameters.useFog && parameters.fog != nullptr ) ss << "#define USE_FOG" << std::endl;
    if ( parameters.useFog && parameters.fog != nullptr && parameters.fog->type() == THREE::FogExp2 ) ss << "#define FOG_EXP2" << std::endl;

    if ( parameters.map ) ss << "#define USE_MAP" <<  std::endl;
    if ( parameters.envMap ) ss << "#define USE_ENVMAP" <<  std::endl;
    if ( parameters.lightMap ) ss << "#define USE_LIGHTMAP" <<  std::endl;
    if ( parameters.bumpMap ) ss << "#define USE_BUMPMAP" <<  std::endl;
    if ( parameters.specularMap ) ss << "#define USE_SPECULARMAP" <<  std::endl;
    if ( parameters.vertexColors ) ss << "#define USE_COLOR" <<  std::endl;

    if ( parameters.metal ) ss << "#define METAL" <<  std::endl;
    if ( parameters.perPixel ) ss << "#define PHONG_PER_PIXEL" <<  std::endl;
    if ( parameters.wrapAround ) ss << "#define WRAP_AROUND" <<  std::endl;
    if ( parameters.doubleSided ) ss << "#define DOUBLE_SIDED" <<  std::endl;

    if ( parameters.shadowMapEnabled ) ss << "#define USE_SHADOWMAP" <<  std::endl;
    if ( parameters.shadowMapSoft ) ss << "#define SHADOWMAP_SOFT" <<  std::endl;
    if ( parameters.shadowMapDebug ) ss << "#define SHADOWMAP_DEBUG" <<  std::endl;
    if ( parameters.shadowMapCascade ) ss << "#define SHADOWMAP_CASCADE" <<  std::endl;

    ss << "uniform mat4 viewMatrix;" << std::endl <<
    "uniform vec3 cameraPosition;" << std::endl;

    return ss.str();

  }();

  auto glFragmentShader = getShader( THREE::ShaderFragment, prefix_fragment + fragmentShader );
  auto glVertexShader   = getShader( THREE::ShaderVertex,   prefix_vertex   + vertexShader );

  GL_CALL( glAttachShader( glProgram, glVertexShader ) );
  GL_CALL( glAttachShader( glProgram, glFragmentShader ) );

  GL_CALL( glLinkProgram( glProgram ) );

  if ( !glTrue( glGetProgramParameter( glProgram, GL_LINK_STATUS ) ) ) {

    console().error() << "Could not initialise shader\n"
                      << "VALIDATE_STATUS: " << glGetProgramParameter( glProgram, GL_VALIDATE_STATUS ) << ", gl error [" << glGetError() << "]";

    glDeleteProgram( glProgram );
    glProgram = 0;

  }

  // clean up

  glDeleteShader( glFragmentShader );
  glFragmentShader = 0;
  glDeleteShader( glVertexShader );
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

      "position", "normal", "uv", "uv2", "tangent", "color",
      "skinVertexA", "skinVertexB", "skinIndex", "skinWeight"

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
    program.uniforms[ id ] = GL_CALL( glGetUniformLocation( program.program, id.c_str() ) );
  }
}

void GLRenderer::cacheAttributeLocations( Program& program, const Identifiers& identifiers ) {
  for ( const auto& id : identifiers ) {
    program.attributes[ id ] = GL_CALL( glGetAttribLocation( program.program, id.c_str() ) );
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
    shader = GL_CALL( glCreateShader( GL_FRAGMENT_SHADER ) );
  } else if ( type == THREE::ShaderVertex ) {
    shader = GL_CALL( glCreateShader( GL_VERTEX_SHADER ) );
  }

  const char* source_str = source.c_str();
  GL_CALL( glShaderSource( shader, 1, &source_str, nullptr ) );
  GL_CALL( glCompileShader( shader ) );

  if ( !glTrue( glGetShaderParameter( shader, GL_COMPILE_STATUS ) ) ) {
    int loglen;
    char logbuffer[1000];
    glGetShaderInfoLog( shader, sizeof( logbuffer ), &loglen, logbuffer );
    console().error( logbuffer );
    console().error() << addLineNumbers( source );
    return 0;
  }

  return shader;

}


// Textures

void GLRenderer::setTexture( Texture& texture, int slot ) {

  if ( texture.needsUpdate ) {

    if ( ! texture.__glInit ) {

      texture.__glInit = true;
      texture.__glTexture = glCreateTexture();

      _info.memory.textures ++;

    }

    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, texture.__glTexture );

#ifdef TODO_glUnPACK
    glPixelStorei( GL_UNPACK_FLIP_Y_WEBGL, texture.flipY );
    glPixelStorei( GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, texture.premultiplyAlpha );
#endif

    const auto& image = texture.image[0];
    const auto isImagePowerOfTwo = Math::isPowerOfTwo( image.width ) && Math::isPowerOfTwo( image.height );
    const auto glFormat = paramThreeToGL( texture.format );
    const auto glType = paramThreeToGL( texture.dataType );

    setTextureParameters( GL_TEXTURE_2D, texture, isImagePowerOfTwo );

    //if ( texture.type() == THREE::DataTexture ) {

    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, image.width, image.height, 0, glFormat, glType, image.data.data() );

    //} else {

    //    glTexImage2D( GL_TEXTURE_2D, 0, glFormat, glFormat, glType, texture.image );

    //}

    if ( texture.generateMipmaps && isImagePowerOfTwo )
      glGenerateMipmap( GL_TEXTURE_2D );

    texture.needsUpdate = false;

    if ( texture.onUpdate ) texture.onUpdate();

  } else {

    glActiveTexture( GL_TEXTURE0 + slot );
    glBindTexture( GL_TEXTURE_2D, texture.__glTexture );

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

    if ( texture.needsUpdate ) {

      if ( ! texture.__glTextureCube ) {

        texture.__glTextureCube = glCreateTexture();

      }

      glActiveTexture( GL_TEXTURE0 + slot );
      glBindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTextureCube );

#ifdef TODO_IMAGE_SCALING
      glPixelStorei( GL_UNPACK_FLIP_Y, texture.flipY );

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
        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, cubeImage[ 0 ].width, cubeImage[ 0 ].height, 0, glFormat, glType, cubeImage[ i ].data.data() );

      }

      if ( texture.generateMipmaps && isImagePowerOfTwo ) {

        glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

      }

      texture.needsUpdate = false;

      if ( texture.onUpdate ) texture.onUpdate();

    } else {

      glActiveTexture( GL_TEXTURE0 + slot );
      glBindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTextureCube );

    }

  }

}

void GLRenderer::setCubeTextureDynamic( Texture& texture, int slot ) {

  glActiveTexture( GL_TEXTURE0 + slot );
  glBindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTexture );

}


// Render targets

void GLRenderer::setupFrameBuffer( Buffer framebuffer, GLRenderTarget& renderTarget, GLenum textureTarget ) {

  glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
  glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureTarget, renderTarget.__glTexture, 0 );

}

void GLRenderer::setupRenderBuffer( Buffer renderbuffer, GLRenderTarget& renderTarget ) {

  glBindRenderbuffer( GL_RENDERBUFFER, renderbuffer );

  if ( renderTarget.depthBuffer && ! renderTarget.stencilBuffer ) {

    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, renderTarget.width, renderTarget.height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );

    /* For some reason this is not working. Defaulting to RGBA4.
    } else if( ! renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

        glRenderbufferStorage( GLrenderBuffer, GL_STENCIL_INDEX8, renderTarget.width, renderTarget.height );
        glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GLrenderBuffer, renderbuffer );
    */
  } else if ( renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

    glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_STENCIL, renderTarget.width, renderTarget.height );
    glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer );

  } else {

    glRenderbufferStorage( GL_RENDERBUFFER, GL_RGBA4, renderTarget.width, renderTarget.height );

  }

}

void GLRenderer::setRenderTarget( GLRenderTarget::Ptr renderTarget ) {

  auto isCube = false;// TODO: ( renderTarget.type() == THREE::WebglRenderTargetCube );

  if ( renderTarget && renderTarget->__glFramebuffer.size() == 0 ) {

    renderTarget->__glTexture = glCreateTexture();

    // Setup texture, create render and frame buffers

    const auto isTargetPowerOfTwo = Math::isPowerOfTwo( renderTarget->width ) && Math::isPowerOfTwo( renderTarget->height );
    const auto glFormat = paramThreeToGL( renderTarget->format );
    const auto glType = paramThreeToGL( renderTarget->dataType );

    if ( isCube ) {

      renderTarget->__glFramebuffer.resize( 6 );
      renderTarget->__glRenderbuffer.resize( 6 );

      glBindTexture( GL_TEXTURE_CUBE_MAP, renderTarget->__glTexture );
      setTextureParameters( GL_TEXTURE_CUBE_MAP, *renderTarget, isTargetPowerOfTwo );

      for ( auto i = 0; i < 6; i ++ ) {

        renderTarget->__glFramebuffer[ i ] = glCreateFramebuffer();
        renderTarget->__glRenderbuffer[ i ] = glCreateRenderbuffer();

        glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, renderTarget->width, renderTarget->height, 0, glFormat, glType, 0 );

        setupFrameBuffer( renderTarget->__glFramebuffer[ i ], *renderTarget, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i );
        setupRenderBuffer( renderTarget->__glRenderbuffer[ i ], *renderTarget );

      }

      if ( isTargetPowerOfTwo ) glGenerateMipmap( GL_TEXTURE_CUBE_MAP );

    } else {

      renderTarget->__glFramebuffer.resize( 1 );
      renderTarget->__glRenderbuffer.resize( 1 );

      renderTarget->__glFramebuffer[ 0 ] = glCreateFramebuffer();
      renderTarget->__glRenderbuffer[ 0 ] = glCreateRenderbuffer();

      glBindTexture( GL_TEXTURE_2D, renderTarget->__glTexture );
      setTextureParameters( GL_TEXTURE_2D, *renderTarget, isTargetPowerOfTwo );

      glTexImage2D( GL_TEXTURE_2D, 0, glFormat, renderTarget->width, renderTarget->height, 0, glFormat, glType, 0 );

      setupFrameBuffer( renderTarget->__glFramebuffer[ 0 ], *renderTarget, GL_TEXTURE_2D );
      setupRenderBuffer( renderTarget->__glRenderbuffer[ 0 ], *renderTarget );

      if ( isTargetPowerOfTwo ) glGenerateMipmap( GL_TEXTURE_2D );

    }

    // Release everything

    if ( isCube ) {
      glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    } else {
      glBindTexture( GL_TEXTURE_2D, 0 );
    }

    glBindRenderbuffer( GL_RENDERBUFFER, 0 );
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

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

    glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
    glViewport( vx, vy, width, height );

    _currentFramebuffer = framebuffer;

  }

  _currentWidth = width;
  _currentHeight = height;

}

void GLRenderer::updateRenderTargetMipmap( GLRenderTarget& renderTarget ) {

  if ( renderTarget.type() == THREE::GLRenderTargetCube ) {

    glBindTexture( GL_TEXTURE_CUBE_MAP, renderTarget.__glTexture );
    glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
    glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

  } else {

    glBindTexture( GL_TEXTURE_2D, renderTarget.__glTexture );
    glGenerateMipmap( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, 0 );

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

  case THREE::RepeatWrapping: return GL_REPEAT;
  case THREE::ClampToEdgeWrapping: return GL_CLAMP_TO_EDGE;
  case THREE::MirroredRepeatWrapping: return GL_MIRRORED_REPEAT;

  case THREE::NearestFilter: return GL_NEAREST;
  case THREE::NearestMipMapNearestFilter: return GL_NEAREST_MIPMAP_NEAREST;
  case THREE::NearestMipMapLinearFilter: return GL_NEAREST_MIPMAP_LINEAR;

  case THREE::LinearFilter: return GL_LINEAR;
  case THREE::LinearMipMapNearestFilter: return GL_LINEAR_MIPMAP_NEAREST;
  case THREE::LinearMipMapLinearFilter: return GL_LINEAR_MIPMAP_LINEAR;

  case THREE::UnsignedByteType: return GL_UNSIGNED_BYTE;
  case THREE::UnsignedShort4444Type: return GL_UNSIGNED_SHORT_4_4_4_4;
  case THREE::UnsignedShort5551Type: return GL_UNSIGNED_SHORT_5_5_5_1;
  case THREE::UnsignedShort565Type: return GL_UNSIGNED_SHORT_5_6_5;

  case THREE::ByteType: return GL_BYTE;
  case THREE::ShortType: return GL_SHORT;
  case THREE::UnsignedShortType: return GL_UNSIGNED_SHORT;
  case THREE::IntType: return GL_INT;
  case THREE::UnsignedIntType: return GL_UNSIGNED_INT;
  case THREE::FloatType: return GL_FLOAT;

  case THREE::AlphaFormat: return GL_ALPHA;
  case THREE::RGBFormat: return GL_RGB;
  case THREE::RGBAFormat: return GL_RGBA;
  case THREE::LuminanceFormat: return GL_LUMINANCE;
  case THREE::LuminanceAlphaFormat: return GL_LUMINANCE_ALPHA;

  case THREE::AddEquation: return GL_FUNC_ADD;
  case THREE::SubtractEquation: return GL_FUNC_SUBTRACT;
  case THREE::ReverseSubtractEquation: return GL_FUNC_REVERSE_SUBTRACT;

  case THREE::ZeroFactor: return GL_ZERO;
  case THREE::OneFactor: return GL_ONE;
  case THREE::SrcColorFactor: return GL_SRC_COLOR;
  case THREE::OneMinusSrcColorFactor: return GL_ONE_MINUS_SRC_COLOR;
  case THREE::SrcAlphaFactor: return GL_SRC_ALPHA;
  case THREE::OneMinusSrcAlphaFactor: return GL_ONE_MINUS_SRC_ALPHA;
  case THREE::DstAlphaFactor: return GL_DST_ALPHA;
  case THREE::OneMinusDstAlphaFactor: return GL_ONE_MINUS_DST_ALPHA;

  case THREE::DstColorFactor: return GL_DST_COLOR;
  case THREE::OneMinusDstColorFactor: return GL_ONE_MINUS_DST_COLOR;
  case THREE::SrcAlphaSaturateFactor: return GL_SRC_ALPHA_SATURATE;

  default: return 0;

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
      hemiLights = 0,
      maxDirLights = 0,
      maxPointLights = 0,
      maxSpotLights = 0,
      maxHemiLights = 0;

  for ( const auto& light : lights ) {
    if ( light->onlyShadow ) continue;

    if ( light->type() == THREE::DirectionalLight ) dirLights ++;
    if ( light->type() == THREE::PointLight ) pointLights ++;
    if ( light->type() == THREE::SpotLight ) spotLights ++;
    if ( light->type() == THREE::SpotLight ) hemiLights ++;
  }

  if ( ( pointLights + spotLights + dirLights ) <= _maxLights ) {

    maxDirLights = dirLights;
    maxPointLights = pointLights;
    maxSpotLights = spotLights;
    maxHemiLights = hemiLights;

  } else {

    maxDirLights = ( int )Math::ceil( ( float )_maxLights * dirLights / ( pointLights + dirLights ) );
    maxPointLights = _maxLights - maxDirLights;

    // these are not really correct

    maxSpotLights = maxPointLights;
    maxHemiLights = maxDirLights;

  }

  LightCount lightCount = { maxDirLights, maxPointLights, maxSpotLights, maxHemiLights };
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

#endif // THREE_GL_RENDERER_IPP