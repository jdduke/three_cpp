#ifndef THREE_STATS_IPP
#define THREE_STATS_IPP

#include <three/extras/stats.hpp>

#include <three/utils.hpp>
#include <three/renderers/gl_renderer.hpp>
#include <three/extras/utils/font.hpp>

#include <sstream>

namespace three {
namespace stats {

using utils::Font;

struct Stats::Impl : public NonCopyable {

  Impl( float reportInterval = 1.f )
    : font( Font::create( threeDataPath( "fonts/consolas.ttf" ) ) ),
      framesSinceReport( 0 ),
      fps( 60 ),
      reportInterval( reportInterval ),
      currentTime( 0 ),
      nextReportTime( currentTime + reportInterval ) { }

  Font::Ptr font;
  int framesSinceReport, fps;
  float reportInterval, currentTime, nextReportTime;

};

/////////////////////////////////////////////////////////////////////////

Stats::Stats() : impl( new Impl() ) { }

Stats::~Stats() { }

void Stats::update( float deltaTime, GLRenderer& renderer ) {

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

    if (true) {
      if (s.framesSinceReport == 0)
        console().log(ss.str().c_str());
    }
    else {
      s.font->render( ss.str().c_str(),
        10.f, ( float )renderer.height() - 30.f,
        ( float )renderer.width(),
        ( float )renderer.height(),
        Color( 0x00FF00 ) );
    }

    renderer.resetStates();

  }

}

} // namespace stats
} // namespace three

#endif // THREE_STATS_HPP