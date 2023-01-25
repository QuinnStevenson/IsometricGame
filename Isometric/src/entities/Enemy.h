#pragma once
#include "Character.h"
#include "Friendly.h"

class Enemy: public Character

{
public:
	Enemy(SDL_Renderer*);

	//Called when new enemy is moving, orients camera to center the enemy
	void setCamera(SDL_Rect& camera);
	
	//Renders enemy to screen
	void render(SDL_Rect&, SDL_Renderer*, SDL_Rect*);

	//Sets the destination tile to the closet friendly unit
	void findClosestUnit(vector<Friendly>, Pathfinding, int[TOTAL_Y_TILES][TOTAL_X_TILES]);

private:
	bool foundUnit;
	bool arrived;
};

