#include "Pathfinding.h"

//Logic for the pathfinding is very straightforward.  It is irrelevant to the games space since we are simply moving on the grid and the world is an isometric projection
//Using only the grid coordinates for now in movement.  This implies that all tiles (besides blocked ones) have the same weight for moving through.  
//This can be changed later to take in effect movement impairing tiles etc.


//Ensures cell is within the map
bool Pathfinding::isValid(int row, int col)
{
	return (row >= 0) && (row < TOTAL_Y_TILES) && (col >= 0) && (col < TOTAL_X_TILES);
}

//Checks if the cell is allowable
bool Pathfinding::isUnblocked(int map[][TOTAL_X_TILES], int row, int col)
{
	if (map[row][col] == 1)
		return true;
	else
		return false;
}

//Check to see if the source is at destination
bool Pathfinding::isDestination(int row, int col, Coord dest)
{
	if (row == dest.first && col == dest.second)
		return true;
	else
		return false;
}

//a^2 + b^2 = c^2
//Hvalue heuristic from destination
double Pathfinding::calculateHValue(int row, int col, Coord dest)
{
	return ((double)sqrt((row - dest.first) * (row - dest.first) + (col - dest.second) * (col - dest.second)));
}

//Function that will populate stack with coordinate pairs.
//Can mainly be used to debug as the path forms, returns the path
stack<Coord> Pathfinding::tracePath(cell cellDetails[][TOTAL_X_TILES], Coord dest, stack<Coord> &returnPath)
{

	int row = dest.first, col = dest.second;

	stack<Coord> Path;

	//Populate stack with values
	while (!(cellDetails[row][col].parent_i == row && cellDetails[row][col].parent_j == col))
	{
		Path.push(make_pair(row, col));
		returnPath.push(make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;
	}

	Path.push(make_pair(row, col));
	returnPath.push(make_pair(row, col));

	while (!Path.empty())
	{
		Path.pop();
	}

	return returnPath;
}

//AStar search algorithm, finding the best path
stack<Coord> Pathfinding::aStarSearch(int map[][TOTAL_X_TILES], Coord src, Coord dest, stack<Coord>& returnPath)
{
	//Base cases checked first

	//Source not in range
	if (isValid(src.first, src.second) == false)
	{
		cout << "INVALID SOURCE" << endl;
		return returnPath;
	}

	//Destination not in range
	if (isValid(dest.first, dest.second) == false)
	{
		cout << "INVALID DESTINATION" << endl;
		return returnPath;
	}

	//Blocked destination or source
	if (isUnblocked(map, src.first, src.second) == false || isUnblocked(map, dest.first, dest.second) == false)
	{	
		cout << "SOURCE OR DESTINATION IS BLOCKED" << endl;
		return returnPath;
	}

	//Check if at destination
	if (isDestination(src.first, src.second, dest) == true)
	{
		cout << "ALREADY AT THE DESTINATION" << endl;
		return returnPath;
	}

	bool closedList[TOTAL_Y_TILES][TOTAL_X_TILES];
	memset(closedList, false, sizeof(closedList));

	//2d array to store information for the specific cells
	cell cellDetails[TOTAL_Y_TILES][TOTAL_X_TILES];

	int i, j;

	//Initializing the 2d array of cells
	for (i = 0; i < TOTAL_Y_TILES; i++)
	{
		for (j = 0; j < TOTAL_X_TILES; j++)
		{
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	//Starting node
	i = src.first, j = src.second;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;
	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;

	//Open list set of pair of pairs
	set<pCoord> openList;

	//Initial cell receives an fscore of 0, since it's the source
	openList.insert(make_pair(0.0, make_pair(i, j)));

	//Will indicate if the destination has been found or not
	bool foundDest = false;

	//Handling different direction.  There are 8 potential paths that a cell can move to
	//However only vertical and horizontal movement is being used due to visuals
	//Diagonal movement can be incorporated later if a more visually pleasing way is found
	//NORTH			(i - 1, j)
	//SOUTH			(i + 1, j)
	//EAST			(i, j + 1)
	//WEST			(i, j - 1)
	//NORTH-EAST	(i - 1, j + 1)
	//NORTH-WEST	(i - 1, j - 1)
	//SOUTH-EAST	(i + 1, j + 1)
	//SOUTH-WEST	(i + 1, j - 1)


	while (!openList.empty())
	{
		pCoord p = *openList.begin();

		//Removing the vertex from the open list
		openList.erase(openList.begin());

		//adding the vertex to the closed list
		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		double gNew, hNew, fNew;

		//NORTH
		if (isValid(i - 1, j) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i - 1, j, dest) == true)
			{
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i - 1][j] == false && isUnblocked(map, i - 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i - 1, j, dest);
				fNew = gNew = hNew;

				//Adding to openList
				if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j)));

					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;
				}
			}
		}

		//South
		if (isValid(i + 1, j) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i + 1, j, dest) == true)
			{
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i + 1][j] == false && isUnblocked(map, i + 1, j) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i + 1, j, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i + 1][j].f == FLT_MAX || cellDetails[i + 1][j].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j)));

					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;
				}
			}
		}

		//East
		if (isValid(i, j + 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i, j + 1, dest) == true)
			{
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i][j + 1] == false && isUnblocked(map, i, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j + 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i][j + 1].f == FLT_MAX || cellDetails[i][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i, j + 1)));

					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;
				}
			}
		}

		//West
		if (isValid(i, j - 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i, j - 1, dest) == true)
			{
				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i][j - 1] == false && isUnblocked(map, i, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.0;
				hNew = calculateHValue(i, j - 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i][j - 1].f == FLT_MAX || cellDetails[i][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i, j - 1)));

					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;
				}
			}
		}

		/*
		//NORTH-EAST
		if (isValid(i - 1, j + 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i - 1, j + 1, dest) == true)
			{
				cellDetails[i - 1][j + 1].parent_i = i;
				cellDetails[i - 1][j + 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i - 1][j + 1] == false && isUnblocked(map, i - 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j + 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i - 1][j + 1].f == FLT_MAX || cellDetails[i - 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j + 1)));

					cellDetails[i - 1][j + 1].f = fNew;
					cellDetails[i - 1][j + 1].g = gNew;
					cellDetails[i - 1][j + 1].h = hNew;
					cellDetails[i - 1][j + 1].parent_i = i;
					cellDetails[i - 1][j + 1].parent_j = j;
				}
			}
		}

		//NORTH-WEST
		if (isValid(i - 1, j - 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i - 1, j - 1, dest) == true)
			{
				cellDetails[i - 1][j - 1].parent_i = i;
				cellDetails[i - 1][j - 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}

			else if (closedList[i - 1][j - 1] == false && isUnblocked(map, i - 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i - 1, j - 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i - 1][j - 1].f == FLT_MAX || cellDetails[i - 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i - 1, j - 1)));

					cellDetails[i - 1][j - 1].f = fNew;
					cellDetails[i - 1][j - 1].g = gNew;
					cellDetails[i - 1][j - 1].h = hNew;
					cellDetails[i - 1][j - 1].parent_i = i;
					cellDetails[i - 1][j - 1].parent_j = j;
				}
			}
		}

		//SOUTH-EAST
		if (isValid(i + 1, j + 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i + 1, j + 1, dest) == true)
			{
				cellDetails[i + 1][j + 1].parent_i = i;
				cellDetails[i + 1][j + 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}

			else if (closedList[i + 1][j + 1] == false && isUnblocked(map, i + 1, j + 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j + 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i + 1][j + 1].f == FLT_MAX || cellDetails[i + 1][j + 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j + 1)));

					cellDetails[i + 1][j + 1].f = fNew;
					cellDetails[i + 1][j + 1].g = gNew;
					cellDetails[i + 1][j + 1].h = hNew;
					cellDetails[i + 1][j + 1].parent_i = i;
					cellDetails[i + 1][j + 1].parent_j = j;
				}
			}
		}

		//SOUTH-WEST
		if (isValid(i + 1, j - 1) == true)
		{
			//Base case, found the destination cell
			if (isDestination(i + 1, j - 1, dest) == true)
			{
				cellDetails[i + 1][j - 1].parent_i = i;
				cellDetails[i + 1][j - 1].parent_j = j;
				cout << "Destination cell found!" << endl;
				tracePath(cellDetails, dest, returnPath);
				foundDest = true;
				return returnPath;
			}
			else if (closedList[i + 1][j - 1] == false && isUnblocked(map, i + 1, j - 1) == true)
			{
				gNew = cellDetails[i][j].g + 1.414;
				hNew = calculateHValue(i + 1, j - 1, dest);
				fNew = gNew + hNew;

				//Adding to openList
				if (cellDetails[i + 1][j - 1].f == FLT_MAX || cellDetails[i + 1][j - 1].f > fNew)
				{
					openList.insert(make_pair(fNew, make_pair(i + 1, j - 1)));

					cellDetails[i + 1][j - 1].f = fNew;
					cellDetails[i + 1][j - 1].g = gNew;
					cellDetails[i + 1][j - 1].h = hNew;
					cellDetails[i + 1][j - 1].parent_i = i;
					cellDetails[i + 1][j - 1].parent_j = j;
				}
			}
		}
		*/
	}

	//If the destination cannot be found, the search will end here
	if (foundDest == false)
		cout << "Could not find a path to the destination!" << endl;

	return returnPath;
}


