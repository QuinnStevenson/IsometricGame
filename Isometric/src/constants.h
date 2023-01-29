#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

using namespace std;

const int TOTAL_PARTICLES = 20;

//Screen dimension constants
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

//The dimensions of the level
const int LEVEL_WIDTH = 3000;
const int LEVEL_HEIGHT = 3000;

//Tile constants
const int TILE_WIDTH = 128;
const int TILE_HEIGHT = 128;
const int TOTAL_TILES = 225;
const int TOTAL_TILE_SPRITES = 3;
const int TILE_LEVEL_WIDTH = 1920;
const int TILE_LEVEL_HEIGHT = 480;
const int TOTAL_X_TILES = 15;
const int TOTAL_Y_TILES = 15;
const int YOFFSET = SCREEN_HEIGHT / 4 - TILE_HEIGHT + 1;
const int XOFFSET = (LEVEL_WIDTH / 2) - TILE_WIDTH;

//Player sprite constants
const int TOTAL_PLAYER_X_SPRITES = 8;
const int TOTAL_PLAYER_Y_SPRITES = 4;

//The player's width and height
static const int PLAYER_WIDTH = 104;
static const int PLAYER_HEIGHT = 80;

//Rain constants
const int RAIN_WIDTH = 9;
const int RAIN_HEIGHT = 9;
const int TOTAL_RAIN_DROPS = 500;
const int TOTAL_RAIN_SPRITES = 8;

//Rain Sprites
const int RAIN_FALL = 0;
const int RAIN_TWO = 1;
const int RAIN_THREE = 2;
const int RAIN_FOUR = 3;
const int RAIN_FIVE = 4;
const int RAIN_SIX = 5;
const int RAIN_SEVEN = 6;
const int RAIN_EIGHT = 7;

//Framerate constants
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//The different tile sprites
const int TILE_EMPTY = 0;
const int TILE_GREY = 1;
const int TILE_YELLOW = 2;