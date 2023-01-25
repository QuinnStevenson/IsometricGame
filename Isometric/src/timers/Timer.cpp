#include "Timer.h"

/* Initializes the timer */
Timer::Timer()
{
	startTicks = 0;
	pausedTicks = 0;

	paused = false;
	started = false;
}

/* Called when timer starts */
void Timer::start() 
{
	started = true;
	paused = false;

	startTicks = SDL_GetTicks();
	pausedTicks = 0;
}

/* Called when timer stops */
void Timer::stop()
{
	started = false;
	paused = false;

	startTicks = 0;
	pausedTicks = 0;
}

/* Called when current timer is paused, stores ticks */
void Timer::pause()
{
	if (started && !paused)
	{
		paused = true;

		pausedTicks = SDL_GetTicks() - startTicks;
		startTicks = 0;
	}
}

/* Called when timer is unpaused, paused ticks subtracted from the current ticks */
void Timer::unpause()
{
	if (started && paused)
	{
		paused = false;
		startTicks = SDL_GetTicks() - pausedTicks;

		pausedTicks = 0;
	}
}

/* Returns the current ticks stored by the timer */
Uint32 Timer::getTicks()
{
	Uint32 time = 0;

	if (started)
	{
		if (paused)
		{
			time = pausedTicks;
		}
		else
		{
			time = SDL_GetTicks() - startTicks;
		}
	}

	return time;
}

/* Returns status of the timer */
bool Timer::isStarted()
{
	return started;
}

/* Returns paused state of the timer */
bool Timer::isPaused()
{
	return paused;
}
