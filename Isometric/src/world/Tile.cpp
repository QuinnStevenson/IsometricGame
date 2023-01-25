
#include "Tile.h"

Tile::Tile(int x, int y, int newType, Texture newTexture)
{
	tileType = newType;

	tileBox.x = x;
	tileBox.y = y;
	tileBox.w = TILE_WIDTH;
	tileBox.h = TILE_HEIGHT;

	hovered = false;
	inserted = false;

	worldCoordinates = make_pair(x, y);

	int newX = x - XOFFSET;
	int newY = y - YOFFSET;

	int tempx = (newX / (TILE_WIDTH/2) + newY / (TILE_HEIGHT / 4)) / 2;
	int tempy = (newY / (TILE_HEIGHT / 4) - (newX / (TILE_WIDTH / 2))) / 2;

	tileCoordinates = make_pair(tempx, tempy);

	tileTexture = newTexture;
}

//Renders tile to the screen
void Tile::render(SDL_Rect& camera, SDL_Renderer* renderer, SDL_Rect* gTileClips)
{
	if (checkCollision(camera, tileBox))
		if(!hovered)
			tileTexture.render(tileBox.x - camera.x, tileBox.y - camera.y, renderer, &gTileClips[tileType]);
		else if (hovered && tileType != 0)
			tileTexture.render(tileBox.x - camera.x, tileBox.y - camera.y, renderer, &gTileClips[TILE_YELLOW]);
}

//Checks collision between the camera and tiles, tiles inside camera will be drawn 
bool Tile::checkCollision(SDL_Rect a, SDL_Rect b)
{
	//The sides of the rectangles
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate the sides of rect A
	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	//Calculate the sides of rect B
	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	//If any of the sides from A are outside of B
	if (bottomA <= topB)
		return false;

	if (topA >= bottomB)
		return false;

	if (rightA <= leftB)
		return false;

	if (leftA >= rightB)
		return false;

	//If none of the sides from A are outside B
	return true;
}

//Handles checking if the mouse cursor is over this specific tile
void Tile::checkHover(int x, int y, int scale)
{
	double width, height;

	width = tileBox.w;
	height = tileBox.h / 2;

	//cout << "MOUSE: " << x << " , " << y << endl;

	if (scale < -1)
	{
		//width = tileBox.w / 2;
		//height = tileBox.h / 4;
	}
	else if (scale > 1)
	{
		//x = x - tileBox.w*4;
		//y = y - tileBox.h*2;
		//width = tileBox.w * 2;
		//height = tileBox.h;
	}

	int leftSide = tileBox.x, rightSide = tileBox.x + tileBox.w;
	int top = tileBox.y, bottom = tileBox.y + tileBox.h;

	double xMidPoint = double(tileBox.x + width / 2);
	double yMidPoint = double(tileBox.y + height / 2);

	double deltaX, deltaY, distanceX, distanceY;

	deltaX = x - xMidPoint;
	deltaY = y - yMidPoint;

	if (deltaX < 0) 
		deltaX *= -1;
	if (deltaY < 0)
		deltaY *= -1;

	distanceX = deltaX / (width / 2);
	distanceY = deltaY / (height / 2);

	if (distanceX + distanceY < 0.85) {
		hovered = true;
		//cout << "MIDPOINT: " << xMidPoint << " , " << yMidPoint << endl;
		//cout << "MOUSE: " << x << " , " << y << endl;
		//cout << "TILE: " << tileBox.x << " , " << tileBox.y << endl;
	}
	else
		hovered = false;
}

//Handler for mouse motion and mouse click.  If a tile is clicked, that becomes the new destination tile for the character
//on mouse motion, the hovered tile will be found
void Tile::handleEvent(SDL_Event& e, Camera gCamera, pair<int, int> &coordinates)
{
	if (e.type == SDL_MOUSEMOTION)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		checkHover(x + gCamera.getCamera().x, y + gCamera.getCamera().y, gCamera.getScale());
	}

	
	if (e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (hovered)
			coordinates = tileCoordinates;
	}
	
}

SDL_Rect Tile::getBox()
{
	return tileBox;
}

Texture Tile::getTexture()
{
	return tileTexture;
}

int Tile::getType()
{
	return tileType;
}

pair<int, int> Tile::getTileCoordinates()
{
	return tileCoordinates;
}

pair<int, int> Tile::getWorldCoordinates()
{
	return worldCoordinates;
}