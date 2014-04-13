#include <three/extras/stats.h>

#include <three/console.h>
#include <three/cameras/orthographic_camera.h>
#include <three/materials/text_2d_material.h>
#include <three/objects/mesh.h>
#include <three/renderers/gl_renderer.h>
#include <three/utils/conversion.h>

#include <three/extras/geometries/text_2d_geometry.h>

#include <sstream>

namespace three {
namespace stats {

struct Stats::Impl : public NonCopyable {

  Impl( GLRenderer& renderer, float reportInterval = 1.f )
    : renderer(renderer),
      font( Font::create( threeDataPath( "fonts/consolas.ttf" ) ) ),
      material( Text2DMaterial::create( *font, Color( 0x11ff11 ) ) ),
      camera( OrthographicCamera::create( -1.f, 1.f, 1.f, -1.f, -1.f, 1.f ) ),
      scene( Scene::create() ),
      framesSinceReport( 0 ),
      fps( 60 ),
      reportInterval( reportInterval ),
      currentTime( 0 ),
      nextReportTime( currentTime + reportInterval ) {

  }

  GLRenderer& renderer;
  Font::Ptr font;
  Text2DMaterial::Ptr material;
  OrthographicCamera::Ptr camera;
  Scene::Ptr scene;
  Mesh::Ptr mesh;

  int framesSinceReport, fps;
  float reportInterval, currentTime, nextReportTime;

};

Stats::Stats( GLRenderer& renderer ) : impl( new Impl( renderer ) ) { }

Stats::~Stats() { }

void Stats::update( float deltaTime, bool render ) {

  auto& s = *impl;

  s.currentTime += deltaTime;

  if ( s.font ) {

    auto& renderer = impl->renderer;

    ++s.framesSinceReport;
    if ( s.currentTime > s.nextReportTime ) {
      s.nextReportTime = s.currentTime + s.reportInterval;
      s.fps = s.framesSinceReport;
      s.framesSinceReport = 0;
    }

    std::stringstream ss;
    ss << "FPS: " << s.fps;

    if ( render ) {

      // Remove any existing text geometry
      if ( s.framesSinceReport == 0 && impl->mesh ) {
        impl->scene->remove( impl->mesh );
        renderer.deallocateObject( *impl->mesh );
        impl->mesh.reset();
      }

      auto mesh = impl->mesh;
      if ( !mesh ) {
        auto geometry = Text2DGeometry::create( ss.str(), impl->font );
        mesh = impl->mesh = Mesh::create( geometry, impl->material );
        impl->scene->add( mesh );
      }

      mesh->position.x = 10.f;
      mesh->position.y = renderer.height() - 30.f;

      impl->camera->left   = 0;
      impl->camera->right  = (float)renderer.width();
      impl->camera->top    = (float)renderer.height();
      impl->camera->bottom = 0;
      impl->camera->updateProjectionMatrix();

      bool oldAutoClear = renderer.autoClear;
      renderer.autoClear = false;
      renderer.render( *impl->scene, *impl->camera );
      renderer.autoClear = oldAutoClear;

    } else if (s.framesSinceReport == 0) {

      console().log(ss.str().c_str());

    }

  }

}

} // namespace stats
} // namespace three
