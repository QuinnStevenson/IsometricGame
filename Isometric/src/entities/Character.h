#pragma once
#include "../Texture.h"
#include "../timers/Timer.h"
#include "../pathing/Pathfinding.h"
#include <queue>

class Character
{
public:
	Character();
	~Character();

	//Sets position and y frame of the entity;
	void setyFrame(int);
	void setPosition(pair<int, int>);
	pair<int, int> tileToCoordinates(pair<int, int>);
	pair<int, int> coordinatesToTile(pair<int, int>);

	void moveHelper(vector<Character*>&, int);
	void moveCommand(vector<Character*>&, int);

	virtual void render(SDL_Rect&, SDL_Renderer*, SDL_Rect*);
	virtual void setCamera(SDL_Rect&);

	virtual void handleEvent(SDL_Event&, const Uint8*, Pathfinding, int [TOTAL_X_TILES][TOTAL_Y_TILES], SDL_Rect&, pair<int, int>);
	virtual void reset();

	//Dumps path if move tokens have been depleted
	void dumpPath();

	bool checkSelected(pair<int, int>);

	int getX();
	int getY();
	bool getMoving();
	bool isSelected();
	pair<int, int> getCurrentTile();



protected:
	SDL_Rect characterBox;

	Texture currentTexture;

	pair<int, int> currentTile;

	pair<int, int> destinationTile;

	Timer frameTimer;

	stack<Coord> movePath;

	int moveTokens;
	bool depletedMoves;

	int currentFrame;
	int yFrame;

	int xToTravel, yToTravel;

	bool selected;

	bool moving;

	bool friendly;

	double velX = 0, velY = 0;
};

