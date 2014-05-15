#ifndef THREE_SHADERS_SHADER_CHUNK_CPP
#define THREE_SHADERS_SHADER_CHUNK_CPP

#include <three/renderers/shaders/shader_chunk.h>

namespace three {
  
    
    const char* ShaderChunk::fog_pars_fragment() {
        return
        
		"#ifdef USE_FOG\n"
        
        "uniform vec3 fogColor;\n"
        
        "#ifdef FOG_EXP2\n"
        
        "uniform float fogDensity;\n"
        
        "#else\n"
        
        "uniform float fogNear;\n"
        "uniform float fogFar;\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::fog_fragment() {
        return
        
		"#ifdef USE_FOG\n"
        
        "float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
        
        "#ifdef FOG_EXP2\n"
        
        "const float LOG2 = 1.442695;\n"
        "float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );\n"
        "fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );\n"
        
        "#else\n"
        
        "float fogFactor = smoothstep( fogNear, fogFar, depth );\n"
        
        "#endif\n"
        
        "gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );\n"
        
		"#endif\n";
	}
    
	// ENVIRONMENT MAP
    const char* ShaderChunk::envmap_pars_fragment() {
        return
        
		"#ifdef USE_ENVMAP\n"
        
        "uniform float reflectivity;\n"
        "uniform samplerCube envMap;\n"
        "uniform float flipEnvMap;\n"
        "uniform int combine;\n"
        
        "#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )\n"
        
        "uniform bool useRefract;\n"
        "uniform float refractionRatio;\n"
        
        "#else\n"
        
        "varying vec3 vReflect;\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::envmap_fragment() {
        return
        
		"#ifdef USE_ENVMAP\n"
        
        "vec3 reflectVec;\n"
        
        "#if defined( USE_BUMPMAP ) || defined( USE_NORMALMAP )\n"
        
        "vec3 cameraToVertex = normalize( vWorldPosition - cameraPosition );\n"
        
        "if ( useRefract ) {\n"
        
        "reflectVec = refract( cameraToVertex, normal, refractionRatio );\n"
        
        "} else { \n"
        
        "reflectVec = reflect( cameraToVertex, normal );\n"
        
        "}\n"
        
        "#else\n"
        
        "reflectVec = vReflect;\n"
        
        "#endif\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "float flipNormal = ( -1.0 + 2.0 * float( gl_FrontFacing ) );\n"
        "vec4 cubeColor = textureCube( envMap, flipNormal * vec3( flipEnvMap * reflectVec.x, reflectVec.yz ) );\n"
        
        "#else\n"
        
        "vec4 cubeColor = textureCube( envMap, vec3( flipEnvMap * reflectVec.x, reflectVec.yz ) );\n"
        
        "#endif\n"
        
        "#ifdef GAMMA_INPUT\n"
        
        "cubeColor.xyz *= cubeColor.xyz;\n"
        
        "#endif\n"
        
        "if ( combine == 1 ) {\n"
        
        "gl_FragColor.xyz = mix( gl_FragColor.xyz, cubeColor.xyz, specularStrength * reflectivity );\n"
        
        "} else if ( combine == 2 ) {\n"
        
        "gl_FragColor.xyz += cubeColor.xyz * specularStrength * reflectivity;\n"
        
        "} else {\n"
        
        "gl_FragColor.xyz = mix( gl_FragColor.xyz, gl_FragColor.xyz * cubeColor.xyz, specularStrength * reflectivity );\n"
        
        "}\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::envmap_pars_vertex() {
        return
        
		"#if defined( USE_ENVMAP ) && ! defined( USE_BUMPMAP ) && ! defined( USE_NORMALMAP )\n"
        
        "varying vec3 vReflect;\n"
        
        "uniform float refractionRatio;\n"
        "uniform bool useRefract;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::worldpos_vertex () {
        return
        
		"#if defined( USE_ENVMAP ) || defined( PHONG ) || defined( LAMBERT ) || defined ( USE_SHADOWMAP )\n"
        
        "#ifdef USE_SKINNING\n"
        
        "vec4 worldPosition = modelMatrix * skinned;\n"
        
        "#endif\n"
        
        "#if defined( USE_MORPHTARGETS ) && ! defined( USE_SKINNING )\n"
        
        "vec4 worldPosition = modelMatrix * vec4( morphed, 1.0 );\n"
        
        "#endif\n"
        
        "#if ! defined( USE_MORPHTARGETS ) && ! defined( USE_SKINNING )\n"
        
        "vec4 worldPosition = modelMatrix * vec4( position, 1.0 );\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::envmap_vertex () {
        return
        
		"#if defined( USE_ENVMAP ) && ! defined( USE_BUMPMAP ) && ! defined( USE_NORMALMAP )\n"
        
        "vec3 worldNormal = mat3( modelMatrix[ 0 ].xyz, modelMatrix[ 1 ].xyz, modelMatrix[ 2 ].xyz ) * objectNormal;\n"
        "worldNormal = normalize( worldNormal );\n"
        
        "vec3 cameraToVertex = normalize( worldPosition.xyz - cameraPosition );\n"
        
        "if ( useRefract ) {\n"
        
        "vReflect = refract( cameraToVertex, worldNormal, refractionRatio );\n"
        
        "} else {\n"
        
        "vReflect = reflect( cameraToVertex, worldNormal );\n"
        
        "}\n"
        
		"#endif\n";
	}
    
	// COLOR MAP (particles)
    const char* ShaderChunk::map_particle_pars_fragment() {
        return
        
		"#ifdef USE_MAP\n"
        
        "uniform sampler2D map;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::map_particle_fragment() {
        return
        
		"#ifdef USE_MAP\n"
        
        "gl_FragColor = gl_FragColor * texture2D( map, vec2( gl_PointCoord.x, 1.0 - gl_PointCoord.y ) );\n"
        
		"#endif\n";
	}
    
	// COLOR MAP (triangles)
    const char* ShaderChunk::map_pars_vertex() {
        return
        
		"#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )\n"
        
        "varying vec2 vUv;\n"
        "uniform vec4 offsetRepeat;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::map_pars_fragment() {
        return
        
		"#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )\n"
        
        "varying vec2 vUv;\n"
        
		"#endif\n"
        
		"#ifdef USE_MAP\n"
        
        "uniform sampler2D map;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::map_vertex() {
        return
        
		"#if defined( USE_MAP ) || defined( USE_BUMPMAP ) || defined( USE_NORMALMAP ) || defined( USE_SPECULARMAP )\n"
        
        "vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::map_fragment() {
        return
        
		"#ifdef USE_MAP\n"
        
        "vec4 texelColor = texture2D( map, vUv );\n"
        
        "#ifdef GAMMA_INPUT\n"
        
        "texelColor.xyz *= texelColor.xyz;\n"
        
        "#endif\n"
        
        "gl_FragColor = gl_FragColor * texelColor;\n"
        
		"#endif\n";
	}
    
	// LIGHT MAP
    const char* ShaderChunk::lightmap_pars_fragment() {
        return
        
		"#ifdef USE_LIGHTMAP\n"
        
        "varying vec2 vUv2;\n"
        "uniform sampler2D lightMap;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lightmap_pars_vertex() {
        return
        
		"#ifdef USE_LIGHTMAP\n"
        
        "varying vec2 vUv2;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lightmap_fragment() {
        return
        
		"#ifdef USE_LIGHTMAP\n"
        
        "gl_FragColor = gl_FragColor * texture2D( lightMap, vUv2 );\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lightmap_vertex() {
        return
        
		"#ifdef USE_LIGHTMAP\n"
        
        "vUv2 = uv2;\n"
        
		"#endif\n";
	}
    
	// BUMP MAP
    const char* ShaderChunk::bumpmap_pars_fragment() {
        return
        
		"#ifdef USE_BUMPMAP\n"
        
        "uniform sampler2D bumpMap;\n"
        "uniform float bumpScale;\n"
        
        // Derivative maps - bump mapping unparametrized surfaces by Morten Mikkelsen
        //	http://mmikkelsen3d.blogspot.sk/2011/07/derivative-maps.html
        
        // Evaluate the derivative of the height w.r.t. screen-space using forward differencing (listing 2)
        
        "vec2 dHdxy_fwd() {\n"
        
        "vec2 dSTdx = dFdx( vUv );\n"
        "vec2 dSTdy = dFdy( vUv );\n"
        
        "float Hll = bumpScale * texture2D( bumpMap, vUv ).x;\n"
        "float dBx = bumpScale * texture2D( bumpMap, vUv + dSTdx ).x - Hll;\n"
        "float dBy = bumpScale * texture2D( bumpMap, vUv + dSTdy ).x - Hll;\n"
        
        "return vec2( dBx, dBy );\n"
        
        "}\n"
        
        "vec3 perturbNormalArb( vec3 surf_pos, vec3 surf_norm, vec2 dHdxy ) {\n"
        
        "vec3 vSigmaX = dFdx( surf_pos );\n"
        "vec3 vSigmaY = dFdy( surf_pos );\n"
        "vec3 vN = surf_norm;"		// normalized
        
        "vec3 R1 = cross( vSigmaY, vN );\n"
        "vec3 R2 = cross( vN, vSigmaX );\n"
        
        "float fDet = dot( vSigmaX, R1 );\n"
        
        "vec3 vGrad = sign( fDet ) * ( dHdxy.x * R1 + dHdxy.y * R2 );\n"
        "return normalize( abs( fDet ) * surf_norm - vGrad );\n"
        
        "}\n"
        
		"#endif\n";
	}
    
	// NORMAL MAP
    const char* ShaderChunk::normalmap_pars_fragment() {
        return
        
		"#ifdef USE_NORMALMAP\n"
        
        "uniform sampler2D normalMap;\n"
        "uniform vec2 normalScale;\n"
        
        // Per-Pixel Tangent Space Normal Mapping
        // http://hacksoflife.blogspot.ch/2009/11/per-pixel-tangent-space-normal-mapping.html
        
        "vec3 perturbNormal2Arb( vec3 eye_pos, vec3 surf_norm ) {\n"
        
        "vec3 q0 = dFdx( eye_pos.xyz );\n"
        "vec3 q1 = dFdy( eye_pos.xyz );\n"
        "vec2 st0 = dFdx( vUv.st );\n"
        "vec2 st1 = dFdy( vUv.st );\n"
        
        "vec3 S = normalize(  q0 * st1.t - q1 * st0.t );\n"
        "vec3 T = normalize( -q0 * st1.s + q1 * st0.s );\n"
        "vec3 N = normalize( surf_norm );\n"
        
        "vec3 mapN = texture2D( normalMap, vUv ).xyz * 2.0 - 1.0;\n"
        "mapN.xy = normalScale * mapN.xy;\n"
        "mat3 tsn = mat3( S, T, N );\n"
        "return normalize( tsn * mapN );\n"
        
        "}\n"
        
		"#endif\n";
	}
    
	// SPECULAR MAP
    const char* ShaderChunk::specularmap_pars_fragment() {
        return
        
		"#ifdef USE_SPECULARMAP\n"
        
        "uniform sampler2D specularMap;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::specularmap_fragment() {
        return
        
		"float specularStrength;\n"
        
		"#ifdef USE_SPECULARMAP\n"
        
        "vec4 texelSpecular = texture2D( specularMap, vUv );\n"
        "specularStrength = texelSpecular.r;\n"
        
		"#else\n"
        
        "specularStrength = 1.0;\n"
        
		"#endif\n";
	}
    
	// LIGHTS LAMBERT
    const char* ShaderChunk::lights_lambert_pars_vertex() {
        return
        
		"uniform vec3 ambient;\n"
		"uniform vec3 diffuse;\n"
		"uniform vec3 emissive;\n"
        
		"uniform vec3 ambientLightColor;\n"
        
		"#if MAX_DIR_LIGHTS > 0\n"
        
        "uniform vec3 directionalLightColor[ MAX_DIR_LIGHTS ];\n"
        "uniform vec3 directionalLightDirection[ MAX_DIR_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_HEMI_LIGHTS > 0\n"
        
        "uniform vec3 hemisphereLightSkyColor[ MAX_HEMI_LIGHTS ];\n"
        "uniform vec3 hemisphereLightGroundColor[ MAX_HEMI_LIGHTS ];\n"
        "uniform vec3 hemisphereLightDirection[ MAX_HEMI_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "uniform vec3 pointLightColor[ MAX_POINT_LIGHTS ];\n"
        "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS ];\n"
        "uniform float pointLightDistance[ MAX_POINT_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "uniform vec3 spotLightColor[ MAX_SPOT_LIGHTS ];\n"
        "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS ];\n"
        "uniform vec3 spotLightDirection[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightDistance[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightAngleCos[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightExponent[ MAX_SPOT_LIGHTS ];\n"
        
		"#endif\n"
        
		"#ifdef WRAP_AROUND\n"
        
        "uniform vec3 wrapRGB;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lights_lambert_vertex() {
        return
        
		"vLightFront = vec3( 0.0 );\n"
        
		"#ifdef DOUBLE_SIDED\n"
        
        "vLightBack = vec3( 0.0 );\n"
        
		"#endif\n"
        
		"transformedNormal = normalize( transformedNormal );\n"
        
		"#if MAX_DIR_LIGHTS > 0\n"
        
		"for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {\n"
        
        "vec4 lDirection = viewMatrix * vec4( directionalLightDirection[ i ], 0.0 );\n"
        "vec3 dirVector = normalize( lDirection.xyz );\n"
        
        "float dotProduct = dot( transformedNormal, dirVector );\n"
        "vec3 directionalLightWeighting = vec3( max( dotProduct, 0.0 ) );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vec3 directionalLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 directionalLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 directionalLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );\n"
        "directionalLightWeighting = mix( directionalLightWeighting, directionalLightWeightingHalf, wrapRGB );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "directionalLightWeightingBack = mix( directionalLightWeightingBack, directionalLightWeightingHalfBack, wrapRGB );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "vLightFront += directionalLightColor[ i ] * directionalLightWeighting;\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vLightBack += directionalLightColor[ i ] * directionalLightWeightingBack;\n"
        
        "#endif\n"
        
		"}\n"
        
		"#endif\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {\n"
        
        "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz - mvPosition.xyz;\n"
        
        "float lDistance = 1.0;\n"
        "if ( pointLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i ] ), 1.0 );\n"
        
        "lVector = normalize( lVector );\n"
        "float dotProduct = dot( transformedNormal, lVector );\n"
        
        "vec3 pointLightWeighting = vec3( max( dotProduct, 0.0 ) );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vec3 pointLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 pointLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 pointLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );\n"
        "pointLightWeighting = mix( pointLightWeighting, pointLightWeightingHalf, wrapRGB );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "pointLightWeightingBack = mix( pointLightWeightingBack, pointLightWeightingHalfBack, wrapRGB );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "vLightFront += pointLightColor[ i ] * pointLightWeighting * lDistance;\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vLightBack += pointLightColor[ i ] * pointLightWeightingBack * lDistance;\n"
        
        "#endif\n"
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {\n"
        
        "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz - mvPosition.xyz;\n"
        
        "float spotEffect = dot( spotLightDirection[ i ], normalize( spotLightPosition[ i ] - worldPosition.xyz ) );\n"
        
        "if ( spotEffect > spotLightAngleCos[ i ] ) {\n"
        
        "spotEffect = max( pow( spotEffect, spotLightExponent[ i ] ), 0.0 );\n"
        
        "float lDistance = 1.0;\n"
        "if ( spotLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i ] ), 1.0 );\n"
        
        "lVector = normalize( lVector );\n"
        
        "float dotProduct = dot( transformedNormal, lVector );\n"
        "vec3 spotLightWeighting = vec3( max( dotProduct, 0.0 ) );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vec3 spotLightWeightingBack = vec3( max( -dotProduct, 0.0 ) );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 spotLightWeightingHalfBack = vec3( max( -0.5 * dotProduct + 0.5, 0.0 ) );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "vec3 spotLightWeightingHalf = vec3( max( 0.5 * dotProduct + 0.5, 0.0 ) );\n"
        "spotLightWeighting = mix( spotLightWeighting, spotLightWeightingHalf, wrapRGB );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "spotLightWeightingBack = mix( spotLightWeightingBack, spotLightWeightingHalfBack, wrapRGB );\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "vLightFront += spotLightColor[ i ] * spotLightWeighting * lDistance * spotEffect;\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vLightBack += spotLightColor[ i ] * spotLightWeightingBack * lDistance * spotEffect;\n"
        
        "#endif\n"
        
        "}\n"
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_HEMI_LIGHTS > 0\n"
        
        "for( int i = 0; i < MAX_HEMI_LIGHTS; i ++ ) {\n"
        
        "vec4 lDirection = viewMatrix * vec4( hemisphereLightDirection[ i ], 0.0 );\n"
        "vec3 lVector = normalize( lDirection.xyz );\n"
        
        "float dotProduct = dot( transformedNormal, lVector );\n"
        
        "float hemiDiffuseWeight = 0.5 * dotProduct + 0.5;\n"
        "float hemiDiffuseWeightBack = -0.5 * dotProduct + 0.5;\n"
        
        "vLightFront += mix( hemisphereLightGroundColor[ i ], hemisphereLightSkyColor[ i ], hemiDiffuseWeight );\n"
        
        "#ifdef DOUBLE_SIDED\n"
        
        "vLightBack += mix( hemisphereLightGroundColor[ i ], hemisphereLightSkyColor[ i ], hemiDiffuseWeightBack );\n"
        
        "#endif\n"
        
        "}\n"
        
		"#endif\n"
        
		"vLightFront = vLightFront * diffuse + ambient * ambientLightColor + emissive;\n"
        
		"#ifdef DOUBLE_SIDED\n"
        
        "vLightBack = vLightBack * diffuse + ambient * ambientLightColor + emissive;\n"
        
		"#endif\n";
	}
    
	// LIGHTS PHONG
    const char* ShaderChunk::lights_phong_pars_vertex() {
        return
        
		"#ifndef PHONG_PER_PIXEL\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS ];\n"
        "uniform float pointLightDistance[ MAX_POINT_LIGHTS ];\n"
        
        "varying vec4 vPointLight[ MAX_POINT_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightDistance[ MAX_SPOT_LIGHTS ];\n"
        
        "varying vec4 vSpotLight[ MAX_SPOT_LIGHTS ];\n"
        
		"#endif\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )\n"
        
        "varying vec3 vWorldPosition;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lights_phong_vertex() {
        return
        
		"#ifndef PHONG_PER_PIXEL\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "for( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {\n"
        
        "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz - mvPosition.xyz;\n"
        
        "float lDistance = 1.0;\n"
        "if ( pointLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i ] ), 1.0 );\n"
        
        "vPointLight[ i ] = vec4( lVector, lDistance );\n"
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "for( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {\n"
        
        "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz - mvPosition.xyz;\n"
        
        "float lDistance = 1.0;\n"
        "if ( spotLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i ] ), 1.0 );\n"
        
        "vSpotLight[ i ] = vec4( lVector, lDistance );\n"
        
        "}\n"
        
		"#endif\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )\n"
        
        "vWorldPosition = worldPosition.xyz;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::lights_phong_pars_fragment() {
        return
        
		"uniform vec3 ambientLightColor;\n"
        
		"#if MAX_DIR_LIGHTS > 0\n"
        
        "uniform vec3 directionalLightColor[ MAX_DIR_LIGHTS ];\n"
        "uniform vec3 directionalLightDirection[ MAX_DIR_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_HEMI_LIGHTS > 0\n"
        
        "uniform vec3 hemisphereLightSkyColor[ MAX_HEMI_LIGHTS ];\n"
        "uniform vec3 hemisphereLightGroundColor[ MAX_HEMI_LIGHTS ];\n"
        "uniform vec3 hemisphereLightDirection[ MAX_HEMI_LIGHTS ];\n"
        
		"#endif\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "uniform vec3 pointLightColor[ MAX_POINT_LIGHTS ];\n"
        
        "#ifdef PHONG_PER_PIXEL\n"
        
        "uniform vec3 pointLightPosition[ MAX_POINT_LIGHTS ];\n"
        "uniform float pointLightDistance[ MAX_POINT_LIGHTS ];\n"
        
        "#else\n"
        
        "varying vec4 vPointLight[ MAX_POINT_LIGHTS ];\n"
        
        "#endif\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "uniform vec3 spotLightColor[ MAX_SPOT_LIGHTS ];\n"
        "uniform vec3 spotLightPosition[ MAX_SPOT_LIGHTS ];\n"
        "uniform vec3 spotLightDirection[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightAngleCos[ MAX_SPOT_LIGHTS ];\n"
        "uniform float spotLightExponent[ MAX_SPOT_LIGHTS ];\n"
        
        "#ifdef PHONG_PER_PIXEL\n"
        
        "uniform float spotLightDistance[ MAX_SPOT_LIGHTS ];\n"
        
        "#else\n"
        
        "varying vec4 vSpotLight[ MAX_SPOT_LIGHTS ];\n"
        
        "#endif\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0 || defined( USE_BUMPMAP )\n"
        
        "varying vec3 vWorldPosition;\n"
        
		"#endif\n"
        
		"#ifdef WRAP_AROUND\n"
        
        "uniform vec3 wrapRGB;\n"
        
		"#endif\n"
        
		"varying vec3 vViewPosition;\n"
		"varying vec3 vNormal;";
        
	}
    const char* ShaderChunk::lights_phong_fragment() {
        return
        
		"vec3 normal = normalize( vNormal );\n"
		"vec3 viewPosition = normalize( vViewPosition );\n"
        
		"#ifdef DOUBLE_SIDED\n"
        
        "normal = normal * ( -1.0 + 2.0 * float( gl_FrontFacing ) );\n"
        
		"#endif\n"
        
		"#ifdef USE_NORMALMAP\n"
        
        "normal = perturbNormal2Arb( -vViewPosition, normal );\n"
        
		"#elif defined( USE_BUMPMAP )\n"
        
        "normal = perturbNormalArb( -vViewPosition, normal, dHdxy_fwd() );\n"
        
		"#endif\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "vec3 pointDiffuse  = vec3( 0.0 );\n"
        "vec3 pointSpecular = vec3( 0.0 );\n"
        
        "for ( int i = 0; i < MAX_POINT_LIGHTS; i ++ ) {\n"
        
        "#ifdef PHONG_PER_PIXEL\n"
        
        "vec4 lPosition = viewMatrix * vec4( pointLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz + vViewPosition.xyz;\n"
        
        "float lDistance = 1.0;\n"
        "if ( pointLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / pointLightDistance[ i ] ), 1.0 );\n"
        
        "lVector = normalize( lVector );\n"
        
        "#else\n"
        
        "vec3 lVector = normalize( vPointLight[ i ].xyz );\n"
        "float lDistance = vPointLight[ i ].w;\n"
        
        "#endif\n"
        
        "float dotProduct = dot( normal, lVector );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "float pointDiffuseWeightFull = max( dotProduct, 0.0 );\n"
        "float pointDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5, 0.0 );\n"
        
        "vec3 pointDiffuseWeight = mix( vec3 ( pointDiffuseWeightFull ), vec3( pointDiffuseWeightHalf ), wrapRGB );\n"
        
        "#else\n"
        
        "float pointDiffuseWeight = max( dotProduct, 0.0 );\n"
        
        "#endif\n"
        
        "pointDiffuse  += diffuse * pointLightColor[ i ] * pointDiffuseWeight * lDistance;\n"
        
        
        "vec3 pointHalfVector = normalize( lVector + viewPosition );\n"
        "float pointDotNormalHalf = max( dot( normal, pointHalfVector ), 0.0 );\n"
        "float pointSpecularWeight = specularStrength * max( pow( pointDotNormalHalf, shininess ), 0.0 );\n"
        
        
        "float specularNormalization = ( shininess + 2.0001 ) / 8.0;\n"
        
        "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVector, pointHalfVector ), 5.0 );\n"
        "pointSpecular += schlick * pointLightColor[ i ] * pointSpecularWeight * pointDiffuseWeight * lDistance * specularNormalization;\n"
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "vec3 spotDiffuse  = vec3( 0.0 );\n"
        "vec3 spotSpecular = vec3( 0.0 );\n"
        
        "for ( int i = 0; i < MAX_SPOT_LIGHTS; i ++ ) {\n"
        
        "#ifdef PHONG_PER_PIXEL\n"
        
        "vec4 lPosition = viewMatrix * vec4( spotLightPosition[ i ], 1.0 );\n"
        "vec3 lVector = lPosition.xyz + vViewPosition.xyz;\n"
        
        "float lDistance = 1.0;\n"
        "if ( spotLightDistance[ i ] > 0.0 )\n"
        "lDistance = 1.0 - min( ( length( lVector ) / spotLightDistance[ i ] ), 1.0 );\n"
        
        "lVector = normalize( lVector );\n"
        
        "#else\n"
        
        "vec3 lVector = normalize( vSpotLight[ i ].xyz );\n"
        "float lDistance = vSpotLight[ i ].w;\n"
        
        "#endif\n"
        
        "float spotEffect = dot( spotLightDirection[ i ], normalize( spotLightPosition[ i ] - vWorldPosition ) );\n"
        
        "if ( spotEffect > spotLightAngleCos[ i ] ) {\n"
        
        "spotEffect = max( pow( spotEffect, spotLightExponent[ i ] ), 0.0 );\n"
        
        
        "float dotProduct = dot( normal, lVector );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "float spotDiffuseWeightFull = max( dotProduct, 0.0 );\n"
        "float spotDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5, 0.0 );\n"
        
        "vec3 spotDiffuseWeight = mix( vec3 ( spotDiffuseWeightFull ), vec3( spotDiffuseWeightHalf ), wrapRGB );\n"
        
        "#else\n"
        
        "float spotDiffuseWeight = max( dotProduct, 0.0 );\n"
        
        "#endif\n"
        
        "spotDiffuse += diffuse * spotLightColor[ i ] * spotDiffuseWeight * lDistance * spotEffect;\n"
        
        
        "vec3 spotHalfVector = normalize( lVector + viewPosition );\n"
        "float spotDotNormalHalf = max( dot( normal, spotHalfVector ), 0.0 );\n"
        "float spotSpecularWeight = specularStrength * max( pow( spotDotNormalHalf, shininess ), 0.0 );\n"
        
        
        "float specularNormalization = ( shininess + 2.0001 ) / 8.0;\n"
        
        "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVector, spotHalfVector ), 5.0 );\n"
        "spotSpecular += schlick * spotLightColor[ i ] * spotSpecularWeight * spotDiffuseWeight * lDistance * specularNormalization * spotEffect;\n"
        
        "}\n"
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_DIR_LIGHTS > 0\n"
        
        "vec3 dirDiffuse  = vec3( 0.0 );\n"
        "vec3 dirSpecular = vec3( 0.0 );"
        
        "for( int i = 0; i < MAX_DIR_LIGHTS; i ++ ) {\n"
        
        "vec4 lDirection = viewMatrix * vec4( directionalLightDirection[ i ], 0.0 );\n"
        "vec3 dirVector = normalize( lDirection.xyz );\n"
    
        
        "float dotProduct = dot( normal, dirVector );\n"
        
        "#ifdef WRAP_AROUND\n"
        
        "float dirDiffuseWeightFull = max( dotProduct, 0.0 );\n"
        "float dirDiffuseWeightHalf = max( 0.5 * dotProduct + 0.5, 0.0 );\n"
        
        "vec3 dirDiffuseWeight = mix( vec3( dirDiffuseWeightFull ), vec3( dirDiffuseWeightHalf ), wrapRGB );\n"
        
        "#else\n"
        
        "float dirDiffuseWeight = max( dotProduct, 0.0 );\n"
        
        "#endif\n"
        
        "dirDiffuse  += diffuse * directionalLightColor[ i ] * dirDiffuseWeight;\n"
        
        
        "vec3 dirHalfVector = normalize( dirVector + viewPosition );\n"
        "float dirDotNormalHalf = max( dot( normal, dirHalfVector ), 0.0 );\n"
        "float dirSpecularWeight = specularStrength * max( pow( dirDotNormalHalf, shininess ), 0.0 );\n"
        
    
        
        "float specularNormalization = ( shininess + 2.0001 ) / 8.0;\n"
        
        
        "vec3 schlick = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( dirVector, dirHalfVector ), 5.0 );\n"
        "dirSpecular += schlick * directionalLightColor[ i ] * dirSpecularWeight * dirDiffuseWeight * specularNormalization;\n"
        
        
        "}\n"
        
		"#endif\n"
        
		"#if MAX_HEMI_LIGHTS > 0\n"
        
        "vec3 hemiDiffuse  = vec3( 0.0 );\n"
        "vec3 hemiSpecular = vec3( 0.0 );"
        
        "for( int i = 0; i < MAX_HEMI_LIGHTS; i ++ ) {\n"
        
        "vec4 lDirection = viewMatrix * vec4( hemisphereLightDirection[ i ], 0.0 );\n"
        "vec3 lVector = normalize( lDirection.xyz );\n"
        
        
        "float dotProduct = dot( normal, lVector );\n"
        "float hemiDiffuseWeight = 0.5 * dotProduct + 0.5;\n"
        
        "vec3 hemiColor = mix( hemisphereLightGroundColor[ i ], hemisphereLightSkyColor[ i ], hemiDiffuseWeight );\n"
        
        "hemiDiffuse += diffuse * hemiColor;\n"
        
        
        "vec3 hemiHalfVectorSky = normalize( lVector + viewPosition );\n"
        "float hemiDotNormalHalfSky = 0.5 * dot( normal, hemiHalfVectorSky ) + 0.5;\n"
        "float hemiSpecularWeightSky = specularStrength * max( pow( hemiDotNormalHalfSky, shininess ), 0.0 );\n"
        
        
        "vec3 lVectorGround = -lVector;\n"
        
        "vec3 hemiHalfVectorGround = normalize( lVectorGround + viewPosition );\n"
        "float hemiDotNormalHalfGround = 0.5 * dot( normal, hemiHalfVectorGround ) + 0.5;\n"
        "float hemiSpecularWeightGround = specularStrength * max( pow( hemiDotNormalHalfGround, shininess ), 0.0 );\n"
        
        "float dotProductGround = dot( normal, lVectorGround );\n"
        
        
        "float specularNormalization = ( shininess + 2.0001 ) / 8.0;\n"
        
        "vec3 schlickSky = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVector, hemiHalfVectorSky ), 5.0 );\n"
        "vec3 schlickGround = specular + vec3( 1.0 - specular ) * pow( 1.0 - dot( lVectorGround, hemiHalfVectorGround ), 5.0 );\n"
        "hemiSpecular += hemiColor * specularNormalization * ( schlickSky * hemiSpecularWeightSky * max( dotProduct, 0.0 ) + schlickGround * hemiSpecularWeightGround * max( dotProductGround, 0.0 ) );\n"
        
        "}\n"
        
		"#endif\n"
        
		"vec3 totalDiffuse = vec3( 0.0 );\n"
		"vec3 totalSpecular = vec3( 0.0 );\n"
        
		"#if MAX_DIR_LIGHTS > 0\n"
        
        "totalDiffuse += dirDiffuse;\n"
        "totalSpecular += dirSpecular;\n"
        
		"#endif\n"
        
		"#if MAX_HEMI_LIGHTS > 0\n"
        
        "totalDiffuse += hemiDiffuse;\n"
        "totalSpecular += hemiSpecular;\n"
        
		"#endif\n"
        
		"#if MAX_POINT_LIGHTS > 0\n"
        
        "totalDiffuse += pointDiffuse;\n"
        "totalSpecular += pointSpecular;\n"
        
		"#endif\n"
        
		"#if MAX_SPOT_LIGHTS > 0\n"
        
        "totalDiffuse += spotDiffuse;\n"
        "totalSpecular += spotSpecular;\n"
        
		"#endif\n"
        
		"#ifdef METAL\n"
        
        "gl_FragColor.xyz = gl_FragColor.xyz * ( emissive + totalDiffuse + ambientLightColor * ambient + totalSpecular );\n"
        
		"#else\n"
        
        "gl_FragColor.xyz = gl_FragColor.xyz * ( emissive + totalDiffuse + ambientLightColor * ambient ) + totalSpecular;\n"
        
		"#endif\n";
	}
    
	// VERTEX COLORS
    const char* ShaderChunk::color_pars_fragment() {
        return
        
		"#ifdef USE_COLOR\n"
        
        "varying vec3 vColor;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::color_fragment() {
        return
        
		"#ifdef USE_COLOR\n"
        
        "gl_FragColor = gl_FragColor * vec4( vColor, 1.0 );\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::color_pars_vertex() {
        return
        
		"#ifdef USE_COLOR\n"
        
        "varying vec3 vColor;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::color_vertex() {
        return
        
		"#ifdef USE_COLOR\n"
        
        "#ifdef GAMMA_INPUT\n"
        
        "vColor = color * color;\n"
        
        "#else\n"
        
        "vColor = color;\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    
	// SKINNING
    const char* ShaderChunk::skinning_pars_vertex() {
        return
        
		"#ifdef USE_SKINNING\n"
        
        "#ifdef BONE_TEXTURE\n"
        
        "uniform sampler2D boneTexture;\n"
        "uniform int boneTextureWidth;\n"
        "uniform int boneTextureHeight;\n"
        
        "mat4 getBoneMatrix( const in float i ) {\n"
        
        "float j = i * 4.0;\n"
        "float x = mod( j, float( boneTextureWidth ) );\n"
        "float y = floor( j / float( boneTextureWidth ) );\n"
        
        "float dx = 1.0 / float( boneTextureWidth );\n"
        "float dy = 1.0 / float( boneTextureHeight );\n"
        
        "y = dy * ( y + 0.5 );\n"
        
        "vec4 v1 = texture2D( boneTexture, vec2( dx * ( x + 0.5 ), y ) );\n"
        "vec4 v2 = texture2D( boneTexture, vec2( dx * ( x + 1.5 ), y ) );\n"
        "vec4 v3 = texture2D( boneTexture, vec2( dx * ( x + 2.5 ), y ) );\n"
        "vec4 v4 = texture2D( boneTexture, vec2( dx * ( x + 3.5 ), y ) );\n"
        
        "mat4 bone = mat4( v1, v2, v3, v4 );\n"
        
        "return bone;\n"
        
        "}\n"
        
        "#else\n"
        
        "uniform mat4 boneGlobalMatrices[ MAX_BONES ];\n"
        
        "mat4 getBoneMatrix( const in float i ) {\n"
        
        "mat4 bone = boneGlobalMatrices[ int(i) ];\n"
        "return bone;\n"
        
        "}\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::skinbase_vertex() {
        return
        
		"#ifdef USE_SKINNING\n"
        
        "mat4 boneMatX = getBoneMatrix( skinIndex.x );\n"
        "mat4 boneMatY = getBoneMatrix( skinIndex.y );\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::skinning_vertex() {
        return
        
		"#ifdef USE_SKINNING\n"
        
        "#ifdef USE_MORPHTARGETS\n"
        
        "vec4 skinVertex = vec4( morphed, 1.0 );\n"
        
        "#else\n"
        
        "vec4 skinVertex = vec4( position, 1.0 );\n"
        
        "#endif\n"
        
        "vec4 skinned  = boneMatX * skinVertex * skinWeight.x;\n"
        "skinned 	  += boneMatY * skinVertex * skinWeight.y;\n"
        
		"#endif\n";
	}
    
	// MORPHING
    const char* ShaderChunk::morphtarget_pars_vertex() {
        return
        
		"#ifdef USE_MORPHTARGETS\n"
        
        "#ifndef USE_MORPHNORMALS\n"
        
        "uniform float morphTargetInfluences[ 8 ];\n"
        
        "#else\n"
        
        "uniform float morphTargetInfluences[ 4 ];\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::morphtarget_vertex() {
        return
        
		"#ifdef USE_MORPHTARGETS\n"
        
        "vec3 morphed = vec3( 0.0 );\n"
        "morphed += ( morphTarget0 - position ) * morphTargetInfluences[ 0 ];\n"
        "morphed += ( morphTarget1 - position ) * morphTargetInfluences[ 1 ];\n"
        "morphed += ( morphTarget2 - position ) * morphTargetInfluences[ 2 ];\n"
        "morphed += ( morphTarget3 - position ) * morphTargetInfluences[ 3 ];\n"
        
        "#ifndef USE_MORPHNORMALS\n"
        
        "morphed += ( morphTarget4 - position ) * morphTargetInfluences[ 4 ];\n"
        "morphed += ( morphTarget5 - position ) * morphTargetInfluences[ 5 ];\n"
        "morphed += ( morphTarget6 - position ) * morphTargetInfluences[ 6 ];\n"
        "morphed += ( morphTarget7 - position ) * morphTargetInfluences[ 7 ];\n"
        
        "#endif\n"
        
        "morphed += position;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::default_vertex () {
        return
        
		"vec4 mvPosition;\n"
        
		"#ifdef USE_SKINNING\n"
        
        "mvPosition = modelViewMatrix * skinned;\n"
        
		"#endif\n"
        
		"#if !defined( USE_SKINNING ) && defined( USE_MORPHTARGETS )\n"
        
        "mvPosition = modelViewMatrix * vec4( morphed, 1.0 );\n"
        
		"#endif\n"
        
		"#if !defined( USE_SKINNING ) && ! defined( USE_MORPHTARGETS )\n"
        
        "mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
        
		"#endif\n"
        
		"gl_Position = projectionMatrix * mvPosition;";
        
	}
    const char* ShaderChunk::morphnormal_vertex() {
        return
        
		"#ifdef USE_MORPHNORMALS\n"
        
        "vec3 morphedNormal = vec3( 0.0 );\n"
        
        "morphedNormal +=  ( morphNormal0 - normal ) * morphTargetInfluences[ 0 ];\n"
        "morphedNormal +=  ( morphNormal1 - normal ) * morphTargetInfluences[ 1 ];\n"
        "morphedNormal +=  ( morphNormal2 - normal ) * morphTargetInfluences[ 2 ];\n"
        "morphedNormal +=  ( morphNormal3 - normal ) * morphTargetInfluences[ 3 ];\n"
        
        "morphedNormal += normal;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::skinnormal_vertex() {
        return
        
		"#ifdef USE_SKINNING\n"
        
        "mat4 skinMatrix = skinWeight.x * boneMatX;\n"
        "skinMatrix 	+= skinWeight.y * boneMatY;\n"
        
        "#ifdef USE_MORPHNORMALS\n"
        
        "vec4 skinnedNormal = skinMatrix * vec4( morphedNormal, 0.0 );\n"
        
        "#else\n"
        
        "vec4 skinnedNormal = skinMatrix * vec4( normal, 0.0 );\n"
        
        "#endif\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::defaultnormal_vertex() {
        return
        
		"vec3 objectNormal;\n"
        
		"#ifdef USE_SKINNING\n"
        
        "objectNormal = skinnedNormal.xyz;\n"
        
		"#endif\n"
        
		"#if !defined( USE_SKINNING ) && defined( USE_MORPHNORMALS )\n"
        
        "objectNormal = morphedNormal;\n"
        
		"#endif\n"
        
		"#if !defined( USE_SKINNING ) && ! defined( USE_MORPHNORMALS )\n"
        
        "objectNormal = normal;\n"
        
		"#endif\n"
        
		"#ifdef FLIP_SIDED\n"
        
        "objectNormal = -objectNormal;\n"
        
		"#endif\n"
        
		"vec3 transformedNormal = normalMatrix * objectNormal;";
        
	}
    
	// SHADOW MAP
    
	// based on SpiderGL shadow map and Fabien Sanglard's GLSL shadow mapping examples
	//  http://spidergl.org/example.php?id=6
	// 	http://fabiensanglard.net/shadowmapping
    const char* ShaderChunk::shadowmap_pars_fragment() {
        return
        
		"#ifdef USE_SHADOWMAP\n"
        
        "uniform sampler2D shadowMap[ MAX_SHADOWS ];\n"
        "uniform vec2 shadowMapSize[ MAX_SHADOWS ];\n"
        
        "uniform float shadowDarkness[ MAX_SHADOWS ];\n"
        "uniform float shadowBias[ MAX_SHADOWS ];\n"
        
        "varying vec4 vShadowCoord[ MAX_SHADOWS ];\n"
        
        "float unpackDepth( const in vec4 rgba_depth ) {\n"
        
        "const vec4 bit_shift = vec4( 1.0 / ( 256.0 * 256.0 * 256.0 ), 1.0 / ( 256.0 * 256.0 ), 1.0 / 256.0, 1.0 );\n"
        "float depth = dot( rgba_depth, bit_shift );\n"
        "return depth;\n"
        
        "}\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::shadowmap_fragment() {
        return
        
		"#ifdef USE_SHADOWMAP\n"
        
        "#ifdef SHADOWMAP_DEBUG\n"
        
        "vec3 frustumColors[3];\n"
        "frustumColors[0] = vec3( 1.0, 0.5, 0.0 );\n"
        "frustumColors[1] = vec3( 0.0, 1.0, 0.8 );\n"
        "frustumColors[2] = vec3( 0.0, 0.5, 1.0 );\n"
        
        "#endif\n"
        
        "#ifdef SHADOWMAP_CASCADE\n"
        
        "int inFrustumCount = 0;\n"
        
        "#endif\n"
        
        "float fDepth;\n"
        "vec3 shadowColor = vec3( 1.0 );\n"
        
        "for( int i = 0; i < MAX_SHADOWS; i ++ ) {\n"
        
        "vec3 shadowCoord = vShadowCoord[ i ].xyz / vShadowCoord[ i ].w;\n"
        
        // "if ( something && something )" 		 breaks ATI OpenGL shader compiler
        // "if ( all( something, something ) )"  using this instead
        
        "bvec4 inFrustumVec = bvec4 ( shadowCoord.x >= 0.0, shadowCoord.x <= 1.0, shadowCoord.y >= 0.0, shadowCoord.y <= 1.0 );\n"
        "bool inFrustum = all( inFrustumVec );\n"
        
        // don't shadow pixels outside of light frustum
        // use just first frustum (for cascades)
        // don't shadow pixels behind far plane of light frustum
        
        "#ifdef SHADOWMAP_CASCADE\n"
        
        "inFrustumCount += int( inFrustum );\n"
        "bvec3 frustumTestVec = bvec3( inFrustum, inFrustumCount == 1, shadowCoord.z <= 1.0 );\n"
        
        "#else\n"
        
        "bvec2 frustumTestVec = bvec2( inFrustum, shadowCoord.z <= 1.0 );\n"
        
        "#endif\n"
        
        "bool frustumTest = all( frustumTestVec );\n"
        
        "if ( frustumTest ) {\n"
        
        "shadowCoord.z += shadowBias[ i ];\n"
        
        "#if defined( SHADOWMAP_TYPE_PCF )\n"
        
        // Percentage-close filtering
        // (9 pixel kernel)
        // http://fabiensanglard.net/shadowmappingPCF/
        
        "float shadow = 0.0;\n"
        
        /*
         // nested loops breaks shader compiler / validator on some ATI cards when using OpenGL
         // must enroll loop manually
         
         "for ( float y = -1.25; y <= 1.25; y += 1.25 )\n"
         "for ( float x = -1.25; x <= 1.25; x += 1.25 ) {\n"
         
         "vec4 rgbaDepth = texture2D( shadowMap[ i ], vec2( x * xPixelOffset, y * yPixelOffset ) + shadowCoord.xy );\n"
         
         // doesn't seem to produce any noticeable visual difference compared to simple "texture2D" lookup
         //"vec4 rgbaDepth = texture2DProj( shadowMap[ i ], vec4( vShadowCoord[ i ].w * ( vec2( x * xPixelOffset, y * yPixelOffset ) + shadowCoord.xy ), 0.05, vShadowCoord[ i ].w ) );\n"
         
         "float fDepth = unpackDepth( rgbaDepth );\n"
         
         "if ( fDepth < shadowCoord.z )\n"
         "shadow += 1.0;\n"
         
         "}\n"
         
         "shadow /= 9.0;\n"
         
         */
        
        "const float shadowDelta = 1.0 / 9.0;\n"
        
        "float xPixelOffset = 1.0 / shadowMapSize[ i ].x;\n"
        "float yPixelOffset = 1.0 / shadowMapSize[ i ].y;\n"
        
        "float dx0 = -1.25 * xPixelOffset;\n"
        "float dy0 = -1.25 * yPixelOffset;\n"
        "float dx1 = 1.25 * xPixelOffset;\n"
        "float dy1 = 1.25 * yPixelOffset;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy0 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy0 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy0 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, 0.0 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, 0.0 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy1 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy1 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "fDepth = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy1 ) ) );\n"
        "if ( fDepth < shadowCoord.z ) shadow += shadowDelta;\n"
        
        "shadowColor = shadowColor * vec3( ( 1.0 - shadowDarkness[ i ] * shadow ) );\n"
        
        "#elif defined( SHADOWMAP_TYPE_PCF_SOFT )\n"
        
        // Percentage-close filtering
        // (9 pixel kernel)
        // http://fabiensanglard.net/shadowmappingPCF/
        
        "float shadow = 0.0;\n"
        
        "float xPixelOffset = 1.0 / shadowMapSize[ i ].x;\n"
        "float yPixelOffset = 1.0 / shadowMapSize[ i ].y;\n"
        
        "float dx0 = -1.0 * xPixelOffset;\n"
        "float dy0 = -1.0 * yPixelOffset;\n"
        "float dx1 = 1.0 * xPixelOffset;\n"
        "float dy1 = 1.0 * yPixelOffset;\n"
        
        "mat3 shadowKernel;\n"
        "mat3 depthKernel;\n"
        
        "depthKernel[0][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy0 ) ) );\n"
        "depthKernel[0][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, 0.0 ) ) );\n"
        "depthKernel[0][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx0, dy1 ) ) );\n"
        "depthKernel[1][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy0 ) ) );\n"
        "depthKernel[1][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy ) );\n"
        "depthKernel[1][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( 0.0, dy1 ) ) );\n"
        "depthKernel[2][0] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy0 ) ) );\n"
        "depthKernel[2][1] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, 0.0 ) ) );\n"
        "depthKernel[2][2] = unpackDepth( texture2D( shadowMap[ i ], shadowCoord.xy + vec2( dx1, dy1 ) ) );\n"
        
        "vec3 shadowZ = vec3( shadowCoord.z );\n"
        "shadowKernel[0] = vec3(lessThan(depthKernel[0], shadowZ ));\n"
        "shadowKernel[0] *= vec3(0.25);\n"
        
        "shadowKernel[1] = vec3(lessThan(depthKernel[1], shadowZ ));\n"
        "shadowKernel[1] *= vec3(0.25);\n"
        
        "shadowKernel[2] = vec3(lessThan(depthKernel[2], shadowZ ));\n"
        "shadowKernel[2] *= vec3(0.25);\n"
        
        "vec2 fractionalCoord = 1.0 - fract( shadowCoord.xy * shadowMapSize[i].xy );\n"
        
        "shadowKernel[0] = mix( shadowKernel[1], shadowKernel[0], fractionalCoord.x );\n"
        "shadowKernel[1] = mix( shadowKernel[2], shadowKernel[1], fractionalCoord.x );\n"
        
        "vec4 shadowValues;\n"
        "shadowValues.x = mix( shadowKernel[0][1], shadowKernel[0][0], fractionalCoord.y );\n"
        "shadowValues.y = mix( shadowKernel[0][2], shadowKernel[0][1], fractionalCoord.y );\n"
        "shadowValues.z = mix( shadowKernel[1][1], shadowKernel[1][0], fractionalCoord.y );\n"
        "shadowValues.w = mix( shadowKernel[1][2], shadowKernel[1][1], fractionalCoord.y );\n"
        
        "shadow = dot( shadowValues, vec4( 1.0 ) );\n"
        
        "shadowColor = shadowColor * vec3( ( 1.0 - shadowDarkness[ i ] * shadow ) );\n"
        
        "#else\n"
        
        "vec4 rgbaDepth = texture2D( shadowMap[ i ], shadowCoord.xy );\n"
        "float fDepth = unpackDepth( rgbaDepth );\n"
        
        "if ( fDepth < shadowCoord.z )\n"
        
        // spot with multiple shadows is darker
        
        "shadowColor = shadowColor * vec3( 1.0 - shadowDarkness[ i ] );\n"
        
        // spot with multiple shadows has the same color as single shadow spot
        
        //"shadowColor = min( shadowColor, vec3( shadowDarkness[ i ] ) );\n"
        
        "#endif\n"
        
        "}\n"
        
        
        "#ifdef SHADOWMAP_DEBUG\n"
        
        "#ifdef SHADOWMAP_CASCADE\n"
        
        "if ( inFrustum && inFrustumCount == 1 ) gl_FragColor.xyz *= frustumColors[ i ];\n"
        
        "#else\n"
        
        "if ( inFrustum ) gl_FragColor.xyz *= frustumColors[ i ];\n"
        
        "#endif\n"
        
        "#endif\n"
        
        "}\n"
        
        "#ifdef GAMMA_OUTPUT\n"
        
        "shadowColor *= shadowColor;\n"
        
        "#endif\n"
        
        "gl_FragColor.xyz = gl_FragColor.xyz * shadowColor;\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::shadowmap_pars_vertex() {
        return
        
		"#ifdef USE_SHADOWMAP\n"
        
        "varying vec4 vShadowCoord[ MAX_SHADOWS ];\n"
        "uniform mat4 shadowMatrix[ MAX_SHADOWS ];\n"
        
		"#endif\n";
	}
    const char* ShaderChunk::shadowmap_vertex() {
        return
        
		"#ifdef USE_SHADOWMAP\n"
        
        "for( int i = 0; i < MAX_SHADOWS; i ++ ) {\n"
        
        "vShadowCoord[ i ] = shadowMatrix[ i ] * worldPosition;\n"
        
        "}\n"
        
		"#endif\n";
	}
    
	// ALPHATEST
    const char* ShaderChunk::alphatest_fragment() {
        return
        
		"#ifdef ALPHATEST\n"
        
        "if ( gl_FragColor.a < ALPHATEST ) discard;\n"
        
		"#endif\n";
	}
    
	// LINEAR SPACE
    const char* ShaderChunk::linear_to_gamma_fragment() {
        return
        
		"#ifdef GAMMA_OUTPUT\n"
        
        "gl_FragColor.xyz = sqrt( gl_FragColor.xyz );\n"
        
		"#endif\n";
	}
    
    


} // namespace three

#endif // THREE_SHADERS_SHADER_CHUNK_CPP