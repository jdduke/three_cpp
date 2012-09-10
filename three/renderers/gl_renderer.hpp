#ifndef THREE_GL_RENDERER
#define THREE_GL_RENDERER

#include <three/common.hpp>

#include <three/gl.hpp>
#include <three/core/geometry.hpp>
#include <three/core/geometry_group.hpp>

namespace three {

class GLRenderTarget;

struct ExtractMaterialAndGeometry : public Visitor {

	ExtractMaterialAndGeometry()
	 : material ( nullptr ), geometry ( nullptr ) { }

	//virtual void operator() (Particle& object) { extract( object ); }
	// TODO: virtual void operator() (Sprite&   object) { material = object.material.get(); }
	virtual void operator() (Mesh&     object) { extract( object ); }
	virtual void operator() (Line&     object) { extract( object ); }

	template < typename Object >
	void extract ( Object& object ) {
		material = object.material.get();
		geometry = object.geometry.get();
	}

	Material* material;
	Geometry* geometry;

};

class GLRenderer {
public:

	struct Parameters {
		Parameters()
		: width ( 1024 ), height ( 768 ),
		precision ( THREE::PrecisionHigh ),
		alpha ( true ),
		premultipliedAlpha ( true ),
		antialias ( false ),
		stencil ( true ),
		preserveDrawingBuffer ( false ),
		clearColor ( 0 ),
		clearAlpha ( 0 ),
		maxLights ( 4 ) { }

		int width, height;
		THREE::PrecisionType precision;
		bool alpha;
		bool premultipliedAlpha;
		bool antialias;
		bool stencil;
		bool preserveDrawingBuffer;
		Color clearColor;
		float clearAlpha;
		int maxLights;
	};

	typedef std::shared_ptr<GLRenderer> Ptr;

	static Ptr create( const Parameters& parameters = Parameters() ) {

		auto renderer = make_shared<GLRenderer>( parameters );

		renderer->initialize();

		return renderer;

	}


protected:

	THREE_DECL GLRenderer( const Parameters& parameters )
	: _width ( parameters.width ),
	_height ( parameters.height ),
	_precision ( parameters.precision ),
	_alpha ( parameters.alpha ),
	_premultipliedAlpha ( parameters.premultipliedAlpha ),
	_antialias ( parameters.antialias ),
	_stencil ( parameters.stencil ),
	_preserveDrawingBuffer ( parameters.preserveDrawingBuffer ),
	_clearColor ( parameters.clearColor ),
	_clearAlpha ( parameters.clearAlpha ),
	_maxLights ( parameters.maxLights ),
	context ( nullptr ),
	autoClear ( true ),
	autoClearColor ( true ),
	autoClearDepth ( true ),
	autoClearStencil ( true ),
	sortObjects ( true ),
	autoUpdateObjects ( true ),
	autoUpdateScene ( true ),
	gammaInput ( false ),
	gammaOutput ( false ),
	physicallyBasedShading ( false ),
	shadowMapEnabled ( false ),
	shadowMapAutoUpdate ( true ),
	shadowMapSoft ( true ),
	shadowMapCullFrontFaces ( true ),
	shadowMapDebug ( false ),
	shadowMapCascade ( false ),
	maxMorphTargets ( 8 ),
	maxMorphNormals ( 4 ),
	autoScaleCubemaps ( true ),
	_programs_counter ( 0 ),
	_currentProgram ( nullptr ),
	_currentFramebuffer ( nullptr ),
	_currentMaterialId ( -1 ),
	_currentGeometryGroupHash ( 0 ),
	_currentCamera ( ),
	_geometryGroupCounter ( 0 ),
	_oldDoubleSided ( -1 ),
	_oldFlipSided ( -1 ),
	_oldBlending ( -1 ),
	_oldBlendEquation ( -1 ),
	_oldBlendSrc ( -1 ),
	_oldBlendDst ( -1 ),
	_oldDepthTest ( -1 ),
	_oldDepthWrite ( -1 ),
	_oldPolygonOffset ( 0 ),
	_oldPolygonOffsetFactor ( 0 ),
	_oldPolygonOffsetUnits ( 0 ),
	_oldLineWidth ( 0 ),
	_viewportX ( 0 ),
	_viewportY ( 0 ),
	_viewportWidth ( 0 ),
	_viewportHeight ( 0 ),
	_currentWidth ( 0 ),
	_currentHeight ( 0 ),
	_lightsNeedUpdate ( true )

	{
		console().log() << "THREE::GLRenderer created";
	}

	int _width, _height;
	THREE::PrecisionType _precision;
	bool _alpha;
	bool _premultipliedAlpha;
	bool _antialias;
	bool _stencil;
	bool _preserveDrawingBuffer;
	Color _clearColor;
	float _clearAlpha;
	int _maxLights;

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

	// TODO: custom render plugins

	//renderPluginsPre = [];
	//renderPluginsPost = [];

private:

	// info

	struct Info {

		struct Memory {

			Memory()
			: programs ( 0 ), geometries ( 0 ), textures ( 0 ) { }

			int programs;
			int geometries;
			int textures;

		} memory;

		struct Render {

			Render()
			: calls ( 0 ), vertices ( 0 ), faces ( 0 ), points ( 0 ) { }

			int calls;
			int vertices;
			int faces;
			int points;

		} render;

	} _info;

	// internal properties

	// TODO: Type
	std::vector<void*> _programs;
	int _programs_counter;

	// internal state cache

	// TODO: Type
	void* _currentProgram;
	void* _currentFramebuffer;
	int _currentMaterialId;
	int _currentGeometryGroupHash;
	Camera::Ptr _currentCamera;
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
	int _oldPolygonOffsetFactor;
	int _oldPolygonOffsetUnits;

	int _oldLineWidth;

	int _viewportX;
	int _viewportY;
	int _viewportWidth;
	int _viewportHeight;
	int _currentWidth;
	int _currentHeight;

	// frustum

	Frustum _frustum;

	 // camera matrices cache

	Matrix4 _projScreenMatrix;
	Matrix4 _projScreenMatrixPS;

	Vector3 _vector3;

	// light arrays cache

	Vector3 _direction;

	bool _lightsNeedUpdate;

	struct Lights {

		std::array<int, 3> ambient;//: [ 0, 0, 0 ],

		struct Directional {
			int length;
			std::vector<Color> colors;
			std::vector<Vector3> positions;
		} directional;

		struct Point {
			int length;
			std::vector<Color> colors;
			std::vector<Vector3> positions;
			std::vector<float> distances;
		} point;

		struct Spot {
			int length;
			std::vector<Color> colors;
			std::vector<Vector3> positions;
			std::vector<float> distances;
			std::vector<Vector3> directions;
			std::vector<float> angles;
			std::vector<float> exponents;
		} spot;

	} _lights;

	void* _gl;

	//auto _glExtensionTextureFloat;
	//auto _glExtensionStandardDerivatives;
	//auto _glExtensionTextureFilterAnisotropic;

	// GPU capabilities

	int _maxVertexTextures;
	int	_maxTextureSize;
	int	_maxCubemapSize;

	int _maxAnisotropy;

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


	THREE_DECL void initialize() {

		console().log() << "THREE::GLRenderer initializing";

		//auto _gl;

		initGL();

		setDefaultGLState();

		//context = _gl;

		// GPU capabilities

		// TODO: Fix
		_maxVertexTextures = 4;//glGetTexParameter( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ),
		_maxTextureSize    = 8192;//glGetTexParameter( GL_MAX_TEXTURE_SIZE ),
		_maxCubemapSize    = 8192;//glGetTexParameter( GL_MAX_CUBE_MAP_TEXTURE_SIZE );
		_maxAnisotropy = 1;//glExtensionTextureFilterAnisotropic ? _gl.getParameter( _glExtensionTextureFilterAnisotropic.MAX_TEXTURE_MAX_ANISOTROPY_EXT ) : 0;
		_supportsVertexTextures = ( _maxVertexTextures > 0 );
		_supportsBoneTextures = false;//_supportsVertexTextures && _glExtensionTextureFloat;

		console().log() << "THREE::GLRenderer initialized";

	}

	THREE_DECL void initGL () {

		// TODO: Force client to initialize opengl

		/*
		if ( glload::LoadFunctions() == glload::LS_LOAD_FAILED ) {
			console().error( "Error loading OpenGL functions" );
		}*/

		/*
		_glExtensionTextureFloat = _gl.getExtension( 'OES_texture_float' );
		_glExtensionStandardDerivatives = _gl.getExtension( 'OES_standard_derivatives' );

		_glExtensionTextureFilterAnisotropic = _gl.getExtension( 'EXT_texture_filter_anisotropic' ) ||
											   _gl.getExtension( 'MOZ_EXT_texture_filter_anisotropic' ) ||
											   _gl.getExtension( 'WEBKIT_EXT_texture_filter_anisotropic' );

		if ( ! _glExtensionTextureFloat ) {

			console.log( 'THREE::WebGLRenderer: Float textures not supported.' );

		}

		if ( ! _glExtensionStandardDerivatives ) {

			console.log( 'THREE::WebGLRenderer: Standard derivatives not supported.' );

		}

		if ( ! _glExtensionTextureFilterAnisotropic ) {

			console.log( 'THREE::WebGLRenderer: Anisotropic texture filtering not supported.' );

		}
		*/

	}

	THREE_DECL void setDefaultGLState () {

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

public:

	// API

	void* getContext () {

		return _gl;

	}

	bool supportsVertexTextures () const {

		return _supportsVertexTextures;

	}

	int getMaxAnisotropy  () const {

		return _maxAnisotropy;

	}

	THREE_DECL void setSize ( int width, int height ) {

		// TODO: Implement
		_width = width;
		_height = height;

		setViewport( 0, 0, _width, _height );

	}

	THREE_DECL void setViewport ( int x = 0, int y = 0, int width = -1, int height = -1 ) {

		_viewportX = x;
		_viewportY = y;

		_viewportWidth  = width  != -1 ? width  : _width;
		_viewportHeight = height != -1 ? height : _height;

		glViewport( _viewportX, _viewportY, _viewportWidth, _viewportHeight );

	}

	THREE_DECL void setScissor ( int x, int y, int width, int height ) {

		glScissor( x, y, width, height );

	}

	THREE_DECL void enableScissorTest ( bool enable ) {

		enable ? glEnable( GL_SCISSOR_TEST ) : glDisable( GL_SCISSOR_TEST );

	}

	// Clearing

	THREE_DECL void setClearColorHex ( int hex, float alpha ) {

		_clearColor.setHex( hex );
		_clearAlpha = alpha;

		glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );

	}

	THREE_DECL void setClearColor ( Color color, float alpha ) {

		_clearColor.copy( color );
		_clearAlpha = alpha;

		glClearColor( _clearColor.r, _clearColor.g, _clearColor.b, _clearAlpha );

	}

	Color getClearColor () const {

		return _clearColor;

	}

	float getClearAlpha () const {

		return _clearAlpha;

	}

	THREE_DECL void clear ( bool color = true, bool depth = true, bool stencil = true ) {

		int bits = 0;

		if ( color )   bits |= GL_COLOR_BUFFER_BIT;
		if ( depth )   bits |= GL_DEPTH_BUFFER_BIT;
		if ( stencil ) bits |= GL_STENCIL_BUFFER_BIT;

		glClear( bits );

	}

	THREE_DECL void clearTarget ( GLRenderTarget& renderTarget, bool color = true, bool depth = true, bool stencil = true ) {

		// TODO: setRenderTarget( renderTarget );
		clear( color, depth, stencil );

	}

	// TODO: Implement

#if 0
	// Plugins

	addPostPlugin = function ( plugin ) {

		plugin.init( this );
		renderPluginsPost.push_back( plugin );

	};

	addPrePlugin = function ( plugin ) {

		plugin.init( this );
		renderPluginsPre.push_back( plugin );

	};
#endif

	// TODO: Implement

#if 0
	// Deallocation

	THREE_DECL void deallocateObject ( Object3D::Ptr object ) {

		if ( ! object.__glInit ) return;

		object.__glInit = false;

		delete object._modelViewMatrix;
		delete object._normalMatrix;

		delete object._normalMatrixArray;
		delete object._modelViewMatrixArray;
		delete object._modelMatrixArray;

		if ( object instanceof THREE::Mesh ) {

			for ( auto g in object.geometry.geometryGroups ) {

				deleteMeshBuffers( object.geometry.geometryGroups[ g ] );

			}

		} else if ( object instanceof THREE::Ribbon ) {

			deleteRibbonBuffers( object.geometry );

		} else if ( object instanceof THREE::Line ) {

			deleteLineBuffers( object.geometry );

		} else if ( object instanceof THREE::ParticleSystem ) {

			deleteParticleBuffers( object.geometry );

		}

	};

	THREE_DECL void deallocateTexture ( Texture::Ptr texture ) {

		if ( ! texture.__glInit ) return;

		texture.__glInit = false;
		glDeleteTexture( texture.__glTexture );

		_info.memory.textures --;

	};

	THREE_DECL void deallocateRenderTarget ( RenderTarget::Ptr renderTarget ) {

		if ( !renderTarget || ! renderTarget.__glTexture ) return;

		glDeleteTexture( renderTarget.__glTexture );

		if ( renderTarget instanceof THREE::WebGLRenderTargetCube ) {

			for ( auto i = 0; i < 6; i ++ ) {

				glDeleteFramebuffer( renderTarget.__glFramebuffer[ i ] );
				glDeleteRenderbuffer( renderTarget.__glRenderbuffer[ i ] );

			}

		} else {

			glDeleteFramebuffer( renderTarget.__glFramebuffer );
			glDeleteRenderbuffer( renderTarget.__glRenderbuffer );

		}

	};

	THREE_DECL void deallocateMaterial ( Material::Ptr material ) {

		auto program = material.program;

		if ( ! program ) return;

		material.program = undefined;

		// only deallocate GL program if this was the last use of shared program
		// assumed there is only single copy of any program in the _programs list
		// (that's how it's constructed)

		auto i, il, programInfo;
		auto deleteProgram = false;

		for ( i = 0, il = _programs.size(); i < il; i ++ ) {

			programInfo = _programs[ i ];

			if ( programInfo.program == program ) {

				programInfo.usedTimes --;

				if ( programInfo.usedTimes == 0 ) {

					deleteProgram = true;

				}

				break;

			}

		}

		if ( deleteProgram ) {

			// avoid using array.splice, this is costlier than creating new array from scratch

			auto newPrograms = [];

			for ( i = 0, il = _programs.size(); i < il; i ++ ) {

				programInfo = _programs[ i ];

				if ( programInfo.program !== program ) {

					newPrograms.push_back( programInfo );

				}

			}

			_programs = newPrograms;

			glDeleteProgram( program );

			_info.memory.programs --;

		}

	};

#endif

	// Rendering

	THREE_DECL void updateShadowMap ( const Scene::Ptr& scene, const Camera::Ptr& camera ) {

		_currentProgram = nullptr;
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

	THREE_DECL void createParticleBuffers ( Geometry& geometry ) {

		geometry.buffer.__glVertexBuffer = glCreateBuffer();
		geometry.buffer.__glColorBuffer = glCreateBuffer();

		_info.memory.geometries ++;

	}

	THREE_DECL void createLineBuffers ( Geometry& geometry ) {

		geometry.buffer.__glVertexBuffer = glCreateBuffer();
		geometry.buffer.__glColorBuffer  = glCreateBuffer();

		_info.memory.geometries ++;

	}

	THREE_DECL void createRibbonBuffers ( Geometry& geometry ) {

		geometry.buffer.__glVertexBuffer = glCreateBuffer();
		geometry.buffer.__glColorBuffer  = glCreateBuffer();

		_info.memory.geometries ++;

	}

	THREE_DECL void createMeshBuffers ( GeometryGroup& geometryGroup ) {

		geometryGroup.buffer.__glVertexBuffer  = glCreateBuffer();
		geometryGroup.buffer.__glNormalBuffer  = glCreateBuffer();
		geometryGroup.buffer.__glTangentBuffer = glCreateBuffer();
		geometryGroup.buffer.__glColorBuffer   = glCreateBuffer();
		geometryGroup.buffer.__glUVBuffer      = glCreateBuffer();
		geometryGroup.buffer.__glUV2Buffer     = glCreateBuffer();

		geometryGroup.buffer.__glSkinVertexABuffer = glCreateBuffer();
		geometryGroup.buffer.__glSkinVertexBBuffer = glCreateBuffer();
		geometryGroup.buffer.__glSkinIndicesBuffer = glCreateBuffer();
		geometryGroup.buffer.__glSkinWeightsBuffer = glCreateBuffer();

		geometryGroup.buffer.__glFaceBuffer = glCreateBuffer();
		geometryGroup.buffer.__glLineBuffer = glCreateBuffer();

		if ( geometryGroup.buffer.numMorphTargets > 0 ) {

			geometryGroup.buffer.__glMorphTargetsBuffers.clear();

			for ( int m = 0, ml = geometryGroup.buffer.numMorphTargets; m < ml; m ++ ) {

				geometryGroup.buffer.__glMorphTargetsBuffers.push_back( glCreateBuffer() );

			}

		}

		if ( geometryGroup.buffer.numMorphNormals > 0 ) {

			geometryGroup.buffer.__glMorphNormalsBuffers.clear();

			for ( int m = 0, ml = geometryGroup.buffer.numMorphNormals; m < ml; m ++ ) {

				geometryGroup.buffer.__glMorphNormalsBuffers.push_back( glCreateBuffer() );

			}

		}

		_info.memory.geometries ++;

	}

	// Buffer deallocation

	THREE_DECL void deleteParticleBuffers ( Geometry& geometry ) {

		glDeleteBuffer( geometry.buffer.__glVertexBuffer );
		glDeleteBuffer( geometry.buffer.__glColorBuffer );

		_info.memory.geometries --;

	}

	THREE_DECL void deleteLineBuffers ( Geometry& geometry ) {

		glDeleteBuffer( geometry.buffer.__glVertexBuffer );
		glDeleteBuffer( geometry.buffer.__glColorBuffer );

		_info.memory.geometries --;

	}

	THREE_DECL void deleteRibbonBuffers ( Geometry& geometry ) {

		glDeleteBuffer( geometry.buffer.__glVertexBuffer );
		glDeleteBuffer( geometry.buffer.__glColorBuffer );

		_info.memory.geometries --;

	}

	THREE_DECL void deleteMeshBuffers ( GeometryGroup& geometryGroup ) {

		glDeleteBuffer( geometryGroup.buffer.__glVertexBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glNormalBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glTangentBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glColorBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glUVBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glUV2Buffer );

		glDeleteBuffer( geometryGroup.buffer.__glSkinVertexABuffer );
		glDeleteBuffer( geometryGroup.buffer.__glSkinVertexBBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glSkinIndicesBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glSkinWeightsBuffer );

		glDeleteBuffer( geometryGroup.buffer.__glFaceBuffer );
		glDeleteBuffer( geometryGroup.buffer.__glLineBuffer );

		if ( geometryGroup.buffer.numMorphTargets > 0 ) {

			for ( int m = 0, ml = geometryGroup.buffer.numMorphTargets; m < ml; m ++ ) {

				glDeleteBuffer( geometryGroup.buffer.__glMorphTargetsBuffers[ m ] );

			}

		}

		if ( geometryGroup.buffer.numMorphNormals > 0 ) {

			for ( int m = 0, ml = geometryGroup.buffer.numMorphNormals; m < ml; m ++ ) {

				glDeleteBuffer( geometryGroup.buffer.__glMorphNormalsBuffers[ m ] );

			}

		}


#ifdef TODO_CUSTOM_ATTRIBUTE_LIST
		if ( geometryGroup.buffer.__glCustomAttributesList ) {

			for ( auto id in geometryGroup.buffer.__glCustomAttributesList ) {

				glDeleteBuffer( geometryGroup.buffer.__glCustomAttributesList[ id ].buffer );

			}

		}

		_info.memory.geometries --;
#endif // TODO_CUSTOM_ATTRIBUTE_LIST

	}




	// Buffer initialization

	THREE_DECL void initCustomAttributes ( Geometry& geometry, Object3D& object ) {

	#ifdef TODO_CUSTOM_ATTRIBUTE_LIST
		auto nvertices = geometry.vertices.size();

		auto material = object.material;

		if ( material.attributes ) {

			if ( geometry.buffer.__glCustomAttributesList == undefined ) {

				geometry.buffer.__glCustomAttributesList = [];

			}

			for ( auto a in material.attributes ) {

				auto attribute = material.attributes[ a ];

				if( !attribute.__glInitialized || attribute.createUniqueBuffers ) {

					attribute.__glInitialized = true;

					auto size = 1;		// "f" and "i"

					if ( attribute.type == "v2" ) size = 2;
					else if ( attribute.type == "v3" ) size = 3;
					else if ( attribute.type == "v4" ) size = 4;
					else if ( attribute.type == "c"  ) size = 3;

					attribute.size = size;

					attribute.array.resize( nvertices * size );

					attribute.buffer = glCreateBuffer();
					attribute.buffer.belongsToAttribute = a;

					attribute.needsUpdate = true;

				}

				geometry.buffer.__glCustomAttributesList.push_back( attribute );

			}

		}

#endif //TODO_CUSTOM_ATTRIBUTE_LIST

	}

	THREE_DECL void initParticleBuffers ( Geometry& geometry, Object3D& object ) {

		auto nvertices = geometry.vertices.size();

		geometry.buffer.__vertexArray.resize( nvertices * 3 );
		geometry.buffer.__colorArray.resize( nvertices * 3 );

		geometry.buffer.__sortArray.clear();

		geometry.buffer.__glParticleCount = nvertices;

		initCustomAttributes ( geometry, object );

	};

	THREE_DECL void initLineBuffers ( Geometry& geometry, Object3D& object ) {

		auto nvertices = geometry.vertices.size();

		geometry.buffer.__vertexArray.resize( nvertices * 3 );
		geometry.buffer.__colorArray.resize( nvertices * 3 );

		geometry.buffer.__glLineCount = nvertices;

		initCustomAttributes ( geometry, object );

	}

	THREE_DECL void initRibbonBuffers ( Geometry& geometry ) {

		auto nvertices = geometry.vertices.size();

		geometry.buffer.__vertexArray.resize( nvertices * 3 );
		geometry.buffer.__colorArray.resize( nvertices * 3 );

		geometry.buffer.__glVertexCount = nvertices;

	}

	THREE_DECL void initMeshBuffers ( GeometryGroup& geometryGroup, Mesh& object ) {

		auto& geometry = *object.geometry;
		auto& faces3 = geometryGroup.faces3;
		auto& faces4 = geometryGroup.faces4;

		auto nvertices = faces3.size() * 3 + faces4.size() * 4;
		auto ntris     = faces3.size() * 1 + faces4.size() * 2;
		auto nlines    = faces3.size() * 3 + faces4.size() * 4;

		auto material = getBufferMaterial( object, geometryGroup );

		auto uvType = bufferGuessUVType( material );
		auto normalType = bufferGuessNormalType( material );
		auto vertexColorType = bufferGuessVertexColorType( material );

		//console.log( "uvType", uvType, "normalType", normalType, "vertexColorType", vertexColorType, object, geometryGroup, material );

		geometryGroup.buffer.__vertexArray.resize( nvertices * 3 );

		if ( normalType ) {

			geometryGroup.buffer.__normalArray.resize( nvertices * 3 );

		}

		if ( geometry.hasTangents ) {

			geometryGroup.buffer.__tangentArray.resize( nvertices * 4 );

		}

		if ( vertexColorType ) {

			geometryGroup.buffer.__colorArray.resize( nvertices * 3 );

		}

		if ( uvType ) {

			if ( geometry.faceUvs.size() > 0 || geometry.faceVertexUvs.size() > 0 ) {

				geometryGroup.buffer.__uvArray.resize( nvertices * 2 );

			}

			if ( geometry.faceUvs.size() > 1 || geometry.faceVertexUvs.size() > 1 ) {

				geometryGroup.buffer.__uv2Array.resize( nvertices * 2 );

			}

		}

		if ( geometry.skinWeights.size() && geometry.skinIndices.size() ) {

			geometryGroup.buffer.__skinVertexAArray.resize( nvertices * 4 );
			geometryGroup.buffer.__skinVertexBArray.resize( nvertices * 4 );
			geometryGroup.buffer.__skinIndexArray.resize( nvertices * 4 );
			geometryGroup.buffer.__skinWeightArray.resize( nvertices * 4 );

		}

		geometryGroup.buffer.__faceArray.resize( ntris * 3 );
		geometryGroup.buffer.__lineArray.resize( nlines * 2 );

		if ( geometryGroup.buffer.numMorphTargets ) {

			geometryGroup.__morphTargetsArrays.clear();

			for ( int m = 0, ml = geometryGroup.buffer.numMorphTargets; m < ml; m ++ ) {

				geometryGroup.__morphTargetsArrays.push_back( std::vector<float>( nvertices * 3 ) );

			}

		}

		if ( geometryGroup.buffer.numMorphNormals ) {

			geometryGroup.__morphNormalsArrays.clear();

			for ( int m = 0, ml = geometryGroup.buffer.numMorphNormals; m < ml; m ++ ) {

				geometryGroup.__morphNormalsArrays.push_back( std::vector<float>( nvertices * 3 ) );

			}

		}

		geometryGroup.buffer.__glFaceCount = ntris * 3;
		geometryGroup.buffer.__glLineCount = nlines * 2;


#ifdef TODO_CUSTOM_ATTRIBUTE_LIST

		// custom attributes

		if ( material.attributes ) {

			if ( geometryGroup.buffer.__glCustomAttributesList == undefined ) {

				geometryGroup.buffer.__glCustomAttributesList = [];

			}

			for ( auto a in material.attributes ) {

				// Do a shallow copy of the attribute object so different geometryGroup chunks use different
				// attribute buffers which are correctly indexed in the setMeshBuffers function

				auto originalAttribute = material.attributes[ a ];

				auto attribute = {};

				for ( auto property in originalAttribute ) {

					attribute[ property ] = originalAttribute[ property ];

				}

				if( !attribute.__glInitialized || attribute.createUniqueBuffers ) {

					attribute.__glInitialized = true;

					auto size = 1;		// "f" and "i"

					if( attribute.type == "v2" ) size = 2;
					else if( attribute.type == "v3" ) size = 3;
					else if( attribute.type == "v4" ) size = 4;
					else if( attribute.type == "c"  ) size = 3;

					attribute.size = size;

					attribute.array.resize( nvertices * size );

					attribute.buffer = glCreateBuffer();
					attribute.buffer.belongsToAttribute = a;

					originalAttribute.needsUpdate = true;
					attribute.__original = originalAttribute;

				}

				geometryGroup.buffer.__glCustomAttributesList.push_back( attribute );

			}

		}

		#endif

		geometryGroup.buffer.__inittedArrays = true;

	}

	THREE_DECL Material* getBufferMaterial( Object3D& object, GeometryGroup& geometryGroup ) {

		ExtractMaterialAndGeometry extract;

		object.visit ( extract );

		Material* material = extract.material;
		Geometry* geometry = extract.geometry;

		if ( material && ! ( material->type() == THREE::MeshFaceMaterial ) ) {

			return material;

		} else if ( geometry && geometryGroup.materialIndex >= 0 ) {

			return geometry->materials[ geometryGroup.materialIndex ].get();

		}

		return nullptr;

	}

	THREE_DECL bool materialNeedsSmoothNormals ( const Material* material ) {

		return material && material->shading == THREE::SmoothShading;

	}

	THREE_DECL THREE::Shading bufferGuessNormalType ( const Material* material ) {

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

	THREE_DECL THREE::Colors bufferGuessVertexColorType ( const Material* material ) {

		if ( material ) {

			return material->vertexColors;

		}

		return THREE::NoColors;

	}

	THREE_DECL bool bufferGuessUVType ( const Material* material ) {

		// material must use some texture to require uvs

		//if ( material && ( material->map || material->lightMap || material->bumpMap || material->specularMap || material->type() == THREE::ShaderMaterial ) ) {
		if ( material && material->type() == THREE::ShaderMaterial ) {

			return true;

		}

		return false;

	}

	//
#ifdef TODO_ATTRIBUTES

	void initDirectBuffers( Geometry& geometry ) {

		auto a, attribute, type;

		for ( a in geometry.attributes ) {

			if ( a == "index" ) {

				type = _gl.ELEMENT_ARRAY_BUFFER;

			} else {

				type = _gl.ARRAY_BUFFER;

			}

			attribute = geometry.attributes[ a ];

			attribute.buffer = glCreateBuffer();

			glBindBuffer( type, attribute.buffer );
			glBufferData( type, attribute.array, _gl.STATIC_DRAW );

		}

	};

	// Buffer setting

	function setParticleBuffers ( geometry, hint, object ) {

		auto v, c, vertex, offset, index, color,

		vertices = geometry.vertices,
		vl = vertices.size(),

		colors = geometry.colors,
		cl = colors.size(),

		vertexArray = geometry.buffer.__vertexArray,
		colorArray = geometry.buffer.__colorArray,

		auto& sortArray = geometry.buffer.__sortArray,

		dirtyVertices = geometry.verticesNeedUpdate,
		dirtyElements = geometry.elementsNeedUpdate,
		dirtyColors = geometry.colorsNeedUpdate,

		customAttributes = geometry.buffer.__glCustomAttributesList,
		i, il,
		a, ca, cal, value,
		customAttribute;

		if ( object.sortParticles ) {

			_projScreenMatrixPS.copy( _projScreenMatrix );
			_projScreenMatrixPS.multiplySelf( object.matrixWorld );

			for ( v = 0; v < vl; v ++ ) {

				vertex = vertices[ v ];

				_vector3.copy( vertex );
				_projScreenMatrixPS.multiplyVector3( _vector3 );

				sortArray[ v ] = [ _vector3.z, v ];

			}

			sortArray.sort( function( a, b ) { return b[ 0 ] - a[ 0 ]; } );

			for ( v = 0; v < vl; v ++ ) {

				vertex = vertices[ sortArray[v][1] ];

				offset = v * 3;

				vertexArray[ offset ]     = vertex.x;
				vertexArray[ offset + 1 ] = vertex.y;
				vertexArray[ offset + 2 ] = vertex.z;

			}

			for ( c = 0; c < cl; c ++ ) {

				offset = c * 3;

				color = colors[ sortArray[c][1] ];

				colorArray[ offset ]     = color.r;
				colorArray[ offset + 1 ] = color.g;
				colorArray[ offset + 2 ] = color.b;

			}

			if ( customAttributes ) {

				for ( i = 0, il = customAttributes.size(); i < il; i ++ ) {

					customAttribute = customAttributes[ i ];

					if ( ! ( customAttribute.boundTo == undefined || customAttribute.boundTo == "vertices" ) ) continue;

					offset = 0;

					cal = customAttribute.value.size();

					if ( customAttribute.size == 1 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							index = sortArray[ ca ][ 1 ];

							customAttribute.array[ ca ] = customAttribute.value[ index ];

						}

					} else if ( customAttribute.size == 2 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							index = sortArray[ ca ][ 1 ];

							value = customAttribute.value[ index ];

							customAttribute.array[ offset ] 	= value.x;
							customAttribute.array[ offset + 1 ] = value.y;

							offset += 2;

						}

					} else if ( customAttribute.size == 3 ) {

						if ( customAttribute.type == "c" ) {

							for ( ca = 0; ca < cal; ca ++ ) {

								index = sortArray[ ca ][ 1 ];

								value = customAttribute.value[ index ];

								customAttribute.array[ offset ]     = value.r;
								customAttribute.array[ offset + 1 ] = value.g;
								customAttribute.array[ offset + 2 ] = value.b;

								offset += 3;

							}

						} else {

							for ( ca = 0; ca < cal; ca ++ ) {

								index = sortArray[ ca ][ 1 ];

								value = customAttribute.value[ index ];

								customAttribute.array[ offset ] 	= value.x;
								customAttribute.array[ offset + 1 ] = value.y;
								customAttribute.array[ offset + 2 ] = value.z;

								offset += 3;

							}

						}

					} else if ( customAttribute.size == 4 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							index = sortArray[ ca ][ 1 ];

							value = customAttribute.value[ index ];

							customAttribute.array[ offset ]      = value.x;
							customAttribute.array[ offset + 1  ] = value.y;
							customAttribute.array[ offset + 2  ] = value.z;
							customAttribute.array[ offset + 3  ] = value.w;

							offset += 4;

						}

					}

				}

			}

		} else {

			if ( dirtyVertices ) {

				for ( v = 0; v < vl; v ++ ) {

					vertex = vertices[ v ];

					offset = v * 3;

					vertexArray[ offset ]     = vertex.x;
					vertexArray[ offset + 1 ] = vertex.y;
					vertexArray[ offset + 2 ] = vertex.z;

				}

			}

			if ( dirtyColors ) {

				for ( c = 0; c < cl; c ++ ) {

					color = colors[ c ];

					offset = c * 3;

					colorArray[ offset ]     = color.r;
					colorArray[ offset + 1 ] = color.g;
					colorArray[ offset + 2 ] = color.b;

				}

			}

			if ( customAttributes ) {

				for ( i = 0, il = customAttributes.size(); i < il; i ++ ) {

					customAttribute = customAttributes[ i ];

					if ( customAttribute.needsUpdate &&
						 ( customAttribute.boundTo == undefined ||
						   customAttribute.boundTo == "vertices") ) {

						cal = customAttribute.value.size();

						offset = 0;

						if ( customAttribute.size == 1 ) {

							for ( ca = 0; ca < cal; ca ++ ) {

								customAttribute.array[ ca ] = customAttribute.value[ ca ];

							}

						} else if ( customAttribute.size == 2 ) {

							for ( ca = 0; ca < cal; ca ++ ) {

								value = customAttribute.value[ ca ];

								customAttribute.array[ offset ] 	= value.x;
								customAttribute.array[ offset + 1 ] = value.y;

								offset += 2;

							}

						} else if ( customAttribute.size == 3 ) {

							if ( customAttribute.type == "c" ) {

								for ( ca = 0; ca < cal; ca ++ ) {

									value = customAttribute.value[ ca ];

									customAttribute.array[ offset ] 	= value.r;
									customAttribute.array[ offset + 1 ] = value.g;
									customAttribute.array[ offset + 2 ] = value.b;

									offset += 3;

								}

							} else {

								for ( ca = 0; ca < cal; ca ++ ) {

									value = customAttribute.value[ ca ];

									customAttribute.array[ offset ] 	= value.x;
									customAttribute.array[ offset + 1 ] = value.y;
									customAttribute.array[ offset + 2 ] = value.z;

									offset += 3;

								}

							}

						} else if ( customAttribute.size == 4 ) {

							for ( ca = 0; ca < cal; ca ++ ) {

								value = customAttribute.value[ ca ];

								customAttribute.array[ offset ]      = value.x;
								customAttribute.array[ offset + 1  ] = value.y;
								customAttribute.array[ offset + 2  ] = value.z;
								customAttribute.array[ offset + 3  ] = value.w;

								offset += 4;

							}

						}

					}

				}

			}

		}

		if ( dirtyVertices || object.sortParticles ) {

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glVertexBuffer );
			glBufferData( GL_ARRAY_BUFFER, vertexArray, hint );

		}

		if ( dirtyColors || object.sortParticles ) {

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glColorBuffer );
			glBufferData( GL_ARRAY_BUFFER, colorArray, hint );

		}

		if ( customAttributes ) {

			for ( i = 0, il = customAttributes.size(); i < il; i ++ ) {

				customAttribute = customAttributes[ i ];

				if ( customAttribute.needsUpdate || object.sortParticles ) {

					glBindBuffer( GL_ARRAY_BUFFER, customAttribute.buffer );
					glBufferData( GL_ARRAY_BUFFER, customAttribute.array, hint );

				}

			}

		}


	};

	function setLineBuffers ( geometry, hint ) {

		auto v, c, vertex, offset, color,

		vertices = geometry.vertices,
		colors = geometry.colors,
		vl = vertices.size(),
		cl = colors.size(),

		vertexArray = geometry.buffer.__vertexArray,
		colorArray = geometry.buffer.__colorArray,

		dirtyVertices = geometry.verticesNeedUpdate,
		dirtyColors = geometry.colorsNeedUpdate,

		customAttributes = geometry.buffer.__glCustomAttributesList,

		i, il,
		a, ca, cal, value,
		customAttribute;

		if ( dirtyVertices ) {

			for ( v = 0; v < vl; v ++ ) {

				vertex = vertices[ v ];

				offset = v * 3;

				vertexArray[ offset ]     = vertex.x;
				vertexArray[ offset + 1 ] = vertex.y;
				vertexArray[ offset + 2 ] = vertex.z;

			}

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glVertexBuffer );
			glBufferData( GL_ARRAY_BUFFER, vertexArray, hint );

		}

		if ( dirtyColors ) {

			for ( c = 0; c < cl; c ++ ) {

				color = colors[ c ];

				offset = c * 3;

				colorArray[ offset ]     = color.r;
				colorArray[ offset + 1 ] = color.g;
				colorArray[ offset + 2 ] = color.b;

			}

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glColorBuffer );
			glBufferData( GL_ARRAY_BUFFER, colorArray, hint );

		}

		if ( customAttributes ) {

			for ( i = 0, il = customAttributes.size(); i < il; i ++ ) {

				customAttribute = customAttributes[ i ];

				if ( customAttribute.needsUpdate &&
					 ( customAttribute.boundTo == undefined ||
					   customAttribute.boundTo == "vertices" ) ) {

					offset = 0;

					cal = customAttribute.value.size();

					if ( customAttribute.size == 1 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							customAttribute.array[ ca ] = customAttribute.value[ ca ];

						}

					} else if ( customAttribute.size == 2 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							value = customAttribute.value[ ca ];

							customAttribute.array[ offset ] 	= value.x;
							customAttribute.array[ offset + 1 ] = value.y;

							offset += 2;

						}

					} else if ( customAttribute.size == 3 ) {

						if ( customAttribute.type == "c" ) {

							for ( ca = 0; ca < cal; ca ++ ) {

								value = customAttribute.value[ ca ];

								customAttribute.array[ offset ] 	= value.r;
								customAttribute.array[ offset + 1 ] = value.g;
								customAttribute.array[ offset + 2 ] = value.b;

								offset += 3;

							}

						} else {

							for ( ca = 0; ca < cal; ca ++ ) {

								value = customAttribute.value[ ca ];

								customAttribute.array[ offset ] 	= value.x;
								customAttribute.array[ offset + 1 ] = value.y;
								customAttribute.array[ offset + 2 ] = value.z;

								offset += 3;

							}

						}

					} else if ( customAttribute.size == 4 ) {

						for ( ca = 0; ca < cal; ca ++ ) {

							value = customAttribute.value[ ca ];

							customAttribute.array[ offset ] 	 = value.x;
							customAttribute.array[ offset + 1  ] = value.y;
							customAttribute.array[ offset + 2  ] = value.z;
							customAttribute.array[ offset + 3  ] = value.w;

							offset += 4;

						}

					}

					glBindBuffer( GL_ARRAY_BUFFER, customAttribute.buffer );
					glBufferData( GL_ARRAY_BUFFER, customAttribute.array, hint );

				}

			}

		}

	}


	function setRibbonBuffers ( geometry, hint ) {

		auto v, c, vertex, offset, color,

		vertices = geometry.vertices,
		colors = geometry.colors,
		vl = vertices.size(),
		cl = colors.size(),

		vertexArray = geometry.buffer.__vertexArray,
		colorArray = geometry.buffer.__colorArray,

		dirtyVertices = geometry.verticesNeedUpdate,
		dirtyColors = geometry.colorsNeedUpdate;

		if ( dirtyVertices ) {

			for ( v = 0; v < vl; v ++ ) {

				vertex = vertices[ v ];

				offset = v * 3;

				vertexArray[ offset ]     = vertex.x;
				vertexArray[ offset + 1 ] = vertex.y;
				vertexArray[ offset + 2 ] = vertex.z;

			}

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glVertexBuffer );
			glBufferData( GL_ARRAY_BUFFER, vertexArray, hint );

		}

		if ( dirtyColors ) {

			for ( c = 0; c < cl; c ++ ) {

				color = colors[ c ];

				offset = c * 3;

				colorArray[ offset ]     = color.r;
				colorArray[ offset + 1 ] = color.g;
				colorArray[ offset + 2 ] = color.b;

			}

			glBindBuffer( GL_ARRAY_BUFFER, geometry.buffer.__glColorBuffer );
			glBufferData( GL_ARRAY_BUFFER, colorArray, hint );

		}

	};

	function setMeshBuffers( geometryGroup, object, hint, dispose, material ) {

		if ( ! geometryGroup.buffer.__inittedArrays ) {

			// console.log( object );
			return;

		}

		auto normalType = bufferGuessNormalType( material ),
		vertexColorType = bufferGuessVertexColorType( material ),
		uvType = bufferGuessUVType( material ),

		needsSmoothNormals = ( normalType == THREE::SmoothShading );

		auto f, fl, fi, face,
		vertexNormals, faceNormal, normal,
		vertexColors, faceColor,
		vertexTangents,
		uv, uv2, v1, v2, v3, v4, t1, t2, t3, t4, n1, n2, n3, n4,
		c1, c2, c3, c4,
		sw1, sw2, sw3, sw4,
		si1, si2, si3, si4,
		sa1, sa2, sa3, sa4,
		sb1, sb2, sb3, sb4,
		m, ml, i, il,
		vn, uvi, uv2i,
		vk, vkl, vka,
		nka, chf, faceVertexNormals,
		a,

		vertexIndex = 0,

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
		offset_customSrc = 0,

		value,

		vertexArray = geometryGroup.buffer.__vertexArray,
		uvArray = geometryGroup.buffer.__uvArray,
		uv2Array = geometryGroup.buffer.__uv2Array,
		normalArray = geometryGroup.buffer.__normalArray,
		tangentArray = geometryGroup.buffer.__tangentArray,
		colorArray = geometryGroup.buffer.__colorArray,

		skinVertexAArray = geometryGroup.buffer.__skinVertexAArray,
		skinVertexBArray = geometryGroup.buffer.__skinVertexBArray,
		skinIndexArray = geometryGroup.buffer.__skinIndexArray,
		skinWeightArray = geometryGroup.buffer.__skinWeightArray,

		morphTargetsArrays = geometryGroup.__morphTargetsArrays,
		morphNormalsArrays = geometryGroup.__morphNormalsArrays,

		customAttributes = geometryGroup.buffer.__glCustomAttributesList,
		customAttribute,

		faceArray = geometryGroup.buffer.__faceArray,
		lineArray = geometryGroup.buffer.__lineArray,

		geometry = object.geometry, // this is shared for all chunks

		dirtyVertices = geometry.verticesNeedUpdate,
		dirtyElements = geometry.elementsNeedUpdate,
		dirtyUvs = geometry.uvsNeedUpdate,
		dirtyNormals = geometry.normalsNeedUpdate,
		dirtyTangents = geometry.tangentsNeedUpdate,
		dirtyColors = geometry.colorsNeedUpdate,
		dirtyMorphTargets = geometry.morphTargetsNeedUpdate,

		vertices = geometry.vertices,
		chunk_faces3 = geometryGroup.faces3,
		chunk_faces4 = geometryGroup.faces4,
		obj_faces = geometry.faces,

		obj_uvs  = geometry.faceVertexUvs[ 0 ],
		obj_uvs2 = geometry.faceVertexUvs[ 1 ],

		obj_colors = geometry.colors,

		obj_skinVerticesA = geometry.skinVerticesA,
		obj_skinVerticesB = geometry.skinVerticesB,
		obj_skinIndices = geometry.skinIndices,
		obj_skinWeights = geometry.skinWeights,

		morphTargets = geometry.morphTargets,
		morphNormals = geometry.morphNormals;

		if ( dirtyVertices ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ] ];

				v1 = vertices[ face.a ];
				v2 = vertices[ face.b ];
				v3 = vertices[ face.c ];

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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

				v1 = vertices[ face.a ];
				v2 = vertices[ face.b ];
				v3 = vertices[ face.c ];
				v4 = vertices[ face.d ];

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

			glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glVertexBuffer );
			glBufferData( GL_ARRAY_BUFFER, vertexArray, hint );

		}

		if ( dirtyMorphTargets ) {

			for ( vk = 0, vkl = morphTargets.size(); vk < vkl; vk ++ ) {

				offset_morphTarget = 0;

				for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

					chf = chunk_faces3[ f ];
					face = obj_faces[ chf ];

					// morph positions

					v1 = morphTargets[ vk ].vertices[ face.a ];
					v2 = morphTargets[ vk ].vertices[ face.b ];
					v3 = morphTargets[ vk ].vertices[ face.c ];

					vka = morphTargetsArrays[ vk ];

					vka[ offset_morphTarget ] 	  = v1.x;
					vka[ offset_morphTarget + 1 ] = v1.y;
					vka[ offset_morphTarget + 2 ] = v1.z;

					vka[ offset_morphTarget + 3 ] = v2.x;
					vka[ offset_morphTarget + 4 ] = v2.y;
					vka[ offset_morphTarget + 5 ] = v2.z;

					vka[ offset_morphTarget + 6 ] = v3.x;
					vka[ offset_morphTarget + 7 ] = v3.y;
					vka[ offset_morphTarget + 8 ] = v3.z;

					// morph normals

					if ( material.morphNormals ) {

						if ( needsSmoothNormals ) {

							faceVertexNormals = morphNormals[ vk ].vertexNormals[ chf ];

							n1 = faceVertexNormals.a;
							n2 = faceVertexNormals.b;
							n3 = faceVertexNormals.c;

						} else {

							n1 = morphNormals[ vk ].faceNormals[ chf ];
							n2 = n1;
							n3 = n1;

						}

						nka = morphNormalsArrays[ vk ];

						nka[ offset_morphTarget ] 	  = n1.x;
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

				for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

					chf = chunk_faces4[ f ];
					face = obj_faces[ chf ];

					// morph positions

					v1 = morphTargets[ vk ].vertices[ face.a ];
					v2 = morphTargets[ vk ].vertices[ face.b ];
					v3 = morphTargets[ vk ].vertices[ face.c ];
					v4 = morphTargets[ vk ].vertices[ face.d ];

					vka = morphTargetsArrays[ vk ];

					vka[ offset_morphTarget ] 	  = v1.x;
					vka[ offset_morphTarget + 1 ] = v1.y;
					vka[ offset_morphTarget + 2 ] = v1.z;

					vka[ offset_morphTarget + 3 ] = v2.x;
					vka[ offset_morphTarget + 4 ] = v2.y;
					vka[ offset_morphTarget + 5 ] = v2.z;

					vka[ offset_morphTarget + 6 ] = v3.x;
					vka[ offset_morphTarget + 7 ] = v3.y;
					vka[ offset_morphTarget + 8 ] = v3.z;

					vka[ offset_morphTarget + 9 ]  = v4.x;
					vka[ offset_morphTarget + 10 ] = v4.y;
					vka[ offset_morphTarget + 11 ] = v4.z;

					// morph normals

					if ( material.morphNormals ) {

						if ( needsSmoothNormals ) {

							faceVertexNormals = morphNormals[ vk ].vertexNormals[ chf ];

							n1 = faceVertexNormals.a;
							n2 = faceVertexNormals.b;
							n3 = faceVertexNormals.c;
							n4 = faceVertexNormals.d;

						} else {

							n1 = morphNormals[ vk ].faceNormals[ chf ];
							n2 = n1;
							n3 = n1;
							n4 = n1;

						}

						nka = morphNormalsArrays[ vk ];

						nka[ offset_morphTarget ] 	  = n1.x;
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

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphTargetsBuffers[ vk ] );
				glBufferData( GL_ARRAY_BUFFER, morphTargetsArrays[ vk ], hint );

				if ( material.morphNormals ) {

					glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphNormalsBuffers[ vk ] );
					glBufferData( GL_ARRAY_BUFFER, morphNormalsArrays[ vk ], hint );

				}

			}

		}

		if ( obj_skinWeights.size() ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ]	];

				// weights

				sw1 = obj_skinWeights[ face.a ];
				sw2 = obj_skinWeights[ face.b ];
				sw3 = obj_skinWeights[ face.c ];

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

				si1 = obj_skinIndices[ face.a ];
				si2 = obj_skinIndices[ face.b ];
				si3 = obj_skinIndices[ face.c ];

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

				sa1 = obj_skinVerticesA[ face.a ];
				sa2 = obj_skinVerticesA[ face.b ];
				sa3 = obj_skinVerticesA[ face.c ];

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

				sb1 = obj_skinVerticesB[ face.a ];
				sb2 = obj_skinVerticesB[ face.b ];
				sb3 = obj_skinVerticesB[ face.c ];

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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

				// weights

				sw1 = obj_skinWeights[ face.a ];
				sw2 = obj_skinWeights[ face.b ];
				sw3 = obj_skinWeights[ face.c ];
				sw4 = obj_skinWeights[ face.d ];

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

				si1 = obj_skinIndices[ face.a ];
				si2 = obj_skinIndices[ face.b ];
				si3 = obj_skinIndices[ face.c ];
				si4 = obj_skinIndices[ face.d ];

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

				sa1 = obj_skinVerticesA[ face.a ];
				sa2 = obj_skinVerticesA[ face.b ];
				sa3 = obj_skinVerticesA[ face.c ];
				sa4 = obj_skinVerticesA[ face.d ];

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

				sb1 = obj_skinVerticesB[ face.a ];
				sb2 = obj_skinVerticesB[ face.b ];
				sb3 = obj_skinVerticesB[ face.c ];
				sb4 = obj_skinVerticesB[ face.d ];

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

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinVertexABuffer );
				glBufferData( GL_ARRAY_BUFFER, skinVertexAArray, hint );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinVertexBBuffer );
				glBufferData( GL_ARRAY_BUFFER, skinVertexBArray, hint );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinIndicesBuffer );
				glBufferData( GL_ARRAY_BUFFER, skinIndexArray, hint );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinWeightsBuffer );
				glBufferData( GL_ARRAY_BUFFER, skinWeightArray, hint );

			}

		}

		if ( dirtyColors && vertexColorType ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ]	];

				vertexColors = face.vertexColors;
				faceColor = face.color;

				if ( vertexColors.size() == 3 && vertexColorType == THREE::VertexColors ) {

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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

				vertexColors = face.vertexColors;
				faceColor = face.color;

				if ( vertexColors.size() == 4 && vertexColorType == THREE::VertexColors ) {

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

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glColorBuffer );
				glBufferData( GL_ARRAY_BUFFER, colorArray, hint );

			}

		}

		if ( dirtyTangents && geometry.hasTangents ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ]	];

				vertexTangents = face.vertexTangents;

				t1 = vertexTangents[ 0 ];
				t2 = vertexTangents[ 1 ];
				t3 = vertexTangents[ 2 ];

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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

				vertexTangents = face.vertexTangents;

				t1 = vertexTangents[ 0 ];
				t2 = vertexTangents[ 1 ];
				t3 = vertexTangents[ 2 ];
				t4 = vertexTangents[ 3 ];

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

			glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glTangentBuffer );
			glBufferData( GL_ARRAY_BUFFER, tangentArray, hint );

		}

		if ( dirtyNormals && normalType ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ]	];

				vertexNormals = face.vertexNormals;
				faceNormal = face.normal;

				if ( vertexNormals.size() == 3 && needsSmoothNormals ) {

					for ( i = 0; i < 3; i ++ ) {

						vn = vertexNormals[ i ];

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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

				vertexNormals = face.vertexNormals;
				faceNormal = face.normal;

				if ( vertexNormals.size() == 4 && needsSmoothNormals ) {

					for ( i = 0; i < 4; i ++ ) {

						vn = vertexNormals[ i ];

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

			glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glNormalBuffer );
			glBufferData( GL_ARRAY_BUFFER, normalArray, hint );

		}

		if ( dirtyUvs && obj_uvs && uvType ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				fi = chunk_faces3[ f ];

				face = obj_faces[ fi ];
				uv = obj_uvs[ fi ];

				if ( uv == undefined ) continue;

				for ( i = 0; i < 3; i ++ ) {

					uvi = uv[ i ];

					uvArray[ offset_uv ]     = uvi.u;
					uvArray[ offset_uv + 1 ] = uvi.v;

					offset_uv += 2;

				}

			}

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				fi = chunk_faces4[ f ];

				face = obj_faces[ fi ];
				uv = obj_uvs[ fi ];

				if ( uv == undefined ) continue;

				for ( i = 0; i < 4; i ++ ) {

					uvi = uv[ i ];

					uvArray[ offset_uv ]     = uvi.u;
					uvArray[ offset_uv + 1 ] = uvi.v;

					offset_uv += 2;

				}

			}

			if ( offset_uv > 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glUVBuffer );
				glBufferData( GL_ARRAY_BUFFER, uvArray, hint );

			}

		}

		if ( dirtyUvs && obj_uvs2 && uvType ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				fi = chunk_faces3[ f ];

				face = obj_faces[ fi ];
				uv2 = obj_uvs2[ fi ];

				if ( uv2 == undefined ) continue;

				for ( i = 0; i < 3; i ++ ) {

					uv2i = uv2[ i ];

					uv2Array[ offset_uv2 ]     = uv2i.u;
					uv2Array[ offset_uv2 + 1 ] = uv2i.v;

					offset_uv2 += 2;

				}

			}

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				fi = chunk_faces4[ f ];

				face = obj_faces[ fi ];
				uv2 = obj_uvs2[ fi ];

				if ( uv2 == undefined ) continue;

				for ( i = 0; i < 4; i ++ ) {

					uv2i = uv2[ i ];

					uv2Array[ offset_uv2 ]     = uv2i.u;
					uv2Array[ offset_uv2 + 1 ] = uv2i.v;

					offset_uv2 += 2;

				}

			}

			if ( offset_uv2 > 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glUV2Buffer );
				glBufferData( GL_ARRAY_BUFFER, uv2Array, hint );

			}

		}

		if ( dirtyElements ) {

			for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces3[ f ]	];

				faceArray[ offset_face ] 	 = vertexIndex;
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

			for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

				face = obj_faces[ chunk_faces4[ f ] ];

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

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.buffer.__glFaceBuffer );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, faceArray, hint );

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.buffer.__glLineBuffer );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, lineArray, hint );

		}

		if ( customAttributes ) {

			for ( i = 0, il = customAttributes.size(); i < il; i ++ ) {

				customAttribute = customAttributes[ i ];

				if ( ! customAttribute.__original.needsUpdate ) continue;

				offset_custom = 0;
				offset_customSrc = 0;

				if ( customAttribute.size == 1 ) {

					if ( customAttribute.boundTo == undefined || customAttribute.boundTo == "vertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces3[ f ]	];

							customAttribute.array[ offset_custom ] 	   = customAttribute.value[ face.a ];
							customAttribute.array[ offset_custom + 1 ] = customAttribute.value[ face.b ];
							customAttribute.array[ offset_custom + 2 ] = customAttribute.value[ face.c ];

							offset_custom += 3;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces4[ f ] ];

							customAttribute.array[ offset_custom ] 	   = customAttribute.value[ face.a ];
							customAttribute.array[ offset_custom + 1 ] = customAttribute.value[ face.b ];
							customAttribute.array[ offset_custom + 2 ] = customAttribute.value[ face.c ];
							customAttribute.array[ offset_custom + 3 ] = customAttribute.value[ face.d ];

							offset_custom += 4;

						}

					} else if ( customAttribute.boundTo == "faces" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							customAttribute.array[ offset_custom ] 	   = value;
							customAttribute.array[ offset_custom + 1 ] = value;
							customAttribute.array[ offset_custom + 2 ] = value;

							offset_custom += 3;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							customAttribute.array[ offset_custom ] 	   = value;
							customAttribute.array[ offset_custom + 1 ] = value;
							customAttribute.array[ offset_custom + 2 ] = value;
							customAttribute.array[ offset_custom + 3 ] = value;

							offset_custom += 4;

						}

					}

				} else if ( customAttribute.size == 2 ) {

					if ( customAttribute.boundTo == undefined || customAttribute.boundTo == "vertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces3[ f ]	];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];

							customAttribute.array[ offset_custom ] 	   = v1.x;
							customAttribute.array[ offset_custom + 1 ] = v1.y;

							customAttribute.array[ offset_custom + 2 ] = v2.x;
							customAttribute.array[ offset_custom + 3 ] = v2.y;

							customAttribute.array[ offset_custom + 4 ] = v3.x;
							customAttribute.array[ offset_custom + 5 ] = v3.y;

							offset_custom += 6;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces4[ f ] ];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];
							v4 = customAttribute.value[ face.d ];

							customAttribute.array[ offset_custom ] 	   = v1.x;
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

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;

							customAttribute.array[ offset_custom ] 	   = v1.x;
							customAttribute.array[ offset_custom + 1 ] = v1.y;

							customAttribute.array[ offset_custom + 2 ] = v2.x;
							customAttribute.array[ offset_custom + 3 ] = v2.y;

							customAttribute.array[ offset_custom + 4 ] = v3.x;
							customAttribute.array[ offset_custom + 5 ] = v3.y;

							offset_custom += 6;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;
							v4 = value;

							customAttribute.array[ offset_custom ] 	   = v1.x;
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

					auto pp;

					if ( customAttribute.type == "c" ) {

						pp = [ "r", "g", "b" ];

					} else {

						pp = [ "x", "y", "z" ];

					}

					if ( customAttribute.boundTo == undefined || customAttribute.boundTo == "vertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces3[ f ]	];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];

							customAttribute.array[ offset_custom ] 	   = v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1 ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2 ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3 ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4 ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5 ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6 ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7 ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8 ] = v3[ pp[ 2 ] ];

							offset_custom += 9;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces4[ f ] ];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];
							v4 = customAttribute.value[ face.d ];

							customAttribute.array[ offset_custom  ] 	= v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1  ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2  ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3  ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4  ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5  ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6  ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7  ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8  ] = v3[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 9  ] = v4[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 10 ] = v4[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 11 ] = v4[ pp[ 2 ] ];

							offset_custom += 12;

						}

					} else if ( customAttribute.boundTo == "faces" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;

							customAttribute.array[ offset_custom ] 	   = v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1 ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2 ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3 ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4 ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5 ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6 ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7 ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8 ] = v3[ pp[ 2 ] ];

							offset_custom += 9;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;
							v4 = value;

							customAttribute.array[ offset_custom  ] 	= v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1  ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2  ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3  ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4  ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5  ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6  ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7  ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8  ] = v3[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 9  ] = v4[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 10 ] = v4[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 11 ] = v4[ pp[ 2 ] ];

							offset_custom += 12;

						}

					} else if ( customAttribute.boundTo == "faceVertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							v1 = value[ 0 ];
							v2 = value[ 1 ];
							v3 = value[ 2 ];

							customAttribute.array[ offset_custom ] 	   = v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1 ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2 ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3 ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4 ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5 ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6 ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7 ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8 ] = v3[ pp[ 2 ] ];

							offset_custom += 9;

						}

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							v1 = value[ 0 ];
							v2 = value[ 1 ];
							v3 = value[ 2 ];
							v4 = value[ 3 ];

							customAttribute.array[ offset_custom  ] 	= v1[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 1  ] = v1[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 2  ] = v1[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 3  ] = v2[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 4  ] = v2[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 5  ] = v2[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 6  ] = v3[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 7  ] = v3[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 8  ] = v3[ pp[ 2 ] ];

							customAttribute.array[ offset_custom + 9  ] = v4[ pp[ 0 ] ];
							customAttribute.array[ offset_custom + 10 ] = v4[ pp[ 1 ] ];
							customAttribute.array[ offset_custom + 11 ] = v4[ pp[ 2 ] ];

							offset_custom += 12;

						}

					}

				} else if ( customAttribute.size == 4 ) {

					if ( customAttribute.boundTo == undefined || customAttribute.boundTo == "vertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces3[ f ]	];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							face = obj_faces[ chunk_faces4[ f ] ];

							v1 = customAttribute.value[ face.a ];
							v2 = customAttribute.value[ face.b ];
							v3 = customAttribute.value[ face.c ];
							v4 = customAttribute.value[ face.d ];

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							v1 = value;
							v2 = value;
							v3 = value;
							v4 = value;

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

					} else if ( customAttribute.boundTo == "faceVertices" ) {

						for ( f = 0, fl = chunk_faces3.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces3[ f ] ];

							v1 = value[ 0 ];
							v2 = value[ 1 ];
							v3 = value[ 2 ];

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

						for ( f = 0, fl = chunk_faces4.size(); f < fl; f ++ ) {

							value = customAttribute.value[ chunk_faces4[ f ] ];

							v1 = value[ 0 ];
							v2 = value[ 1 ];
							v3 = value[ 2 ];
							v4 = value[ 3 ];

							customAttribute.array[ offset_custom  ] 	= v1.x;
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

				}

				glBindBuffer( GL_ARRAY_BUFFER, customAttribute.buffer );
				glBufferData( GL_ARRAY_BUFFER, customAttribute.array, hint );

			}

		}

		if ( dispose ) {

			delete geometryGroup.buffer.__inittedArrays;
			delete geometryGroup.buffer.__colorArray;
			delete geometryGroup.buffer.__normalArray;
			delete geometryGroup.buffer.__tangentArray;
			delete geometryGroup.buffer.__uvArray;
			delete geometryGroup.buffer.__uv2Array;
			delete geometryGroup.buffer.__faceArray;
			delete geometryGroup.buffer.__vertexArray;
			delete geometryGroup.buffer.__lineArray;
			delete geometryGroup.buffer.__skinVertexAArray;
			delete geometryGroup.buffer.__skinVertexBArray;
			delete geometryGroup.buffer.__skinIndexArray;
			delete geometryGroup.buffer.__skinWeightArray;

		}

	};

	function setDirectBuffers ( geometry, hint, dispose ) {

		auto attributes = geometry.attributes;

		auto index = attributes[ "index" ];
		auto position = attributes[ "position" ];
		auto normal = attributes[ "normal" ];
		auto uv = attributes[ "uv" ];
		auto color = attributes[ "color" ];
		auto tangent = attributes[ "tangent" ];

		if ( geometry.elementsNeedUpdate && index !== undefined ) {

			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index.buffer );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER, index.array, hint );

		}

		if ( geometry.verticesNeedUpdate && position !== undefined ) {

			glBindBuffer( GL_ARRAY_BUFFER, position.buffer );
			glBufferData( GL_ARRAY_BUFFER, position.array, hint );

		}

		if ( geometry.normalsNeedUpdate && normal !== undefined ) {

			glBindBuffer( GL_ARRAY_BUFFER, normal.buffer );
			glBufferData( GL_ARRAY_BUFFER, normal.array, hint );

		}

		if ( geometry.uvsNeedUpdate && uv !== undefined ) {

			glBindBuffer( GL_ARRAY_BUFFER, uv.buffer );
			glBufferData( GL_ARRAY_BUFFER, uv.array, hint );

		}

		if ( geometry.colorsNeedUpdate && color !== undefined ) {

			glBindBuffer( GL_ARRAY_BUFFER, color.buffer );
			glBufferData( GL_ARRAY_BUFFER, color.array, hint );

		}

		if ( geometry.tangentsNeedUpdate && tangent !== undefined ) {

			glBindBuffer( GL_ARRAY_BUFFER, tangent.buffer );
			glBufferData( GL_ARRAY_BUFFER, tangent.array, hint );

		}

		if ( dispose ) {

			for ( auto i in geometry.attributes ) {

				delete geometry.attributes[ i ].array;

			}

		}

	};

	// Buffer rendering

	renderBufferImmediate = function ( object, program, material ) {

		if ( object.hasPositions && ! object.__glVertexBuffer ) object.__glVertexBuffer = glCreateBuffer();
		if ( object.hasNormals && ! object.__glNormalBuffer ) object.__glNormalBuffer = glCreateBuffer();
		if ( object.hasUvs && ! object.__glUvBuffer ) object.__glUvBuffer = glCreateBuffer();
		if ( object.hasColors && ! object.__glColorBuffer ) object.__glColorBuffer = glCreateBuffer();

		if ( object.hasPositions ) {

			glBindBuffer( GL_ARRAY_BUFFER, object.__glVertexBuffer );
			glBufferData( GL_ARRAY_BUFFER, object.positionArray, _gl.DYNAMIC_DRAW );
			glEnableVertexAttribArray( program.attributes.position );
			glvertexAttribPointer( program.attributes.position, 3, _gl.FLOAT, false, 0, 0 );

		}

		if ( object.hasNormals ) {

			glBindBuffer( GL_ARRAY_BUFFER, object.__glNormalBuffer );

			if ( material.shading == THREE::FlatShading ) {

				auto nx, ny, nz,
					nax, nbx, ncx, nay, nby, ncy, naz, nbz, ncz,
					normalArray,
					i, il = object.count * 3;

				for( i = 0; i < il; i += 9 ) {

					normalArray = object.normalArray;

					nax  = normalArray[ i ];
					nay  = normalArray[ i + 1 ];
					naz  = normalArray[ i + 2 ];

					nbx  = normalArray[ i + 3 ];
					nby  = normalArray[ i + 4 ];
					nbz  = normalArray[ i + 5 ];

					ncx  = normalArray[ i + 6 ];
					ncy  = normalArray[ i + 7 ];
					ncz  = normalArray[ i + 8 ];

					nx = ( nax + nbx + ncx ) / 3;
					ny = ( nay + nby + ncy ) / 3;
					nz = ( naz + nbz + ncz ) / 3;

					normalArray[ i ] 	 = nx;
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

			glBufferData( GL_ARRAY_BUFFER, object.normalArray, _gl.DYNAMIC_DRAW );
			glEnableVertexAttribArray( program.attributes.normal );
			glvertexAttribPointer( program.attributes.normal, 3, _gl.FLOAT, false, 0, 0 );

		}

		if ( object.hasUvs && material.map ) {

			glBindBuffer( GL_ARRAY_BUFFER, object.__glUvBuffer );
			glBufferData( GL_ARRAY_BUFFER, object.uvArray, _gl.DYNAMIC_DRAW );
			glEnableVertexAttribArray( program.attributes.uv );
			glvertexAttribPointer( program.attributes.uv, 2, _gl.FLOAT, false, 0, 0 );

		}

		if ( object.hasColors && material.vertexColors !== THREE::NoColors ) {

			glBindBuffer( GL_ARRAY_BUFFER, object.__glColorBuffer );
			glBufferData( GL_ARRAY_BUFFER, object.colorArray, _gl.DYNAMIC_DRAW );
			glEnableVertexAttribArray( program.attributes.color );
			glvertexAttribPointer( program.attributes.color, 3, _gl.FLOAT, false, 0, 0 );

		}

		gldrawArrays( GL_TRIANGLES, 0, object.count );

		object.count = 0;

	};

	renderBufferDirect = function ( camera, lights, fog, material, geometry, object ) {

		if ( material.visible == false ) return;

		auto program, attributes, linewidth, primitives, a, attribute;

		program = setProgram( camera, lights, fog, material, object );

		attributes = program.attributes;

		auto updateBuffers = false;
		auto wireframeBit = material.wireframe ? 1 : 0;
		auto geometryHash = ( geometry.id * 0xffffff ) + ( program.id * 2 ) + wireframeBit;

		if ( geometryHash !== _currentGeometryGroupHash ) {

			_currentGeometryGroupHash = geometryHash;
			updateBuffers = true;

		}

		// render mesh

		if ( object instanceof THREE::Mesh ) {

			auto offsets = geometry.offsets;

			// if there is more than 1 chunk
			// must set attribute pointers to use new offsets for each chunk
			// even if geometry and materials didn't change

			if ( offsets.size() > 1 ) updateBuffers = true;

			for ( auto i = 0, il = offsets.size(); i < il; ++ i ) {

				auto startIndex = offsets[ i ].index;

				if ( updateBuffers ) {

					// vertices

					auto position = geometry.attributes[ "position" ];
					auto positionSize = position.itemSize;

					glBindBuffer( GL_ARRAY_BUFFER, position.buffer );
					glvertexAttribPointer( attributes.position, positionSize, _gl.FLOAT, false, 0, startIndex * positionSize * 4 ); // 4 bytes per Float32

					// normals

					auto normal = geometry.attributes[ "normal" ];

					if ( attributes.normal >= 0 && normal ) {

						auto normalSize = normal.itemSize;

						glBindBuffer( GL_ARRAY_BUFFER, normal.buffer );
						glvertexAttribPointer( attributes.normal, normalSize, _gl.FLOAT, false, 0, startIndex * normalSize * 4 );

					}

					// uvs

					auto uv = geometry.attributes[ "uv" ];

					if ( attributes.uv >= 0 && uv ) {

						if ( uv.buffer ) {

							auto uvSize = uv.itemSize;

							glBindBuffer( GL_ARRAY_BUFFER, uv.buffer );
							glvertexAttribPointer( attributes.uv, uvSize, _gl.FLOAT, false, 0, startIndex * uvSize * 4 );

							glEnableVertexAttribArray( attributes.uv );

						} else {

							glDisableVertexAttribArray( attributes.uv );

						}

					}

					// colors

					auto color = geometry.attributes[ "color" ];

					if ( attributes.color >= 0 && color ) {

						auto colorSize = color.itemSize;

						glBindBuffer( GL_ARRAY_BUFFER, color.buffer );
						glvertexAttribPointer( attributes.color, colorSize, _gl.FLOAT, false, 0, startIndex * colorSize * 4 );

					}

					// tangents

					auto tangent = geometry.attributes[ "tangent" ];

					if ( attributes.tangent >= 0 && tangent ) {

						auto tangentSize = tangent.itemSize;

						glBindBuffer( GL_ARRAY_BUFFER, tangent.buffer );
						glvertexAttribPointer( attributes.tangent, tangentSize, _gl.FLOAT, false, 0, startIndex * tangentSize * 4 );

					}

					// indices

					auto index = geometry.attributes[ "index" ];

					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index.buffer );

				}

				// render indexed triangles

				gldrawElements( GL_TRIANGLES, offsets[ i ].count, _gl.UNSIGNED_SHORT, offsets[ i ].start * 2 ); // 2 bytes per Uint16

				_info.render.calls ++;
				_info.render.vertices += offsets[ i ].count; // not really true, here vertices can be shared
				_info.render.faces += offsets[ i ].count / 3;

			}

		}

	};

	renderBuffer = function ( camera, lights, fog, material, geometryGroup, object ) {

		if ( material.visible == false ) return;

		auto program, attributes, linewidth, primitives, a, attribute, i, il;

		program = setProgram( camera, lights, fog, material, object );

		attributes = program.attributes;

		auto updateBuffers = false,
			wireframeBit = material.wireframe ? 1 : 0,
			geometryGroupHash = ( geometryGroup.id * 0xffffff ) + ( program.id * 2 ) + wireframeBit;

		if ( geometryGroupHash !== _currentGeometryGroupHash ) {

			_currentGeometryGroupHash = geometryGroupHash;
			updateBuffers = true;

		}

		// vertices

		if ( !material.morphTargets && attributes.position >= 0 ) {

			if ( updateBuffers ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glVertexBuffer );
				glvertexAttribPointer( attributes.position, 3, _gl.FLOAT, false, 0, 0 );

			}

		} else {

			if ( object.morphTargetBase ) {

				setupMorphTargets( material, geometryGroup, object );

			}

		}


		if ( updateBuffers ) {

			// custom attributes

			// Use the per-geometryGroup custom attribute arrays which are setup in initMeshBuffers

			if ( geometryGroup.buffer.__glCustomAttributesList ) {

				for ( i = 0, il = geometryGroup.buffer.__glCustomAttributesList.size(); i < il; i ++ ) {

					attribute = geometryGroup.buffer.__glCustomAttributesList[ i ];

					if( attributes[ attribute.buffer.belongsToAttribute ] >= 0 ) {

						glBindBuffer( GL_ARRAY_BUFFER, attribute.buffer );
						glvertexAttribPointer( attributes[ attribute.buffer.belongsToAttribute ], attribute.size, _gl.FLOAT, false, 0, 0 );

					}

				}

			}


			// colors

			if ( attributes.color >= 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glColorBuffer );
				glvertexAttribPointer( attributes.color, 3, _gl.FLOAT, false, 0, 0 );

			}

			// normals

			if ( attributes.normal >= 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glNormalBuffer );
				glvertexAttribPointer( attributes.normal, 3, _gl.FLOAT, false, 0, 0 );

			}

			// tangents

			if ( attributes.tangent >= 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glTangentBuffer );
				glvertexAttribPointer( attributes.tangent, 4, _gl.FLOAT, false, 0, 0 );

			}

			// uvs

			if ( attributes.uv >= 0 ) {

				if ( geometryGroup.buffer.__glUVBuffer ) {

					glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glUVBuffer );
					glvertexAttribPointer( attributes.uv, 2, _gl.FLOAT, false, 0, 0 );

					glEnableVertexAttribArray( attributes.uv );

				} else {

					glDisableVertexAttribArray( attributes.uv );

				}

			}

			if ( attributes.uv2 >= 0 ) {

				if ( geometryGroup.buffer.__glUV2Buffer ) {

					glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glUV2Buffer );
					glvertexAttribPointer( attributes.uv2, 2, _gl.FLOAT, false, 0, 0 );

					glEnableVertexAttribArray( attributes.uv2 );

				} else {

					glDisableVertexAttribArray( attributes.uv2 );

				}

			}

			if ( material.skinning &&
				 attributes.skinVertexA >= 0 && attributes.skinVertexB >= 0 &&
				 attributes.skinIndex >= 0 && attributes.skinWeight >= 0 ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinVertexABuffer );
				glvertexAttribPointer( attributes.skinVertexA, 4, _gl.FLOAT, false, 0, 0 );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinVertexBBuffer );
				glvertexAttribPointer( attributes.skinVertexB, 4, _gl.FLOAT, false, 0, 0 );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinIndicesBuffer );
				glvertexAttribPointer( attributes.skinIndex, 4, _gl.FLOAT, false, 0, 0 );

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glSkinWeightsBuffer );
				glvertexAttribPointer( attributes.skinWeight, 4, _gl.FLOAT, false, 0, 0 );

			}

		}

		// render mesh

		if ( object instanceof THREE::Mesh ) {

			// wireframe

			if ( material.wireframe ) {

				setLineWidth( material.wireframeLinewidth );

				if ( updateBuffers ) _gl.bindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.buffer.__glLineBuffer );
				gldrawElements( GL_LINES, geometryGroup.buffer.__glLineCount, _gl.UNSIGNED_SHORT, 0 );

			// triangles

			} else {

				if ( updateBuffers ) _gl.bindBuffer( GL_ELEMENT_ARRAY_BUFFER, geometryGroup.buffer.__glFaceBuffer );
				gldrawElements( GL_TRIANGLES, geometryGroup.buffer.__glFaceCount, _gl.UNSIGNED_SHORT, 0 );

			}

			_info.render.calls ++;
			_info.render.vertices += geometryGroup.buffer.__glFaceCount;
			_info.render.faces += geometryGroup.buffer.__glFaceCount / 3;

		// render lines

		} else if ( object instanceof THREE::Line ) {

			primitives = ( object.type == THREE::LineStrip ) ? _gl.LINE_STRIP : _gl.LINES;

			setLineWidth( material.linewidth );

			gldrawArrays( primitives, 0, geometryGroup.buffer.__glLineCount );

			_info.render.calls ++;

		// render particles

		} else if ( object instanceof THREE::ParticleSystem ) {

			gldrawArrays( GL_POINTS, 0, geometryGroup.buffer.__glParticleCount );

			_info.render.calls ++;
			_info.render.points += geometryGroup.buffer.__glParticleCount;

		// render ribbon

		} else if ( object instanceof THREE::Ribbon ) {

			gldrawArrays( GL_TRIANGLE_STRIP, 0, geometryGroup.buffer.__glVertexCount );

			_info.render.calls ++;

		}

	};

	function setupMorphTargets ( material, geometryGroup, object ) {

		// set base

		auto attributes = material.program.attributes;

		if ( object.morphTargetBase !== -1 ) {

			glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphTargetsBuffers[ object.morphTargetBase ] );
			glvertexAttribPointer( attributes.position, 3, _gl.FLOAT, false, 0, 0 );

		} else if ( attributes.position >= 0 ) {

			glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glVertexBuffer );
			glvertexAttribPointer( attributes.position, 3, _gl.FLOAT, false, 0, 0 );

		}

		if ( object.morphTargetForcedOrder.size() ) {

			// set forced order

			auto m = 0;
			auto order = object.morphTargetForcedOrder;
			auto influences = object.morphTargetInfluences;

			while ( m < material.numSupportedMorphTargets && m < order.size() ) {

				glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphTargetsBuffers[ order[ m ] ] );
				glvertexAttribPointer( attributes[ "morphTarget" + m ], 3, _gl.FLOAT, false, 0, 0 );

				if ( material.morphNormals ) {

					glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphNormalsBuffers[ order[ m ] ] );
					glvertexAttribPointer( attributes[ "morphNormal" + m ], 3, _gl.FLOAT, false, 0, 0 );

				}

				object.__glMorphTargetInfluences[ m ] = influences[ order[ m ] ];

				m ++;
			}

		} else {

			// find the most influencing

			auto influence, activeInfluenceIndices = [];
			auto influences = object.morphTargetInfluences;
			auto i, il = influences.size();

			for ( i = 0; i < il; i ++ ) {

				influence = influences[ i ];

				if ( influence > 0 ) {

					activeInfluenceIndices.push_back( [ i, influence ] );

				}

			}

			if ( activeInfluenceIndices.size() > material.numSupportedMorphTargets ) {

				activeInfluenceIndices.sort( numericalSort );
				activeInfluenceIndices.size() = material.numSupportedMorphTargets;

			} else if ( activeInfluenceIndices.size() > material.numSupportedMorphNormals ) {

				activeInfluenceIndices.sort( numericalSort );

			} else if ( activeInfluenceIndices.size() == 0 ) {

				activeInfluenceIndices.push_back( [ 0, 0 ] );

			};

			auto influenceIndex, m = 0;

			while ( m < material.numSupportedMorphTargets ) {

				if ( activeInfluenceIndices[ m ] ) {

					influenceIndex = activeInfluenceIndices[ m ][ 0 ];

					glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphTargetsBuffers[ influenceIndex ] );

					glvertexAttribPointer( attributes[ "morphTarget" + m ], 3, _gl.FLOAT, false, 0, 0 );

					if ( material.morphNormals ) {

						glBindBuffer( GL_ARRAY_BUFFER, geometryGroup.buffer.__glMorphNormalsBuffers[ influenceIndex ] );
						glvertexAttribPointer( attributes[ "morphNormal" + m ], 3, _gl.FLOAT, false, 0, 0 );

					}

					object.__glMorphTargetInfluences[ m ] = influences[ influenceIndex ];

				} else {

					glvertexAttribPointer( attributes[ "morphTarget" + m ], 3, _gl.FLOAT, false, 0, 0 );

					if ( material.morphNormals ) {

						glvertexAttribPointer( attributes[ "morphNormal" + m ], 3, _gl.FLOAT, false, 0, 0 );

					}

					object.__glMorphTargetInfluences[ m ] = 0;

				}

				m ++;

			}

		}

		// load updated influences uniform

		if ( material.program.uniforms.morphTargetInfluences !== null ) {

			gluniform1fv( material.program.uniforms.morphTargetInfluences, object.__glMorphTargetInfluences );

		}

	};

	// Sorting

	function painterSort ( a, b ) {

		return b.z - a.z;

	};

	function numericalSort ( a, b ) {

		return b[ 1 ] - a[ 1 ];

	};


	// Rendering

	render = function ( scene, camera, renderTarget, forceClear ) {

		if ( camera instanceof THREE::Camera == false ) {

			console.error( 'THREE::WebGLRenderer.render: camera is not an instance of THREE::Camera.' );
			return;

		}

		auto i, il,

		webglObject, object,
		renderList,

		lights = scene.__lights,
		fog = scene.fog;

		// reset caching for this frame

		_currentMaterialId = -1;
		_lightsNeedUpdate = true;

		// update scene graph

		if ( autoUpdateScene ) scene.updateMatrixWorld();

		// update camera matrices and frustum

		if ( camera.parent == undefined ) camera.updateMatrixWorld();

		if ( ! camera._viewMatrixArray ) camera._viewMatrixArray.resize( 16 );
		if ( ! camera._projectionMatrixArray ) camera._projectionMatrixArray.resize( 16 );

		camera.matrixWorldInverse.getInverse( camera.matrixWorld );

		camera.matrixWorldInverse.flattenToArray( camera._viewMatrixArray );
		camera.projectionMatrix.flattenToArray( camera._projectionMatrixArray );

		_projScreenMatrix.multiply( camera.projectionMatrix, camera.matrixWorldInverse );
		_frustum.setFromMatrix( _projScreenMatrix );

		// update WebGL objects

		if ( autoUpdateObjects ) initWebGLObjects( scene );

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

		renderList = scene.__glObjects;

		for ( i = 0, il = renderList.size(); i < il; i ++ ) {

			webglObject = renderList[ i ];
			object = webglObject.object;

			webglObject.render = false;

			if ( object.visible ) {

				if ( ! ( object instanceof THREE::Mesh || object instanceof THREE::ParticleSystem ) || ! ( object.frustumCulled ) || _frustum.contains( object ) ) {

					//object.matrixWorld.flattenToArray( object._modelMatrixArray );

					setupMatrices( object, camera );

					unrollBufferMaterial( webglObject );

					webglObject.render = true;

					if ( sortObjects ) {

						if ( object.renderDepth ) {

							webglObject.z = object.renderDepth;

						} else {

							_vector3.copy( object.matrixWorld.getPosition() );
							_projScreenMatrix.multiplyVector3( _vector3 );

							webglObject.z = _vector3.z;

						}

					}

				}

			}

		}

		if ( sortObjects ) {

			renderList.sort( painterSort );

		}

		// set matrices for immediate objects

		renderList = scene.__glObjectsImmediate;

		for ( i = 0, il = renderList.size(); i < il; i ++ ) {

			webglObject = renderList[ i ];
			object = webglObject.object;

			if ( object.visible ) {

				/*
				if ( object.matrixAutoUpdate ) {

					object.matrixWorld.flattenToArray( object._modelMatrixArray );

				}
				*/

				setupMatrices( object, camera );

				unrollImmediateBufferMaterial( webglObject );

			}

		}

		if ( scene.overrideMaterial ) {

			auto material = scene.overrideMaterial;

			setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst );
			setDepthTest( material.depthTest );
			setDepthWrite( material.depthWrite );
			setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );

			renderObjects( scene.__glObjects, false, "", camera, lights, fog, true, material );
			renderObjectsImmediate( scene.__glObjectsImmediate, "", camera, lights, fog, false, material );

		} else {

			// opaque pass (front-to-back order)

			setBlending( THREE::NormalBlending );

			renderObjects( scene.__glObjects, true, "opaque", camera, lights, fog, false );
			renderObjectsImmediate( scene.__glObjectsImmediate, "opaque", camera, lights, fog, false );

			// transparent pass (back-to-front order)

			renderObjects( scene.__glObjects, false, "transparent", camera, lights, fog, true );
			renderObjectsImmediate( scene.__glObjectsImmediate, "transparent", camera, lights, fog, true );

		}

		// custom render plugins (post pass)

		renderPlugins( renderPluginsPost, scene, camera );


		// Generate mipmap if we're using any kind of mipmap filtering

		if ( renderTarget && renderTarget.generateMipmaps && renderTarget.minFilter !== THREE::NearestFilter && renderTarget.minFilter !== THREE::LinearFilter ) {

			updateRenderTargetMipmap( renderTarget );

		}

		// Ensure depth buffer writing is enabled so it can be cleared on next render

		setDepthTest( true );
		setDepthWrite( true );

		// _gl.finish();

	};

	function renderPlugins( plugins, scene, camera ) {

		if ( ! plugins.size() ) return;

		for ( auto i = 0, il = plugins.size(); i < il; i ++ ) {

			// reset state for plugin (to start from clean slate)

			_currentProgram = null;
			_currentCamera = null;

			_oldBlending = -1;
			_oldDepthTest = -1;
			_oldDepthWrite = -1;
			_oldDoubleSided = -1;
			_oldFlipSided = -1;
			_currentGeometryGroupHash = -1;
			_currentMaterialId = -1;

			_lightsNeedUpdate = true;

			plugins[ i ].render( scene, camera, _currentWidth, _currentHeight );

			// reset state after plugin (anything could have changed)

			_currentProgram = null;
			_currentCamera = null;

			_oldBlending = -1;
			_oldDepthTest = -1;
			_oldDepthWrite = -1;
			_oldDoubleSided = -1;
			_oldFlipSided = -1;
			_currentGeometryGroupHash = -1;
			_currentMaterialId = -1;

			_lightsNeedUpdate = true;

		}

	};

	function renderObjects ( renderList, reverse, materialType, camera, lights, fog, useBlending, overrideMaterial ) {

		auto webglObject, object, buffer, material, start, end, delta;

		if ( reverse ) {

			start = renderList.size() - 1;
			end = -1;
			delta = -1;

		} else {

			start = 0;
			end = renderList.size();
			delta = 1;
		}

		for ( auto i = start; i !== end; i += delta ) {

			webglObject = renderList[ i ];

			if ( webglObject.render ) {

				object = webglObject.object;
				buffer = webglObject.buffer;

				if ( overrideMaterial ) {

					material = overrideMaterial;

				} else {

					material = webglObject[ materialType ];

					if ( ! material ) continue;

					if ( useBlending ) _setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst );

					_setDepthTest( material.depthTest );
					_setDepthWrite( material.depthWrite );
					setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );

				}

				_setMaterialFaces( material );

				if ( buffer instanceof THREE::BufferGeometry ) {

					_renderBufferDirect( camera, lights, fog, material, buffer, object );

				} else {

					_renderBuffer( camera, lights, fog, material, buffer, object );

				}

			}

		}

	};

	function renderObjectsImmediate ( renderList, materialType, camera, lights, fog, useBlending, overrideMaterial ) {

		auto webglObject, object, material, program;

		for ( auto i = 0, il = renderList.size(); i < il; i ++ ) {

			webglObject = renderList[ i ];
			object = webglObject.object;

			if ( object.visible ) {

				if ( overrideMaterial ) {

					material = overrideMaterial;

				} else {

					material = webglObject[ materialType ];

					if ( ! material ) continue;

					if ( useBlending ) _setBlending( material.blending, material.blendEquation, material.blendSrc, material.blendDst );

					_setDepthTest( material.depthTest );
					_setDepthWrite( material.depthWrite );
					setPolygonOffset( material.polygonOffset, material.polygonOffsetFactor, material.polygonOffsetUnits );

				}

				_renderImmediateObject( camera, lights, fog, material, object );

			}

		}

	};

	renderImmediateObject = function ( camera, lights, fog, material, object ) {

		auto program = setProgram( camera, lights, fog, material, object );

		_currentGeometryGroupHash = -1;

		_setMaterialFaces( material );

		if ( object.immediateRenderCallback ) {

			object.immediateRenderCallback( program, _gl, _frustum );

		} else {

			object.render( function( object ) { _renderBufferImmediate( object, program, material ); } );

		}

	};

	function unrollImmediateBufferMaterial ( globject ) {

		auto object = globject.object,
			material = object.material;

		if ( material.transparent ) {

			globject.transparent = material;
			globject.opaque = null;

		} else {

			globject.opaque = material;
			globject.transparent = null;

		}

	};

	function unrollBufferMaterial ( globject ) {

		auto object = globject.object,
			buffer = globject.buffer,
			material, materialIndex, meshMaterial;

		meshMaterial = object.material;

		if ( meshMaterial instanceof THREE::MeshFaceMaterial ) {

			materialIndex = buffer.materialIndex;

			if ( materialIndex >= 0 ) {

				material = object.geometry.materials[ materialIndex ];

				if ( material.transparent ) {

					globject.transparent = material;
					globject.opaque = null;

				} else {

					globject.opaque = material;
					globject.transparent = null;

				}

			}

		} else {

			material = meshMaterial;

			if ( material ) {

				if ( material.transparent ) {

					globject.transparent = material;
					globject.opaque = null;

				} else {

					globject.opaque = material;
					globject.transparent = null;

				}

			}

		}

	};

	// Geometry splitting

	function sortFacesByMaterial ( geometry ) {

		auto f, fl, face, materialIndex, vertices,
			materialHash, groupHash,
			hash_map = {};

		auto numMorphTargets = geometry.morphTargets.size();
		auto numMorphNormals = geometry.morphNormals.size();

		geometry.geometryGroups = {};

		for ( f = 0, fl = geometry.faces.size(); f < fl; f ++ ) {

			face = geometry.faces[ f ];
			materialIndex = face.materialIndex;

			materialHash = ( materialIndex !== undefined ) ? materialIndex : -1;

			if ( hash_map[ materialHash ] == undefined ) {

				hash_map[ materialHash ] = { 'hash': materialHash, 'counter': 0 };

			}

			groupHash = hash_map[ materialHash ].hash + '_' + hash_map[ materialHash ].counter;

			if ( geometry.geometryGroups[ groupHash ] == undefined ) {

				geometry.geometryGroups[ groupHash ] = { 'faces3': [], 'faces4': [], 'materialIndex': materialIndex, 'vertices': 0, 'numMorphTargets': numMorphTargets, 'numMorphNormals': numMorphNormals };

			}

			vertices = face instanceof THREE::Face3 ? 3 : 4;

			if ( geometry.geometryGroups[ groupHash ].vertices + vertices > 65535 ) {

				hash_map[ materialHash ].counter += 1;
				groupHash = hash_map[ materialHash ].hash + '_' + hash_map[ materialHash ].counter;

				if ( geometry.geometryGroups[ groupHash ] == undefined ) {

					geometry.geometryGroups[ groupHash ] = { 'faces3': [], 'faces4': [], 'materialIndex': materialIndex, 'vertices': 0, 'numMorphTargets': numMorphTargets, 'numMorphNormals': numMorphNormals };

				}

			}

			if ( face instanceof THREE::Face3 ) {

				geometry.geometryGroups[ groupHash ].faces3.push_back( f );

			} else {

				geometry.geometryGroups[ groupHash ].faces4.push_back( f );

			}

			geometry.geometryGroups[ groupHash ].vertices += vertices;

		}

		geometry.geometryGroupsList = [];

		for ( auto g in geometry.geometryGroups ) {

			geometry.geometryGroups[ g ].id = _geometryGroupCounter ++;

			geometry.geometryGroupsList.push_back( geometry.geometryGroups[ g ] );

		}

	};

	// Objects refresh

	initWebGLObjects = function ( scene ) {

		if ( !scene.__glObjects ) {

			scene.__glObjects = [];
			scene.__glObjectsImmediate = [];
			scene.__glSprites = [];
			scene.__glFlares = [];

		}

		while ( scene.__objectsAdded.size() ) {

			addObject( scene.__objectsAdded[ 0 ], scene );
			scene.__objectsAdded.splice( 0, 1 );

		}

		while ( scene.__objectsRemoved.size() ) {

			removeObject( scene.__objectsRemoved[ 0 ], scene );
			scene.__objectsRemoved.splice( 0, 1 );

		}

		// update must be called after objects adding / removal

		for ( auto o = 0, ol = scene.__glObjects.size(); o < ol; o ++ ) {

			updateObject( scene.__glObjects[ o ].object );

		}

	};

	// Objects adding

	function addObject ( object, scene ) {

		auto g, geometry, geometryGroup;

		if ( ! object.__glInit ) {

			object.__glInit = true;

			object._modelViewMatrix = new THREE::Matrix4();
			object._normalMatrix = new THREE::Matrix3();

			if ( object instanceof THREE::Mesh ) {

				geometry = object.geometry;

				if ( geometry instanceof THREE::Geometry ) {

					if ( geometry.geometryGroups == undefined ) {

						sortFacesByMaterial( geometry );

					}

					// create separate VBOs per geometry chunk

					for ( g in geometry.geometryGroups ) {

						geometryGroup = geometry.geometryGroups[ g ];

						// initialise VBO on the first access

						if ( ! geometryGroup.buffer.__glVertexBuffer ) {

							createMeshBuffers( geometryGroup );
							initMeshBuffers( geometryGroup, object );

							geometry.verticesNeedUpdate = true;
							geometry.morphTargetsNeedUpdate = true;
							geometry.elementsNeedUpdate = true;
							geometry.uvsNeedUpdate = true;
							geometry.normalsNeedUpdate = true;
							geometry.tangentsNeedUpdate = true;
							geometry.colorsNeedUpdate = true;

						}

					}

				} else if ( geometry instanceof THREE::BufferGeometry ) {

					initDirectBuffers( geometry );

				}

			} else if ( object instanceof THREE::Ribbon ) {

				geometry = object.geometry;

				if( ! geometry.buffer.__glVertexBuffer ) {

					createRibbonBuffers( geometry );
					initRibbonBuffers( geometry );

					geometry.verticesNeedUpdate = true;
					geometry.colorsNeedUpdate = true;

				}

			} else if ( object instanceof THREE::Line ) {

				geometry = object.geometry;

				if( ! geometry.buffer.__glVertexBuffer ) {

					createLineBuffers( geometry );
					initLineBuffers( geometry, object );

					geometry.verticesNeedUpdate = true;
					geometry.colorsNeedUpdate = true;

				}

			} else if ( object instanceof THREE::ParticleSystem ) {

				geometry = object.geometry;

				if ( ! geometry.buffer.__glVertexBuffer ) {

					createParticleBuffers( geometry );
					initParticleBuffers( geometry, object );

					geometry.verticesNeedUpdate = true;
					geometry.colorsNeedUpdate = true;

				}

			}

		}

		if ( ! object.__glActive ) {

			if ( object instanceof THREE::Mesh ) {

				geometry = object.geometry;

				if ( geometry instanceof THREE::BufferGeometry ) {

					addBuffer( scene.__glObjects, geometry, object );

				} else {

					for ( g in geometry.geometryGroups ) {

						geometryGroup = geometry.geometryGroups[ g ];

						addBuffer( scene.__glObjects, geometryGroup, object );

					}

				}

			} else if ( object instanceof THREE::Ribbon ||
						object instanceof THREE::Line ||
						object instanceof THREE::ParticleSystem ) {

				geometry = object.geometry;
				addBuffer( scene.__glObjects, geometry, object );

			} else if ( object instanceof THREE::ImmediateRenderObject || object.immediateRenderCallback ) {

				addBufferImmediate( scene.__glObjectsImmediate, object );

			} else if ( object instanceof THREE::Sprite ) {

				scene.__glSprites.push_back( object );

			} else if ( object instanceof THREE::LensFlare ) {

				scene.__glFlares.push_back( object );

			}

			object.__glActive = true;

		}

	};

	function addBuffer ( objlist, buffer, object ) {

		objlist.push_back(
			{
				buffer: buffer,
				object: object,
				opaque: null,
				transparent: null
			}
		);

	};

	function addBufferImmediate ( objlist, object ) {

		objlist.push_back(
			{
				object: object,
				opaque: null,
				transparent: null
			}
		);

	};

	// Objects updates

	function updateObject ( object ) {

		auto geometry = object.geometry,
			geometryGroup, customAttributesDirty, material;

		if ( object instanceof THREE::Mesh ) {

			if ( geometry instanceof THREE::BufferGeometry ) {

				if ( geometry.verticesNeedUpdate || geometry.elementsNeedUpdate ||
					 geometry.uvsNeedUpdate || geometry.normalsNeedUpdate ||
					 geometry.colorsNeedUpdate || geometry.tangentsNeedUpdate ) {

					setDirectBuffers( geometry, _gl.DYNAMIC_DRAW, !geometry.dynamic );

				}

				geometry.verticesNeedUpdate = false;
				geometry.elementsNeedUpdate = false;
				geometry.uvsNeedUpdate = false;
				geometry.normalsNeedUpdate = false;
				geometry.colorsNeedUpdate = false;
				geometry.tangentsNeedUpdate = false;

			} else {

				// check all geometry groups

				for( auto i = 0, il = geometry.geometryGroupsList.size(); i < il; i ++ ) {

					geometryGroup = geometry.geometryGroupsList[ i ];

					material = getBufferMaterial( object, geometryGroup );

					customAttributesDirty = material.attributes && areCustomAttributesDirty( material );

					if ( geometry.verticesNeedUpdate || geometry.morphTargetsNeedUpdate || geometry.elementsNeedUpdate ||
						 geometry.uvsNeedUpdate || geometry.normalsNeedUpdate ||
						 geometry.colorsNeedUpdate || geometry.tangentsNeedUpdate || customAttributesDirty ) {

						setMeshBuffers( geometryGroup, object, _gl.DYNAMIC_DRAW, !geometry.dynamic, material );

					}

				}

				geometry.verticesNeedUpdate = false;
				geometry.morphTargetsNeedUpdate = false;
				geometry.elementsNeedUpdate = false;
				geometry.uvsNeedUpdate = false;
				geometry.normalsNeedUpdate = false;
				geometry.colorsNeedUpdate = false;
				geometry.tangentsNeedUpdate = false;

				material.attributes && clearCustomAttributes( material );

			}

		} else if ( object instanceof THREE::Ribbon ) {

			if ( geometry.verticesNeedUpdate || geometry.colorsNeedUpdate ) {

				setRibbonBuffers( geometry, _gl.DYNAMIC_DRAW );

			}

			geometry.verticesNeedUpdate = false;
			geometry.colorsNeedUpdate = false;

		} else if ( object instanceof THREE::Line ) {

			material = getBufferMaterial( object, geometryGroup );

			customAttributesDirty = material.attributes && areCustomAttributesDirty( material );

			if ( geometry.verticesNeedUpdate ||  geometry.colorsNeedUpdate || customAttributesDirty ) {

				setLineBuffers( geometry, _gl.DYNAMIC_DRAW );

			}

			geometry.verticesNeedUpdate = false;
			geometry.colorsNeedUpdate = false;

			material.attributes && clearCustomAttributes( material );

		} else if ( object instanceof THREE::ParticleSystem ) {

			material = getBufferMaterial( object, geometryGroup );

			customAttributesDirty = material.attributes && areCustomAttributesDirty( material );

			if ( geometry.verticesNeedUpdate || geometry.colorsNeedUpdate || object.sortParticles || customAttributesDirty ) {

				setParticleBuffers( geometry, _gl.DYNAMIC_DRAW, object );

			}

			geometry.verticesNeedUpdate = false;
			geometry.colorsNeedUpdate = false;

			material.attributes && clearCustomAttributes( material );

		}

	};

	// Objects updates - custom attributes check

	function areCustomAttributesDirty ( material ) {

		for ( auto a in material.attributes ) {

			if ( material.attributes[ a ].needsUpdate ) return true;

		}

		return false;

	};

	function clearCustomAttributes ( material ) {

		for ( auto a in material.attributes ) {

			material.attributes[ a ].needsUpdate = false;

		}

	};

	// Objects removal

	function removeObject ( object, scene ) {

		if ( object instanceof THREE::Mesh  ||
			 object instanceof THREE::ParticleSystem ||
			 object instanceof THREE::Ribbon ||
			 object instanceof THREE::Line ) {

			removeInstances( scene.__glObjects, object );

		} else if ( object instanceof THREE::Sprite ) {

			removeInstancesDirect( scene.__glSprites, object );

		} else if ( object instanceof THREE::LensFlare ) {

			removeInstancesDirect( scene.__glFlares, object );

		} else if ( object instanceof THREE::ImmediateRenderObject || object.immediateRenderCallback ) {

			removeInstances( scene.__glObjectsImmediate, object );

		}

		object.__glActive = false;

	};

	function removeInstances ( objlist, object ) {

		for ( auto o = objlist.size() - 1; o >= 0; o -- ) {

			if ( objlist[ o ].object == object ) {

				objlist.splice( o, 1 );

			}

		}

	};

	function removeInstancesDirect ( objlist, object ) {

		for ( auto o = objlist.size() - 1; o >= 0; o -- ) {

			if ( objlist[ o ] == object ) {

				objlist.splice( o, 1 );

			}

		}

	};

	// Materials

	initMaterial = function ( material, lights, fog, object ) {

		auto u, a, identifiers, i, parameters, maxLightCount, maxBones, maxShadows, shaderID;

		if ( material instanceof THREE::MeshDepthMaterial ) {

			shaderID = 'depth';

		} else if ( material instanceof THREE::MeshNormalMaterial ) {

			shaderID = 'normal';

		} else if ( material instanceof THREE::MeshBasicMaterial ) {

			shaderID = 'basic';

		} else if ( material instanceof THREE::MeshLambertMaterial ) {

			shaderID = 'lambert';

		} else if ( material instanceof THREE::MeshPhongMaterial ) {

			shaderID = 'phong';

		} else if ( material instanceof THREE::LineBasicMaterial ) {

			shaderID = 'basic';

		} else if ( material instanceof THREE::ParticleBasicMaterial ) {

			shaderID = 'particle_basic';

		}

		if ( shaderID ) {

			setMaterialShaders( material, THREE::ShaderLib[ shaderID ] );

		}

		// heuristics to create shader parameters according to lights in the scene
		// (not to blow over maxLights budget)

		maxLightCount = allocateLights( lights );

		maxShadows = allocateShadows( lights );

		maxBones = allocateBones( object );

		parameters = {

			map: !!material.map,
			envMap: !!material.envMap,
			lightMap: !!material.lightMap,
			bumpMap: !!material.bumpMap,
			specularMap: !!material.specularMap,

			vertexColors: material.vertexColors,

			fog: fog,
			useFog: material.fog,

			sizeAttenuation: material.sizeAttenuation,

			skinning: material.skinning,
			maxBones: maxBones,
			useVertexTexture: _supportsBoneTextures && object && object.useVertexTexture,
			boneTextureWidth: object && object.boneTextureWidth,
			boneTextureHeight: object && object.boneTextureHeight,

			morphTargets: material.morphTargets,
			morphNormals: material.morphNormals,
			maxMorphTargets: maxMorphTargets,
			maxMorphNormals: maxMorphNormals,

			maxDirLights: maxLightCount.directional,
			maxPointLights: maxLightCount.point,
			maxSpotLights: maxLightCount.spot,

			maxShadows: maxShadows,
			shadowMapEnabled: shadowMapEnabled && object.receiveShadow,
			shadowMapSoft: shadowMapSoft,
			shadowMapDebug: shadowMapDebug,
			shadowMapCascade: shadowMapCascade,

			alphaTest: material.alphaTest,
			metal: material.metal,
			perPixel: material.perPixel,
			wrapAround: material.wrapAround,
			doubleSided: material.side == THREE::DoubleSide

		};

		material.program = buildProgram( shaderID, material.fragmentShader, material.vertexShader, material.uniforms, material.attributes, parameters );

		auto attributes = material.program.attributes;

		if ( attributes.position >= 0 ) _gl.enableVertexAttribArray( attributes.position );
		if ( attributes.color >= 0 ) _gl.enableVertexAttribArray( attributes.color );
		if ( attributes.normal >= 0 ) _gl.enableVertexAttribArray( attributes.normal );
		if ( attributes.tangent >= 0 ) _gl.enableVertexAttribArray( attributes.tangent );

		if ( material.skinning &&
			 attributes.skinVertexA >=0 && attributes.skinVertexB >= 0 &&
			 attributes.skinIndex >= 0 && attributes.skinWeight >= 0 ) {

			glEnableVertexAttribArray( attributes.skinVertexA );
			glEnableVertexAttribArray( attributes.skinVertexB );
			glEnableVertexAttribArray( attributes.skinIndex );
			glEnableVertexAttribArray( attributes.skinWeight );

		}

		if ( material.attributes ) {

			for ( a in material.attributes ) {

				if( attributes[ a ] !== undefined && attributes[ a ] >= 0 ) _gl.enableVertexAttribArray( attributes[ a ] );

			}

		}

		if ( material.morphTargets ) {

			material.numSupportedMorphTargets = 0;

			auto id, base = "morphTarget";

			for ( i = 0; i < maxMorphTargets; i ++ ) {

				id = base + i;

				if ( attributes[ id ] >= 0 ) {

					glEnableVertexAttribArray( attributes[ id ] );
					material.numSupportedMorphTargets ++;

				}

			}

		}

		if ( material.morphNormals ) {

			material.numSupportedMorphNormals = 0;

			auto id, base = "morphNormal";

			for ( i = 0; i < maxMorphNormals; i ++ ) {

				id = base + i;

				if ( attributes[ id ] >= 0 ) {

					glEnableVertexAttribArray( attributes[ id ] );
					material.numSupportedMorphNormals ++;

				}

			}

		}

		material.uniformsList = [];

		for ( u in material.uniforms ) {

			material.uniformsList.push_back( [ material.uniforms[ u ], u ] );

		}

	};

	function setMaterialShaders( material, shaders ) {

		material.uniforms = THREE::UniformsUtils.clone( shaders.uniforms );
		material.vertexShader = shaders.vertexShader;
		material.fragmentShader = shaders.fragmentShader;

	};

	function setProgram( camera, lights, fog, material, object ) {

		if ( material.needsUpdate ) {

			if ( material.program ) _deallocateMaterial( material );

			_initMaterial( material, lights, fog, object );
			material.needsUpdate = false;

		}

		if ( material.morphTargets ) {

			if ( ! object.__glMorphTargetInfluences ) {

				object.__glMorphTargetInfluences.resize( _maxMorphTargets );

			}

		}

		auto refreshMaterial = false;

		auto program = material.program,
			p_uniforms = program.uniforms,
			m_uniforms = material.uniforms;

		if ( program !== _currentProgram ) {

			gluseProgram( program );
			_currentProgram = program;

			refreshMaterial = true;

		}

		if ( material.id !== _currentMaterialId ) {

			_currentMaterialId = material.id;
			refreshMaterial = true;

		}

		if ( refreshMaterial || camera !== _currentCamera ) {

			gluniformMatrix4fv( p_uniforms.projectionMatrix, false, camera._projectionMatrixArray );

			if ( camera !== _currentCamera ) _currentCamera = camera;

		}

		if ( refreshMaterial ) {

			// refresh uniforms common to several materials

			if ( fog && material.fog ) {

				refreshUniformsFog( m_uniforms, fog );

			}

			if ( material instanceof THREE::MeshPhongMaterial ||
				 material instanceof THREE::MeshLambertMaterial ||
				 material.lights ) {

				if ( _lightsNeedUpdate ) {

					setupLights( program, lights );
					_lightsNeedUpdate = false;

				}

				refreshUniformsLights( m_uniforms, _lights );

			}

			if ( material instanceof THREE::MeshBasicMaterial ||
				 material instanceof THREE::MeshLambertMaterial ||
				 material instanceof THREE::MeshPhongMaterial ) {

				refreshUniformsCommon( m_uniforms, material );

			}

			// refresh single material specific uniforms

			if ( material instanceof THREE::LineBasicMaterial ) {

				refreshUniformsLine( m_uniforms, material );

			} else if ( material instanceof THREE::ParticleBasicMaterial ) {

				refreshUniformsParticle( m_uniforms, material );

			} else if ( material instanceof THREE::MeshPhongMaterial ) {

				refreshUniformsPhong( m_uniforms, material );

			} else if ( material instanceof THREE::MeshLambertMaterial ) {

				refreshUniformsLambert( m_uniforms, material );

			} else if ( material instanceof THREE::MeshDepthMaterial ) {

				m_uniforms.mNear.value = camera.near;
				m_uniforms.mFar.value = camera.far;
				m_uniforms.opacity.value = material.opacity;

			} else if ( material instanceof THREE::MeshNormalMaterial ) {

				m_uniforms.opacity.value = material.opacity;

			}

			if ( object.receiveShadow && ! material._shadowPass ) {

				refreshUniformsShadow( m_uniforms, lights );

			}

			// load common uniforms

			loadUniformsGeneric( program, material.uniformsList );

			// load material specific uniforms
			// (shader material also gets them for the sake of genericity)

			if ( material instanceof THREE::ShaderMaterial ||
				 material instanceof THREE::MeshPhongMaterial ||
				 material.envMap ) {

				if ( p_uniforms.cameraPosition !== null ) {

					auto position = camera.matrixWorld.getPosition();
					gluniform3f( p_uniforms.cameraPosition, position.x, position.y, position.z );

				}

			}

			if ( material instanceof THREE::MeshPhongMaterial ||
				 material instanceof THREE::MeshLambertMaterial ||
				 material instanceof THREE::ShaderMaterial ||
				 material.skinning ) {

				if ( p_uniforms.viewMatrix !== null ) {

					gluniformMatrix4fv( p_uniforms.viewMatrix, false, camera._viewMatrixArray );

				}

			}

		}

		if ( material.skinning ) {

			if ( _supportsBoneTextures && object.useVertexTexture ) {

				if ( p_uniforms.boneTexture !== null ) {

					// shadowMap texture array starts from 6
					// texture unit 12 should leave space for 6 shadowmaps

					auto textureUnit = 12;

					gluniform1i( p_uniforms.boneTexture, textureUnit );
					_setTexture( object.boneTexture, textureUnit );

				}

			} else {

				if ( p_uniforms.boneGlobalMatrices !== null ) {

					gluniformMatrix4fv( p_uniforms.boneGlobalMatrices, false, object.boneMatrices );

				}

			}

		}

		loadUniformsMatrices( p_uniforms, object );

		if ( p_uniforms.modelMatrix !== null ) {

			gluniformMatrix4fv( p_uniforms.modelMatrix, false, object.matrixWorld.elements );

		}

		return program;

	};

	// Uniforms (refresh uniforms objects)

	function refreshUniformsCommon ( uniforms, material ) {

		uniforms.opacity.value = material.opacity;

		if ( _gammaInput ) {

			uniforms.diffuse.value.copyGammaToLinear( material.color );

		} else {

			uniforms.diffuse.value = material.color;

		}

		uniforms.map.texture = material.map;
		uniforms.lightMap.texture = material.lightMap;
		uniforms.specularMap.texture = material.specularMap;

		if ( material.bumpMap ) {

			uniforms.bumpMap.texture = material.bumpMap;
			uniforms.bumpScale.value = material.bumpScale;

		}

		// uv repeat and offset setting priorities
		//	1. color map
		//	2. specular map
		//	3. bump map

		auto uvScaleMap;

		if ( material.map ) {

			uvScaleMap = material.map;

		} else if ( material.specularMap ) {

			uvScaleMap = material.specularMap;

		} else if ( material.bumpMap ) {

			uvScaleMap = material.bumpMap;

		}

		if ( uvScaleMap !== undefined ) {

			auto offset = uvScaleMap.offset;
			auto repeat = uvScaleMap.repeat;

			uniforms.offsetRepeat.value.set( offset.x, offset.y, repeat.x, repeat.y );

		}

		uniforms.envMap.texture = material.envMap;
		uniforms.flipEnvMap.value = ( material.envMap instanceof THREE::WebGLRenderTargetCube ) ? 1 : -1;

		if ( _gammaInput ) {

			//uniforms.reflectivity.value = material.reflectivity * material.reflectivity;
			uniforms.reflectivity.value = material.reflectivity;

		} else {

			uniforms.reflectivity.value = material.reflectivity;

		}

		uniforms.refractionRatio.value = material.refractionRatio;
		uniforms.combine.value = material.combine;
		uniforms.useRefract.value = material.envMap && material.envMap.mapping instanceof THREE::CubeRefractionMapping;

	};

	function refreshUniformsLine ( uniforms, material ) {

		uniforms.diffuse.value = material.color;
		uniforms.opacity.value = material.opacity;

	};

	function refreshUniformsParticle ( uniforms, material ) {

		uniforms.psColor.value = material.color;
		uniforms.opacity.value = material.opacity;
		uniforms.size.value = material.size;
		uniforms.scale.value = _canvas.height / 2.0; // TODO: Cache

		uniforms.map.texture = material.map;

	};

	function refreshUniformsFog ( uniforms, fog ) {

		uniforms.fogColor.value = fog.color;

		if ( fog instanceof THREE::Fog ) {

			uniforms.fogNear.value = fog.near;
			uniforms.fogFar.value = fog.far;

		} else if ( fog instanceof THREE::FogExp2 ) {

			uniforms.fogDensity.value = fog.density;

		}

	};

	function refreshUniformsPhong ( uniforms, material ) {

		uniforms.shininess.value = material.shininess;

		if ( _gammaInput ) {

			uniforms.ambient.value.copyGammaToLinear( material.ambient );
			uniforms.emissive.value.copyGammaToLinear( material.emissive );
			uniforms.specular.value.copyGammaToLinear( material.specular );

		} else {

			uniforms.ambient.value = material.ambient;
			uniforms.emissive.value = material.emissive;
			uniforms.specular.value = material.specular;

		}

		if ( material.wrapAround ) {

			uniforms.wrapRGB.value.copy( material.wrapRGB );

		}

	};

	function refreshUniformsLambert ( uniforms, material ) {

		if ( _gammaInput ) {

			uniforms.ambient.value.copyGammaToLinear( material.ambient );
			uniforms.emissive.value.copyGammaToLinear( material.emissive );

		} else {

			uniforms.ambient.value = material.ambient;
			uniforms.emissive.value = material.emissive;

		}

		if ( material.wrapAround ) {

			uniforms.wrapRGB.value.copy( material.wrapRGB );

		}

	};

	function refreshUniformsLights ( uniforms, lights ) {

		uniforms.ambientLightColor.value = lights.ambient;

		uniforms.directionalLightColor.value = lights.directional.colors;
		uniforms.directionalLightDirection.value = lights.directional.positions;

		uniforms.pointLightColor.value = lights.point.colors;
		uniforms.pointLightPosition.value = lights.point.positions;
		uniforms.pointLightDistance.value = lights.point.distances;

		uniforms.spotLightColor.value = lights.spot.colors;
		uniforms.spotLightPosition.value = lights.spot.positions;
		uniforms.spotLightDistance.value = lights.spot.distances;
		uniforms.spotLightDirection.value = lights.spot.directions;
		uniforms.spotLightAngle.value = lights.spot.angles;
		uniforms.spotLightExponent.value = lights.spot.exponents;

	};

	function refreshUniformsShadow ( uniforms, lights ) {

		if ( uniforms.shadowMatrix ) {

			auto j = 0;

			for ( auto i = 0, il = lights.size(); i < il; i ++ ) {

				auto light = lights[ i ];

				if ( ! light.castShadow ) continue;

				if ( light instanceof THREE::SpotLight || ( light instanceof THREE::DirectionalLight && ! light.shadowCascade ) ) {

					uniforms.shadowMap.texture[ j ] = light.shadowMap;
					uniforms.shadowMapSize.value[ j ] = light.shadowMapSize;

					uniforms.shadowMatrix.value[ j ] = light.shadowMatrix;

					uniforms.shadowDarkness.value[ j ] = light.shadowDarkness;
					uniforms.shadowBias.value[ j ] = light.shadowBias;

					j ++;

				}

			}

		}

	};

	// Uniforms (load to GPU)

	function loadUniformsMatrices ( uniforms, object ) {

		gluniformMatrix4fv( uniforms.modelViewMatrix, false, object._modelViewMatrix.elements );

		if ( uniforms.normalMatrix ) {

			gluniformMatrix3fv( uniforms.normalMatrix, false, object._normalMatrix.elements );

		}

	};

	function loadUniformsGeneric ( program, uniforms ) {

		auto uniform, value, type, location, texture, i, il, j, jl, offset;

		for ( j = 0, jl = uniforms.size(); j < jl; j ++ ) {

			location = program.uniforms[ uniforms[ j ][ 1 ] ];
			if ( !location ) continue;

			uniform = uniforms[ j ][ 0 ];

			type = uniform.type;
			value = uniform.value;

			if ( type == "i" ) { // single integer

				gluniform1i( location, value );

			} else if ( type == "f" ) { // single float

				gluniform1f( location, value );

			} else if ( type == "v2" ) { // single THREE::Vector2

				gluniform2f( location, value.x, value.y );

			} else if ( type == "v3" ) { // single THREE::Vector3

				gluniform3f( location, value.x, value.y, value.z );

			} else if ( type == "v4" ) { // single THREE::Vector4

				gluniform4f( location, value.x, value.y, value.z, value.w );

			} else if ( type == "c" ) { // single THREE::Color

				gluniform3f( location, value.r, value.g, value.b );

			} else if ( type == "iv1" ) { // flat array of integers (JS or typed array)

				gluniform1iv( location, value );

			} else if ( type == "iv" ) { // flat array of integers with 3 x N size (JS or typed array)

				gluniform3iv( location, value );

			} else if ( type == "fv1" ) { // flat array of floats (JS or typed array)

				gluniform1fv( location, value );

			} else if ( type == "fv" ) { // flat array of floats with 3 x N size (JS or typed array)

				gluniform3fv( location, value );

			} else if ( type == "v2v" ) { // array of THREE::Vector2

				if ( uniform._array == undefined ) {

					uniform._array.resize( 2 * value.size() );

				}

				for ( i = 0, il = value.size(); i < il; i ++ ) {

					offset = i * 2;

					uniform._array[ offset ] 	 = value[ i ].x;
					uniform._array[ offset + 1 ] = value[ i ].y;

				}

				gluniform2fv( location, uniform._array );

			} else if ( type == "v3v" ) { // array of THREE::Vector3

				if ( uniform._array == undefined ) {

					uniform._array.resize( 3 * value.size() );

				}

				for ( i = 0, il = value.size(); i < il; i ++ ) {

					offset = i * 3;

					uniform._array[ offset ] 	 = value[ i ].x;
					uniform._array[ offset + 1 ] = value[ i ].y;
					uniform._array[ offset + 2 ] = value[ i ].z;

				}

				gluniform3fv( location, uniform._array );

			} else if ( type == "v4v" ) { // array of THREE::Vector4

				if ( uniform._array == undefined ) {

					uniform._array.resize( 4 * value.size() );

				}

				for ( i = 0, il = value.size(); i < il; i ++ ) {

					offset = i * 4;

					uniform._array[ offset ] 	 = value[ i ].x;
					uniform._array[ offset + 1 ] = value[ i ].y;
					uniform._array[ offset + 2 ] = value[ i ].z;
					uniform._array[ offset + 3 ] = value[ i ].w;

				}

				gluniform4fv( location, uniform._array );

			} else if ( type == "m4") { // single THREE::Matrix4

				if ( uniform._array == undefined ) {

					uniform._array.resize( 16 );

				}

				value.flattenToArray( uniform._array );
				gluniformMatrix4fv( location, false, uniform._array );

			} else if ( type == "m4v" ) { // array of THREE::Matrix4

				if ( uniform._array == undefined ) {

					uniform._array.resize( 16 * value.size() );

				}

				for ( i = 0, il = value.size(); i < il; i ++ ) {

					value[ i ].flattenToArrayOffset( uniform._array, i * 16 );

				}

				gluniformMatrix4fv( location, false, uniform._array );

			} else if ( type == "t" ) { // single THREE::Texture (2d or cube)

				gluniform1i( location, value );

				texture = uniform.texture;

				if ( !texture ) continue;

				if ( texture.image instanceof Array && texture.image.size() == 6 ) {

					setCubeTexture( texture, value );

				} else if ( texture instanceof THREE::WebGLRenderTargetCube ) {

					setCubeTextureDynamic( texture, value );

				} else {

					_setTexture( texture, value );

				}

			} else if ( type == "tv" ) { // array of THREE::Texture (2d)

				if ( uniform._array == undefined ) {

					uniform._array = [];

					for( i = 0, il = uniform.texture.size(); i < il; i ++ ) {

						uniform._array[ i ] = value + i;

					}

				}

				gluniform1iv( location, uniform._array );

				for( i = 0, il = uniform.texture.size(); i < il; i ++ ) {

					texture = uniform.texture[ i ];

					if ( !texture ) continue;

					_setTexture( texture, uniform._array[ i ] );

				}

			}

		}

	};

	function setupMatrices ( object, camera ) {

		object._modelViewMatrix.multiply( camera.matrixWorldInverse, object.matrixWorld );

		object._normalMatrix.getInverse( object._modelViewMatrix );
		object._normalMatrix.transpose();

	};

	function setupLights ( program, lights ) {

		auto l, ll, light, n,
		r = 0, g = 0, b = 0,
		color, position, intensity, distance,

		zlights = _lights,

		dcolors = zlights.directional.colors,
		dpositions = zlights.directional.positions,

		pcolors = zlights.point.colors,
		ppositions = zlights.point.positions,
		pdistances = zlights.point.distances,

		scolors = zlights.spot.colors,
		spositions = zlights.spot.positions,
		sdistances = zlights.spot.distances,
		sdirections = zlights.spot.directions,
		sangles = zlights.spot.angles,
		sexponents = zlights.spot.exponents,

		dlength = 0,
		plength = 0,
		slength = 0,

		doffset = 0,
		poffset = 0,
		soffset = 0;

		for ( l = 0, ll = lights.size(); l < ll; l ++ ) {

			light = lights[ l ];

			if ( light.onlyShadow || ! light.visible ) continue;

			color = light.color;
			intensity = light.intensity;
			distance = light.distance;

			if ( light instanceof THREE::AmbientLight ) {

				if ( _gammaInput ) {

					r += color.r * color.r;
					g += color.g * color.g;
					b += color.b * color.b;

				} else {

					r += color.r;
					g += color.g;
					b += color.b;

				}

			} else if ( light instanceof THREE::DirectionalLight ) {

				doffset = dlength * 3;

				if ( _gammaInput ) {

					dcolors[ doffset ]     = color.r * color.r * intensity * intensity;
					dcolors[ doffset + 1 ] = color.g * color.g * intensity * intensity;
					dcolors[ doffset + 2 ] = color.b * color.b * intensity * intensity;

				} else {

					dcolors[ doffset ]     = color.r * intensity;
					dcolors[ doffset + 1 ] = color.g * intensity;
					dcolors[ doffset + 2 ] = color.b * intensity;

				}

				_direction.copy( light.matrixWorld.getPosition() );
				_direction.subSelf( light.target.matrixWorld.getPosition() );
				_direction.normalize();

				dpositions[ doffset ]     = _direction.x;
				dpositions[ doffset + 1 ] = _direction.y;
				dpositions[ doffset + 2 ] = _direction.z;

				dlength += 1;

			} else if( light instanceof THREE::PointLight ) {

				poffset = plength * 3;

				if ( _gammaInput ) {

					pcolors[ poffset ]     = color.r * color.r * intensity * intensity;
					pcolors[ poffset + 1 ] = color.g * color.g * intensity * intensity;
					pcolors[ poffset + 2 ] = color.b * color.b * intensity * intensity;

				} else {

					pcolors[ poffset ]     = color.r * intensity;
					pcolors[ poffset + 1 ] = color.g * intensity;
					pcolors[ poffset + 2 ] = color.b * intensity;

				}

				position = light.matrixWorld.getPosition();

				ppositions[ poffset ]     = position.x;
				ppositions[ poffset + 1 ] = position.y;
				ppositions[ poffset + 2 ] = position.z;

				pdistances[ plength ] = distance;

				plength += 1;

			} else if( light instanceof THREE::SpotLight ) {

				soffset = slength * 3;

				if ( _gammaInput ) {

					scolors[ soffset ]     = color.r * color.r * intensity * intensity;
					scolors[ soffset + 1 ] = color.g * color.g * intensity * intensity;
					scolors[ soffset + 2 ] = color.b * color.b * intensity * intensity;

				} else {

					scolors[ soffset ]     = color.r * intensity;
					scolors[ soffset + 1 ] = color.g * intensity;
					scolors[ soffset + 2 ] = color.b * intensity;

				}

				position = light.matrixWorld.getPosition();

				spositions[ soffset ]     = position.x;
				spositions[ soffset + 1 ] = position.y;
				spositions[ soffset + 2 ] = position.z;

				sdistances[ slength ] = distance;

				_direction.copy( position );
				_direction.subSelf( light.target.matrixWorld.getPosition() );
				_direction.normalize();

				sdirections[ soffset ]     = _direction.x;
				sdirections[ soffset + 1 ] = _direction.y;
				sdirections[ soffset + 2 ] = _direction.z;

				sangles[ slength ] = Math::cos( light.angle );
				sexponents[ slength ] = light.exponent;

				slength += 1;

			}

		}

		// null eventual remains from removed lights
		// (this is to avoid if in shader)

		for ( l = dlength * 3, ll = dcolors.size(); l < ll; l ++ ) dcolors[ l ] = 0.0;
		for ( l = plength * 3, ll = pcolors.size(); l < ll; l ++ ) pcolors[ l ] = 0.0;
		for ( l = slength * 3, ll = scolors.size(); l < ll; l ++ ) scolors[ l ] = 0.0;

		zlights.directional.size() = dlength;
		zlights.point.size() = plength;
		zlights.spot.size() = slength;

		zlights.ambient[ 0 ] = r;
		zlights.ambient[ 1 ] = g;
		zlights.ambient[ 2 ] = b;

	};

	// GL state setting

	setFaceCulling = function ( cullFace, frontFace ) {

		if ( cullFace ) {

			if ( !frontFace || frontFace == "ccw" ) {

				glFrontFace( GL_CCW );

			} else {

				glFrontFace( GL_CW );

			}

			if( cullFace == "back" ) {

				glCullFace( GL_BACK );

			} else if( cullFace == "front" ) {

				glCullFace( GL_FRONT );

			} else {

				glCullFace( GL_FRONT_AND_BACK );

			}

			glEnable( GL_CULL_FACE );

		} else {

			glDisable( GL_CULL_FACE );

		}

	};

	setMaterialFaces = function ( material ) {

		auto doubleSided = material.side == THREE::DoubleSide;
		auto flipSided = material.side == THREE::BackSide;

		if ( _oldDoubleSided !== doubleSided ) {

			if ( doubleSided ) {

				glDisable( GL_CULL_FACE );

			} else {

				glEnable( GL_CULL_FACE );

			}

			_oldDoubleSided = doubleSided;

		}

		if ( _oldFlipSided !== flipSided ) {

			if ( flipSided ) {

				glFrontFace( GL_CW );

			} else {

				glFrontFace( GL_CCW );

			}

			_oldFlipSided = flipSided;

		}

	};

	setDepthTest = function ( depthTest ) {

		if ( _oldDepthTest !== depthTest ) {

			if ( depthTest ) {

				glEnable( GL_DEPTH_TEST );

			} else {

				glDisable( GL_DEPTH_TEST );

			}

			_oldDepthTest = depthTest;

		}

	};

	setDepthWrite = function ( depthWrite ) {

		if ( _oldDepthWrite !== depthWrite ) {

			glDepthMask( depthWrite );
			_oldDepthWrite = depthWrite;

		}

	};

	function setLineWidth ( width ) {

		if ( width !== _oldLineWidth ) {

			gllineWidth( width );

			_oldLineWidth = width;

		}

	};

	function setPolygonOffset ( polygonoffset, factor, units ) {

		if ( _oldPolygonOffset !== polygonoffset ) {

			if ( polygonoffset ) {

				glEnable( GL_POLYGON_OFFSET_FILL );

			} else {

				glDisable( GL_POLYGON_OFFSET_FILL );

			}

			_oldPolygonOffset = polygonoffset;

		}

		if ( polygonoffset && ( _oldPolygonOffsetFactor !== factor || _oldPolygonOffsetUnits !== units ) ) {

			glpolygonOffset( factor, units );

			_oldPolygonOffsetFactor = factor;
			_oldPolygonOffsetUnits = units;

		}

	};

	setBlending = function ( blending, blendEquation, blendSrc, blendDst ) {

		if ( blending !== _oldBlending ) {

			if ( blending == THREE::NoBlending ) {

				glDisable( GL_BLEND );

			} else if ( blending == THREE::AdditiveBlending ) {

				glEnable( GL_BLEND );
				glBlendEquation( GL_FUNC_ADD );
				glBlendFunc( GL_SRC_ALPHA, _gl.ONE );

			} else if ( blending == THREE::SubtractiveBlending ) {

				// TODO: Find blendFuncSeparate() combination
				glEnable( GL_BLEND );
				glBlendEquation( GL_FUNC_ADD );
				glBlendFunc( GL_ZERO, _gl.ONE_MINUS_SRC_COLOR );

			} else if ( blending == THREE::MultiplyBlending ) {

				// TODO: Find blendFuncSeparate() combination
				glEnable( GL_BLEND );
				glBlendEquation( GL_FUNC_ADD );
				glBlendFunc( GL_ZERO, _gl.SRC_COLOR );

			} else if ( blending == THREE::CustomBlending ) {

				glEnable( GL_BLEND );

			} else {

				glEnable( GL_BLEND );
				glBlendEquationSeparate( GL_FUNC_ADD, _gl.FUNC_ADD );
				glBlendFuncSeparate( GL_SRC_ALPHA, _gl.ONE_MINUS_SRC_ALPHA, _gl.ONE, _gl.ONE_MINUS_SRC_ALPHA );

			}

			_oldBlending = blending;

		}

		if ( blending == THREE::CustomBlending ) {

			if ( blendEquation !== _oldBlendEquation ) {

				glBlendEquation( paramThreeToGL( blendEquation ) );

				_oldBlendEquation = blendEquation;

			}

			if ( blendSrc !== _oldBlendSrc || blendDst !== _oldBlendDst ) {

				glBlendFunc( paramThreeToGL( blendSrc ), paramThreeToGL( blendDst ) );

				_oldBlendSrc = blendSrc;
				_oldBlendDst = blendDst;

			}

		} else {

			_oldBlendEquation = null;
			_oldBlendSrc = null;
			_oldBlendDst = null;

		}

	};

	// Shaders

	function buildProgram ( shaderID, fragmentShader, vertexShader, uniforms, attributes, parameters ) {

		auto p, pl, program, code;
		auto chunks = [];

		// Generate code

		if ( shaderID ) {

			chunks.push_back( shaderID );

		} else {

			chunks.push_back( fragmentShader );
			chunks.push_back( vertexShader );

		}

		for ( p in parameters ) {

			chunks.push_back( p );
			chunks.push_back( parameters[ p ] );

		}

		code = chunks.join();

		// Check if code has been already compiled

		for ( p = 0, pl = _programs.size(); p < pl; p ++ ) {

			auto programInfo = _programs[ p ];

			if ( programInfo.code == code ) {

				// console.log( "Code already compiled." /*: \n\n" + code*/ );

				programInfo.usedTimes ++;

				return programInfo.program;

			}

		}

		//console.log( "building new program " );

		//

		program = glCreateProgram();

		auto prefix_vertex = [

			"precision " + _precision + " float;",

			_supportsVertexTextures ? "#define VERTEX_TEXTURES" : "",

			_gammaInput ? "#define GAMMA_INPUT" : "",
			_gammaOutput ? "#define GAMMA_OUTPUT" : "",
			_physicallyBasedShading ? "#define PHYSICALLY_BASED_SHADING" : "",

			"#define MAX_DIR_LIGHTS " + parameters.maxDirLights,
			"#define MAX_POINT_LIGHTS " + parameters.maxPointLights,
			"#define MAX_SPOT_LIGHTS " + parameters.maxSpotLights,

			"#define MAX_SHADOWS " + parameters.maxShadows,

			"#define MAX_BONES " + parameters.maxBones,

			parameters.map ? "#define USE_MAP" : "",
			parameters.envMap ? "#define USE_ENVMAP" : "",
			parameters.lightMap ? "#define USE_LIGHTMAP" : "",
			parameters.bumpMap ? "#define USE_BUMPMAP" : "",
			parameters.specularMap ? "#define USE_SPECULARMAP" : "",
			parameters.vertexColors ? "#define USE_COLOR" : "",

			parameters.skinning ? "#define USE_SKINNING" : "",
			parameters.useVertexTexture ? "#define BONE_TEXTURE" : "",
			parameters.boneTextureWidth ? "#define N_BONE_PIXEL_X " + parameters.boneTextureWidth.toFixed( 1 ) : "",
			parameters.boneTextureHeight ? "#define N_BONE_PIXEL_Y " + parameters.boneTextureHeight.toFixed( 1 ) : "",

			parameters.morphTargets ? "#define USE_MORPHTARGETS" : "",
			parameters.morphNormals ? "#define USE_MORPHNORMALS" : "",
			parameters.perPixel ? "#define PHONG_PER_PIXEL" : "",
			parameters.wrapAround ? "#define WRAP_AROUND" : "",
			parameters.doubleSided ? "#define DOUBLE_SIDED" : "",

			parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "",
			parameters.shadowMapSoft ? "#define SHADOWMAP_SOFT" : "",
			parameters.shadowMapDebug ? "#define SHADOWMAP_DEBUG" : "",
			parameters.shadowMapCascade ? "#define SHADOWMAP_CASCADE" : "",

			parameters.sizeAttenuation ? "#define USE_SIZEATTENUATION" : "",

			"uniform mat4 modelMatrix;",
			"uniform mat4 modelViewMatrix;",
			"uniform mat4 projectionMatrix;",
			"uniform mat4 viewMatrix;",
			"uniform mat3 normalMatrix;",
			"uniform vec3 cameraPosition;",

			"attribute vec3 position;",
			"attribute vec3 normal;",
			"attribute vec2 uv;",
			"attribute vec2 uv2;",

			"#ifdef USE_COLOR",

				"attribute vec3 color;",

			"#endif",

			"#ifdef USE_MORPHTARGETS",

				"attribute vec3 morphTarget0;",
				"attribute vec3 morphTarget1;",
				"attribute vec3 morphTarget2;",
				"attribute vec3 morphTarget3;",

				"#ifdef USE_MORPHNORMALS",

					"attribute vec3 morphNormal0;",
					"attribute vec3 morphNormal1;",
					"attribute vec3 morphNormal2;",
					"attribute vec3 morphNormal3;",

				"#else",

					"attribute vec3 morphTarget4;",
					"attribute vec3 morphTarget5;",
					"attribute vec3 morphTarget6;",
					"attribute vec3 morphTarget7;",

				"#endif",

			"#endif",

			"#ifdef USE_SKINNING",

				"attribute vec4 skinVertexA;",
				"attribute vec4 skinVertexB;",
				"attribute vec4 skinIndex;",
				"attribute vec4 skinWeight;",

			"#endif",

			""

		].join("\n");

		auto prefix_fragment = [

			"precision " + _precision + " float;",

			parameters.bumpMap ? "#extension GL_OES_standard_derivatives : enable" : "",

			"#define MAX_DIR_LIGHTS " + parameters.maxDirLights,
			"#define MAX_POINT_LIGHTS " + parameters.maxPointLights,
			"#define MAX_SPOT_LIGHTS " + parameters.maxSpotLights,

			"#define MAX_SHADOWS " + parameters.maxShadows,

			parameters.alphaTest ? "#define ALPHATEST " + parameters.alphaTest: "",

			_gammaInput ? "#define GAMMA_INPUT" : "",
			_gammaOutput ? "#define GAMMA_OUTPUT" : "",
			_physicallyBasedShading ? "#define PHYSICALLY_BASED_SHADING" : "",

			( parameters.useFog && parameters.fog ) ? "#define USE_FOG" : "",
			( parameters.useFog && parameters.fog instanceof THREE::FogExp2 ) ? "#define FOG_EXP2" : "",

			parameters.map ? "#define USE_MAP" : "",
			parameters.envMap ? "#define USE_ENVMAP" : "",
			parameters.lightMap ? "#define USE_LIGHTMAP" : "",
			parameters.bumpMap ? "#define USE_BUMPMAP" : "",
			parameters.specularMap ? "#define USE_SPECULARMAP" : "",
			parameters.vertexColors ? "#define USE_COLOR" : "",

			parameters.metal ? "#define METAL" : "",
			parameters.perPixel ? "#define PHONG_PER_PIXEL" : "",
			parameters.wrapAround ? "#define WRAP_AROUND" : "",
			parameters.doubleSided ? "#define DOUBLE_SIDED" : "",

			parameters.shadowMapEnabled ? "#define USE_SHADOWMAP" : "",
			parameters.shadowMapSoft ? "#define SHADOWMAP_SOFT" : "",
			parameters.shadowMapDebug ? "#define SHADOWMAP_DEBUG" : "",
			parameters.shadowMapCascade ? "#define SHADOWMAP_CASCADE" : "",

			"uniform mat4 viewMatrix;",
			"uniform vec3 cameraPosition;",
			""

		].join("\n");

		auto glFragmentShader = getShader( "fragment", prefix_fragment + fragmentShader );
		auto glVertexShader = getShader( "vertex", prefix_vertex + vertexShader );

		glattachShader( program, glVertexShader );
		glattachShader( program, glFragmentShader );

		gllinkProgram( program );

		if ( !_gl.getProgramParameter( program, _gl.LINK_STATUS ) ) {

			console.error( "Could not initialise shader\n" + "VALIDATE_STATUS: " + _gl.getProgramParameter( program, _gl.VALIDATE_STATUS ) + ", gl error [" + _gl.getError() + "]" );

		}

		// clean up

		glDeleteShader( glFragmentShader );
		glDeleteShader( glVertexShader );

		//console.log( prefix_fragment + fragmentShader );
		//console.log( prefix_vertex + vertexShader );

		program.uniforms = {};
		program.attributes = {};

		auto identifiers, u, a, i;

		// cache uniform locations

		identifiers = [

			'viewMatrix', 'modelViewMatrix', 'projectionMatrix', 'normalMatrix', 'modelMatrix', 'cameraPosition',
			'morphTargetInfluences'

		];

		if ( parameters.useVertexTexture ) {

			identifiers.push_back( 'boneTexture' );

		} else {

			identifiers.push_back( 'boneGlobalMatrices' );

		}

		for ( u in uniforms ) {

			identifiers.push_back( u );

		}

		cacheUniformLocations( program, identifiers );

		// cache attributes locations

		identifiers = [

			"position", "normal", "uv", "uv2", "tangent", "color",
			"skinVertexA", "skinVertexB", "skinIndex", "skinWeight"

		];

		for ( i = 0; i < parameters.maxMorphTargets; i ++ ) {

			identifiers.push_back( "morphTarget" + i );

		}

		for ( i = 0; i < parameters.maxMorphNormals; i ++ ) {

			identifiers.push_back( "morphNormal" + i );

		}

		for ( a in attributes ) {

			identifiers.push_back( a );

		}

		cacheAttributeLocations( program, identifiers );

		program.id = _programs_counter ++;

		_programs.push_back( { program: program, code: code, usedTimes: 1 } );

		_info.memory.programs = _programs.size();

		return program;

	};

	// Shader parameters cache

	function cacheUniformLocations ( program, identifiers ) {

		auto i, l, id;

		for( i = 0, l = identifiers.size(); i < l; i ++ ) {

			id = identifiers[ i ];
			program.uniforms[ id ] = _gl.getUniformLocation( program, id );

		}

	};

	function cacheAttributeLocations ( program, identifiers ) {

		auto i, l, id;

		for( i = 0, l = identifiers.size(); i < l; i ++ ) {

			id = identifiers[ i ];
			program.attributes[ id ] = _gl.getAttribLocation( program, id );

		}

	};

	function addLineNumbers ( string ) {

		auto chunks = string.split( "\n" );

		for ( auto i = 0, il = chunks.size(); i < il; i ++ ) {

			// Chrome reports shader errors on lines
			// starting counting from 1

			chunks[ i ] = ( i + 1 ) + ": " + chunks[ i ];

		}

		return chunks.join( "\n" );

	};

	function getShader ( type, string ) {

		auto shader;

		if ( type == "fragment" ) {

			shader = glCreateShader( GL_FRAGMENT_SHADER );

		} else if ( type == "vertex" ) {

			shader = glCreateShader( GL_VERTEX_SHADER );

		}

		glshaderSource( shader, string );
		glcompileShader( shader );

		if ( !_gl.getShaderParameter( shader, _gl.COMPILE_STATUS ) ) {

			console.error( GL_getShaderInfoLog( shader ) );
			console.error( addLineNumbers( string ) );
			return null;

		}

		return shader;

	};

	// Textures


	function isPowerOfTwo ( value ) {

		return ( value & ( value - 1 ) ) == 0;

	};

	function setTextureParameters ( textureType, texture, isImagePowerOfTwo ) {

		if ( isImagePowerOfTwo ) {

			gltexParameteri( textureType, _gl.TEXTURE_WRAP_S, paramThreeToGL( texture.wrapS ) );
			gltexParameteri( textureType, _gl.TEXTURE_WRAP_T, paramThreeToGL( texture.wrapT ) );

			gltexParameteri( textureType, _gl.TEXTURE_MAG_FILTER, paramThreeToGL( texture.magFilter ) );
			gltexParameteri( textureType, _gl.TEXTURE_MIN_FILTER, paramThreeToGL( texture.minFilter ) );

		} else {

			gltexParameteri( textureType, _gl.TEXTURE_WRAP_S, _gl.CLAMP_TO_EDGE );
			gltexParameteri( textureType, _gl.TEXTURE_WRAP_T, _gl.CLAMP_TO_EDGE );

			gltexParameteri( textureType, _gl.TEXTURE_MAG_FILTER, filterFallback( texture.magFilter ) );
			gltexParameteri( textureType, _gl.TEXTURE_MIN_FILTER, filterFallback( texture.minFilter ) );

		}

		if ( _glExtensionTextureFilterAnisotropic && texture.type !== THREE::FloatType ) {

			if ( texture.anisotropy > 1 || texture.__oldAnisotropy ) {

				gltexParameterf( textureType, _glExtensionTextureFilterAnisotropic.TEXTURE_MAX_ANISOTROPY_EXT, Math::min( texture.anisotropy, _maxAnisotropy ) );
				texture.__oldAnisotropy = texture.anisotropy;

			}

		}

	};

	setTexture = function ( texture, slot ) {

		if ( texture.needsUpdate ) {

			if ( ! texture.__glInit ) {

				texture.__glInit = true;
				texture.__glTexture = glCreateTexture();

				_info.memory.textures ++;

			}

			glactiveTexture( GL_TEXTURE0 + slot );
			glBindTexture( GL_TEXTURE_2D, texture.__glTexture );

			glpixelStorei( GL_UNPACK_FLIP_Y_WEBGL, texture.flipY );
			glpixelStorei( GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, texture.premultiplyAlpha );

			auto image = texture.image,
			isImagePowerOfTwo = isPowerOfTwo( image.width ) && isPowerOfTwo( image.height ),
			glFormat = paramThreeToGL( texture.format ),
			glType = paramThreeToGL( texture.type );

			setTextureParameters( GL_TEXTURE_2D, texture, isImagePowerOfTwo );

			if ( texture instanceof THREE::DataTexture ) {

				gltexImage2D( GL_TEXTURE_2D, 0, glFormat, image.width, image.height, 0, glFormat, glType, image.data );

			} else {

				gltexImage2D( GL_TEXTURE_2D, 0, glFormat, glFormat, glType, texture.image );

			}

			if ( texture.generateMipmaps && isImagePowerOfTwo ) _gl.generateMipmap( GL_TEXTURE_2D );

			texture.needsUpdate = false;

			if ( texture.onUpdate ) texture.onUpdate();

		} else {

			glactiveTexture( GL_TEXTURE0 + slot );
			glBindTexture( GL_TEXTURE_2D, texture.__glTexture );

		}

	};

	function clampToMaxSize ( image, maxSize ) {

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

	}

	function setCubeTexture ( texture, slot ) {

		if ( texture.image.size() == 6 ) {

			if ( texture.needsUpdate ) {

				if ( ! texture.image.__glTextureCube ) {

					texture.image.__glTextureCube = glCreateTexture();

				}

				glactiveTexture( GL_TEXTURE0 + slot );
				glBindTexture( GL_TEXTURE_CUBE_MAP, texture.image.__glTextureCube );

				glpixelStorei( GL_UNPACK_FLIP_Y_WEBGL, texture.flipY );

				auto cubeImage = [];

				for ( auto i = 0; i < 6; i ++ ) {

					if ( _autoScaleCubemaps ) {

						cubeImage[ i ] = clampToMaxSize( texture.image[ i ], _maxCubemapSize );

					} else {

						cubeImage[ i ] = texture.image[ i ];

					}

				}

				auto image = cubeImage[ 0 ],
				isImagePowerOfTwo = isPowerOfTwo( image.width ) && isPowerOfTwo( image.height ),
				glFormat = paramThreeToGL( texture.format ),
				glType = paramThreeToGL( texture.type );

				setTextureParameters( GL_TEXTURE_CUBE_MAP, texture, isImagePowerOfTwo );

				for ( auto i = 0; i < 6; i ++ ) {

					gltexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, glFormat, glType, cubeImage[ i ] );

				}

				if ( texture.generateMipmaps && isImagePowerOfTwo ) {

					glgenerateMipmap( GL_TEXTURE_CUBE_MAP );

				}

				texture.needsUpdate = false;

				if ( texture.onUpdate ) texture.onUpdate();

			} else {

				glactiveTexture( GL_TEXTURE0 + slot );
				glBindTexture( GL_TEXTURE_CUBE_MAP, texture.image.__glTextureCube );

			}

		}

	};

	function setCubeTextureDynamic ( texture, slot ) {

		glactiveTexture( GL_TEXTURE0 + slot );
		glBindTexture( GL_TEXTURE_CUBE_MAP, texture.__glTexture );

	};

	// Render targets

	function setupFrameBuffer ( framebuffer, renderTarget, textureTarget ) {

		glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
		glframebufferTexture2D( GL_FRAMEBUFFER, _gl.COLOR_ATTACHMENT0, textureTarget, renderTarget.__glTexture, 0 );

	};

	function setupRenderBuffer ( renderbuffer, renderTarget  ) {

		glBindRenderbuffer( GL_RENDERBUFFER, renderbuffer );

		if ( renderTarget.depthBuffer && ! renderTarget.stencilBuffer ) {

			glrenderbufferStorage( GL_RENDERBUFFER, _gl.DEPTH_COMPONENT16, renderTarget.width, renderTarget.height );
			glframebufferRenderbuffer( GL_FRAMEBUFFER, _gl.DEPTH_ATTACHMENT, _gl.RENDERBUFFER, renderbuffer );

		/* For some reason this is not working. Defaulting to RGBA4.
		} else if( ! renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

			glrenderbufferStorage( GL_RENDERBUFFER, _gl.STENCIL_INDEX8, renderTarget.width, renderTarget.height );
			glframebufferRenderbuffer( GL_FRAMEBUFFER, _gl.STENCIL_ATTACHMENT, _gl.RENDERBUFFER, renderbuffer );
		*/
		} else if( renderTarget.depthBuffer && renderTarget.stencilBuffer ) {

			glrenderbufferStorage( GL_RENDERBUFFER, _gl.DEPTH_STENCIL, renderTarget.width, renderTarget.height );
			glframebufferRenderbuffer( GL_FRAMEBUFFER, _gl.DEPTH_STENCIL_ATTACHMENT, _gl.RENDERBUFFER, renderbuffer );

		} else {

			glrenderbufferStorage( GL_RENDERBUFFER, _gl.RGBA4, renderTarget.width, renderTarget.height );

		}

	};

	setRenderTarget = function ( GLRenderTarget& renderTarget ) {

		auto isCube = ( renderTarget instanceof THREE::WebGLRenderTargetCube );

		if ( renderTarget && ! renderTarget.__glFramebuffer ) {

			if ( renderTarget.depthBuffer == undefined ) renderTarget.depthBuffer = true;
			if ( renderTarget.stencilBuffer == undefined ) renderTarget.stencilBuffer = true;

			renderTarget.__glTexture = glCreateTexture();

			// Setup texture, create render and frame buffers

			auto isTargetPowerOfTwo = isPowerOfTwo( renderTarget.width ) && isPowerOfTwo( renderTarget.height ),
				glFormat = paramThreeToGL( renderTarget.format ),
				glType = paramThreeToGL( renderTarget.type );

			if ( isCube ) {

				renderTarget.__glFramebuffer = [];
				renderTarget.__glRenderbuffer = [];

				glBindTexture( GL_TEXTURE_CUBE_MAP, renderTarget.__glTexture );
				setTextureParameters( GL_TEXTURE_CUBE_MAP, renderTarget, isTargetPowerOfTwo );

				for ( auto i = 0; i < 6; i ++ ) {

					renderTarget.__glFramebuffer[ i ] = glCreateFramebuffer();
					renderTarget.__glRenderbuffer[ i ] = glCreateRenderbuffer();

					gltexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glFormat, renderTarget.width, renderTarget.height, 0, glFormat, glType, null );

					setupFrameBuffer( renderTarget.__glFramebuffer[ i ], renderTarget, _gl.TEXTURE_CUBE_MAP_POSITIVE_X + i );
					setupRenderBuffer( renderTarget.__glRenderbuffer[ i ], renderTarget );

				}

				if ( isTargetPowerOfTwo ) _gl.generateMipmap( GL_TEXTURE_CUBE_MAP );

			} else {

				renderTarget.__glFramebuffer = glCreateFramebuffer();
				renderTarget.__glRenderbuffer = glCreateRenderbuffer();

				glBindTexture( GL_TEXTURE_2D, renderTarget.__glTexture );
				setTextureParameters( GL_TEXTURE_2D, renderTarget, isTargetPowerOfTwo );

				gltexImage2D( GL_TEXTURE_2D, 0, glFormat, renderTarget.width, renderTarget.height, 0, glFormat, glType, null );

				setupFrameBuffer( renderTarget.__glFramebuffer, renderTarget, _gl.TEXTURE_2D );
				setupRenderBuffer( renderTarget.__glRenderbuffer, renderTarget );

				if ( isTargetPowerOfTwo ) _gl.generateMipmap( GL_TEXTURE_2D );

			}

			// Release everything

			if ( isCube ) {

				glBindTexture( GL_TEXTURE_CUBE_MAP, null );

			} else {

				glBindTexture( GL_TEXTURE_2D, null );

			}

			glBindRenderbuffer( GL_RENDERBUFFER, null );
			glBindFramebuffer( GL_FRAMEBUFFER, null);

		}

		auto framebuffer, width, height, vx, vy;

		if ( renderTarget ) {

			if ( isCube ) {

				framebuffer = renderTarget.__glFramebuffer[ renderTarget.activeCubeFace ];

			} else {

				framebuffer = renderTarget.__glFramebuffer;

			}

			width = renderTarget.width;
			height = renderTarget.height;

			vx = 0;
			vy = 0;

		} else {

			framebuffer = null;

			width = _viewportWidth;
			height = _viewportHeight;

			vx = _viewportX;
			vy = _viewportY;

		}

		if ( framebuffer !== _currentFramebuffer ) {

			glBindFramebuffer( GL_FRAMEBUFFER, framebuffer );
			glViewport( vx, vy, width, height );

			_currentFramebuffer = framebuffer;

		}

		_currentWidth = width;
		_currentHeight = height;

	};

	function updateRenderTargetMipmap ( renderTarget ) {

		if ( renderTarget instanceof THREE::WebGLRenderTargetCube ) {

			glBindTexture( GL_TEXTURE_CUBE_MAP, renderTarget.__glTexture );
			glgenerateMipmap( GL_TEXTURE_CUBE_MAP );
			glBindTexture( GL_TEXTURE_CUBE_MAP, null );

		} else {

			glBindTexture( GL_TEXTURE_2D, renderTarget.__glTexture );
			glgenerateMipmap( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, null );

		}

	};

	// Fallback filters for non-power-of-2 textures

	int filterFallback ( int f ) {

		if ( f == THREE::NearestFilter || f == THREE::NearestMipMapNearestFilter || f == THREE::NearestMipMapLinearFilter ) {

			return _gl.NEAREST;

		}

		return _gl.LINEAR;

	};

	// Map THREE::cpp constants to WebGL constants

	int paramThreeToGL ( int p ) {

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

		};
	};

	// Allocations

	int allocateBones ( const Object3D::Ptr& object ) {

		if ( _supportsBoneTextures && object && object.useVertexTexture ) {

			return 1024;

		} else {

			// default for when object is not specified
			// ( for example when prebuilding shader
			//   to be used with multiple objects )
			//
			// 	- leave some extra space for other uniforms
			//  - limit here is ANGLE's 254 max uniform vectors
			//    (up to 54 should be safe)

			auto nVertexUniforms = glGetParameter( GL_MAX_VERTEX_UNIFORM_VECTORS );
			auto nVertexMatrices = Math::floor( ( nVertexUniforms - 20 ) / 4 );

			auto maxBones = nVertexMatrices;

			if ( object && object.type() == THREE::SkinnedMesh ) {

				maxBones = Math::min( object.bones.size(), maxBones );

				if ( maxBones < object.bones.size() ) {

					console.warn( "WebGLRenderer: too many bones - " + object.bones.size() + ", this GPU supports just " + maxBones + " (try OpenGL instead of ANGLE)" );

				}

			}

			return maxBones;

		}

	};

	THREE_DECL void allocateLights ( std::vector<Light::Ptr> lights ) {

		auto l, ll, light, dirLights, pointLights, spotLights, maxDirLights, maxPointLights, maxSpotLights;

		dirLights = pointLights = spotLights = maxDirLights = maxPointLights = maxSpotLights = 0;

		for ( l = 0, ll = lights.size(); l < ll; l ++ ) {

			light = lights[ l ];

			if ( light.onlyShadow ) continue;

			if ( light instanceof THREE::DirectionalLight ) dirLights ++;
			if ( light instanceof THREE::PointLight ) pointLights ++;
			if ( light instanceof THREE::SpotLight ) spotLights ++;

		}

		if ( ( pointLights + spotLights + dirLights ) <= _maxLights ) {

			maxDirLights = dirLights;
			maxPointLights = pointLights;
			maxSpotLights = spotLights;

		} else {

			maxDirLights = Math::ceil( _maxLights * dirLights / ( pointLights + dirLights ) );
			maxPointLights = _maxLights - maxDirLights;
			maxSpotLights = maxPointLights; // this is not really correct

		}

		return { 'directional' : maxDirLights, 'point' : maxPointLights, 'spot': maxSpotLights };

	};

	THREE_DECL void allocateShadows ( std::vector<Light::Ptr> lights ) {

		int maxShadows = 0;

		for ( auto& light : lights ) {

			if ( ! light.castShadow ) continue;

			if ( light instanceof THREE::SpotLight ) maxShadows ++;
			if ( light instanceof THREE::DirectionalLight && ! light.shadowCascade ) maxShadows ++;

		}

		return maxShadows;

	}

};

#endif // DISABLED CODE

}; // class GLRenderer

} // namespace three

#endif // THREE_GL_RENDERER