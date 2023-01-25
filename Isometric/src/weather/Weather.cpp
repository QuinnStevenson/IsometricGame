#include "Weather.h"


Weather::Weather()
{
	activated = false;
	weatherType = 0;

	textureClips[RAIN_FALL].x = 0;
	textureClips[RAIN_FALL].y = 0;
	textureClips[RAIN_FALL].w = 9;
	textureClips[RAIN_FALL].h = 9;

	textureClips[RAIN_TWO].x = 9;
	textureClips[RAIN_TWO].y = 0;
	textureClips[RAIN_TWO].w = 9;
	textureClips[RAIN_TWO].h = 9;

	textureClips[RAIN_THREE].x = 18;
	textureClips[RAIN_THREE].y = 0;
	textureClips[RAIN_THREE].w = 9;
	textureClips[RAIN_THREE].h = 9;

	textureClips[RAIN_FOUR].x = 27;
	textureClips[RAIN_FOUR].y = 0;
	textureClips[RAIN_FOUR].w = 9;
	textureClips[RAIN_FOUR].h = 9;

	textureClips[RAIN_FIVE].x = 36;
	textureClips[RAIN_FIVE].y = 0;
	textureClips[RAIN_FIVE].w = 9;
	textureClips[RAIN_FIVE].h = 9;

	textureClips[RAIN_SIX].x = 45;
	textureClips[RAIN_SIX].y = 0;
	textureClips[RAIN_SIX].w = 9;
	textureClips[RAIN_SIX].h = 9;

	textureClips[RAIN_SEVEN].x = 54;
	textureClips[RAIN_SEVEN].y = 0;
	textureClips[RAIN_SEVEN].w = 9;
	textureClips[RAIN_SEVEN].h = 9;

	textureClips[RAIN_EIGHT].x = 63;
	textureClips[RAIN_EIGHT].y = 0;
	textureClips[RAIN_EIGHT].w = 9;
	textureClips[RAIN_EIGHT].h = 9;
}

void Weather::setWeather(SDL_Renderer* gRenderer, int newWeather, SDL_Rect camera)
{
	//Quickly Free the current weather
	freeWeather();

	switch (newWeather)
	{
		//Rain
		case 0:
			if (!currentTexture.loadFromFile("sprites/Particles/rain/rainsheet.png", gRenderer))
			{
				cout << "Unable to load rain spritesheet!" << endl;
			}

			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				currentWeather[i] = new Rain(camera, currentTexture);
			}

			weatherType = 0;
			break;

		//Snow
		case 1:
			if (!currentTexture.loadFromFile("sprites/Particles/snow/snowsheet.png", gRenderer))
			{
				cout << "Unable to load snow spritesheet!" << endl;
			}

			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				currentSnow[i] = new Snow(camera, currentTexture);
			}

			weatherType = 1;
			break;
	}
}

void Weather::renderParticles(SDL_Rect camera, SDL_Renderer* gRenderer)
{
	switch (weatherType)
	{
		//Rain
		case 0:
			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				if (currentWeather[i]->isDead())
				{
					delete currentWeather[i];
					currentWeather[i] = new Rain(camera, currentTexture);
				}
			}

			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				currentWeather[i]->render(gRenderer, textureClips);
			}
			break;

		//Snow
		case 1:
			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				if (currentSnow[i]->isDead())
				{
					delete currentSnow[i];
					currentSnow[i] = new Snow(camera, currentTexture);
				}
			}

			for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
			{
				currentSnow[i]->render(gRenderer, textureClips);
			}
			break;
	}
}

/* Function handler for keyboard events activating/deactivating weather*/
void Weather::handleEvent(SDL_Event& e, SDL_Renderer* gRenderer, SDL_Rect camera)
{
	//If a key was pressed
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		//Adjust the velocity
		switch (e.key.keysym.sym)
		{
			//Rain swap
			case SDLK_8: 
				activated = !activated;
				setWeather(gRenderer, 0, camera); 
				break;
			//Snow swap
			case SDLK_9: 
				activated = !activated;
				setWeather(gRenderer, 1, camera);
				break;
			//Cancels weather effects
			case SDLK_0: 
				if (activated)
				{
					cout << "Shutting down weather!" << endl;
					activated = !activated;
					freeWeather();
				}
				else
				{
					cout << "No weather currently!" << endl;
				}
				break;
		}
	}
}

bool Weather::getActivated()
{
	return activated;
}

void Weather::freeWeather()
{
	for (int i = 0; i < TOTAL_RAIN_DROPS; ++i)
	{
		if (currentWeather[i] == NULL)
		{
			delete currentWeather[i];
			currentWeather[i] = NULL;
		}

		if (currentSnow[i] == NULL)
		{
			delete currentSnow[i];
			currentSnow[i] = NULL;
		}
	}

	currentTexture.free();
}

Weather::~Weather()
{
	freeWeather();
}
