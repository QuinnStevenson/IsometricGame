#include "Enemy.h"



Enemy::Enemy(SDL_Renderer* gRenderer)
{
	if (!currentTexture.loadFromFile("sprites/enemysheet.png", gRenderer))
	{
		cout << "Failed to load the enemy texture!" << endl;
	}

	Timer newTimer;
	frameTimer = newTimer;
	frameTimer.start();

	friendly = false;
	foundUnit = false;
	arrived = false;
}

void Enemy::findClosestUnit(vector<Friendly> friendlyUnits, Pathfinding pathHandler, int map[TOTAL_Y_TILES][TOTAL_X_TILES])
{
	if (!foundUnit)
	{
		pair<int, int> closestTile;
		int smallestSum;

		for (int i = 0; i < friendlyUnits.size(); i++)
		{
			if (i == 0)
			{
				closestTile = friendlyUnits[i].getCurrentTile();
				smallestSum = abs(friendlyUnits[i].getX() - getX()) + abs(friendlyUnits[i].getY() - getY());
			}
			else
			{
				int newSum = abs(friendlyUnits[i].getX() - getX()) + abs(friendlyUnits[i].getY() - getY());

				if (newSum < smallestSum) {
					smallestSum = newSum;
					closestTile = friendlyUnits[i].getCurrentTile();
				}
			}
		}

		destinationTile = closestTile;

		movePath = pathHandler.aStarSearch(map, currentTile, destinationTile, movePath);
		moving = true;
		foundUnit = true;
	}
}

void Enemy::setCamera(SDL_Rect& camera)
{
	//Center the camera over the dot
	camera.x = (characterBox.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
	camera.y = (characterBox.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	//Keep the camera in bounds
	if (camera.x < 0)
		camera.x = 0;

	if (camera.y < 0)
		camera.y = 0;

	if (camera.x > LEVEL_WIDTH - camera.w)
		camera.x = LEVEL_WIDTH - camera.w;

	if (camera.y > LEVEL_HEIGHT - camera.h)
		camera.y = LEVEL_HEIGHT - camera.h;

}

//Render function draws player to the screen
void Enemy::render(SDL_Rect& camera, SDL_Renderer* renderer, SDL_Rect* playerClips)
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

	//Rendering texture call
	currentTexture.render(characterBox.x - camera.x, characterBox.y - camera.y, renderer, &playerClips[currentFrame]);
}