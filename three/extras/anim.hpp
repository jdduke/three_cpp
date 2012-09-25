#ifndef THREE_ANIM_HPP
#define THREE_ANIM_HPP

#include <three/extras/noncopyable.hpp>

#include <thread>

namespace three {
namespace anim {

class AnimFrameRequest : public NonCopyable {
public:
    explicit AnimFrameRequest( float frameRate = 60 )
     : frameTime ( 1.f / frameRate ),
       lastTime ( clock.getElapsedTime() ) { }

    typedef std::function<bool(float)> Callback;

    bool operator() (const Callback& callback) {

        auto time = elapsedTime();
        auto deltaTime = ( time - lastTime );
        auto sleepTime = frameTime - deltaTime;

        if ( sleepTime > 0 ) {
            std::this_thread::sleep_for( std::chrono::milliseconds( (int)(sleepTime * 1000) ) );
            deltaTime = frameTime;
        }

        lastTime = time;

        return callback( deltaTime );

    }

    float elapsedTime() {
        return clock.getElapsedTime();
    }

    void reset() {
        lastTime = 0;
        clock.stop();
    }

private:
    Clock clock;
    float frameTime;
    float lastTime;
};

/////////////////////////////////////////////////////////////////////////

typedef std::function<bool(float)> Update;
typedef std::function<bool(float)> Render;

void gameLoop(Update update, Render render) {

  anim::AnimFrameRequest requestAnimFrame;

  while ( requestAnimFrame( [&]( float deltaTime ) {
    return update( deltaTime ) && render( deltaTime );
  } ) );

}


} // namespace anim
} // namespace three

#endif // THREE_ANIM_HPP