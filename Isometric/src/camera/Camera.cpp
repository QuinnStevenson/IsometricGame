#include "Camera.h"



Camera::Camera()
{
	x = 0, y = 0;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	scale = 1;

	cameraBox = { x, y, width, height };
}

void Camera::handleEvent(SDL_Event& e, const Uint8* keystate, SDL_Renderer* gRenderer, SDL_Window* gWindow)
{
	//Mouse wheel handles integer scaled zooming utilizing the nearest pixel algorithm
	//200% zoomed in an out.  Doing this however will make tile selection unusable
	//This is an incorrect way of handling how a zoom function should work.  However I don't have
	//The current knowledge in handling best graphics processing practices.  
	if (e.type == SDL_MOUSEWHEEL)
	{
		if (e.wheel.y > 0)
		{
			if (scale == -1)
				scale = 1;

			if (scale == 2)
				scale = 2;
			else
				scale++;

			if (scale == 0)
				scale++;

			if (scale >= 1) 
			{
				cameraBox.w = SCREEN_WIDTH / abs(scale);
				cameraBox.h = SCREEN_HEIGHT / abs(scale);
				SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH / abs(scale), SCREEN_HEIGHT / abs(scale));
			}
			else 
			{
				cameraBox.w = SCREEN_WIDTH * abs(scale);
				cameraBox.h = SCREEN_HEIGHT * abs(scale);
				SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH * abs(scale), SCREEN_HEIGHT * abs(scale));
			}
		}

		if (e.wheel.y < 0)
		{
			if (scale == -2)
				scale = -2;
			else
				scale--;

			if (scale == 0)
				scale--;

			if (scale >= 1) 
			{
				cameraBox.w = SCREEN_WIDTH / abs(scale);
				cameraBox.h = SCREEN_HEIGHT / abs(scale);
				SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH / abs(scale), SCREEN_HEIGHT / abs(scale));
			}
			else
			{
				cameraBox.w = SCREEN_WIDTH * abs(scale);
				cameraBox.h = SCREEN_HEIGHT * abs(scale);
				SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH * abs(scale), SCREEN_HEIGHT * abs(scale));
			}
		}
	}

	//If left mouse button has been pressed capture the mouse
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (e.button.button == SDL_BUTTON_RIGHT) {
			clicked = true;
			SDL_SetWindowGrab(gWindow, SDL_TRUE);
		}
	}

	//If right mouse button has been release, release the mouse
	if (e.type == SDL_MOUSEBUTTONUP) 
	{
		if (e.button.button == SDL_BUTTON_RIGHT) {
			clicked = false;
			SDL_SetWindowGrab(gWindow, SDL_FALSE);
		}
	}


	//Key camera motion is looking horrible right now
	/*
	if (e.type == SDL_KEYDOWN)
	{
		if (keystate[SDL_SCANCODE_W])
			w_key = true;

		if (keystate[SDL_SCANCODE_A])
			a_key = true;

		if (keystate[SDL_SCANCODE_S])
			s_key = true;

		if (keystate[SDL_SCANCODE_D])
			d_key = true;
	}

	if (e.type == SDL_KEYUP)
	{
		if (!keystate[SDL_SCANCODE_W])
			w_key = false;

		if (!keystate[SDL_SCANCODE_A])
			a_key = false;

		if (!keystate[SDL_SCANCODE_S])
			s_key = false;

		if (!keystate[SDL_SCANCODE_D])
			d_key = false;
	}
	*/

	//Camera Motion
	if (clicked)
	{
		cameraBox.x -= e.motion.xrel;
		cameraBox.y -= e.motion.yrel;
	}
	else
	{
		/*
		if (w_key)
			cameraBox.y -= 10;

		if (a_key)
			cameraBox.x -= 10;

		if (s_key)
			cameraBox.y += 10;

		if (d_key)
			cameraBox.x += 10;
			*/
	}

	//Keep the camera in bounds
	if (cameraBox.x < 0)
		cameraBox.x = 0;

	if (cameraBox.y < 0)
		cameraBox.y = 0;

	if (cameraBox.x > LEVEL_WIDTH - cameraBox.w)
		cameraBox.x = LEVEL_WIDTH - cameraBox.w;

	if (cameraBox.y > LEVEL_HEIGHT - cameraBox.h)
		cameraBox.y = LEVEL_HEIGHT - cameraBox.h;
	
}

int Camera::getScale()
{
	return scale;
}

SDL_Rect& Camera::getCamera()
{
	return cameraBox;
}

