#include "Snow.h"

Snow::Snow(SDL_Rect camera, Texture snowTexture)
{
	//Sets initial x position for snow particle
	initX = rand() % ((camera.x + camera.w) - camera.x + 1);

	//Max and min for the snowflake y spawning
	int min = camera.y - 400;
	int max = camera.y;

	int random = rand() % ((abs(min) + abs(max)) + 1);

	//Minimum spawning point for a snowflake
	snowYMin = camera.y - random;

	//Chooses a random end point for the snowflake
	snowYMax = rand() % ((camera.y + camera.h) - camera.y + 1);

	//Sets snow flakes random y position
	snowY = snowYMin;

	//Starting snowflake frame
	snowFrame = 0;

	//Y Velocity for snowflake
	snowVelY = 2;

	//X Veloctiy for snowflake
	snowVelX = 1;

	//Setting boundaries for snow x plane sway
	snowLeftBoundary = initX - 20;
	snowRightBoundary = initX + 20;

	snowX = snowLeftBoundary + rand() % ((abs(snowLeftBoundary) + abs(snowRightBoundary)) - abs(snowLeftBoundary) + 1);

	//Sets a point i
	snowYStart = snowY;
	snowYDelta = 0;

	centerPoint = snowLeftBoundary + (snowRightBoundary - snowLeftBoundary);

	increasing = true;

	currentTexture = snowTexture;
}

void Snow::render(SDL_Renderer* renderer, SDL_Rect* snowClips)
{
	if (snowY > snowYMax)
	{
		snowFrame++;
	}
	else
	{
		if (snowYDelta > 32) {
			snowYDelta = 0;
		}

		snowY += snowVelY;
		snowYDelta += snowVelY;

		//Following logic implements an arc movement for each individual snowflake
		if (snowX <= snowLeftBoundary)
		{
			increasing = true;
		}
		else if (snowX >= snowRightBoundary)
		{
			increasing = false;
		}

		if (increasing && snowX > centerPoint)
		{

			snowVelX--;
		}
		else if (increasing && snowX < centerPoint)
		{
			snowVelX++;
		}
		else if (!increasing && snowX > centerPoint)
		{
			snowVelX--;
		}
		else if (!increasing && snowX < centerPoint)
		{
			snowVelX++;
		}

		if (increasing)
		{
			snowX += int(snowVelX * 0.05);
		}
		else {
			snowX += int(snowVelX * 0.05);
		}		
	}

	/* Draw rain to screen */
	if (!isDead())
	{
		currentTexture.render(snowX, snowY, renderer, &snowClips[snowFrame]);
	}
}

bool Snow::isDead()
{
	return snowFrame > 7;
}