#pragma once
#include "constants.h"
#include "world/Tile.h"
#include "entities/Character.h"
#include "entities/Enemy.h"
#include "entities/Friendly.h"
#include "Texture.h"
#include "weather/Weather.h"
#include "timers/Timer.h"
#include "camera/Camera.h"
#include "pathing/Pathfinding.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <queue>
#include <vector>


class GameHandler
{
public:
	GameHandler();

	//Initializing SDL tools
	bool init();

	//Initializing tiles, parameter is the tile map
	bool setTiles(vector<vector<Tile*>>&);

	//Loading all textures and media, parameter is tile map
	bool loadMedia(vector<vector<Tile*>>&);

	//Function runs through checks on initializing requirements for game
	bool startup(vector<vector<Tile*>>&);

	//Initializing enemy and friendly units
	void initializeUnits();

	//Main function to run the game
	void begin();

	//Handles rendering all entities to screen
	void handleRender(vector<vector<Tile*>>);

	//Handles user input events
	void handleEvents(vector<vector<Tile*>>, bool&);

	//Function called when game is terminated
	void close(vector<vector<Tile*>>&);

	//Insertion sort on character y values
	void insertionSort();

private:
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;

	//SDL event types
	SDL_Event e;
	const Uint8* keystate;

	//Tile Textures
	Texture tileTexture;

	//Sprite clips from the tile sheet
	SDL_Rect tileClips[TOTAL_TILE_SPRITES];

	//Sprite clips from the player sheet
	SDL_Rect playerClips[TOTAL_PLAYER_X_SPRITES * TOTAL_PLAYER_Y_SPRITES];

	//Weather class
	Weather currentWeather;

	//Music temporary
	Mix_Music* intro = NULL;

	//Stores information based on tiles, used in the pathfinding algorithm to check allowable cells
	int grid[TOTAL_X_TILES][TOTAL_Y_TILES];

	//Pointers to all characters that need to be rendered
	vector<Character*> allCharacters;

	//Right now holding vectors of enemies/friendlies, will be needed later for game code
	vector<Enemy> enemies;
	vector<Friendly> friendlies;

	//Target Destination
	pair<int, int> destination;

	//Used for finding beginning and end of path
	Coord src;
	Coord dest;
	
	//Camera class, for updating position of screen in terms of the world
	Camera mainCamera;

	//Pathfinding class used for A* pathfinding between cells
	Pathfinding pathHandler;

	bool playerTurn;

	int currentEnemy;

	bool enemiesReady;
	//TTF_Font* mainFont;
	//Texture fontTexture;

};

