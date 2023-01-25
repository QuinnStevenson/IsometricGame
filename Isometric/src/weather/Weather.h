#pragma once

#include "../constants.h"
#include "../Texture.h"
#include "Rain.h"
#include "Snow.h"

class Weather
{
public:
	Weather();
	~Weather();

	/* Main draw call to render particles to screen */
	void renderParticles(SDL_Rect, SDL_Renderer*);

	/* Handles swapping between weather effects */
	void setWeather(SDL_Renderer*, int, SDL_Rect);

	/* Toggling weather effects */
	void handleEvent(SDL_Event&, SDL_Renderer*, SDL_Rect);

	/* Frees the weather arrays */
	void freeWeather();

	bool getActivated();

private:
	Rain* currentWeather[TOTAL_RAIN_DROPS];
	Snow* currentSnow[TOTAL_RAIN_DROPS];

	Texture currentTexture;
	SDL_Rect textureClips[TOTAL_RAIN_SPRITES];

	bool activated;
	int weatherType;
};

