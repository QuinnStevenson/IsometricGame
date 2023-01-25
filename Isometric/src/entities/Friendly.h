#pragma once
#include "Character.h"

class Friendly: public Character
{
public:
	Friendly(SDL_Renderer* gRenderer);
	~Friendly();

	void render(SDL_Rect&, SDL_Renderer*, SDL_Rect*);
	void handleEvent(SDL_Event&, const Uint8*, Pathfinding, int [TOTAL_X_TILES][TOTAL_Y_TILES], SDL_Rect&, pair<int, int>);

	void reset();

private:
	Texture highlightTexture;

	pair<int, int> finalDestinationTile;
	pair<int, int> originTile;

	pair<int, int> sourceCoord;
	pair<int, int> destinationCoord;

	bool sourceSelection = false;
	bool destinationSelection = false;
};

