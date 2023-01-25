#pragma once

#include "../constants.h"

class Timer
{
public:
	Timer();

	//Clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	Uint32 getTicks();

	//Status of timer
	bool isStarted();
	bool isPaused();

private:
	//Clock time when timer starts
	Uint32 startTicks;

	//Ticks stored on pause
	Uint32 pausedTicks;

	bool paused;
	bool started;
};

