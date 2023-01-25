#pragma once
#include "../constants.h"
#include "../Texture.h"

class Rain
{
public:
	Rain(SDL_Rect, Texture);

	void render(SDL_Renderer*, SDL_Rect*);

	bool isDead();

private:
	int rainX, rainY;

	int rainYMax;
	int rainYMin;

	int rainFrame;

	int rainVelY;

	Texture currentTexture;
};

