#ifndef THREE_LOADER_H
#define THREE_LOADER_H

#include <three/common.h>

#include <three/utils/noncopyable.h>

#include <iomanip>
#include <functional>

namespace three {

class Loader : NonCopyable {
public:

  Loader( bool showStatus = true )
    : showStatus( showStatus ) {

    if ( showStatus )
      onStatus = []( const char * status ) { console().log( status ); };

  }

protected:

  bool showStatus;

  typedef std::function<void( const char* )> StatusCallback;
  StatusCallback onStatus;

  typedef std::function<void( void )> Callback;
  Callback onLoadStart;
  Callback onLoadProgress;
  Callback onLoadComplete;

  //////////////////////////////////////////////////////////////////////////

  struct Progress {
    float total;
    float loaded;
  };

  void updateProgress( Progress progress ) {

    std::stringstream message( "Loaded " );

    if ( progress.total ) {
      message << std::setprecision( 0 ) << 100.f* progress.loaded / progress.total << "%";
    } else {
      message << std::setprecision( 2 ) << ( progress.loaded / 1000 ) << " KB";
    }

    if ( onStatus )
      onStatus( message.str().c_str() );

  }

  template < typename Scope >
  void initMaterials( Scope& scope, const std::vector<Material::Ptr>& materials, const std::string& texturePath ) {
    scope.materials.clear();
    for ( size_t i = 0; i < materials.size(); ++ i ) {
      scope.materials.push_back( createMaterial( *materials[ i ], texturePath ) );
    }
  }

  template < typename Scope >
  bool hasNormals( Scope& scope ) {
    for ( auto& m : scope.materials ) {
      if ( m->type() == enums::ShaderMaterial ) return true;
    }
    return false;
  }


  Material::Ptr createMaterial( Material& m, const std::string& texturePath ) {

    Material::Ptr material;

#ifdef TODO_CREATE_MATERIAL
    function load_image( where, url ) {

      var image = new Image();

      image.onload = function() {

        if ( !is_pow2( this.width ) || !is_pow2( this.height ) ) {

          var width = nearest_pow2( this.width );
          var height = nearest_pow2( this.height );

          where.image.width = width;
          where.image.height = height;
          where.image.getContext( '2d' ).drawImage( this, 0, 0, width, height );

        } else {

          where.image = this;

        }

        where.needsUpdate = true;

      };

      image.crossOrigin = _this.crossOrigin;
      image.src = url;

    }

    Texture::Ptr create_texture( where, name, sourceFile, repeat, offset, wrap, anisotropy ) {

      const auto isCompressed = sourceFile.toLowerCase().endsWith( ".dds" );
      const auto fullPath = texturePath + "/" + sourceFile;

      if ( isCompressed ) {
        var texture = THREE.ImageUtils.loadCompressedTexture( fullPath );
        where[ name ] = texture;
      } else {
        var texture = document.createElement( 'canvas' );
        where[ name ] = new THREE.Texture( texture );
      }

      where[ name ].sourceFile = sourceFile;

      if ( repeat ) {
        where[ name ].repeat.set( repeat[ 0 ], repeat[ 1 ] );
        if ( repeat[ 0 ] != = 1 ) where[ name ].wrapS = THREE.RepeatWrapping;
        if ( repeat[ 1 ] != = 1 ) where[ name ].wrapT = THREE.RepeatWrapping;
      }

      if ( offset ) {
        where[ name ].offset.set( offset[ 0 ], offset[ 1 ] );
      }

      if ( wrap ) {
        var wrapMap = {
"repeat": THREE.RepeatWrapping,
"mirror": THREE.MirroredRepeatWrapping
        }
        if ( wrapMap[ wrap[ 0 ] ] != = undefined ) where[ name ].wrapS = wrapMap[ wrap[ 0 ] ];
        if ( wrapMap[ wrap[ 1 ] ] != = undefined ) where[ name ].wrapT = wrapMap[ wrap[ 1 ] ];
      }

      if ( anisotropy ) {
        where[ name ].anisotropy = anisotropy;
      }

      if ( ! isCompressed ) {
        load_image( where[ name ], fullPath );
      }

    }

    inline int rgb2hex( Color rgb ) {
      return ( rgb[ 0 ] * 255 << 16 ) + ( rgb[ 1 ] * 255 << 8 ) + rgb[ 2 ] * 255;
    }

    // defaults

    const char* mtype = "MeshLambertMaterial";
    Parameters mpars = Parameters().add("color", Color(0xeeeeee))
                                   .add("opacity", 1.0)
                                   .add("wireframe", false);

    // parameters from model file
    if ( m.shading ) {
      var shading = m.shading.toLowerCase();
      if ( shading == = "phong" ) mtype = "MeshPhongMaterial";
      else if ( shading == = "basic" ) mtype = "MeshBasicMaterial";
    }

    if ( m.blending != = undefined && THREE[ m.blending ] != = undefined ) {
      mpars.blending = THREE[ m.blending ];
    }

    if ( m.transparent != = undefined || m.opacity < 1.0 ) {
      mpars.transparent = m.transparent;
    }

    if ( m.depthTest != = undefined ) {
      mpars.depthTest = m.depthTest;
    }

    if ( m.depthWrite != = undefined ) {
      mpars.depthWrite = m.depthWrite;
    }

    if ( m.visible != = undefined ) {
      mpars.visible = m.visible;
    }

    if ( m.flipSided != = undefined ) {
      mpars.side = THREE.BackSide;
    }

    if ( m.doubleSided != = undefined ) {
      mpars.side = THREE.DoubleSide;
    }

    if ( m.wireframe != = undefined ) {
      mpars.wireframe = m.wireframe;
    }

    if ( m.vertexColors != = undefined ) {
      if ( m.vertexColors == = "face" ) {
        mpars.vertexColors = THREE.FaceColors;
      } else if ( m.vertexColors ) {
        mpars.vertexColors = THREE.VertexColors;
      }
    }

    // colors

    if ( m.colorDiffuse ) {
      mpars.color = rgb2hex( m.colorDiffuse );
    } else if ( m.DbgColor ) {
      mpars.color = m.DbgColor;
    }

    if ( m.colorSpecular ) {
      mpars.specular = rgb2hex( m.colorSpecular );
    }

    if ( m.colorAmbient ) {
      mpars.ambient = rgb2hex( m.colorAmbient );
    }

    // modifiers

    if ( m.transparency ) {
      mpars.opacity = m.transparency;
    }

    if ( m.specularCoef ) {
      mpars.shininess = m.specularCoef;
    }

    // textures

    if ( m.mapDiffuse && texturePath ) {
      create_texture( mpars, "map", m.mapDiffuse, m.mapDiffuseRepeat, m.mapDiffuseOffset, m.mapDiffuseWrap, m.mapDiffuseAnisotropy );
    }

    if ( m.mapLight && texturePath ) {
      create_texture( mpars, "lightMap", m.mapLight, m.mapLightRepeat, m.mapLightOffset, m.mapLightWrap, m.mapLightAnisotropy );
    }

    if ( m.mapBump && texturePath ) {
      create_texture( mpars, "bumpMap", m.mapBump, m.mapBumpRepeat, m.mapBumpOffset, m.mapBumpWrap, m.mapBumpAnisotropy );
    }

    if ( m.mapNormal && texturePath ) {
      create_texture( mpars, "normalMap", m.mapNormal, m.mapNormalRepeat, m.mapNormalOffset, m.mapNormalWrap, m.mapNormalAnisotropy );
    }

    if ( m.mapSpecular && texturePath ) {
      create_texture( mpars, "specularMap", m.mapSpecular, m.mapSpecularRepeat, m.mapSpecularOffset, m.mapSpecularWrap, m.mapSpecularAnisotropy );
    }

    //

    if ( m.mapBumpScale ) {
      mpars.bumpScale = m.mapBumpScale;
    }

    // special case for normal mapped material

    if ( m.mapNormal ) {

      var shader = THREE.ShaderUtils.lib[ "normal" ];
      var uniforms = THREE.UniformsUtils.clone( shader.uniforms );

      uniforms[ "tNormal" ].value = mpars.normalMap;

      if ( m.mapNormalFactor ) {
        uniforms[ "uNormalScale" ].value.set( m.mapNormalFactor, m.mapNormalFactor );
      }

      if ( mpars.map ) {
        uniforms[ "tDiffuse" ].value = mpars.map;
        uniforms[ "enableDiffuse" ].value = true;
      }

      if ( mpars.specularMap ) {
        uniforms[ "tSpecular" ].value = mpars.specularMap;
        uniforms[ "enableSpecular" ].value = true;
      }

      if ( mpars.lightMap ) {
        uniforms[ "tAO" ].value = mpars.lightMap;
        uniforms[ "enableAO" ].value = true;
      }

      // for the moment don't handle displacement texture

      uniforms[ "uDiffuseColor" ].value.setHex( mpars.color );
      uniforms[ "uSpecularColor" ].value.setHex( mpars.specular );
      uniforms[ "uAmbientColor" ].value.setHex( mpars.ambient );

      uniforms[ "uShininess" ].value = mpars.shininess;

      if ( mpars.opacity != = undefined ) {

        uniforms[ "uOpacity" ].value = mpars.opacity;

      }

      var parameters = { fragmentShader: shader.fragmentShader, vertexShader: shader.vertexShader, uniforms: uniforms, lights: true, fog: true };

      var material = new THREE.ShaderMaterial( parameters );

    } else {

      var material = new THREE[ mtype ]( mpars );

    }

    if ( m.DbgName != = undefined ) material.name = m.DbgName;

#endif

    return material;

  }

};

} // namespace three

#endif // THREE_LOADER_H
