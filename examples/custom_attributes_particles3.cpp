#include "common.h"

#include <three/core/geometry.h>
#include <three/cameras/perspective_camera.h>
#include <three/extras/image_utils.h>
#include <three/objects/mesh.h>
#include <three/materials/shader_material.h>
#include <three/materials/mesh_face_material.h>
#include <three/objects/particle_system.h>
#include <three/renderers/renderer_parameters.h>
#include <three/renderers/gl_renderer.h>

#include <three/extras/geometries/cube_geometry.h>
#include <three/extras/geometries/sphere_geometry.h>
#include <three/extras/geometry_utils.h>

const std::string vertexShader =
"attribute float size;\n"
"attribute vec4 ca;\n"
"varying vec4 vColor;\n"
"void main() {\n"
"  vColor = ca;\n"
"  vec4 mvPosition = modelViewMatrix * vec4( position, 1.0 );\n"
"  gl_PointSize = size * ( 150.0 / length( mvPosition.xyz ) );\n"
"  gl_Position = projectionMatrix * mvPosition;\n"
"}\n";

const std::string fragmentShader =
"uniform vec3 color;\n"
"uniform sampler2D texture;\n"
"varying vec4 vColor;\n"
"void main() {\n"
"  vec4 outColor = texture2D( texture, gl_PointCoord );\n"
"  if ( outColor.a < 0.5 ) discard;\n"
"  gl_FragColor = outColor * vec4( color * vColor.xyz, 1.0 );\n"
"  float depth = gl_FragCoord.z / gl_FragCoord.w;\n"
"  const vec3 fogColor = vec3( 0.0 );\n"
"  float fogFactor = smoothstep( 200.0, 600.0, depth );\n"
"  gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );\n"
"}\n";

using namespace three;

void custom_attributes_particles3( GLRenderer::Ptr renderer ) {

 auto camera = PerspectiveCamera::create(
    40, ( float )renderer->width() / renderer->height(), 1, 1000
  );
  camera->position.z = 500;

  auto scene = Scene::create();

  auto texture = ImageUtils::loadTexture( threeDataPath( "textures/sprites/ball.png" ) );
  texture->wrapS = texture->wrapT = enums::RepeatWrapping;

  Attributes attributes;
  attributes[ "size" ] = Attribute( enums::f );
  attributes[ "ca" ]   = Attribute( enums::c );

  Uniforms uniforms;
  uniforms[ "color" ]      = Uniform( enums::c, Color( 0xffffff ) );
  uniforms[ "texture" ]    = Uniform( enums::t, texture.get() );

  auto shaderMaterial = ShaderMaterial::create(
    vertexShader,
    fragmentShader,
    uniforms,
    attributes
  );

  // Geometries
  auto geometry = Geometry::create();

  geometry->vertices.reserve( 100000 );

  const auto radius = 100.f, inner = 0.6f * radius;
  for ( auto i = 0; i < 100000; i ++ ) {

    Vector3 vertex( Math::random( -1.f, 1.f ),
                    Math::random( -1.f, 1.f ),
                    Math::random( -1.f, 1.f ) );
    vertex.multiplyScalar( radius );

    if ( ( vertex.x > inner || vertex.x < -inner ) ||
         ( vertex.y > inner || vertex.y < -inner ) ||
         ( vertex.z > inner || vertex.z < -inner )  )

      geometry->vertices.push_back( vertex );

  }
  const auto vc1 = geometry->vertices.size();

  // geometry 2

  auto dummyMaterial = MeshFaceMaterial::create();

  const auto radius2 = 200.f;
  auto geometry2 = CubeGeometry::create( radius2, 0.1f * radius2, 0.1f * radius2, 50, 5, 5 );

  auto addGeo = [&]( const Geometry::Ptr& geo, float x, float y, float z, float ry ) {
    auto m = Mesh::create( geo, dummyMaterial );
    m->position.set( x, y, z );
    m->rotation().y( ry );

    GeometryUtils::merge( *geometry, *m );
  };

  // side 1

  addGeo( geometry2, 0,  110,  110, 0 );
  addGeo( geometry2, 0,  110, -110, 0 );
  addGeo( geometry2, 0, -110,  110, 0 );
  addGeo( geometry2, 0, -110, -110, 0 );

  // side 2

  addGeo( geometry2,  110,  110, 0, Math::PI()/2 );
  addGeo( geometry2,  110, -110, 0, Math::PI()/2 );
  addGeo( geometry2, -110,  110, 0, Math::PI()/2 );
  addGeo( geometry2, -110, -110, 0, Math::PI()/2 );

  // corner edges

  auto geometry3 = CubeGeometry::create( 0.1f * radius2, radius2 * 1.2f, 0.1f * radius2, 5, 60, 5 );

  addGeo( geometry3,  110, 0,  110, 0 );
  addGeo( geometry3,  110, 0, -110, 0 );
  addGeo( geometry3, -110, 0,  110, 0 );
  addGeo( geometry3, -110, 0, -110, 0 );

  // particle system

  auto object = ParticleSystem::create( geometry, shaderMaterial );
  object->geometry->dynamic = true;

  const auto& vertices = object->geometry->vertices;
  const auto vertexCount = vertices.size();

  std::vector<float> valuesSize( vertexCount );
  std::vector<Color> valuesColor( vertexCount );

  for ( size_t v = 0; v < vertexCount; v++ ) {

    valuesColor[ v ] = Color( 0xffffff );

    if ( v < vc1 ) {
      valuesSize[ v ] = 10;
      valuesColor[ v ].setHSL( 0.5f  + 0.2f * ( (float)v / vc1 ),
                               1.f,
                               0.5f );
    } else {
      valuesSize[ v ] = 55;
      valuesColor[ v ].setHSL( 0.1f,
                               1.f,
                               0.5f );
    }

  }

  auto& size  = shaderMaterial->attributes[ "size" ];
  auto& color = shaderMaterial->attributes[ "ca" ];

  size.value  = std::move(valuesSize);
  color.value = std::move(valuesColor);

  scene->add( object );

  /////////////////////////////////////////////////////////////////////////

  auto running = true, renderStats = true;
  sdl::addEventListener( SDL_KEYDOWN, [&]( const sdl::Event& e ) {
    switch (e.key.keysym.sym) {
    case SDLK_q:
    case SDLK_ESCAPE:
      running = false; break;
    default:
      renderStats = !renderStats; break;
    };
  } );

  sdl::addEventListener( SDL_QUIT, [&]( const sdl::Event& ) {
    running = false;
  } );

  sdl::addEventListener( SDL_VIDEORESIZE, [&]( const sdl::Event event ) {
    camera->aspect = ( float )event.resize.w / event.resize.h;
    camera->updateProjectionMatrix();
    renderer->setSize( event.resize.w, event.resize.h );
  } );

  /////////////////////////////////////////////////////////////////////////

  stats::Stats stats( *renderer );

  auto time = 0.f;

  anim::gameLoop( [&]( float dt ) -> bool {

    time += dt;
    object->rotation().set( 0, 0.02f * time, 0.02f * time );

    auto& sizes = size.value.cast<std::vector<float>>();
    for( size_t i = 0; i < sizes.size(); i++ ) {
      if ( i < vc1 )
        sizes[ i ] = 26.f + 32.f * Math::sin( 0.1f * i + time * 3.f );
    }
    size.needsUpdate = true;

    renderer->render( *scene, *camera );

    stats.update( dt, renderStats );

    return running;

  }, 2000 );

}

int main( int argc, char* argv[] ) {

  RendererParameters parameters;
  parameters.clearColor = Color( 0x000000 );
  parameters.clearAlpha = 1.f;
  parameters.vsync = false;

  RunExample( custom_attributes_particles3, parameters );

  return 0;
}
