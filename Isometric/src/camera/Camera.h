#pragma once
#include "../constants.h"

class Camera
{
public:
	Camera();

	void handleEvent(SDL_Event&, const Uint8*, SDL_Renderer*, SDL_Window*);

	SDL_Rect& getCamera();

	int getScale();

private:
	SDL_Rect cameraBox;

	int x, y, width, height;

	bool clicked = false;

	bool w_key = false, a_key = false, s_key = false, d_key = false;

	int scale;
};

