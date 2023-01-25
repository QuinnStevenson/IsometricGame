#pragma once

#include "../constants.h"
#include "../Texture.h"

class Snow
{
public:
	Snow(SDL_Rect, Texture);

	void render(SDL_Renderer*, SDL_Rect*);

	bool isDead();

private:
	int snowX, snowY;

	int snowYMax, snowYMin;

	int snowFrame;

	int snowVelY, snowVelX;

	int snowLeftBoundary, snowRightBoundary;

	int snowYStart, snowYDelta, initX, centerPoint;

	bool increasing;

	Texture currentTexture;
};

