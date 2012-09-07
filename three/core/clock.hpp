#ifndef _CLOCK_HPP_
#define _CLOCK_HPP_

#include <chrono>

namespace three {

class Clock {
public:
	typedef std::chrono::time_point<std::chrono::system_clock> time_point;
	typedef std::chrono::duration<float, std::chrono::seconds> duration;

	Clock(bool autostart = true)
		: mbAutoStart(autostart), mbRunning(false) {

	}

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

	duration getDelta() {
		duration diff;

		if (mbAutoStart && !mbRunning) {
			start();
		}

		if (mbRunning) {
			auto newTime = std::chrono::system_clock::now();
			diff = std::chrono::duration_cast<duration>(newTime - oldTime);
			oldTime = newTime;
		}

		return diff;
	}

	time_point mStartTime;
	time_point mOldTime;
	duration mElapsedTime;
	bool mbAutoStart;
	bool mbRunning;
};

}

#endif