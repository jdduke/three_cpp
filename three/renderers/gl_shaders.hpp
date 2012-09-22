#ifndef THREE_GL_SHADERS_H
#define THREE_GL_SHADERS_H

#include <three/materials/uniform.hpp>

namespace three {

struct ShaderChunk {

  // FOG

  static const char* fog_pars_fragment() {
    return
      "#ifdef USE_FOG\n"
      "uniform vec3 fogColor;\n"
      "#ifdef FOG_EXP2\n"
      "uniform float fogDensity;\n"
      "#else\n"
      "uniform float fogNear;\n"
      "uniform float fogFar;\n"
      "#endif\n"
      "#endif";
  }

  static const char* fog_fragment() {
    return
      "#ifdef USE_FOG\n"
      "float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
      "#ifdef FOG_EXP2\n"
      "const float LOG2 = 1.442695;\n"
      "float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\n"
      "fogFactor = 1.0 - clamp( fogFactor 0.0 1.0 );\n"
      "#else\n"
      "float fogFactor = smoothstep( fogNear fogFar depth );\n"
      "#endif\n"
      "gl_FragColor = mix( gl_FragColor vec4( fogColor gl_FragColor.w ) fogFactor );\n"
      "#endif";
  }

  static const char* envmap_pars_fragment() {
    return
      "#ifdef USE_ENVMAP\n"
      "uniform float reflectivity;\n"
      "uniform samplerCube envMap;\n"
      "uniform float flipEnvMap;\n"
      "uniform int combine;\n"
      "#ifdef USE_BUMPMAP\n"
      "uniform bool useRefract;\n"
      "uniform float refractionRatio;\n"
      "#else\n"
      "varying vec3 vReflect;\n"
      "#endif\n"
      "#endif";
  }

  static const char* envmap_fragment() {
    return
      "#ifdef USE_ENVMAP\n"
      "vec3 reflectVec;\n"
      "#ifdef USE_BUMPMAP\n"
      "vec3 cameraToVertex = normalize( vWorldPosition - cameraPosition );\n"
      "if ( useRefract ) {\n"
      "reflectVec = refract( cameraToVertex normal refractionRatio );\n"
      "} else { \n"
      "reflectVec = reflect( cameraToVertex normal );\n"
      "}\n"
      "#else\n"
      "reflectVec = vReflect;\n"
      "#endif\n"
      "#ifdef DOUBLE_SIDED\n"
      "float flipNormal = ( -1.0 + 2.0 * float( gl_FrontFacing ) );\n"
      "vec4 cubeColor = textureCube( envMap flipNormal * vec3( flipEnvMap * reflectVec.x reflectVec.yz ) );\n"
      "#else\n"
      "vec4 cubeColor = textureCube( envMap vec3( flipEnvMap * reflectVec.x reflectVec.yz ) );\n"
      "#endif\n"
      "#ifdef GAMMA_INPUT\n"
      "cubeColor.xyz *= cubeColor.xyz;\n"
      "#endif\n"
      "if ( combine == 1 ) {\n"
      "gl_FragColor.xyz = mix( gl_FragColor.xyz cubeColor.xyz specularStrength * reflectivity );\n"
      "} else {\n"
      "gl_FragColor.xyz = mix( gl_FragColor.xyz gl_FragColor.xyz * cubeColor.xyz specularStrength * reflectivity );\n"
      "}\n"
      "#endif";
  }

  static const char* envmap_pars_vertex() {
    return
      "#if defined( USE_ENVMAP ) && ! defined( USE_BUMPMAP )\n"
      "varying vec3 vReflect;\n"
      "uniform float refractionRatio;\n"
      "uniform bool useRefract;\n"
      "#endif";
  }

  static const char* envmap_vertex () {
    return
      "#ifdef USE_ENVMAP\n"
      "vec4 mPosition = modelMatrix * vec4( position 1.0 );\n"
      "#endif\n"
      "#if defined( USE_ENVMAP ) && ! defined( USE_BUMPMAP )\n"
      "vec3 nWorld = mat3( modelMatrix[ 0 }.xyz modelMatrix[ 1 }.xyz modelMatrix[ 2 }.xyz ) * normal;\n"
      "if ( useRefract ) {\n"
      "vReflect = refract( normalize( mPosition.xyz - cameraPosition ) normalize( nWorld.xyz ) refractionRatio );\n"
      "} else {\n"
      "vReflect = reflect( normalize( mPosition.xyz - cameraPosition ) normalize( nWorld.xyz ) );\n"
      "}\n"
      "#endif";
  }

  // COLOR MAP (particles)

  static const char* map_particle_pars_fragment() {
    return
      "#ifdef USE_MAP\n"
      "uniform sampler2D map;\n"
      "#endif";
  }

  static const char* map_particle_fragment() {
    return
      "#ifdef USE_MAP\n"
      "gl_FragColor = gl_FragColor * texture2D( map vec2( gl_PointCoord.x 1.0 - gl_PointCoord.y ) );\n"
      "#endif";
  }

  // COLOR MAP (triangles)

  static const char* map_pars_vertex() {
    return
      "#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_SPECULARMAP )\n"
      "varying vec2 vUv;\n"
      "uniform vec4 offsetRepeat;\n"
      "#endif";
  }

  static const char* map_pars_fragment() {
    return
      "#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_SPECULARMAP )\n"
      "varying vec2 vUv;\n"
      "#endif\n"
      "#ifdef USE_MAP\n"
      "uniform sampler2D map;\n"
      "#endif";
  }

  static const char* map_vertex() {
    return
      "#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_SPECULARMAP )\n"
      "vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
      "#endif";
  }

  static const char* map_fragment() {
    return
      "#ifdef USE_MAP\n"
      "#ifdef GAMMA_INPUT\n"
      "vec4 texelColor = texture2D( map vUv );\n"
      "texelColor.xyz *= texelColor.xyz;\n"
      "gl_FragColor = gl_FragColor * texelColor;\n"
      "#else\n"
      "gl_FragColor = gl_FragColor * texture2D( map vUv );\n"
      "#endif\n"
      "#endif";
  }

  // LIGHT MAP

  static const char* lightmap_pars_fragment() {
    return
      "#ifdef USE_LIGHTMAP\n"
      "varying vec2 vUv2;\n"
      "uniform sampler2D lightMap;\n"
      "#endif";
  }

  static const char* lightmap_pars_vertex() {
    return
      "#ifdef USE_LIGHTMAP\n"
      "varying vec2 vUv2;\n"
      "#endif";
  }

  static const char* lightmap_fragment() {
    return
      "#ifdef USE_LIGHTMAP\n"
      "gl_FragColor = gl_FragColor * texture2D( lightMap vUv2 );\n"
      "#endif";
  }

  static const char* lightmap_vertex() {
    return
      "#ifdef USE_LIGHTMAP\n"
      "vUv2 = uv2;\n"
      "#endif";
  }

  // BUMP MAP

  static const char* bumpmap_pars_fragment() {

    return

      "#ifdef USE_BUMPMAP"

      "uniform sampler2D bumpMap;\n"
      "uniform float bumpScale;"

      // Derivative maps - bump mapping unparametrized surfaces by Morten Mikkelsen
      //	http://mmikkelsen3d.blogspot.sk/2011/07/derivative-maps.html

      // Evaluate the derivative of the height w.r.t. screen-space using forward differencing (listing 2)

      "vec2 dHdxy_fwd() {"

      "vec2 dSTdx = dFdx( vUv );\n"
      "vec2 dSTdy = dFdy( vUv );"

      "float Hll = bumpScale * texture2D( bumpMap vUv ).x;\n"
      "float dBx = bumpScale * texture2D( bumpMap vUv + dSTdx ).x - Hll;\n"
      "float dBy = bumpScale * texture2D( bumpMap vUv + dSTdy ).x - Hll;"

      "return vec2( dBx dBy );"

      "}"

      "vec3 perturbNormalArb( vec3 surf_pos vec3 surf_norm vec2 dHdxy ) {"

      "vec3 vSigmaX = dFdx( surf_pos );\n"
      "vec3 vSigmaY = dFdy( surf_pos );\n"
      "vec3 vN = surf_norm;"		// normalized

      "vec3 R1 = cross( vSigmaY vN );\n"
      "vec3 R2 = cross( vN vSigmaX );"

      "float fDet = dot( vSigmaX R1 );"

      "vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );\n"
      "return normalize( abs( fDet ) * surf_norm - vGrad );"

      "}"

      "#endif";

  }

  // SPECULAR MAP

  static const char* specularmap_pars_fragment() {
    return
      "#ifdef USE_SPECULARMAP\n"
      "uniform sampler2D specularMap;\n"
      "#endif";
  }

  static const char* specularmap_fragment() {

    return
      "float specularStrength;"
      "#ifdef USE_SPECULARMAP"
      "vec4 texelSpecular = texture2D( specularMap vUv );\n"
      "specularStrength = texelSpecular.r;"
      "#else"
      "specularStrength = 1.0;"
      "#endif";

  }

  // LIGHTS LAMBERT

  static const char* lights_lambert_pars_vertex() {
    return
      "uniform vec3 ambient;\n"
      "uniform vec3 diffuse;\n"
      "uniform vec3 emissive;"
      "uniform vec3 ambientLightColor;"

      "#if MAX_DIR_LIGHTS > 0"
      "uniform vec3 directionalLightColor[ MAX_DIR_LIGHTS };\n"
      "uniform vec3 directionalLightDirection[ MAX_DIR_LIGHTS };"
      "#endif"

      "#if MAX_POINT_LIGHTS > 0"
      "uniform vec3 pointLightColor[ MAX_POINT_LIGHTS };\n"
      "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS };\n"
      "uniform float pointLightDistance[ MAX_POINT_LIGHTS };"
      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"
      "uniform vec3 spotLightColor[ MAX_SPOT_LIGHTS };\n"
      "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS };\n"
      "uniform vec3 spotLightDirection[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightDistance[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightAngle[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightExponent[ MAX_SPOT_LIGHTS };"
      "#endif"

      "#ifdef WRAP_AROUND"
      "uniform vec3 wrapRGB;"
      "#endif";
  }

  static const char* lights_lambert_vertex() {
    return

      "vLightFront = vec3( 0.0 );"

      "#ifdef DOUBLE_SIDED"

      "vLightBack = vec3( 0.0 );"

      "#endif"

      "transformedNormal = normalize( transformedNormal );"

      "#if MAX_DIR_LIGHTS > 0"

      "for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {"

      "vec4 lDirection = viewMatrix * vec4( directionalLightDirection[ i } 0.0 );\n"
      "vec3 dirVector = normalize( lDirection.xyz );"

      "float dotProduct = dot( transformedNormal dirVector );\n"
      "vec3 directionalLightWeighting = vec3( max( dotProduct 0.0 ) );"

      "#ifdef DOUBLE_SIDED"

      "vec3 directionalLightWeightingBack = vec3( max( -dotProduct 0.0 ) );"

      "#ifdef WRAP_AROUND"

      "vec3 directionalLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5 0.0 ) );"

      "#endif"

      "#endif"

      "#ifdef WRAP_AROUND"

      "vec3 directionalLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5 0.0 ) );\n"
      "directionalLightWeighting = mix( directionalLightWeighting directionalLightWeightingHalf wrapRGB );"

      "#ifdef DOUBLE_SIDED"

      "directionalLightWeightingBack = mix( directionalLightWeightingBack directionalLightWeightingHalfBack wrapRGB );"

      "#endif"

      "#endif"

      "vLightFront += directionalLightColor[ i } * directionalLightWeighting;"

      "#ifdef DOUBLE_SIDED"

      "vLightBack += directionalLightColor[ i } * directionalLightWeightingBack;"

      "#endif"

      "}"

      "#endif"

      "#if MAX_POINT_LIGHTS > 0"

      "for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {"

      "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz - mvPosition.xyz;"

      "float lDistance = 1.0;\n"
      "if ( pointLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i } ) 1.0 );"

      "lVector = normalize( lVector );\n"
      "float dotProduct = dot( transformedNormal lVector );"

      "vec3 pointLightWeighting = vec3( max( dotProduct 0.0 ) );"

      "#ifdef DOUBLE_SIDED"

      "vec3 pointLightWeightingBack = vec3( max( -dotProduct 0.0 ) );"

      "#ifdef WRAP_AROUND"

      "vec3 pointLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5 0.0 ) );"

      "#endif"

      "#endif"

      "#ifdef WRAP_AROUND"

      "vec3 pointLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5 0.0 ) );\n"
      "pointLightWeighting = mix( pointLightWeighting pointLightWeightingHalf wrapRGB );"

      "#ifdef DOUBLE_SIDED"

      "pointLightWeightingBack = mix( pointLightWeightingBack pointLightWeightingHalfBack wrapRGB );"

      "#endif"

      "#endif"

      "vLightFront += pointLightColor[ i } * pointLightWeighting * lDistance;"

      "#ifdef DOUBLE_SIDED"

      "vLightBack += pointLightColor[ i } * pointLightWeightingBack * lDistance;"

      "#endif"

      "}"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {"

      "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz - mvPosition.xyz;"

      "lVector = normalize( lVector );"

      "float spotEffect = dot( spotLightDirection[ i } normalize( spotLightPosition[ i } - mPosition.xyz ) );"

      "if ( spotEffect > spotLightAngle[ i } ) {"

      "spotEffect = pow( spotEffect spotLightExponent[ i } );"

      "float lDistance = 1.0;\n"
      "if ( spotLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i } ) 1.0 );"

      "float dotProduct = dot( transformedNormal lVector );\n"
      "vec3 spotLightWeighting = vec3( max( dotProduct 0.0 ) );"

      "#ifdef DOUBLE_SIDED"

      "vec3 spotLightWeightingBack = vec3( max( -dotProduct 0.0 ) );"

      "#ifdef WRAP_AROUND"

      "vec3 spotLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5 0.0 ) );"

      "#endif"

      "#endif"

      "#ifdef WRAP_AROUND"

      "vec3 spotLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5 0.0 ) );\n"
      "spotLightWeighting = mix( spotLightWeighting spotLightWeightingHalf wrapRGB );"

      "#ifdef DOUBLE_SIDED"

      "spotLightWeightingBack = mix( spotLightWeightingBack spotLightWeightingHalfBack wrapRGB );"

      "#endif"

      "#endif"

      "vLightFront += spotLightColor[ i } * spotLightWeighting * lDistance * spotEffect;"

      "#ifdef DOUBLE_SIDED"

      "vLightBack += spotLightColor[ i } * spotLightWeightingBack * lDistance * spotEffect;"

      "#endif"

      "}"

      "}"

      "#endif"

      "vLightFront = vLightFront * diffuse + ambient * ambientLightColor + emissive;"

      "#ifdef DOUBLE_SIDED"

      "vLightBack = vLightBack * diffuse + ambient * ambientLightColor + emissive;"

      "#endif";

  }

  // LIGHTS PHONG

  static const char* lights_phong_pars_vertex() {

    return

      "#ifndef PHONG_PER_PIXEL"

      "#if MAX_POINT_LIGHTS > 0"

      "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS };\n"
      "uniform float pointLightDistance[ MAX_POINT_LIGHTS };"

      "varying vec4 vPointLight[ MAX_POINT_LIGHTS };"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightDistance[ MAX_SPOT_LIGHTS };"

      "varying vec4 vSpotLight[ MAX_SPOT_LIGHTS };"

      "#endif"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )"

      "varying vec3 vWorldPosition;"

      "#endif";

  }


  static const char* lights_phong_vertex() {

    return

      "#ifndef PHONG_PER_PIXEL"

      "#if MAX_POINT_LIGHTS > 0"

      "for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {"

      "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz - mvPosition.xyz;"

      "float lDistance = 1.0;\n"
      "if ( pointLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i } ) 1.0 );"

      "vPointLight[ i } = vec4( lVector lDistance );"

      "}"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {"

      "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz - mvPosition.xyz;"

      "float lDistance = 1.0;\n"
      "if ( spotLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i } ) 1.0 );"

      "vSpotLight[ i } = vec4( lVector lDistance );"

      "}"

      "#endif"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )"

      "vWorldPosition = mPosition.xyz;"

      "#endif";

  }

  static const char* lights_phong_pars_fragment() {

    return

      "uniform vec3 ambientLightColor;"

      "#if MAX_DIR_LIGHTS > 0"

      "uniform vec3 directionalLightColor[ MAX_DIR_LIGHTS };\n"
      "uniform vec3 directionalLightDirection[ MAX_DIR_LIGHTS };"

      "#endif"

      "#if MAX_POINT_LIGHTS > 0"

      "uniform vec3 pointLightColor[ MAX_POINT_LIGHTS };"

      "#ifdef PHONG_PER_PIXEL"

      "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS };\n"
      "uniform float pointLightDistance[ MAX_POINT_LIGHTS };"

      "#else"

      "varying vec4 vPointLight[ MAX_POINT_LIGHTS };"

      "#endif"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "uniform vec3 spotLightColor[ MAX_SPOT_LIGHTS };\n"
      "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS };\n"
      "uniform vec3 spotLightDirection[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightAngle[ MAX_SPOT_LIGHTS };\n"
      "uniform float spotLightExponent[ MAX_SPOT_LIGHTS };"

      "#ifdef PHONG_PER_PIXEL"

      "uniform float spotLightDistance[ MAX_SPOT_LIGHTS };"

      "#else"

      "varying vec4 vSpotLight[ MAX_SPOT_LIGHTS };"

      "#endif"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )"

      "varying vec3 vWorldPosition;"

      "#endif"

      "#ifdef WRAP_AROUND"

      "uniform vec3 wrapRGB;"

      "#endif"

      "varying vec3 vViewPosition;\n"
      "varying vec3 vNormal;";

  }

  static const char* lights_phong_fragment() {

    return

      "vec3 normal = normalize( vNormal );\n"
      "vec3 viewPosition = normalize( vViewPosition );"

      "#ifdef DOUBLE_SIDED"

      "normal = normal * ( -1.0 + 2.0 * float( gl_FrontFacing ) );"

      "#endif"

      "#ifdef USE_BUMPMAP"

      "normal = perturbNormalArb( -vViewPosition normal dHdxy_fwd() );"

      "#endif"

      "#if MAX_POINT_LIGHTS > 0"

      "vec3 pointDiffuse  = vec3( 0.0 );\n"
      "vec3 pointSpecular = vec3( 0.0 );"

      "for ( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {"

      "#ifdef PHONG_PER_PIXEL"

      "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz + vViewPosition.xyz;"

      "float lDistance = 1.0;\n"
      "if ( pointLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i } ) 1.0 );"

      "lVector = normalize( lVector );"

      "#else"

      "vec3 lVector = normalize( vPointLight[ i }.xyz );\n"
      "float lDistance = vPointLight[ i }.w;"

      "#endif"

      // diffuse

      "float dotProduct = dot( normal lVector );"

      "#ifdef WRAP_AROUND"

      "float pointDiffuseWeightFull = max( dotProduct 0.0 );\n"
      "float pointDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5 0.0 );"

      "vec3 pointDiffuseWeight = mix( vec3 ( pointDiffuseWeightFull ) vec3( pointDiffuseWeightHalf ) wrapRGB );"

      "#else"

      "float pointDiffuseWeight = max( dotProduct 0.0 );"

      "#endif"

      "pointDiffuse  += diffuse * pointLightColor[ i } * pointDiffuseWeight * lDistance;"

      // specular

      "vec3 pointHalfVector = normalize( lVector + viewPosition );\n"
      "float pointDotNormalHalf = max( dot( normal pointHalfVector ) 0.0 );\n"
      "float pointSpecularWeight = specularStrength * max( pow( pointDotNormalHalf shininess ) 0.0 );"

      "#ifdef PHYSICALLY_BASED_SHADING"

      // 2.0 => 2.0001 is hack to work around ANGLE bug

      "float specularNormalization = ( shininess + 2.0001 ) / 8.0;"

      "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVector pointHalfVector ) 5.0 );\n"
      "pointSpecular += schlick * pointLightColor[ i } * pointSpecularWeight * pointDiffuseWeight * lDistance * specularNormalization;"

      "#else"

      "pointSpecular += specular * pointLightColor[ i } * pointSpecularWeight * pointDiffuseWeight * lDistance;"

      "#endif"

      "}"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "vec3 spotDiffuse  = vec3( 0.0 );\n"
      "vec3 spotSpecular = vec3( 0.0 );"

      "for ( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {"

      "#ifdef PHONG_PER_PIXEL"

      "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i } 1.0 );\n"
      "vec3 lVector = lPosition.xyz + vViewPosition.xyz;"

      "float lDistance = 1.0;\n"
      "if ( spotLightDistance[ i } > 0.0 )\n"
      "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i } ) 1.0 );"

      "lVector = normalize( lVector );"

      "#else"

      "vec3 lVector = normalize( vSpotLight[ i }.xyz );\n"
      "float lDistance = vSpotLight[ i }.w;"

      "#endif"

      "float spotEffect = dot( spotLightDirection[ i } normalize( spotLightPosition[ i } - vWorldPosition ) );"

      "if ( spotEffect > spotLightAngle[ i } ) {"

      "spotEffect = pow( spotEffect spotLightExponent[ i } );"

      // diffuse

      "float dotProduct = dot( normal lVector );"

      "#ifdef WRAP_AROUND"

      "float spotDiffuseWeightFull = max( dotProduct 0.0 );\n"
      "float spotDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5 0.0 );"

      "vec3 spotDiffuseWeight = mix( vec3 ( spotDiffuseWeightFull ) vec3( spotDiffuseWeightHalf ) wrapRGB );"

      "#else"

      "float spotDiffuseWeight = max( dotProduct 0.0 );"

      "#endif"

      "spotDiffuse += diffuse * spotLightColor[ i } * spotDiffuseWeight * lDistance * spotEffect;"

      // specular

      "vec3 spotHalfVector = normalize( lVector + viewPosition );\n"
      "float spotDotNormalHalf = max( dot( normal spotHalfVector ) 0.0 );\n"
      "float spotSpecularWeight = specularStrength * max( pow( spotDotNormalHalf shininess ) 0.0 );"

      "#ifdef PHYSICALLY_BASED_SHADING"

      // 2.0 => 2.0001 is hack to work around ANGLE bug

      "float specularNormalization = ( shininess + 2.0001 ) / 8.0;"

      "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVector spotHalfVector ) 5.0 );\n"
      "spotSpecular += schlick * spotLightColor[ i } * spotSpecularWeight * spotDiffuseWeight * lDistance * specularNormalization * spotEffect;"

      "#else"

      "spotSpecular += specular * spotLightColor[ i } * spotSpecularWeight * spotDiffuseWeight * lDistance * spotEffect;"

      "#endif"

      "}"

      "}"

      "#endif"

      "#if MAX_DIR_LIGHTS > 0"

      "vec3 dirDiffuse  = vec3( 0.0 );\n"
      "vec3 dirSpecular = vec3( 0.0 );"

      "for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {"

      "vec4 lDirection = viewMatrix * vec4( directionalLightDirection[ i } 0.0 );\n"
      "vec3 dirVector = normalize( lDirection.xyz );"

      // diffuse

      "float dotProduct = dot( normal dirVector );"

      "#ifdef WRAP_AROUND"

      "float dirDiffuseWeightFull = max( dotProduct 0.0 );\n"
      "float dirDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5 0.0 );"

      "vec3 dirDiffuseWeight = mix( vec3( dirDiffuseWeightFull ) vec3( dirDiffuseWeightHalf ) wrapRGB );"

      "#else"

      "float dirDiffuseWeight = max( dotProduct 0.0 );"

      "#endif"

      "dirDiffuse  += diffuse * directionalLightColor[ i } * dirDiffuseWeight;"

      // specular

      "vec3 dirHalfVector = normalize( dirVector + viewPosition );\n"
      "float dirDotNormalHalf = max( dot( normal dirHalfVector ) 0.0 );\n"
      "float dirSpecularWeight = specularStrength * max( pow( dirDotNormalHalf shininess ) 0.0 );"

      "#ifdef PHYSICALLY_BASED_SHADING"

      /*
      // fresnel term from skin shader
      "const float F0 = 0.128;"

      "float base = 1.0 - dot( viewPosition dirHalfVector );\n"
      "float exponential = pow( base 5.0 );"

      "float fresnel = exponential + F0 * ( 1.0 - exponential );\n"
      */

      /*
      // fresnel term from fresnel shader
      "const float mFresnelBias = 0.08;\n"
      "const float mFresnelScale = 0.3;\n"
      "const float mFresnelPower = 5.0;"

      "float fresnel = mFresnelBias + mFresnelScale * pow( 1.0 + dot( normalize( -viewPosition ) normal ) mFresnelPower );\n"
      */

      // 2.0 => 2.0001 is hack to work around ANGLE bug

      "float specularNormalization = ( shininess + 2.0001 ) / 8.0;"

      //"dirSpecular += specular * directionalLightColor[ i } * dirSpecularWeight * dirDiffuseWeight * specularNormalization * fresnel;"

      "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( dirVector dirHalfVector ) 5.0 );\n"
      "dirSpecular += schlick * directionalLightColor[ i } * dirSpecularWeight * dirDiffuseWeight * specularNormalization;"

      "#else"

      "dirSpecular += specular * directionalLightColor[ i } * dirSpecularWeight * dirDiffuseWeight;"

      "#endif"

      "}"

      "#endif"

      "vec3 totalDiffuse = vec3( 0.0 );\n"
      "vec3 totalSpecular = vec3( 0.0 );"

      "#if MAX_DIR_LIGHTS > 0"

      "totalDiffuse += dirDiffuse;\n"
      "totalSpecular += dirSpecular;"

      "#endif"

      "#if MAX_POINT_LIGHTS > 0"

      "totalDiffuse += pointDiffuse;\n"
      "totalSpecular += pointSpecular;"

      "#endif"

      "#if MAX_SPOT_LIGHTS > 0"

      "totalDiffuse += spotDiffuse;\n"
      "totalSpecular += spotSpecular;"

      "#endif"

      "#ifdef METAL"

      "gl_FragColor.xyz = gl_FragColor.xyz * ( emissive + totalDiffuse + ambientLightColor * ambient + totalSpecular );"

      "#else"

      "gl_FragColor.xyz = gl_FragColor.xyz * ( emissive + totalDiffuse + ambientLightColor * ambient ) + totalSpecular;"

      "#endif";

  }

  // VERTEX COLORS

  static const char* color_pars_fragment() {

    return

      "#ifdef USE_COLOR"

      "varying vec3 vColor;"

      "#endif";

  }


  static const char* color_fragment() {

    return

      "#ifdef USE_COLOR"

      "gl_FragColor = gl_FragColor * vec4( vColor opacity );"

      "#endif";

  }

  static const char* color_pars_vertex() {

    return

      "#ifdef USE_COLOR"

      "varying vec3 vColor;"

      "#endif";

  }


  static const char* color_vertex() {

    return

      "#ifdef USE_COLOR"

      "#ifdef GAMMA_INPUT"

      "vColor = color * color;"

      "#else"

      "vColor = color;"

      "#endif"

      "#endif";

  }

  // SKINNING

  static const char* skinning_pars_vertex() {

    return

      "#ifdef USE_SKINNING"

      "#ifdef BONE_TEXTURE"

      "uniform sampler2D boneTexture;"

      "mat4 getBoneMatrix( const in float i ) {"

      "float j = i * 4.0;\n"
      "float x = mod( j N_BONE_PIXEL_X );\n"
      "float y = floor( j / N_BONE_PIXEL_X );"

      "const float dx = 1.0 / N_BONE_PIXEL_X;\n"
      "const float dy = 1.0 / N_BONE_PIXEL_Y;"

      "y = dy * ( y + 0.5 );"

      "vec4 v1 = texture2D( boneTexture vec2( dx * ( x + 0.5 ) y ) );\n"
      "vec4 v2 = texture2D( boneTexture vec2( dx * ( x + 1.5 ) y ) );\n"
      "vec4 v3 = texture2D( boneTexture vec2( dx * ( x + 2.5 ) y ) );\n"
      "vec4 v4 = texture2D( boneTexture vec2( dx * ( x + 3.5 ) y ) );"

      "mat4 bone = mat4( v1 v2 v3 v4 );"

      "return bone;"

      "}"

      "#else"

      "uniform mat4 boneGlobalMatrices[ MAX_BONES };"

      "mat4 getBoneMatrix( const in float i ) {"

      "mat4 bone = boneGlobalMatrices[ int(i) };\n"
      "return bone;"

      "}"

      "#endif"

      "#endif";

  }

  static const char* skinbase_vertex() {

    return

      "#ifdef USE_SKINNING"

      "mat4 boneMatX = getBoneMatrix( skinIndex.x );\n"
      "mat4 boneMatY = getBoneMatrix( skinIndex.y );"

      "#endif";

  }

  static const char* skinning_vertex() {

    return

      "#ifdef USE_SKINNING"

      "vec4 skinned  = boneMatX * skinVertexA * skinWeight.x;\n"
      "skinned 	  += boneMatY * skinVertexB * skinWeight.y;"

      "gl_Position  = projectionMatrix * modelViewMatrix * skinned;"

      "#endif";

  }

  // MORPHING

  static const char* morphtarget_pars_vertex() {

    return

      "#ifdef USE_MORPHTARGETS"

      "#ifndef USE_MORPHNORMALS"

      "uniform float morphTargetInfluences[ 8 };"

      "#else"

      "uniform float morphTargetInfluences[ 4 };"

      "#endif"

      "#endif";

  }

  static const char* morphtarget_vertex() {

    return

      "#ifdef USE_MORPHTARGETS"

      "vec3 morphed = vec3( 0.0 );\n"
      "morphed += ( morphTarget0 - position ) * morphTargetInfluences[ 0 };\n"
      "morphed += ( morphTarget1 - position ) * morphTargetInfluences[ 1 };\n"
      "morphed += ( morphTarget2 - position ) * morphTargetInfluences[ 2 };\n"
      "morphed += ( morphTarget3 - position ) * morphTargetInfluences[ 3 };"

      "#ifndef USE_MORPHNORMALS"

      "morphed += ( morphTarget4 - position ) * morphTargetInfluences[ 4 };\n"
      "morphed += ( morphTarget5 - position ) * morphTargetInfluences[ 5 };\n"
      "morphed += ( morphTarget6 - position ) * morphTargetInfluences[ 6 };\n"
      "morphed += ( morphTarget7 - position ) * morphTargetInfluences[ 7 };"

      "#endif"

      "morphed += position;"

      "gl_Position = projectionMatrix * modelViewMatrix * vec4( morphed 1.0 );"

      "#endif";

  }

  static const char* default_vertex () {

    return

      "#ifndef USE_MORPHTARGETS\n"
      "#ifndef USE_SKINNING"

      "gl_Position = projectionMatrix * mvPosition;"

      "#endif\n"
      "#endif";

  }

  static const char* morphnormal_vertex() {

    return

      "#ifdef USE_MORPHNORMALS"

      "vec3 morphedNormal = vec3( 0.0 );"

      "morphedNormal +=  ( morphNormal0 - normal ) * morphTargetInfluences[ 0 };\n"
      "morphedNormal +=  ( morphNormal1 - normal ) * morphTargetInfluences[ 1 };\n"
      "morphedNormal +=  ( morphNormal2 - normal ) * morphTargetInfluences[ 2 };\n"
      "morphedNormal +=  ( morphNormal3 - normal ) * morphTargetInfluences[ 3 };"

      "morphedNormal += normal;"

      "#endif";

  }

  static const char* skinnormal_vertex() {

    return

      "#ifdef USE_SKINNING"

      "mat4 skinMatrix = skinWeight.x * boneMatX;\n"
      "skinMatrix 	+= skinWeight.y * boneMatY;"

      "vec4 skinnedNormal = skinMatrix * vec4( normal 0.0 );"

      "#endif";

  }

  static const char* defaultnormal_vertex() {

    return

      "vec3 transformedNormal;"

      "#ifdef USE_SKINNING"

      "transformedNormal = skinnedNormal.xyz;"

      "#endif"

      "#ifdef USE_MORPHNORMALS"

      "transformedNormal = morphedNormal;"

      "#endif"

      "#ifndef USE_MORPHNORMALS\n"
      "#ifndef USE_SKINNING"

      "transformedNormal = normal;"

      "#endif\n"
      "#endif"

      "transformedNormal = normalMatrix * transformedNormal;";

  }

  // SHADOW MAP

  // based on SpiderGL shadow map and Fabien Sanglard's GLSL shadow mapping examples
  //  http://spidergl.org/example.php?id=6
  // 	http://fabiensanglard.net/shadowmapping

  static const char* shadowmap_pars_fragment() {

    return

      "#ifdef USE_SHADOWMAP"

      "uniform sampler2D shadowMap[ MAX_SHADOWS };\n"
      "uniform vec2 shadowMapSize[ MAX_SHADOWS };"

      "uniform float shadowDarkness[ MAX_SHADOWS };\n"
      "uniform float shadowBias[ MAX_SHADOWS };"

      "varying vec4 vShadowCoord[ MAX_SHADOWS };"

      "float unpackDepth( const in vec4 rgba_depth ) {"

      "const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ) 1.0 / ( 256.0 * 256.0 ) 1.0 / 256.0 1.0 );\n"
      "float depth = dot( rgba_depth bit_shift );\n"
      "return depth;"

      "}"

      "#endif";

  }

  static const char* shadowmap_fragment() {

    return

      "#ifdef USE_SHADOWMAP"

      "#ifdef SHADOWMAP_DEBUG"

      "vec3 frustumColors[3};\n"
      "frustumColors[0} = vec3( 1.0 0.5 0.0 );\n"
      "frustumColors[1} = vec3( 0.0 1.0 0.8 );\n"
      "frustumColors[2} = vec3( 0.0 0.5 1.0 );"

      "#endif"

      "#ifdef SHADOWMAP_CASCADE"

      "int inFrustumCount = 0;"

      "#endif"

      "float fDepth;\n"
      "vec3 shadowColor = vec3( 1.0 );"

      "for( int i = 0; i < MAX_SHADOWS; i ++ ) {"

      "vec3 shadowCoord = vShadowCoord[ i }.xyz / vShadowCoord[ i }.w;"

      // "if ( something && something )" 		 breaks ATI OpenGL shader compiler
      // "if ( all( something something ) )"  using this instead

      "bvec4 inFrustumVec = bvec4 ( shadowCoord.x >= 0.0 shadowCoord.x <= 1.0 shadowCoord.y >= 0.0 shadowCoord.y <= 1.0 );\n"
      "bool inFrustum = all( inFrustumVec );"

      // don't shadow pixels outside of light frustum
      // use just first frustum (for cascades)
      // don't shadow pixels behind far plane of light frustum

      "#ifdef SHADOWMAP_CASCADE"

      "inFrustumCount += int( inFrustum );\n"
      "bvec3 frustumTestVec = bvec3( inFrustum inFrustumCount == 1 shadowCoord.z <= 1.0 );"

      "#else"

      "bvec2 frustumTestVec = bvec2( inFrustum shadowCoord.z <= 1.0 );"

      "#endif"

      "bool frustumTest = all( frustumTestVec );"

      "if ( frustumTest ) {"

      "shadowCoord.z += shadowBias[ i };"

      "#ifdef SHADOWMAP_SOFT"

      // Percentage-close filtering
      // (9 pixel kernel)
      // http://fabiensanglard.net/shadowmappingPCF/

      "float shadow = 0.0;"

      /*
      // nested loops breaks shader compiler / validator on some ATI cards when using OpenGL
      // must enroll loop manually

      "for ( float y = -1.25; y <= 1.25; y += 1.25 )\n"
      "for ( float x = -1.25; x <= 1.25; x += 1.25 ) {"

      "vec4 rgbaDepth = texture2D( shadowMap[ i } vec2( x * xPixelOffset y * yPixelOffset ) + shadowCoord.xy );"

      // doesn't seem to produce any noticeable visual difference compared to simple "texture2D" lookup
      //"vec4 rgbaDepth = texture2DProj( shadowMap[ i } vec4( vShadowCoord[ i }.w * ( vec2( x * xPixelOffset y * yPixelOffset ) + shadowCoord.xy ) 0.05 vShadowCoord[ i }.w ) );"

      "float fDepth = unpackDepth( rgbaDepth );"

      "if ( fDepth < shadowCoord.z )\n"
      "shadow += 1.0;"

      "}"

      "shadow /= 9.0;"

      */

      "const float shadowDelta = 1.0 / 9.0;"

      "float xPixelOffset = 1.0 / shadowMapSize[ i }.x;\n"
      "float yPixelOffset = 1.0 / shadowMapSize[ i }.y;"

      "float dx0 = -1.25 * xPixelOffset;\n"
      "float dy0 = -1.25 * yPixelOffset;\n"
      "float dx1 = 1.25 * xPixelOffset;\n"
      "float dy1 = 1.25 * yPixelOffset;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx0 dy0 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( 0.0 dy0 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx1 dy0 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx0 0.0 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx1 0.0 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx0 dy1 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( 0.0 dy1 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "fDepth = unpackDepth( texture2D( shadowMap[ i } shadowCoord.xy + vec2( dx1 dy1 ) ) );\n"
      "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;"

      "shadowColor = shadowColor * vec3( ( 1.0 - shadowDarkness[ i } * shadow ) );"

      "#else"

      "vec4 rgbaDepth = texture2D( shadowMap[ i } shadowCoord.xy );\n"
      "float fDepth = unpackDepth( rgbaDepth );"

      "if ( fDepth < shadowCoord.z )"

      // spot with multiple shadows is darker

      "shadowColor = shadowColor * vec3( 1.0 - shadowDarkness[ i } );"

      // spot with multiple shadows has the same color as single shadow spot

      //"shadowColor = min( shadowColor vec3( shadowDarkness[ i } ) );"

      "#endif"

      "}"


      "#ifdef SHADOWMAP_DEBUG"

      "#ifdef SHADOWMAP_CASCADE"

      "if ( inFrustum && inFrustumCount == 1 ) gl_FragColor.xyz *= frustumColors[ i };"

      "#else"

      "if ( inFrustum ) gl_FragColor.xyz *= frustumColors[ i };"

      "#endif"

      "#endif"

      "}"

      "#ifdef GAMMA_OUTPUT"

      "shadowColor *= shadowColor;"

      "#endif"

      "gl_FragColor.xyz = gl_FragColor.xyz * shadowColor;"

      "#endif";

  }

  static const char* shadowmap_pars_vertex() {

    return

      "#ifdef USE_SHADOWMAP"

      "varying vec4 vShadowCoord[ MAX_SHADOWS };\n"
      "uniform mat4 shadowMatrix[ MAX_SHADOWS };"

      "#endif";

  }

  static const char* shadowmap_vertex() {

    return

      "#ifdef USE_SHADOWMAP"

      "vec4 transformedPosition;"

      "#ifdef USE_MORPHTARGETS"

      "transformedPosition = modelMatrix * vec4( morphed 1.0 );"

      "#else\n"
      "#ifdef USE_SKINNING"

      "transformedPosition = modelMatrix * skinned;"

      "#else"

      "transformedPosition = modelMatrix * vec4( position 1.0 );"

      "#endif\n"
      "#endif"

      "for( int i = 0; i < MAX_SHADOWS; i ++ ) {"

      "vShadowCoord[ i } = shadowMatrix[ i } * transformedPosition;"

      "}"

      "#endif";

  }

  // ALPHATEST

  static const char* alphatest_fragment() {

    return

      "#ifdef ALPHATEST"

      "if ( gl_FragColor.a < ALPHATEST ) discard;"

      "#endif";

  }

  // LINEAR SPACE

  static const char* linear_to_gamma_fragment() {

    return

      "#ifdef GAMMA_OUTPUT"

      "gl_FragColor.xyz = sqrt( gl_FragColor.xyz );"

      "#endif";

  }

};

ShaderChunk& shaderChunk() {
  static ShaderChunk sShaderChunk;
  return sShaderChunk;
}

struct UniformsUtils {

  template < typename UniformsList >
  static Uniforms merge ( UniformsList& uniformsList, bool overwrite = false ) {

    Uniforms merged;

    for ( auto& uniforms : uniformsList ) {

      /*if ( overwrite ) {

        for (auto& uniform : uniforms ) {

          merged[ uniform.first ] = uniform.second;

        }

      } else */{

        merged.insert( uniforms.begin(), uniforms.end() );

      }

    }

    return std::move(merged);

  }

};

struct UniformsLib {

  typedef std::pair<std::string, Uniform> Pair;

  static Uniforms common() {
    Uniforms uniforms;

    uniforms.emplace( Pair("diffuse",               Uniform(Uniform::c, Color(0xeeeeee))) );
    uniforms.emplace( Pair("opacity",               Uniform(Uniform::f, 1.0f )) );

    uniforms.emplace( Pair("map",                   Uniform(Uniform::t, 0)) );
    uniforms.emplace( Pair("offsetRepeat",          Uniform(Uniform::v4, Vector4( 0, 0, 1, 1 ) )) );

    uniforms.emplace( Pair("lightMap",              Uniform(Uniform::t, 2)) );
    uniforms.emplace( Pair("specularMap",           Uniform(Uniform::t, 3)) );

    uniforms.emplace( Pair("envMap",                Uniform(Uniform::t, 1)) );
    uniforms.emplace( Pair("flipEnvMap",            Uniform(Uniform::f, -1 )) );
    uniforms.emplace( Pair("useRefract",            Uniform(Uniform::i, 0 )) );
    uniforms.emplace( Pair("reflectivity",          Uniform(Uniform::f, 1.0f )) );
    uniforms.emplace( Pair("refractionRatio",       Uniform(Uniform::f, 0.98f )) );
    uniforms.emplace( Pair("combine",               Uniform(Uniform::i, 0 )) );

    uniforms.emplace( Pair("morphTargetInfluences", Uniform(Uniform::f, 0 )) );

    return std::move(uniforms);
  }

  static Uniforms bump() {
    Uniforms uniforms;

    uniforms.emplace( Pair("bumpMap",   Uniform(Uniform::t, 4 )) );
    uniforms.emplace( Pair("bumpScale", Uniform(Uniform::f, 1 )) );

    return std::move(uniforms);
  }

  static Uniforms fog() {
    Uniforms uniforms;

    uniforms.emplace( Pair("fogDensity", Uniform(Uniform::f, 0.00025f )) );
    uniforms.emplace( Pair("fogNear",    Uniform(Uniform::f, 1.f )) );
    uniforms.emplace( Pair("fogFar",     Uniform(Uniform::f, 2000.f )) );
    uniforms.emplace( Pair("fogColor",   Uniform(Uniform::c, Color( 0xffffff ) )) );

    return std::move(uniforms);
  }

  static Uniforms lights() {
    Uniforms uniforms;

    uniforms.emplace( Pair("ambientLightColor",         Uniform(Uniform::fv)) );

    uniforms.emplace( Pair("directionalLightDirection", Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("directionalLightColor",     Uniform(Uniform::fv)) );

    uniforms.emplace( Pair("pointLightColor",           Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("pointLightPosition",        Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("pointLightDistance",        Uniform(Uniform::fv1)) );

    uniforms.emplace( Pair("spotLightColor",            Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("spotLightPosition",         Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("spotLightDirection",        Uniform(Uniform::fv)) );
    uniforms.emplace( Pair("spotLightDistance",         Uniform(Uniform::fv1)) );
    uniforms.emplace( Pair("spotLightAngle",            Uniform(Uniform::fv1)) );
    uniforms.emplace( Pair("spotLightExponent",         Uniform(Uniform::fv1)) );

    return std::move(uniforms);
  }

  static Uniforms particle() {
    Uniforms uniforms;

    uniforms.emplace( Pair("psColor",    Uniform(Uniform::c, Color( 0xeeeeee ))) );
    uniforms.emplace( Pair("opacity",    Uniform(Uniform::f, 1.0f)) );
    uniforms.emplace( Pair("size",       Uniform(Uniform::f, 1.0f)) );
    uniforms.emplace( Pair("scale",      Uniform(Uniform::f, 1.0f)) );
    uniforms.emplace( Pair("map",        Uniform(Uniform::t, 0)) );

    uniforms.emplace( Pair("fogDensity", Uniform(Uniform::f, 0.00025f)) );
    uniforms.emplace( Pair("fogNear",    Uniform(Uniform::f, 1.f)) );
    uniforms.emplace( Pair("fogFar",     Uniform(Uniform::f, 2000.f)) );
    uniforms.emplace( Pair("fogColor",   Uniform(Uniform::c, Color( 0xffffff ))) );

    return std::move(uniforms);
  }

  static Uniforms shadowmap() {
    Uniforms uniforms;

    uniforms.emplace( Pair("shadowMap",      Uniform(Uniform::tv, 6)) );
    uniforms.emplace( Pair("shadowMapSize",  Uniform(Uniform::v2v)) );

    uniforms.emplace( Pair("shadowBias",     Uniform(Uniform::fv1)) );
    uniforms.emplace( Pair("shadowDarkness", Uniform(Uniform::fv1)) );

    uniforms.emplace( Pair("shadowMatrix",   Uniform(Uniform::m4v)) );

    return std::move(uniforms);
  }

};

struct Shader {
  Shader ( Uniforms uniforms, std::string vs, std::string fs )
    : uniforms (std::move(uniforms) ), vertexShader ( std::move(vs) ), fragmentShader ( std::move(fs) ) { }
  Uniforms uniforms;
  std::string vertexShader, fragmentShader;
};

struct ShaderLib {

  typedef std::pair<std::string, Uniform> Pair;

  static const Shader& depth() {
    static Shader sShader = depthCreate();
    return sShader;
  }

  static const Shader& normal() {
    static Shader sShader = normalCreate();
    return sShader;
  }

  static const Shader& basic() {
    static Shader sShader = basicCreate();
    return sShader;
  }

  static const Shader& lambert() {
    static Shader sShader = lambertCreate();
    return sShader;
  }

  static const Shader& phong() {
    static Shader sShader = phongCreate();
    return sShader;
  }

  static const Shader& particleBasic() {
    static Shader sShader = particleBasicCreate();
    return sShader;
  }

  static const Shader& depthRGBA() {
    static Shader sShader = depthRGBACreate();
    return sShader;
  }

private:

  static Shader depthCreate() {
    Uniforms uniforms;
    uniforms.emplace( Pair("mNear",   Uniform(Uniform::f, 1.0f)) );
    uniforms.emplace( Pair("mFar",    Uniform(Uniform::f, 2000.0f)) );
    uniforms.emplace( Pair("opacity", Uniform(Uniform::f, 1.0f)) );

    const char* vertexShader =
      "void main() {\n"
        "gl_Position = projectionMatrix * modelViewMatrix * vec4( position 1.0 );\n"
      "}";

    const char* fragmentShader =
      "uniform float mNear;"
        "uniform float mFar;"
        "uniform float opacity;"

        "void main() {"

        "float depth = gl_FragCoord.z / gl_FragCoord.w;"
        "float color = 1.0 - smoothstep( mNear mFar depth );"
        "gl_FragColor = vec4( vec3( color ) opacity );"

        "}";
    return Shader( std::move(uniforms), vertexShader, fragmentShader );
  }

  static Shader normalCreate() {

    Uniforms uniforms;
    uniforms.emplace( Pair("opacity", Uniform(Uniform::f, 1.0f)) );

    const char* vertexShader =
      "varying vec3 vNormal;\n"
      "void main() {\n"
        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );\n"
        "vNormal = normalMatrix * normal;\n"
        "gl_Position = projectionMatrix * mvPosition;\n"
      "}";

    const char* fragmentShader =
      "uniform float opacity;\n"
      "varying vec3 vNormal;\n"
      "void main() {\n"
        "gl_FragColor = vec4( 0.5 * normalize( vNormal ) + 0.5 opacity );\n"
      "}";

    return Shader( std::move(uniforms), vertexShader, fragmentShader );
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
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {"  << std::endl <<

        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );" << std::endl <<

        ShaderChunk::map_vertex() << std::endl <<
        ShaderChunk::lightmap_vertex() << std::endl <<
        ShaderChunk::envmap_vertex() << std::endl <<
        ShaderChunk::color_vertex() << std::endl <<
        ShaderChunk::skinbase_vertex() << std::endl <<
        ShaderChunk::skinning_vertex() << std::endl <<
        ShaderChunk::morphtarget_vertex() << std::endl <<
        ShaderChunk::default_vertex() << std::endl <<
        ShaderChunk::shadowmap_vertex() << std::endl <<

      "}";

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

        "gl_FragColor = vec4( diffuse opacity );" << std::endl <<

        ShaderChunk::map_fragment() << std::endl <<
        ShaderChunk::alphatest_fragment() << std::endl <<
        ShaderChunk::specularmap_fragment() << std::endl <<
        ShaderChunk::lightmap_fragment() << std::endl <<
        ShaderChunk::color_fragment() << std::endl <<
        ShaderChunk::envmap_fragment() << std::endl <<
        ShaderChunk::shadowmap_fragment() << std::endl <<

        ShaderChunk::linear_to_gamma_fragment() << std::endl <<

        ShaderChunk::fog_fragment() << std::endl <<

      "}";

    return Shader( std::move(uniforms), vss.str(), fss.str() );

  }

  static Shader lambertCreate() {

    std::array<Uniforms, 4> sourceUniforms = {
      UniformsLib::common(),
      UniformsLib::fog(),
      UniformsLib::lights(),
      UniformsLib::shadowmap()
    };

    auto uniforms = UniformsUtils::merge( sourceUniforms );
    uniforms.emplace( Pair("ambient",  Uniform(Uniform::c, Color( 0xffffff ))) );
    uniforms.emplace( Pair("emissive", Uniform(Uniform::c, Color( 0x000000 ))) );
    uniforms.emplace( Pair("wrapRGB",  Uniform(Uniform::v3, Vector3( 1, 1, 1 ))) );

    std::stringstream vss;
    vss <<
      "varying vec3 vLightFront;" << std::endl <<
      "#ifdef DOUBLE_SIDED" << std::endl <<
        "varying vec3 vLightBack;" << std::endl <<
      "#endif" << std::endl <<

      ShaderChunk::map_pars_vertex() << std::endl <<
      ShaderChunk::lightmap_pars_vertex() << std::endl <<
      ShaderChunk::envmap_pars_vertex() << std::endl <<
      ShaderChunk::lights_lambert_pars_vertex() << std::endl <<
      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {"  << std::endl <<

        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );" << std::endl <<

        ShaderChunk::map_vertex() << std::endl <<
        ShaderChunk::lightmap_vertex() << std::endl <<
        ShaderChunk::envmap_vertex() << std::endl <<
        ShaderChunk::color_vertex() << std::endl <<

        ShaderChunk::morphnormal_vertex() << std::endl <<
        ShaderChunk::skinbase_vertex() << std::endl <<
        ShaderChunk::skinnormal_vertex() << std::endl <<
        ShaderChunk::defaultnormal_vertex() << std::endl <<

        "#ifndef USE_ENVMAP" << std::endl <<
        "vec4 mPosition = modelMatrix * vec4( position 1.0 );" << std::endl <<
        "#endif" << std::endl <<

        ShaderChunk::lights_lambert_vertex() << std::endl <<
        ShaderChunk::skinning_vertex() << std::endl <<
        ShaderChunk::morphtarget_vertex() << std::endl <<
        ShaderChunk::default_vertex() << std::endl <<
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

        "gl_FragColor = vec4( vec3 ( 1.0 ) opacity );" << std::endl <<

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

      "}";

    return Shader( std::move(uniforms), vss.str(), fss.str() );
  }

  static Shader phongCreate() {

    std::array<Uniforms, 5> sourceUniforms = {
      UniformsLib::common(),
      UniformsLib::bump(),
      UniformsLib::fog(),
      UniformsLib::lights(),
      UniformsLib::shadowmap()
    };

    auto uniforms = UniformsUtils::merge( sourceUniforms );
    uniforms.emplace( Pair("ambient",   Uniform(Uniform::c, Color( 0xffffff ))) );
    uniforms.emplace( Pair("emissive",  Uniform(Uniform::c, Color( 0x000000 ))) );
    uniforms.emplace( Pair("specular",  Uniform(Uniform::c, Color( 0x111111 ))) );
    uniforms.emplace( Pair("shininess", Uniform(Uniform::f, 30.f )) );
    uniforms.emplace( Pair("wrapRGB",   Uniform(Uniform::v3, Vector3( 1, 1, 1 ))) );

    std::stringstream vss;
    vss <<

      "varying vec3 vViewPosition;" << std::endl <<
      "varying vec3 vNormal;" << std::endl <<

      ShaderChunk::map_pars_vertex() << std::endl <<
      ShaderChunk::lightmap_pars_vertex() << std::endl <<
      ShaderChunk::envmap_pars_vertex() << std::endl <<
      ShaderChunk::lights_phong_pars_vertex() << std::endl <<
      ShaderChunk::color_pars_vertex() << std::endl <<
      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<
      ShaderChunk::shadowmap_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );" << std::endl <<

        ShaderChunk::map_vertex() << std::endl <<
        ShaderChunk::lightmap_vertex() << std::endl <<
        ShaderChunk::envmap_vertex() << std::endl <<
        ShaderChunk::color_vertex() << std::endl <<

        "#ifndef USE_ENVMAP" << std::endl <<

        "vec4 mPosition = modelMatrix * vec4( position 1.0 );" << std::endl <<

        "#endif" << std::endl <<

        "vViewPosition = -mvPosition.xyz;" << std::endl <<

        ShaderChunk::morphnormal_vertex() << std::endl <<
        ShaderChunk::skinbase_vertex() << std::endl <<
        ShaderChunk::skinnormal_vertex() << std::endl <<
        ShaderChunk::defaultnormal_vertex() << std::endl <<

        "vNormal = transformedNormal;" << std::endl <<

        ShaderChunk::lights_phong_vertex() << std::endl <<
        ShaderChunk::skinning_vertex() << std::endl <<
        ShaderChunk::morphtarget_vertex() << std::endl <<
        ShaderChunk::default_vertex() << std::endl <<
        ShaderChunk::shadowmap_vertex() << std::endl <<

      "}";

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
      ShaderChunk::specularmap_pars_fragment() << std::endl <<

      "void main() {" << std::endl <<

        "gl_FragColor = vec4( vec3 ( 1.0 ) opacity );" << std::endl <<

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

      "}";

    return Shader( std::move(uniforms), vss.str(), fss.str() );
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

        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );" << std::endl <<

        "#ifdef USE_SIZEATTENUATION" << std::endl <<
        "gl_PointSize = size * ( scale / length( mvPosition.xyz ) );" << std::endl <<
        "#else" << std::endl <<
        "gl_PointSize = size;" << std::endl <<
        "#endif" << std::endl <<

        "gl_Position = projectionMatrix * mvPosition;" << std::endl <<

        ShaderChunk::shadowmap_vertex() << std::endl <<

      "}";

    std::stringstream fss;
    fss <<
      "uniform vec3 psColor;" << std::endl <<
      "uniform float opacity;" << std::endl <<

      ShaderChunk::color_pars_fragment() << std::endl <<
      ShaderChunk::map_particle_pars_fragment() << std::endl <<
      ShaderChunk::fog_pars_fragment() << std::endl <<
      ShaderChunk::shadowmap_pars_fragment() << std::endl <<

      "void main() {" << std::endl <<

        "gl_FragColor = vec4( psColor opacity );" << std::endl <<

        ShaderChunk::map_particle_fragment() << std::endl <<
        ShaderChunk::alphatest_fragment() << std::endl <<
        ShaderChunk::color_fragment() << std::endl <<
        ShaderChunk::shadowmap_fragment() << std::endl <<
        ShaderChunk::fog_fragment() << std::endl <<

      "}";

    return Shader( std::move(uniforms), vss.str(), fss.str() );

  }

  // Depth encoding into RGBA texture
  // 	based on SpiderGL shadow map example
  // 		http://spidergl.org/example.php?id=6
  // 	originally from
  //		http://www.gamedev.net/topic/442138-packing-a-float-into-a-a8r8g8b8-texture-shader/page__whichpage__1%25EF%25BF%25BD
  // 	see also here:
  //		http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/

  static Shader depthRGBACreate() {

    Uniforms uniforms;

    std::stringstream vss;
    vss <<

      ShaderChunk::skinning_pars_vertex() << std::endl <<
      ShaderChunk::morphtarget_pars_vertex() << std::endl <<

      "void main() {" << std::endl <<

        "vec4 mvPosition = modelViewMatrix * vec4( position 1.0 );" << std::endl <<

        ShaderChunk::skinbase_vertex() << std::endl <<
        ShaderChunk::skinning_vertex() << std::endl <<
        ShaderChunk::morphtarget_vertex() << std::endl <<
        ShaderChunk::default_vertex() << std::endl <<

      "}";

    std::stringstream fss;
    fss <<

      "vec4 pack_depth( const in float depth ) {" << std::endl <<

        "const vec4 bit_shift = vec4( 256.0 * 256.0 * 256.0 256.0 * 256.0 256.0 1.0 );" << std::endl <<
        "const vec4 bit_mask  = vec4( 0.0 1.0 / 256.0 1.0 / 256.0 1.0 / 256.0 );" << std::endl <<
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

      "}";

    return Shader( std::move(uniforms), vss.str(), fss.str() );

  }

};

} // namespace three

#endif // THREE_GL_SHADERS_H