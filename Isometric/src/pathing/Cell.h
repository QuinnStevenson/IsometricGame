#pragma once

//Struct to hold information for individual cells
//Used for A* pathfinding

//Holds the pair coordinates for cells
typedef pair<int, int> Coord;

//Shortcut for the coord type, double value holds the f score
typedef pair<double, pair<int, int>> pCoord;

//Struct for each of the individual cells
struct cell
{
	//X and Y index of its parent cell
	int parent_i, parent_j;
	//Algorithm score, f = g + h
	double f, g, h;
};