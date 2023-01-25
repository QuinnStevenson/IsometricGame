#pragma once
#include "../Texture.h"
#include "../camera/Camera.h"

#include <queue>

class Tile
{
public:
	Tile(int, int, int, Texture);

	//Renders the tile to the screen
	void render(SDL_Rect&, SDL_Renderer*, SDL_Rect*);

	//Checks bounding collision of tiles and camera position.  Only tiles within the camera are rendered
	bool checkCollision(SDL_Rect, SDL_Rect);

	//Checks to see if the mouse is currently over a tile
	void checkHover(int, int, int);

	//Event handler, currently only handles mouse motion
	void handleEvent(SDL_Event&, Camera, pair<int, int>&);

	Texture getTexture();
	int getType();
	SDL_Rect getBox();

	pair<int, int> getWorldCoordinates();

	pair<int, int> getTileCoordinates();

private:
	SDL_Rect tileBox;

	Texture tileTexture;

	bool hovered, inserted;

	pair<int, int> worldCoordinates;

	pair<int, int> tileCoordinates;

	int tileType;
};