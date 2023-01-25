#include "Texture.h"


/* Initializing the texture */
Texture::Texture()
{
	textureHeight = 0;
	textureWidth = 0;

	currentTexture = NULL;
}

/* Deinit makes call on free() function */
Texture::~Texture()
{
	free();
}

/* Loads texture, path is indicated and renderer is provided */
bool Texture::loadFromFile(string path, SDL_Renderer* renderer)
{
	//free() call to ensure the new texture is cleared before creation 
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		cout << "Unable to load image!: " << IMG_GetError() << endl;
	}
	else
	{
		//This will erase the cyan color in an image on call, RGB values should be changed accordingly
		//SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

		if (renderer == NULL)
		{
			cout << "Renderer not set up" << endl;
		}

		if (newTexture == NULL)
		{
			cout << "UNable to create texture!: " << SDL_GetError() << endl;
		}
		else
		{
			textureWidth = loadedSurface->w;
			textureHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	currentTexture = newTexture;
	return currentTexture != NULL;
}

/*
  Will create a text surface based off of true type font + whatever test is being rendered
  It cannot handle constantly changing text.  If the text changes, a new surface will have to be
  created.  This is too demanding when there is fast paced changes to text.  The only other way
  around this would to create some form of text font atlas and build words/sentences based off of
  string values and the respective letters from the atlas.  
*/

bool Texture::loadFromRenderedText(string text, SDL_Color textColor, TTF_Font* targetFont, SDL_Renderer* gRenderer)
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid(targetFont, text.c_str(), textColor);
	SDL_Texture* newTexture = NULL;

	if (textSurface == NULL)
	{
		cout << "Failure to load true type font!" << endl;
	}
	else
	{
		newTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);

		if (newTexture == NULL)
		{
			cout << "Failure to make font texture from surface!" << endl;
		}
		else
		{
			textureWidth = textSurface->w;
			textureHeight = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return newTexture != NULL;
}
/* Main render function for a texture.  If a clip is provided the texture will render the splice of the overall texture */
void Texture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{

	SDL_Rect renderQuad = { x, y, textureWidth, textureHeight };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Extended version of the SDL_RenderCopy function to provide more tools 
	SDL_RenderCopyEx(renderer, currentTexture, clip, &renderQuad, angle, center, flip);
}

/* Setting color of texture */
void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(currentTexture, red, green, blue);
}

/* Setting blending mode */
void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(currentTexture, blending);
}

/* Setting alpha channel */
void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(currentTexture, alpha);
}

/* Returning height */
int Texture::getHeight()
{
	return textureHeight;
}

/* Returning width */
int Texture::getWidth()
{
	return textureWidth;
}

/* Returning texture */
SDL_Texture* Texture::getTexture()
{
	return currentTexture;
}

/* Freeing texture if it's being used */
void Texture::free()
{
	if (currentTexture != NULL)
	{
		currentTexture = NULL;
		textureHeight = 0;
		textureWidth = 0;
	}
}