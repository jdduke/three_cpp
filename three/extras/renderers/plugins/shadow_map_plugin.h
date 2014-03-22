#ifndef THREE_SHADOW_MAP_PLUGIN_HPP
#define THREE_SHADOW_MAP_PLUGIN_HPP

#include <three/common.h>

namespace three {

class ShadowMapPlugin : public IPlugin {

  void* _gl;
  GLRenderer* _renderer;
  Material::Ptr _depthMaterial,
           _depthMaterialMorph,
           _depthMaterialSkin,
           _depthMaterialMorphSkin;

  Frustum _frustum;
  Matrix4 _projScreenMatrix;

  Vector3 _min;
  Vector3 _max;

  Projector __projector;

  virtual void init( GLRenderer& renderer ) {

    _gl = renderer.context;
    _renderer = renderer;

    const auto& depthShader = ShaderLib::depthRGBA();
    Uniforms depthUniforms  = depthShader.uniforms;

    _depthMaterial          = ShaderMaterial::create( depthShader.vertexShader, depthShader.fragmentShader, depthUniforms );
    _depthMaterialMorph     = ShaderMaterial::create( depthShader.vertexShader, depthShader.fragmentShader, depthUniforms, Attributes(), Material::Parameters().add( "morphTargets", true ) );
    _depthMaterialSkin      = ShaderMaterial::create( depthShader.vertexShader, depthShader.fragmentShader, depthUniforms, Attributes(), Material::Parameters().add( "skinning", true ) );
    _depthMaterialMorphSkin = ShaderMaterial::create( depthShader.vertexShader, depthShader.fragmentShader, depthUniforms, Attributes(), Material::Parameters().add( "morphTargets", true ).add( "skinning", true ) );

    _depthMaterial._shadowPass          = true;
    _depthMaterialMorph._shadowPass     = true;
    _depthMaterialSkin._shadowPass      = true;
    _depthMaterialMorphSkin._shadowPass = true;

  }

  void render( Scenee& scene, Camera& camera ) {
    if ( !( _renderer->shadowMapEnabled &&
            _renderer->shadowMapAutoUpdate ) ) {
      return;
    }
    update( scene, camera );
  }

  void update( Scene scene, Camera& camera ) {

    // set GL state for depth map

    glClearColor( 1, 1, 1, 1 );
    glDisable( GL_BLEND );

    glEnable( GL_CULL_FACE );
    glFrontFace( GL_CCW );

    if ( _renderer->shadowMapCullFrontFaces ) {
      glCullFace( GL_FRONT );
    } else {
      glCullFace( GL_BACK );
    }

    _renderer->setDepthTest( true );

    // preprocess lights
    //  - skip lights that are not casting shadows
    //  - create virtual lights for cascaded shadow maps

    std::vector<Light::Ptr> lights;

    for ( i = 0, il = scene.__lights.size(); i < il; i ++ ) {

      auto& light = *scene.__lights[ i ];

      if ( ! light.castShadow ) continue;

      if ( ( light.type() == THREE.DirectionalLight ) && light.shadowCascade ) {

        auto& dlight = static_cast<DirectionalLight&>( light );

        if ( dlight.shadowCascadeCount != dlight.shadowCascadeArray.size() )
          dlight.shadowCascadeArray.resize( dlight.shadowCascadeCount );

        for ( int n = 0; n < dlight.shadowCascadeCount; n ++ ) {

          VirtualLight::Ptr virtualLight;

          if ( !dlight.shadowCascadeArray[ n ] ) {

            virtualLight = createVirtualLight( light, n );
            virtualLight->originalCamera = camera;

            auto gyro = Gyroscope::create();
            gyro->position = dlight.shadowCascadeOffset;

            gyro->add( virtualLight );
            gyro->add( virtualLight->target );

            camera.add( gyro );

            dlight.shadowCascadeArray[ n ] = virtualLight;

            console().log( "Created virtualLight", virtualLight );

          } else {
            virtualLight = dlight.shadowCascadeArray[ n ];
          }

          updateVirtualLight( light, n );

          lights.push_back( virtualLight );

        }

      } else {
        lights.push_back( light );
      }

    }

    // render depth map
    for ( size_t i = 0, il = lights.size(); i < il; i ++ ) {

      auto& light = *lights[ i ];

      if ( ! light.shadowMap ) {

      	var pars = { minFilter: THREE.LinearFilter, magFilter: THREE.LinearFilter, format: THREE.RGBAFormat };

        light.shadowMap = GLRenderTarget::create( light.shadowMapWidth, light.shadowMapHeight, pars );
        light.shadowMapSize = Vector2( light.shadowMapWidth, light.shadowMapHeight );

        light.shadowMatrix = Matrix4();

      }

      if ( ! light.shadowCamera ) {

        if ( light.type() == THREE.SpotLight ) {
          light.shadowCamera = PerspectiveCamera::create( light.shadowCameraFov, light.shadowMapWidth / light.shadowMapHeight, light.shadowCameraNear, light.shadowCameraFar );
        } else if ( light.type() == THREE.DirectionalLight ) {
          light.shadowCamera = OrthographicCamera::Create( light.shadowCameraLeft, light.shadowCameraRight, light.shadowCameraTop, light.shadowCameraBottom, light.shadowCameraNear, light.shadowCameraFar );
        } else {
          console().error( "Unsupported light type for shadow" );
          continue;
        }

        scene.add( light.shadowCamera );

        if ( _renderer->autoUpdateScene ) scene.updateMatrixWorld();

      }

      if ( light.shadowCameraVisible && ! light.cameraHelper ) {
      	// TODO
        light.cameraHelper = CameraHelper::create( light.shadowCamera );
        light.shadowCamera.add( light.cameraHelper );
      }

      if ( light.isVirtual && virtualLight.originalCamera == camera ) {
        updateShadowCamera( camera, light );
      }

      auto& shadowMap    = *light.shadowMap;
      auto& shadowMatrix = light.shadowMatrix;
      auto& shadowCamera = *light.shadowCamera;

      shadowCamera.position.copy( light.matrixWorld.getPosition() );
      shadowCamera.lookAt( light.target.matrixWorld.getPosition() );
      shadowCamera.updateMatrixWorld();

      shadowCamera.matrixWorldInverse.getInverse( shadowCamera.matrixWorld );

      if ( light.cameraHelper )        light.cameraHelper.visible = light.shadowCameraVisible;
      if ( light.shadowCameraVisible ) light.cameraHelper.update();

      // compute shadow matrix

      shadowMatrix.set( 0.5f, 0.0f, 0.0f, 0.5f,
                        0.0f, 0.5f, 0.0f, 0.5f,
                        0.0f, 0.0f, 0.5f, 0.5f,
                        0.0f, 0.0f, 0.0f, 1.0f );

      shadowMatrix.multiplySelf( shadowCamera.projectionMatrix );
      shadowMatrix.multiplySelf( shadowCamera.matrixWorldInverse );

      // update camera matrices and frustum

      if ( ! shadowCamera._viewMatrixArray ) shadowCamera._viewMatrixArray = new Float32Array( 16 );
      if ( ! shadowCamera._projectionMatrixArray ) shadowCamera._projectionMatrixArray = new Float32Array( 16 );

      shadowCamera.matrixWorldInverse.flattenToArray( shadowCamera._viewMatrixArray );
      shadowCamera.projectionMatrix.flattenToArray( shadowCamera._projectionMatrixArray );

      _projScreenMatrix.multiply( shadowCamera.projectionMatrix, shadowCamera.matrixWorldInverse );
      _frustum.setFromMatrix( _projScreenMatrix );

      // render shadow map

      _renderer->setRenderTarget( shadowMap );
      _renderer->clear();

      // set object matrices & frustum culling

      renderList = scene.__glObjects;

      for ( j = 0, jl = renderList.length; j < jl; j ++ ) {

        glObject = renderList[ j ];
        object = glObject.object;

        glObject.render = false;

        if ( object.visible && object.castShadow ) {

          if ( !( object instanceof THREE.Mesh ) || !( object.frustumCulled ) || _frustum.contains( object ) ) {
            object._modelViewMatrix.multiply( shadowCamera.matrixWorldInverse, object.matrixWorld );
            glObject.render = true;
          }

        }

      }

      // render regular objects


      IFog::Ptr fog;

      for ( size_t j = 0, jl = renderList.size(); j < jl; j ++ ) {

        auto& glObject = *renderList[ j ];

        if ( glObject.render ) {

          auto& object = *glObject.object;
          auto& buffer = *glObject.buffer;

          // culling is overriden globally for all objects
          // while rendering depth map

          Material::Ptr material;

          if ( object.customDepthMaterial ) {
            material = object.customDepthMaterial;
          } else if ( object.type() == THREE.SkinnedMesh ) {
            material = object.geometry->morphTargets.length ? _depthMaterialMorphSkin : _depthMaterialSkin;
          } else if ( object.geometry->morphTargets.length ) {
            material = _depthMaterialMorph;
          } else {
            material = _depthMaterial;
          }

          if ( buffer.type() == THREE.BufferGeometry ) {
            _renderer->renderBufferDirect( shadowCamera, scene.__lights, fog, material, buffer, object );
          } else {
            _renderer->renderBuffer( shadowCamera, scene.__lights, fog, material, buffer, object );
          }

        }

      }

      // set matrices and render immediate objects

      auto& renderList = scene.__glObjectsImmediate;

      for ( size_t j = 0, jl = renderList.size(); j < jl; j ++ ) {

        auto& glObject = renderList[ j ];
        auto& object = *glObject.object;

        if ( object.visible && object.castShadow ) {
          object._modelViewMatrix.multiply( shadowCamera.matrixWorldInverse, object.matrixWorld );
          _renderer->renderImmediateObject( shadowCamera, scene.__lights, fog, _depthMaterial, object );
        }

      }

    }

    // restore GL state

    const auto clearColor = _renderer->getClearColor();
    const auto clearAlpha = _renderer->getClearAlpha();

    glClearColor( clearColor.r, clearColor.g, clearColor.b, clearAlpha );
    glEnable( GL_BLEND );

    if ( _renderer->shadowMapCullFrontFaces ) {
      glCullFace( GL_BACK );
    }

  }

  Light::Ptr createVirtualLight( DirectionalLight& light, int cascade ) {

    auto virtualLight = DirectionalLight::create();

    virtualLight->isVirtual = true;

    virtualLight->onlyShadow = true;
    virtualLight->castShadow = true;

		virtualLight->shadowCameraNear = light.shadowCameraNear;
		virtualLight->shadowCameraFar  = light.shadowCameraFar;

		virtualLight->shadowCameraLeft   = light.shadowCameraLeft;
		virtualLight->shadowCameraRight  = light.shadowCameraRight;
		virtualLight->shadowCameraBottom = light.shadowCameraBottom;
		virtualLight->shadowCameraTop    = light.shadowCameraTop;

    virtualLight->shadowCameraVisible = light.shadowCameraVisible;

    virtualLight->shadowDarkness = light.shadowDarkness;

		virtualLight->shadowBias      = light.shadowCascadeBias[ cascade ];
		virtualLight->shadowMapWidth  = light.shadowCascadeWidth[ cascade ];
		virtualLight->shadowMapHeight = light.shadowCascadeHeight[ cascade ];

    virtualLight->pointsWorld.resize( 8 );
    virtualLight->pointsFrustum.resize( 8 );

		auto& pointsWorld   = virtualLight->pointsWorld;
		auto& pointsFrustum = virtualLight->pointsFrustum;

    for ( int i = 0; i < 8; i ++ ) {
			pointsWorld[ i ]   = new THREE.Vector3();
			pointsFrustum[ i ] = new THREE.Vector3();
    }

    const auto nearZ = light.shadowCascadeNearZ[ cascade ];
    const auto farZ = light.shadowCascadeFarZ[ cascade ];

    pointsFrustum[ 0 ].set( -1, -1, nearZ );
    pointsFrustum[ 1 ].set( 1, -1, nearZ );
    pointsFrustum[ 2 ].set( -1,  1, nearZ );
    pointsFrustum[ 3 ].set( 1,  1, nearZ );

    pointsFrustum[ 4 ].set( -1, -1, farZ );
    pointsFrustum[ 5 ].set( 1, -1, farZ );
    pointsFrustum[ 6 ].set( -1,  1, farZ );
    pointsFrustum[ 7 ].set( 1,  1, farZ );

    return virtualLight;

  }

  // Synchronize virtual light with the original light

  void updateVirtualLight( DirectionalLight& light, int cascade ) {

    auto& virtualLight = *light.shadowCascadeArray[ cascade ];

    virtualLight.position.copy( light.position );
    virtualLight.target.position.copy( light.target.position );
    virtualLight.lookAt( virtualLight.target );

    virtualLight.shadowCameraVisible = light.shadowCameraVisible;
    virtualLight.shadowDarkness = light.shadowDarkness;

    virtualLight.shadowBias = light.shadowCascadeBias[ cascade ];

    var nearZ = light.shadowCascadeNearZ[ cascade ];
    var farZ = light.shadowCascadeFarZ[ cascade ];

    auto& pointsFrustum = virtualLight.pointsFrustum;

    pointsFrustum[ 0 ].z = nearZ;
    pointsFrustum[ 1 ].z = nearZ;
    pointsFrustum[ 2 ].z = nearZ;
    pointsFrustum[ 3 ].z = nearZ;

    pointsFrustum[ 4 ].z = farZ;
    pointsFrustum[ 5 ].z = farZ;
    pointsFrustum[ 6 ].z = farZ;
    pointsFrustum[ 7 ].z = farZ;

  }

  // Fit shadow camera's ortho frustum to camera frustum

  void updateShadowCamera( Camera& camera, DirectionalLight& light ) {

    auto& shadowCamera  = *light.shadowCamera;
    auto& pointsFrustum = light.pointsFrustum;
    auto& pointsWorld   = light.pointsWorld;

    _min.set( Math::INF(), Math::INF(), Math::INF() );
    _max.set( -Math::INF(), -Math::INF(), -Math::INF() );

    for ( int i = 0; i < 8; i ++ ) {

      auto& p = pointsWorld[ i ];

      p.copy( pointsFrustum[ i ] );
      __projector.unprojectVector( p, camera );

      shadowCamera.matrixWorldInverse.multiplyVector3( p );

      if ( p.x < _min.x ) _min.x = p.x;
      if ( p.x > _max.x ) _max.x = p.x;

      if ( p.y < _min.y ) _min.y = p.y;
      if ( p.y > _max.y ) _max.y = p.y;

      if ( p.z < _min.z ) _min.z = p.z;
      if ( p.z > _max.z ) _max.z = p.z;

    }

		shadowCamera.left   = _min.x;
		shadowCamera.right  = _max.x;
		shadowCamera.top    = _max.y;
		shadowCamera.bottom = _min.y;

    // can't really fit near/far
    //shadowCamera.near = _min.z;
    //shadowCamera.far = _max.z;

    shadowCamera.updateProjectionMatrix();

  }

};

} // namespace three

#endif // THREE_SHADOW_MAP_PLUGIN_HPP