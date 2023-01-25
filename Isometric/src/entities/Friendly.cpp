#include "Friendly.h"

Friendly::Friendly(SDL_Renderer* gRenderer)
{
	if (!currentTexture.loadFromFile("sprites/friendlysheet.png", gRenderer))
	{
		cout << "Failed to load the player texture!" << endl;
	}

	if (!highlightTexture.loadFromFile("sprites/selection/selection.png", gRenderer))
	{
		cout << "Failed to load the highlight texture";
	}

	Timer newTimer;
	frameTimer = newTimer;
	frameTimer.start();

	friendly = true;
}



Friendly::~Friendly()
{
	highlightTexture.free();
}

void Friendly::handleEvent(SDL_Event& e, const Uint8* keystate, Pathfinding pathHandler, int grid[TOTAL_X_TILES][TOTAL_Y_TILES], SDL_Rect& camera, pair<int, int> destination)
{
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		//On left mouse button we need to select a character
		//afterwards we must find the destination to move the character if selected
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			checkSelected(destination);

			if (!moving)
			{
				if (selected)
				{
					sourceCoord = currentTile;
					originTile = sourceCoord;
					sourceSelection = true;
				}
				
				if (selected && currentTile != destination)
				{
					destinationCoord = destination;
					finalDestinationTile = destination;
					destinationSelection = true;
					cout << "ATTEMPTING TO MOVE FROM TILE: (" << sourceCoord.first << " , " << sourceCoord.second << ")  TO TILE: (" << destinationCoord.first << " , " << destinationCoord.second << ")" << endl;
					movePath = pathHandler.aStarSearch(grid, sourceCoord, destinationCoord, movePath);
					moving = true;
				}
			}
		}
	}

	if (e.type == SDL_MOUSEBUTTONUP)
	{
		//Deselect if right mouse is clicked
		if (e.button.button == SDL_BUTTON_RIGHT)
		{
			if (!moving) {
				destinationTile = currentTile;
				destinationSelection = false;
				sourceSelection = false;
				destinationCoord = currentTile;
				selected = false;
			}
		}
	}
}

//Render function draws player to the screen
void Friendly::render(SDL_Rect& camera, SDL_Renderer* renderer, SDL_Rect* playerClips)
{
	//Tracks the current seconds between the frames
	float seconds = (frameTimer.getTicks() / 1000.f);

	//Tells which y index of the sprite sheet to draw
	switch (yFrame)
	{
	case 0:
		if (currentFrame >= 7)
			currentFrame = 0;
		break;

	case 1:
		if (currentFrame >= 15)
			currentFrame = 8;
		break;
	case 2:
		if (currentFrame >= 23)
			currentFrame = 16;
		break;
	case 3:
		if (currentFrame >= 31)
			currentFrame = 24;
		break;
	}

	//Increments the x index frame from the sprite sheet every 0.3 seconds
	if (seconds > 0.3) {
		currentFrame++;
		frameTimer.stop();
		frameTimer.start();
	}

	//Next two renders are for highlighting the source and destination tiles, could be moved from Player
	if (sourceSelection)
	{
		pair<int, int> newSource = tileToCoordinates(originTile);
		highlightTexture.render(newSource.first - camera.x, (newSource.second - TILE_HEIGHT / 2 + 13) - camera.y, renderer);
	}

	if (destinationSelection)
	{
		pair<int, int> newDest = tileToCoordinates(finalDestinationTile);
		highlightTexture.render(newDest.first - camera.x, (newDest.second - TILE_HEIGHT / 2 + 13) - camera.y, renderer);
	}

	//Rendering texture call
	currentTexture.render(characterBox.x - camera.x, characterBox.y - camera.y, renderer, &playerClips[currentFrame]);
}

void Friendly::reset()
{
	sourceSelection = false;
}
