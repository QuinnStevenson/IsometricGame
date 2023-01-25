#include "Character.h"



Character::Character()
{
	characterBox.x = 0;
	characterBox.y = 0;
	characterBox.w = PLAYER_WIDTH;
	characterBox.h = PLAYER_HEIGHT;

	currentFrame = 0;
	yFrame = 1;

	currentTile = make_pair(0, 0);
	destinationTile = make_pair(0, 0);

	xToTravel = 0;
	yToTravel = 0;

	velX = 0, velY = 0;

	moveTokens = 5;

	moving = false;
	selected = false;
	depletedMoves = false;
}


Character::~Character()
{
	currentTexture.free();
}

void Character::handleEvent(SDL_Event& e, const Uint8* keystate, Pathfinding pathHandler, int grid[TOTAL_X_TILES][TOTAL_Y_TILES], SDL_Rect& camera, pair<int, int> destination)
{
	//cout << "Enemy" << endl;
}

//Derived classes have different rendering
void Character::render(SDL_Rect& camera, SDL_Renderer* renderer, SDL_Rect* playerClips)
{

}

//Enemy class will allow centering camera over enemy
void Character::setCamera(SDL_Rect& camera) 
{

}

void Character::reset()
{

}

void Character::moveHelper(vector<Character*> &allCharacters, int index)
{
	//Determines which y index to animate
	if (velX < 0 && velY > 0)
		setyFrame(0);
	else if (velX > 0 && velY > 0)
		setyFrame(1);
	else if (velX < 0 && velY < 0)
		setyFrame(2);
	else if (velX > 0 && velY < 0)
		setyFrame(3);
	else if (velX == 0 && velY > 0)
		setyFrame(1);
	else if (velX == 0 && velY < 0)
		setyFrame(2);
	else if (velX > 0 && velY == 0)
		setyFrame(1);
	else if (velX < 0 && velY == 0)
		setyFrame(0);

	characterBox.x += int(velX);
	xToTravel -= int(abs(velX));

	characterBox.y += int(velY);
	yToTravel -= int(abs(velY));

	//int currentY = getY() + PLAYER_HEIGHT;

	//cout << "CURRENT INDEX: " << index << endl;

	//Handles swapping characters based on their y position.  The higher the y position, the last to draw
	/*
	if (index == allCharacters.size() - 1)
	{
		if (currentY < allCharacters[index - 1]->getY() + PLAYER_HEIGHT)
			swap(allCharacters[index], allCharacters[index - 1]);
	}
	else if (index == 0)
	{
		if (currentY > allCharacters[index + 1]->getY() + PLAYER_HEIGHT)
			swap(allCharacters[index + 1], allCharacters[index]);
	}
	else
	{
		if (currentY > allCharacters[index + 1]->getY() + PLAYER_HEIGHT)
			swap(allCharacters[index + 1], allCharacters[index]);

		if (currentY < allCharacters[index - 1]->getY() + PLAYER_HEIGHT)
			swap(allCharacters[index], allCharacters[index - 1]);
	}
	*/
}

//Handles moving the character through the best path
void Character::moveCommand(vector<Character*> &allCharacters, int index)
{
	if (movePath.empty())
	{
		currentTile = destinationTile;
		reset();
		moving = false;
		return;
	}

	/*
	if (moveTokens == 0)
	{
		currentTile = destinationTile;
		reset();
		moving = false;
		depletedMoves = true;
		cout << "Cannot move any more tiles!" << endl;
		return;
	}
	*/

	pair<int, int> firstTile = movePath.top();

	if (currentTile.first == firstTile.first && currentTile.second == firstTile.second)
	{
		movePath.pop();

		moveTokens--;

		if (!movePath.empty())
		{
			pair<int, int> secondTile = movePath.top();

			destinationTile = secondTile;

			pair<int, int> firstCoords = tileToCoordinates(firstTile);
			pair<int, int> secondCoords = tileToCoordinates(secondTile);

			int xDistance = secondCoords.first - firstCoords.first;
			int yDistance = secondCoords.second - firstCoords.second;

			//Chooses direction velocities, in need of improvement
			if (xDistance == 0 && yDistance == -64)
			{
				velX = 0, velY = -4.5;
			}
			else if (xDistance == 64 && yDistance == -32)
			{
				velX = 4; velY = -2;
			}
			else if (xDistance == 128 && yDistance == 0)
			{
				velX = 4.5, velY = 0;
			}
			else if (xDistance == 64 && yDistance == 32)
			{
				velX = 4, velY = 2;
			}
			else if (xDistance == 0 && yDistance == 64)
			{
				velX = 0, velY = 4.5;
			}
			else if (xDistance == -64 && yDistance == 32)
			{
				velX = -4, velY = 2;
			}
			else if (xDistance == -128 && yDistance == 0)
			{
				velX = -4.5, velY = 0;
			}
			else if (xDistance == -64 && yDistance == -32)
			{
				velX = -4, velY = -2;
			}

			xToTravel = abs(xDistance);
			yToTravel = abs(yDistance);
		}
		else {
			currentTile = destinationTile;
			reset();
			moving = false;
			return;
		}
	}

	//Once player has reached their destination tile, reset and start again
	if (xToTravel <= 0 && yToTravel <= 0)
	{
		currentTile = destinationTile;
		velX = 0, velY = 0;
	}

	moveHelper(allCharacters, index);
}

//Converts tile to x and y values (0, 1) -> (64, 32)
pair<int, int> Character::tileToCoordinates(pair<int, int> tile)
{
	int xCoord = XOFFSET + (tile.first - tile.second) * TILE_WIDTH / 2;
	int yCoord = YOFFSET + (tile.first + tile.second) * TILE_HEIGHT / 4;

	return make_pair(xCoord, yCoord);
}

//Converts x and y values to tile (64, 32) -> (0, 1)
pair<int, int> Character::coordinatesToTile(pair<int, int> coordinates)
{
	int x = coordinates.first - XOFFSET;
	int y = coordinates.second - YOFFSET;

	int xTile = (x / (TILE_WIDTH / 2) + y / (TILE_HEIGHT / 4)) / 2;
	int yTile = (y / (TILE_HEIGHT / 4) - (x / (TILE_WIDTH / 2))) / 2;

	return make_pair(xTile, yTile);
}

//Sets character to tile coordinates
void Character::setPosition(pair<int, int> tileCoordinates)
{
	int x, y;

	x = XOFFSET + (tileCoordinates.first - tileCoordinates.second) * TILE_WIDTH / 2;
	y = YOFFSET + (tileCoordinates.first + tileCoordinates.second) * TILE_HEIGHT / 4;

	characterBox.x = x + (TILE_WIDTH - PLAYER_WIDTH) / 2;
	characterBox.y = y - (TILE_HEIGHT - PLAYER_HEIGHT) / 2;

	currentTile = tileCoordinates;
}

//Checks to see if character is selected
bool Character::checkSelected(pair<int, int> targetTile)
{
	if (currentTile.first == targetTile.first && currentTile.second == targetTile.second) {
		selected = true;
		return true;
	}
	else
		return false;

}

int Character::getX()
{
	return characterBox.x;
}

int Character::getY()
{
	return characterBox.y;
}

bool Character::getMoving()
{
	return moving;
}

bool Character::isSelected()
{
	return selected;
}

pair<int, int> Character::getCurrentTile()
{
	return currentTile;
}

//Sets y index for the spritesheet
void Character::setyFrame(int newyFrame)
{
	yFrame = newyFrame;

	switch (yFrame)
	{
	case 0: currentFrame = 0; break;
	case 1: currentFrame = 8; break;
	case 2: currentFrame = 16; break;
	case 3: currentFrame = 24; break;
	}
}