#include "Rain.h"

Rain::Rain(SDL_Rect camera, Texture rainTexture)
{
	rainX = rand()%((camera.x + camera.w) - camera.x + 1);

	//Max and min for the raindrop y spawning
	int min = camera.y - 400;
	int max = camera.y;

	int random = rand() % ((abs(min) + abs(max)) + 1);

	rainYMin = camera.y - random;

	rainYMax = rand()%((camera.y + camera.h) - camera.y + 1);

	rainY = rainYMin;

	rainFrame = 0;

	rainVelY = 5;

	currentTexture = rainTexture;
}

void Rain::render(SDL_Renderer* renderer, SDL_Rect* rainClips)
{
	if (rainY >= rainYMax)
	{
		rainFrame++;
	}
	else
	{
		rainY += rainVelY;
	}

	if (!isDead())
	{
		currentTexture.render(rainX, rainY, renderer, &rainClips[rainFrame]);
	}
	
}

bool Rain::isDead()
{
	return rainFrame > 7;
}
