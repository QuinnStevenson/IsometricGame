#include "GameHandler.h"

GameHandler::GameHandler()
{
	src = make_pair(0, 0);
	dest = make_pair(0, 0);
	keystate = SDL_GetKeyboardState(NULL);
	playerTurn = true;
	currentEnemy = 0;
	enemiesReady = false;
}

//Initializing 2d vector of tiles for the game.  Also handles setting up sprite clips
bool GameHandler::setTiles(vector<vector<Tile*>>& tiles)
{
	bool success = true;
	int x, y;

	std::ifstream map("maps/map1.map");

	if (!map.is_open())
	{
		cout << "Unable to load map data from file!" << endl;
		success = false;
	}
	else
	{
		//Initializing tiles
		for (int i = 0; i < TOTAL_Y_TILES; i++)
		{
			for (int j = 0; j < TOTAL_X_TILES; j++)
			{
				int tileType = -1;

				map >> tileType;

				grid[j][i] = tileType;

				x = XOFFSET + (j - i) * TILE_WIDTH / 2;
				y = YOFFSET + (j + i) * TILE_HEIGHT / 4;

				tiles[j][i] = new Tile(x, y, tileType, tileTexture);

				if (map.fail())
				{
					cout << "Error with map: unexpected end of file!" << endl;
					success = false;
					break;
				}
			}
		}

		//Setting sprite clips
		if (success)
		{
			for (int i = 0; i < TOTAL_TILE_SPRITES; ++i)
			{
				tileClips[i].x = i * TILE_WIDTH;
				tileClips[i].y = 0;
				tileClips[i].w = TILE_WIDTH;
				tileClips[i].h = TILE_HEIGHT;
			}


			for (int i = 0; i < TOTAL_PLAYER_Y_SPRITES; ++i)
			{
				for (int j = 0; j < TOTAL_PLAYER_X_SPRITES; ++j)
				{
					playerClips[j + TOTAL_PLAYER_X_SPRITES * i].x = j * PLAYER_WIDTH;
					playerClips[j + TOTAL_PLAYER_X_SPRITES * i].y = i * PLAYER_HEIGHT;
					playerClips[j + TOTAL_PLAYER_X_SPRITES * i].w = PLAYER_WIDTH;
					playerClips[j + TOTAL_PLAYER_X_SPRITES * i].h = PLAYER_HEIGHT;
				}
			}
		}
	}

	map.close();

	return success;
}

//Loading textures and media
bool GameHandler::loadMedia(vector<vector<Tile*>>& tiles)
{
	bool success = true;

	if (!tileTexture.loadFromFile("sprites/tiles/128x128/tilesheet.png", gRenderer))
	{
		cout << "Failed to load the tile texture!" << endl;
		success = false;
	}

	if (!setTiles(tiles))
	{
		cout << "Unable to load the tile set!" << endl;
		success = false;
	}

	intro = Mix_LoadMUS("music/intro.mp3");
	if (intro == NULL)
	{
		cout << "Music failed to load" << endl;
	}

	//Generally rendering ttf font text is costly.  Appropriate method would be to create a font atlas and call upon letters when building sentences and words.
	/*
	if(!fontTexture.loadFromRenderedText("fonts/8-bit pusab.ttf", gRenderer, mainFont, gRenderer))
	{
		cout << "Failed to initialize true type font!" << endl;
		success = false;
	}
	*/
	
	return success;
}

//Initializing SDL2 tools
bool GameHandler::init()
{
	bool success = true;

	//Initializing SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "SDL Could not initialized!: " << SDL_GetError() << endl;
		success = false;
	}
	else {
		//Sets texture filtering to nearest pixel
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"))
		{
			cout << "Warning: Nearest pixel texture filtering not enabled!" << endl;
			success = false;
		}

		//Creating the window
		gWindow = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			cout << "Failure to create window!: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//Initialized the renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

			if (gRenderer == NULL)
			{
				cout << "Failure to initialize the renderer!:" << SDL_GetError() << endl;
				success = false;
			}
			else
			{
				//Initial render colour
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;

				//Enables the use for png loading
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not be initialized!: " << SDL_GetError() << endl;
					success = false;
				}

				//Enables the use of true type fonts
				if (TTF_Init() == -1)
				{
					cout << "TTF could not be initialized!: " << SDL_GetError() << endl;
					success = false;
				}

				//Enables the use of audio
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					cout << "SDL_Mixer could not initialize! SDL_mixer error: " << SDL_GetError() << endl;
					success = false;
				}
			}
		}
	}

	return success;
}

//Startup function to load all utilities for the game
bool GameHandler::startup(vector<vector<Tile*>>& tileMap)
{
	bool success = true;

	if (init())
		cout << "SDL tools loaded!" << endl;
	else
		cout << "SDL tools failed to load!" << endl; success = false;

	if (loadMedia(tileMap))
		cout << "Loaded media!" << endl;
	else
		cout << "Failed to load appropriate media!" << endl; success = false;

	if (setTiles(tileMap))
		cout << "Tile have been set!" << endl;
	else
		cout << "Failed to set tiles!" << endl; success = false;

	return success;
}

//Function initializes characters for the game
void GameHandler::initializeUnits()
{
	int totalUnits = 6;
	
	Character* newCharacter;

	srand((unsigned int)time(NULL));

	//Generating friendly and enemy units.  Later will need to be reworked
	for (int i = 0; i < totalUnits; i++)
	{
		int randX = rand() % (TOTAL_X_TILES);
		int randY = rand() % (TOTAL_Y_TILES);

		if (grid[randX][randY] == 0)
		{
			i--;
		}
		else
		{
			if (i < 3) 
			{
				friendlies.push_back(Friendly(gRenderer));
				friendlies[i].setPosition(make_pair(randX, randY));
				friendlies[i].setyFrame(0);

			}
			else
			{
				enemies.push_back(Enemy(gRenderer));
				enemies[i - 3].setPosition(make_pair(randX, randY));
				enemies[i - 3].setyFrame(0);
			}

			//Need a way to track which tiles are currently occupied by characters
			//grid[randX][randY] = 0;
		}

		insertionSort();
	}

	//Pushes all entities to the allCharacters vector
	for (int i = 0; i < totalUnits; i++)
	{
		if (i < 3)
		{
			newCharacter = &friendlies[i];
			allCharacters.push_back(newCharacter);
		}
		else
		{
			newCharacter = &enemies[i - 3];
			allCharacters.push_back(newCharacter);
		}
	}
}

//Handles rendering everything to the screen
void GameHandler::handleRender(vector<vector<Tile*>> tileMap)
{
	//Render Tiles back-to-front
	for (int i = 0; i < TOTAL_Y_TILES; ++i)
	{
		for (int j = 0; j < TOTAL_X_TILES; ++j)
		{
			tileMap[i][j]->render(mainCamera.getCamera(), gRenderer, tileClips);
		}
	}

	//Render characters
	for (int i = 0; i < allCharacters.size(); i++)
		allCharacters[i]->render(mainCamera.getCamera(), gRenderer, playerClips);


	//Renders the current weather
	if (currentWeather.getActivated())
		currentWeather.renderParticles(mainCamera.getCamera(), gRenderer);

	SDL_RenderPresent(gRenderer);
}

//Handles all events from the user
void GameHandler::handleEvents(vector<vector<Tile*>> tileMap, bool &quit)
{
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
			quit = true;

		if (playerTurn)
		{
			if (e.type == SDL_KEYDOWN)
				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN)
					playerTurn = false;

			mainCamera.handleEvent(e, keystate, gRenderer, gWindow);

			//Handles mouse over tiles and clicking tiles
			for (int i = 0; i < TOTAL_Y_TILES; ++i)
				for (int j = 0; j < TOTAL_X_TILES; ++j)
					tileMap[i][j]->handleEvent(e, mainCamera, destination);

			for (int i = 0; i < allCharacters.size(); i++)
				allCharacters[i]->handleEvent(e, keystate, pathHandler, grid, mainCamera.getCamera(), destination);
		}


		//Press 8 for rain, 9 for snow, 0 to stop
		currentWeather.handleEvent(e, gRenderer, mainCamera.getCamera());
	}
}

void GameHandler::insertionSort()
{
	int currentY, i, j;

	for (i = 1; i < allCharacters.size(); i++)
	{
		currentY = allCharacters[i]->getY() + PLAYER_HEIGHT;

		j = i - 1;

		while (j >= 0 && allCharacters[j]->getY() + PLAYER_HEIGHT > currentY)
		{
			swap(allCharacters[j + 1], allCharacters[j]);
			j = j - 1;
		}
	}
}

//Function to deallocate memory and free anything in use
void GameHandler::close(vector<vector<Tile*>>& tiles)
{
	//Deallocate tiles
	for (int i = 0; i < TOTAL_Y_TILES; ++i)
		for (int j = 0; j < TOTAL_X_TILES; j++)
			delete tiles[i][j];

	enemies.clear();
	friendlies.clear();
	allCharacters.clear();

	//Free Music
	Mix_FreeMusic(intro);
	intro = NULL;

	//Free Textures
	tileTexture.free();

	//Free SDL tools
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);

	gRenderer = NULL;
	gWindow = NULL;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}


//Initial function call to start the game
void GameHandler::begin()
{
	vector<vector<Tile*>> tileMap(TOTAL_Y_TILES, vector<Tile*>(TOTAL_X_TILES, NULL));

	startup(tileMap);

	initializeUnits();

	bool quit = false;

	while (!quit)
	{
		//if (Mix_PlayingMusic() == 0)
		//{
		//	Mix_PlayMusic(intro, -1);
		//}

		//Clearing the renderer
		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(gRenderer);

		//Function call handling event input logic
		handleEvents(tileMap, quit);

		//Handles enemy AI
		if (!playerTurn)
		{
			if (currentEnemy == enemies.size()) {
				playerTurn = true;
				enemiesReady = false;
			}
			else 
			{
				if (!enemiesReady) {
					for (int i = 0; i < enemies.size(); i++)
					{
						enemies[i].findClosestUnit(friendlies, pathHandler, grid);
					}
				}

				enemiesReady = true;

				enemies[currentEnemy].setCamera(mainCamera.getCamera());

				if (enemies[currentEnemy].getMoving())
					enemies[currentEnemy].moveCommand(allCharacters, currentEnemy);
				else
					currentEnemy++;
			}
		}

		//Handles moving the characters
		if (playerTurn)
		{
			for (int i = 0; i < allCharacters.size(); i++)
				if (allCharacters[i]->getMoving())
					allCharacters[i]->moveCommand(allCharacters, i);
		}

		//Function call to render the to screen
		handleRender(tileMap);
	}

	close(tileMap);
}
