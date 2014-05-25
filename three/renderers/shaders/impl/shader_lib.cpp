#include <three/renderers/shaders/shader_lib.h>

#include <three/renderers/shaders/uniforms_lib.h>
#include <three/renderers/shaders/uniforms_utils.h>
#include <three/renderers/shaders/shader_chunk.h>

#include <three/config.h>
#include <three/constants.h>

#include <three/materials/uniform.h>

#include <three/renderers/gl/gl_shader.h>

#include <three/math/math.h>
#include <three/math/color.h>
#include <three/math/vector3.h>
#include <three/math/vector4.h>

#include <array>
#include <sstream>


namespace three {

  namespace detail {

    static Shader depthCreate() {

      Uniforms uniforms;

      uniforms.add( "mNear",   Uniform( enums::f, 1.0f ) )
      .add( "mFar",    Uniform( enums::f, 2000.0f ) )
      .add( "opacity", Uniform( enums::f, 1.0f ) );

      const char* vertexShader =
        "void main() {\n"
          "gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );\n"
        "}\n";

      const char* fragmentShader =
        "uniform float mNear;\n"
        "uniform float mFar;\n"
        "uniform float opacity;\n"

        "void main() {\n"
          "float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
          "float color = 1.0 - smoothstep( mNear, mFar, depth );\n"
          "gl_FragColor = vec4( vec3( color ), opacity );\n"
        "}\n";

      return Shader( std::move( uniforms ), vertexShader, fragmentShader );
    }

    static Shader normalCreate() {

      Uniforms uniforms;
      uniforms.add( "opacity", Uniform( enums::f, 1.0f ) );

      std::stringstream vss;
      vss <<
        "varying vec3 vNormal;" << std::endl <<
        ShaderChunk::morphtarget_pars_vertex() << std::endl <<
        "void main() {" << std::endl <<
          "vNormal = normalize( normalMatrix * normal );" << std::endl <<
          ShaderChunk::morphtarget_vertex() << std::endl <<
          ShaderChunk::default_vertex() << std::endl <<
        "}" << std::endl;

      const char* fragmentShader =
        "uniform float opacity;\n"
        "varying vec3 vNormal;\n"
        "void main() {\n"
          "gl_FragColor = vec4( 0.5 * normalize( vNormal ) + 0.5, opacity );\n"
        "}\n";

      return Shader( std::move( uniforms ), vss.str(), fragmentShader );
    }

    static Shader basicCreate() {

      std::array<Uniforms, 3> sourceUniforms = {
        UniformsLib::common(),
        UniformsLib::fog(),
        UniformsLib::shadowmap()
      };

      auto uniforms = UniformsUtils::merge( sourceUniforms );

      std::stringstream vss;
      vss <<
      ShaderChunk::map_pars_vertex() << std::endl <<
      ShaderChunk::lightmap_pars_vertex() << std::endl <<
      ShaderChunk::envmap_pars_vertex() << std::endl <<
      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

        ShaderChunk::map_vertex() << std::endl <<
        ShaderChunk::lightmap_vertex() << std::endl <<
        ShaderChunk::color_vertex() << std::endl <<
        ShaderChunk::skinbase_vertex() << std::endl <<

        "#ifdef USE_ENVMAP" << std::endl <<

        ShaderChunk::morphnormal_vertex() << std::endl <<
        ShaderChunk::skinnormal_vertex() << std::endl <<
        ShaderChunk::defaultnormal_vertex() << std::endl <<

        "#endif" << std::endl <<

        ShaderChunk::morphtarget_vertex() << std::endl <<
        ShaderChunk::skinning_vertex() << std::endl <<
        ShaderChunk::default_vertex() << std::endl <<

        ShaderChunk::worldpos_vertex() << std::endl <<
        ShaderChunk::envmap_vertex() << std::endl <<
        ShaderChunk::shadowmap_vertex() << std::endl <<

      "}" << std::endl;

      std::stringstream fss;

      fss <<
      "uniform vec3 diffuse;" << std::endl <<
      "uniform float opacity;" << std::endl <<

      ShaderChunk::color_pars_fragment() << std::endl <<
      ShaderChunk::map_pars_fragment() << std::endl <<
      ShaderChunk::lightmap_pars_fragment() << std::endl <<
      ShaderChunk::envmap_pars_fragment() << std::endl <<
      ShaderChunk::fog_pars_fragment() << std::endl <<
      ShaderChunk::shadowmap_pars_fragment() << std::endl <<
      ShaderChunk::specularmap_pars_fragment() << std::endl <<

      "void main() {"  << std::endl <<

      "gl_FragColor = vec4( diffuse, opacity );" << std::endl <<

      ShaderChunk::map_fragment() << std::endl <<
      ShaderChunk::alphatest_fragment() << std::endl <<
      ShaderChunk::specularmap_fragment() << std::endl <<
      ShaderChunk::lightmap_fragment() << std::endl <<
      ShaderChunk::color_fragment() << std::endl <<
      ShaderChunk::envmap_fragment() << std::endl <<
      ShaderChunk::shadowmap_fragment() << std::endl <<

      ShaderChunk::linear_to_gamma_fragment() << std::endl <<

      ShaderChunk::fog_fragment() << std::endl <<

      "}" << std::endl;

      return Shader( std::move( uniforms ), vss.str(), fss.str() );

    }

    static Shader dashedCreate() {

      std::array<Uniforms, 2> sourceUniforms = {
        UniformsLib::common(),
        UniformsLib::fog()
      };

      auto uniforms = UniformsUtils::merge( sourceUniforms );

      uniforms.add( "scale",   Uniform( enums::f, 1.0f ) )
              .add( "dashSize",    Uniform( enums::f, 1.0f ) )
              .add( "totalSize", Uniform( enums::f, 2.0f ) );

       std::stringstream vss;
        vss <<
        "uniform float scale;" << std::endl <<
        "attribute float lineDistance;" << std::endl <<

        "varying float vLineDistance;" << std::endl <<

        ShaderChunk::color_pars_vertex() << std::endl <<

        "void main() {" << std::endl <<

          ShaderChunk::color_vertex() << std::endl <<

          "vLineDistance = scale * lineDistance;" << std::endl <<

          "vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );" << std::endl <<
          "gl_Position = projectionMatrix * mvPosition;" << std::endl <<

        "}" << std::endl;

       std::stringstream fss;
        fss <<
        "uniform vec3 diffuse;" << std::endl <<
        "uniform float opacity;" << std::endl <<

        "uniform float dashSize;" << std::endl <<
        "uniform float totalSize;" << std::endl <<

        "varying float vLineDistance;" << std::endl <<

        ShaderChunk::color_pars_fragment() << std::endl <<
        ShaderChunk::fog_pars_fragment() << std::endl <<

        "void main() {" << std::endl <<

          "if ( mod( vLineDistance, totalSize ) > dashSize ) {" << std::endl <<

            "discard;" << std::endl <<

          "}" << std::endl <<

          "gl_FragColor = vec4( diffuse, opacity );" << std::endl <<

          ShaderChunk::color_fragment() << std::endl <<
          ShaderChunk::fog_fragment() << std::endl <<

        "}" << std::endl;

      return Shader( std::move( uniforms ), "", "" );
    }

    static Shader lambertCreate() {

      std::array<Uniforms, 4> sourceUniforms = {
        UniformsLib::common(),
        UniformsLib::fog(),
        UniformsLib::lights(),
        UniformsLib::shadowmap()
      };

      auto uniforms = UniformsUtils::merge( sourceUniforms );
      uniforms.add( "ambient",  Uniform( enums::c, Color( 0xffffff ) ) )
      .add( "emissive", Uniform( enums::c, Color( 0x000000 ) ) )
      .add( "wrapRGB",  Uniform( enums::v3, Vector3( 1, 1, 1 ) ) );

      std::stringstream vss;
      vss <<
      "#define LAMBERT" << std::endl <<
      "varying vec3 vLightFront;" << std::endl <<
      "#ifdef DOUBLE_SIDED" << std::endl <<
        "varying vec3 vLightBack;" << std::endl <<
      "#endif" << std::endl <<

      ShaderChunk::map_pars_vertex() << std::endl <<
      ShaderChunk::lightmap_pars_vertex() << std::endl <<
      ShaderChunk::envmap_pars_vertex() << std::endl <<
      ShaderChunk::lights_lambert_pars_vertex() << std::endl <<
      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {"  << std::endl <<

      ShaderChunk::map_vertex() << std::endl <<
      ShaderChunk::lightmap_vertex() << std::endl <<
      ShaderChunk::color_vertex() << std::endl <<

      ShaderChunk::morphnormal_vertex() << std::endl <<
      ShaderChunk::skinbase_vertex() << std::endl <<
      ShaderChunk::skinnormal_vertex() << std::endl <<
      ShaderChunk::defaultnormal_vertex() << std::endl <<

      ShaderChunk::morphtarget_vertex() << std::endl <<
      ShaderChunk::skinning_vertex() << std::endl <<
      ShaderChunk::default_vertex() << std::endl <<

      ShaderChunk::worldpos_vertex() << std::endl <<
      ShaderChunk::envmap_vertex() << std::endl <<
      ShaderChunk::lights_lambert_vertex() << std::endl <<
      ShaderChunk::shadowmap_vertex() << std::endl <<

      "}";

      std::stringstream fss;
      fss <<

      "uniform float opacity;" << std::endl <<
      "varying vec3 vLightFront;" << std::endl <<
      "#ifdef DOUBLE_SIDED" << std::endl <<
      "varying vec3 vLightBack;" << std::endl <<
      "#endif" << std::endl <<

      ShaderChunk::color_pars_fragment() << std::endl <<
      ShaderChunk::map_pars_fragment() << std::endl <<
      ShaderChunk::lightmap_pars_fragment() << std::endl <<
      ShaderChunk::envmap_pars_fragment() << std::endl <<
      ShaderChunk::fog_pars_fragment() << std::endl <<
      ShaderChunk::shadowmap_pars_fragment() << std::endl <<
      ShaderChunk::specularmap_pars_fragment() << std::endl <<

      "void main() {" << std::endl <<

      "gl_FragColor = vec4( vec3 ( 1.0 ), opacity );" << std::endl <<

      ShaderChunk::map_fragment() << std::endl <<
      ShaderChunk::alphatest_fragment() << std::endl <<
      ShaderChunk::specularmap_fragment() << std::endl <<

      "#ifdef DOUBLE_SIDED" << std::endl <<

      //"float isFront = float( gl_FrontFacing );"
      //"gl_FragColor.xyz *= isFront * vLightFront + ( 1.0 - isFront ) * vLightBack;"

      "if ( gl_FrontFacing )" << std::endl <<
      "gl_FragColor.xyz *= vLightFront;" << std::endl <<
      "else" << std::endl <<
      "gl_FragColor.xyz *= vLightBack;" << std::endl <<
      "#else" << std::endl <<
      "gl_FragColor.xyz *= vLightFront;" << std::endl <<
      "#endif" << std::endl <<

      ShaderChunk::lightmap_fragment() << std::endl <<
      ShaderChunk::color_fragment() << std::endl <<
      ShaderChunk::envmap_fragment() << std::endl <<
      ShaderChunk::shadowmap_fragment() << std::endl <<

      ShaderChunk::linear_to_gamma_fragment() << std::endl <<

      ShaderChunk::fog_fragment() << std::endl <<

      "}" << std::endl;

      return Shader( std::move( uniforms ), vss.str(), fss.str() );
    }

    static Shader phongCreate() {

      std::array<Uniforms, 6> sourceUniforms = {
        UniformsLib::common(),
        UniformsLib::bump(),
        UniformsLib::normalmap(),
        UniformsLib::fog(),
        UniformsLib::lights(),
        UniformsLib::shadowmap()
      };

      auto uniforms = UniformsUtils::merge( sourceUniforms );
      uniforms.add( "ambient",   Uniform( enums::c, Color( 0xffffff ) ) )
      .add( "emissive",  Uniform( enums::c, Color( 0x000000 ) ) )
      .add( "specular",  Uniform( enums::c, Color( 0x111111 ) ) )
      .add( "shininess", Uniform( enums::f, 30.f ) )
      .add( "wrapRGB",   Uniform( enums::v3, Vector3( 1, 1, 1 ) ) );

      std::stringstream vss;
      vss <<
      "#define PHONG" << std::endl <<
      "varying vec3 vViewPosition;" << std::endl <<
      "varying vec3 vNormal;" << std::endl <<

      ShaderChunk::map_pars_vertex() << std::endl <<
      ShaderChunk::lightmap_pars_vertex() << std::endl <<
      ShaderChunk::envmap_pars_vertex() << std::endl <<
      ShaderChunk::lights_phong_pars_vertex() << std::endl <<
      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

      ShaderChunk::map_vertex() << std::endl <<
      ShaderChunk::lightmap_vertex() << std::endl <<
      ShaderChunk::color_vertex() << std::endl <<

      ShaderChunk::morphnormal_vertex() << std::endl <<
      ShaderChunk::skinbase_vertex() << std::endl <<
      ShaderChunk::skinnormal_vertex() << std::endl <<
      ShaderChunk::defaultnormal_vertex() << std::endl <<

      "vNormal = transformedNormal;" << std::endl <<

      ShaderChunk::morphtarget_vertex() << std::endl <<
      ShaderChunk::skinning_vertex() << std::endl <<
      ShaderChunk::default_vertex() << std::endl <<

      ShaderChunk::worldpos_vertex() << std::endl <<
      ShaderChunk::envmap_vertex() << std::endl <<
      ShaderChunk::lights_phong_vertex() << std::endl <<
      ShaderChunk::shadowmap_vertex() << std::endl <<

      "}" << std::endl;

      std::stringstream fss;
      fss <<
      "uniform vec3 diffuse;" << std::endl <<
      "uniform float opacity;" << std::endl <<

      "uniform vec3 ambient;" << std::endl <<
      "uniform vec3 emissive;" << std::endl <<
      "uniform vec3 specular;" << std::endl <<
      "uniform float shininess;" << std::endl <<

      ShaderChunk::color_pars_fragment() << std::endl <<
      ShaderChunk::map_pars_fragment() << std::endl <<
      ShaderChunk::lightmap_pars_fragment() << std::endl <<
      ShaderChunk::envmap_pars_fragment() << std::endl <<
      ShaderChunk::fog_pars_fragment() << std::endl <<
      ShaderChunk::lights_phong_pars_fragment() << std::endl <<
      ShaderChunk::shadowmap_pars_fragment() << std::endl <<
      ShaderChunk::bumpmap_pars_fragment() << std::endl <<
      ShaderChunk::normalmap_pars_fragment() << std::endl <<
      ShaderChunk::specularmap_pars_fragment() << std::endl <<

      "void main() {" << std::endl <<

      "gl_FragColor = vec4( vec3 ( 1.0 ), opacity );" << std::endl <<

      ShaderChunk::map_fragment() << std::endl <<
      ShaderChunk::alphatest_fragment() << std::endl <<
      ShaderChunk::specularmap_fragment() << std::endl <<

      ShaderChunk::lights_phong_fragment() << std::endl <<

      ShaderChunk::lightmap_fragment() << std::endl <<
      ShaderChunk::color_fragment() << std::endl <<
      ShaderChunk::envmap_fragment() << std::endl <<
      ShaderChunk::shadowmap_fragment() << std::endl <<

      ShaderChunk::linear_to_gamma_fragment() << std::endl <<

      ShaderChunk::fog_fragment() << std::endl <<

      "}" << std::endl;

      return Shader( std::move( uniforms ), vss.str(), fss.str() );
    }

    static Shader particleBasicCreate() {

      std::array<Uniforms, 2> sourceUniforms = {
        UniformsLib::particle(),
        UniformsLib::shadowmap()
      };

      auto uniforms = UniformsUtils::merge( sourceUniforms );

      std::stringstream vss;
      vss <<
      "uniform float size;" << std::endl <<
      "uniform float scale;" << std::endl <<

      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

      ShaderChunk::color_vertex() << std::endl <<

      "vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );" << std::endl <<

      "#ifdef USE_SIZEATTENUATION" << std::endl <<
      "gl_PointSize = size * ( scale / length( mvPosition.xyz ) );" << std::endl <<
      "#else" << std::endl <<
      "gl_PointSize = size;" << std::endl <<
      "#endif" << std::endl <<

      "gl_Position = projectionMatrix * mvPosition;" << std::endl <<

      ShaderChunk::worldpos_vertex() << std::endl <<
      ShaderChunk::shadowmap_vertex() << std::endl <<

      "}" << std::endl;

      std::stringstream fss;
      fss <<
      "uniform vec3 psColor;" << std::endl <<
      "uniform float opacity;" << std::endl <<

      ShaderChunk::color_pars_fragment() << std::endl <<
      ShaderChunk::map_particle_pars_fragment() << std::endl <<
      ShaderChunk::fog_pars_fragment() << std::endl <<
      ShaderChunk::shadowmap_pars_fragment() << std::endl <<

      "void main() {" << std::endl <<

      "gl_FragColor = vec4( psColor, opacity );" << std::endl <<

      ShaderChunk::map_particle_fragment() << std::endl <<
      ShaderChunk::alphatest_fragment() << std::endl <<
      ShaderChunk::color_fragment() << std::endl <<
      ShaderChunk::shadowmap_fragment() << std::endl <<
      ShaderChunk::fog_fragment() << std::endl <<

      "}" << std::endl;

      return Shader( std::move( uniforms ), vss.str(), fss.str() );

    }

// Depth encoding into RGBA texture
//  based on SpiderGL shadow map example
//    http://spidergl.org/example.php?id=6
//  originally from
//    http://www.gamedev.net/topic/442138-packing-a-float-into-a-a8r8g8b8-texture-shader/page__whichpage__1%25EF%25BF%25BD
//  see also here:
//    http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/

    static Shader depthRGBACreate() {

      Uniforms uniforms;

      std::stringstream vss;
      vss <<

      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

      ShaderChunk::skinbase_vertex() << std::endl <<
      ShaderChunk::morphtarget_vertex() << std::endl <<
      ShaderChunk::skinning_vertex() << std::endl <<
      ShaderChunk::default_vertex() << std::endl <<

      "}" << std::endl;

      std::stringstream fss;
      fss <<

      "vec4 pack_depth( const in float depth ) {" << std::endl <<

      "const vec4 bit_shift = vec4( 256.0 * 256.0 * 256.0, 256.0 * 256.0, 256.0, 1.0 );" << std::endl <<
      "const vec4 bit_mask  = vec4( 0.0, 1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0 );" << std::endl <<
      "vec4 res = fract( depth * bit_shift );" << std::endl <<
      "res -= res.xxyz * bit_mask;" << std::endl <<
      "return res;" << std::endl <<

      "}" << std::endl <<

      "void main() {" << std::endl <<

      "gl_FragData[ 0 } = pack_depth( gl_FragCoord.z );" << std::endl <<

      //"gl_FragData[ 0 } = pack_depth( gl_FragCoord.z / gl_FragCoord.w );" << std::endl <<
      //"float z = ( ( gl_FragCoord.z / gl_FragCoord.w ) - 3.0 ) / ( 4000.0 - 3.0 );" << std::endl <<
      //"gl_FragData[ 0 } = pack_depth( z );" << std::endl <<
      //"gl_FragData[ 0 } = vec4( z z z 1.0 );" << std::endl <<

        "}" << std::endl;

        return Shader( std::move( uniforms ), vss.str(), fss.str() );

      }

  } // namespace detail

    const Shader& ShaderLib::depth() {
        static Shader sShader = detail::depthCreate();
        return sShader;
    }

    const Shader& ShaderLib::normal() {
        static Shader sShader = detail::normalCreate();
        return sShader;
    }

    const Shader& ShaderLib::basic() {
        static Shader sShader = detail::basicCreate();
        return sShader;
    }

    const Shader& ShaderLib::dashed() {
        static Shader sShader = detail::dashedCreate();
        return sShader;
    }

    const Shader& ShaderLib::lambert() {
        static Shader sShader = detail::lambertCreate();
        return sShader;
    }

    const Shader& ShaderLib::phong() {
        static Shader sShader = detail::phongCreate();
        return sShader;
    }

    const Shader& ShaderLib::particleBasic() {
        static Shader sShader = detail::particleBasicCreate();
        return sShader;
    }

    const Shader& ShaderLib::depthRGBA() {
        static Shader sShader = detail::depthRGBACreate();
        return sShader;
    }

} // namespace three
