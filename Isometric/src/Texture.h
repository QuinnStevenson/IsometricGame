#pragma once

#include "constants.h"
#include<SDL_ttf.h>

class Texture
{
public:
	Texture();
	~Texture();

	/* Loads texture from file */
	bool loadFromFile(string, SDL_Renderer*);

	/* Loads textures from true type fonts */
	bool loadFromRenderedText(string, SDL_Color, TTF_Font*, SDL_Renderer*);

	/* Sets the color for the texture */
	void setColor(Uint8, Uint8, Uint8);

	/* Sets the blend mode for the texture */
	void setBlendMode(SDL_BlendMode);

	/* Sets the alpha channel of the texture */
	void setAlpha(Uint8);

	/* Render function called to draw texture to screen */
	void render(int, int, SDL_Renderer*, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	/* Frees the current stored pointer to associated texture */
	void free();

	/* Returns width of texture */
	int getWidth();

	/* returns height of the texture */
	int getHeight();

	/* Returns pointer to the texture */
	SDL_Texture* getTexture();

private:
	int textureWidth;
	int textureHeight;

	SDL_Texture* currentTexture;
};

