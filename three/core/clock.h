#ifndef THREE_CLOCK_HPP
#define THREE_CLOCK_HPP

#include <three/common.hpp>

#include <chrono>

namespace three {

class Clock {
public:
  typedef std::chrono::time_point< std::chrono::system_clock > Time;
  typedef std::chrono::duration< float > Duration;

  Clock( bool autostart = true )
    : mElapsedTime( Duration::zero() ),
      mbAutoStart( autostart ),
      mbRunning( false ) { }

  void start() {
    mStartTime = std::chrono::system_clock::now();
    mOldTime = mStartTime;
    mbRunning = true;
  }

  void stop() {
    getElapsedTime();
    mbRunning = false;
  }

  float getElapsedTime() {
    mElapsedTime += getDelta();
    return mElapsedTime.count();
  }

private:

  Duration getDelta() {
    Duration diff;

    if ( mbAutoStart && !mbRunning ) {
      start();
    }

    if ( mbRunning ) {
      auto newTime = std::chrono::system_clock::now();
      diff = std::chrono::duration_cast< Duration > ( newTime - mOldTime );
      mOldTime = newTime;
    }

    return diff;
  }

  Time mStartTime;
  Time mOldTime;
  Duration mElapsedTime;
  bool mbAutoStart;
  bool mbRunning;
};

} // namespace three

#endif // THREE_CLOCK_HPP
