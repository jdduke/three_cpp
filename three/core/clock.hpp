#ifndef THREE_CLOCK_HPP
#define THREE_CLOCK_HPP

#include <three/common.hpp>

#include <chrono>

namespace three {

class Clock {
public:
    typedef std::chrono::time_point< std::chrono::system_clock > Time;
    typedef std::chrono::duration< float, std::chrono::seconds > Duration;

    Clock ( bool autostart = true )
     : mbAutoStart ( autostart ), mbRunning ( false ) { }

    void start() {
        mStartTime = std::chrono::system_clock::now();
        mOldTime = mStartTime;
        mRunning = true;
    }

    void stop() {
        getElapsedTime();
        mRunning = false;
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
            diff = std::chrono::duration_cast< Duration > ( newTime - oldTime );
            oldTime = newTime;
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