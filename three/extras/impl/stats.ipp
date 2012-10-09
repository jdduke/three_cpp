#ifndef THREE_STATS_IPP
#define THREE_STATS_IPP

#include <three/extras/stats.hpp>

#include <three/cameras/orthographic_camera.hpp>
#include <three/materials/text_2d_material.hpp>
#include <three/objects/mesh.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/utils/conversion.hpp>

#include <three/extras/geometries/text_2d_geometry.hpp>
#include <three/extras/utils/font2.hpp>

#include <sstream>

namespace three {
namespace stats {

struct Stats::Impl : public NonCopyable {

  Impl( float reportInterval = 1.f )
    : show( true ),
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

  bool show;
  Font::Ptr font;
  Text2DMaterial::Ptr material;
  OrthographicCamera::Ptr camera;
  Scene::Ptr scene;

  int framesSinceReport, fps;
  float reportInterval, currentTime, nextReportTime;

};

/////////////////////////////////////////////////////////////////////////

Stats::Stats() : impl( new Impl() ) { }

Stats::~Stats() { }

void Stats::updateImpl( float deltaTime, GLRenderer* renderer ) {

  auto& s = *impl;

  s.currentTime += deltaTime;

  if ( s.font ) {

    ++s.framesSinceReport;
    if ( s.currentTime > s.nextReportTime ) {
      s.nextReportTime = s.currentTime + s.reportInterval;
      s.fps = s.framesSinceReport;
      s.framesSinceReport = 0;
   }

    std::stringstream ss; ss << "FPS: " << s.fps;

    if ( renderer ) {

      auto geometry = Text2DGeometry::create( ss.str(), impl->font );
      auto mesh     = Mesh::create( geometry, impl->material );
      mesh->position.x = 10.f;
      mesh->position.y = renderer->height() - 30.f;

      impl->camera->left   = 0; 
      impl->camera->right  = (float)renderer->width(); 
      impl->camera->top    = (float)renderer->height(); 
      impl->camera->bottom = 0;
      impl->camera->updateProjectionMatrix();

      bool oldAutoClear = renderer->autoClear;
      renderer->autoClear = false;

      impl->scene->add( mesh );

      renderer->render( *impl->scene, *impl->camera );
      renderer->autoClear = oldAutoClear;

      impl->scene->remove( mesh );
      renderer->deallocateObject( *mesh );

    } else if (s.framesSinceReport == 0) {

      console().log(ss.str().c_str());

    }

  }

}

} // namespace stats
} // namespace three

#endif // THREE_STATS_HPP