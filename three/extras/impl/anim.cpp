#ifndef THREE_ANIM_CPP
#define THREE_ANIM_CPP

#include <three/extras/anim.h>

#include <three/core/clock.h>
#include <three/utils/noncopyable.h>

#include <three/extras/sdl.h>

#include <thread>

namespace three {
namespace anim {

namespace detail {

typedef std::function<void(float)> Callback;

class ScopedTimer : public NonCopyable {
public:

  ScopedTimer( Clock& clock, Callback callback )
    : clock( clock ), startTime( clock.getElapsedTime() ), callback ( std::move( callback ) ) { }

  ~ScopedTimer() {
    if ( callback )
      callback( clock.getElapsedTime() - startTime );
  }

private:
  Clock& clock;
  float startTime;
  Callback callback;
};

class AnimFrameRequest : public NonCopyable {
public:
  AnimFrameRequest( float frameRate = 60 )
    : targetDeltaTime( 1.f / frameRate ),
      lastTime( clock.getElapsedTime() ) { }

  Clock clock;
  float targetDeltaTime;
  float lastTime;

  bool operator()( const Update& update ) {

    detail::ScopedTimer onFrameEnd( clock, [this]( float deltaTime ) {
      // Throttle the thread at the end of the frame
      auto sleepTime = targetDeltaTime - deltaTime;
      if ( sleepTime > 0 ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( ( int )( sleepTime * 1000 ) ) );
      }
    } );

    sdl::processEvents();

    {
      auto time = clock.getElapsedTime();
      auto deltaTime = time - lastTime;
      lastTime = time;

      if ( !update( deltaTime ) )
        return false;
    }

    sdl::swapBuffers();

    return true;

  }

}; // AnimFrameRequest

} // namespace detail

/////////////////////////////////////////////////////////////////////////

void gameLoop( Update update, float frameRate /*= 60*/ ) {
  detail::AnimFrameRequest requestAnimFrame( frameRate );
  while ( requestAnimFrame( update ) ) ;
}

void gameLoop( Update update, Render render, float frameRate /*= 60*/ ) {
  return gameLoop( [&]( float dt ) {
    return update( dt ) && render( dt );
  }, frameRate );
}

} // namespace anim
} // namespace three

#endif // THREE_ANIM_CPP