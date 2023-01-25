#pragma once
#include <stdio.h>
#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include "../constants.h"
#include "Cell.h"

using namespace std;

class Pathfinding
{
public:
	//Check to see if the cell is a valid cell inside the map
	bool isValid(int, int);

	//Check to see if the cell itself is a blocked cell, 0 means its blocked for now
	bool isUnblocked(int[][TOTAL_X_TILES], int, int);

	//Check to see if the source and destination cell is the same
	bool isDestination(int, int, Coord);

	//Calculates the hValue for the cell based on the destination cell, uses a distance formula
	double calculateHValue(int, int, Coord);

	//Will print the path that has been found
	stack<Coord> tracePath(cell[][TOTAL_X_TILES], Coord, stack<Coord>&);

	//Algortihm to find the best path
	stack<Coord> aStarSearch(int[][TOTAL_X_TILES], Coord, Coord, stack<Coord>&);
};

