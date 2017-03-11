#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Branch structure, takes instance of maze when reaching a branch and allows return to it once current path has been fully explored
struct Branch
{
	int x;
	int y;
	int length; // Current path length at time of reaching branch
	int paths; // Number of directions left open in the branch, branch is deleted once this number reaches 0
	int mazeInstance[22][22];
	Branch* next; // the next newest branch reached
	Branch* prev; // The next oldest branch reached
};

bool checkDirection(char dir, int maze[22][22], int x, int y, Branch* branch);

int longestPath(int maze[22][22], int startX, int startY, int endX, int endY) // Finds the longest possible path from the start point in the maze to the end point
{
	if (startX < 1 || startX > 20 || startY < 0 || startY > 20) // The start point cannot be on the border of the array, according to the specifications
	{
		throw "Error: Cannot start outside of the board or on the edge";
	}
	else if (endX < 1 || endX > 20 || endY < 0 || endY > 20) // The end point cannot be on the border of the array, according to the specifications
	{
		throw "Error: Cannot set the end cell outside of the board or on the edge";
	}
	else if (maze[startY][startX] == 0)
	{
		throw "Error: Cannot start the maze on a wall tile";
	}
	else if (maze[endY][endX] == 0)
	{
		return -1;
	}

	srand(time(NULL));

	// Setting initial variables
	bool canUp = false;
	bool canDown = false;
	bool canLeft = false;
	bool canRight = false;
	int numDir = 0;
	int dir = 0;
	int length = 1;
	int longestPath = 0;

	int x = startX;
	int y = startY;

	// Creates a copy of the original maze that will mark the traveled cells as walled off
	int visitedMaze[22][22];
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			visitedMaze[i][j] = maze[i][j];
		}
	}

	// Branch pointers, for the purpose of keeping track of the oldest and newest branches
	Branch* elderBranch = 0;
	Branch* newBranch = 0;
	Branch* curBranch = 0;

	while (true) // Traversing the maze itself
	{
		length++;
		canUp = false;
		canDown = false;
		canLeft = false;
		canRight = false;

		if (x == 1) // Checking if each direction is traversable, ignoring certain directions depending on the current location. Uses the checkDirection function
		{
			canRight = checkDirection('r', visitedMaze, x, y, newBranch);
		}
		else if (x == 20)
		{
			canLeft = checkDirection('l', visitedMaze, x, y, newBranch);
		}
		else
		{
			canRight = checkDirection('r', visitedMaze, x, y, newBranch);
			canLeft = checkDirection('l', visitedMaze, x, y, newBranch);
		}

		if (y == 1)
		{
			canDown = checkDirection('d', visitedMaze, x, y, newBranch);
		}
		else if (y == 20)
		{
			canUp = checkDirection('u', visitedMaze, x, y, newBranch);
		}
		else
		{
			canUp = checkDirection('u', visitedMaze, x, y, newBranch);
			canDown = checkDirection('d', visitedMaze, x, y, newBranch);
		}

		if (canUp)
			numDir++;
		if (canDown)
			numDir++;
		if (canLeft)
			numDir++;
		if (canRight)
			numDir++;

		if (numDir == 0) // If there is nowhere left to go
		{
			if (elderBranch == 0) // If there are no branches to return to, all possible cells have been explored. Otherwise, reset to the newest branch
			{
				if (longestPath != 0) // If the end was found previously, return the longest path, else, return -1
				{
					return longestPath;
				}
				return -1;
			}

			x = newBranch->x;
			y = newBranch->y;
			length = newBranch->length - 1; // Because length is iterated at the beginning of each loop and would be one too high otherwise

			for (int i = 0; i < 22; i++) // Resetting visited maze to the branch instance
			{
				for (int j = 0; j < 22; j++)
				{
					visitedMaze[i][j] = newBranch->mazeInstance[i][j];
				}
			}
		}
		else if (numDir == 1) // If there is only one direction to traverse
		{
			if (newBranch != 0) // If a branch exists
			{
				if (newBranch->x == x && newBranch->y == y) // If the current location is the newest branch location. After the last direction is traversed, the number of paths will become 0, so the branch needs to be deleted
				{
					curBranch = elderBranch;
					while (curBranch != 0) // Iterate through branches starting with the oldest
					{
						if (curBranch->x == x && curBranch->y == y)
						{
							if (curBranch == elderBranch) // If the current branch is the oldest branch
							{
								if (elderBranch->next != 0) // If the oldest branch is not the only branch
								{
									// The next oldest branch becomes the elderbranch and the previous elderbranch is deleted
									elderBranch = elderBranch->next;
									delete curBranch;
									elderBranch->prev = 0;
									curBranch = 0;
									break;
								}
								else // If the oldest branch is the only branch, which it should be. The branch is deleted and set to 0 so it is made NULL instead of being filled with random values
								{
									delete elderBranch;
									elderBranch = 0;
									break;
								}
							}
							else if (curBranch == newBranch) // If the current branch is the newest branch. The next newest branch becomes newbranch and the previous newbranch is deleted
							{
								newBranch = newBranch->prev;
								delete curBranch;
								newBranch->next = 0;
								curBranch = 0;
								break;
							}
							else // Catch in case something goes wrong, should never be used
							{
								curBranch->prev->next = curBranch->next;
								curBranch->next->prev = curBranch->prev;
								delete curBranch;
								curBranch = 0;
								break;
							}
						}
						curBranch = curBranch->next; // How the branches are iterated
					}
					curBranch = 0; // Resetting the variable for future use
				}
			}

			visitedMaze[y][x] = 0; // Setting the current location to 0, thereby walling it off

			// Traversing in the new direction
			if (canUp)
				y = y - 1;
			else if (canDown)
				y = y + 1;
			else if (canLeft)
				x = x - 1;
			else if (canRight)
				x = x + 1;
		}
		else // If there are multiple possible directions to travel in, a branch is reached
		{
			visitedMaze[y][x] = 0; // Set branch location to 0, so the program cannot accidentally return to it by looping around

			if (elderBranch == 0) // If no branch exists, create one
			{
				elderBranch = new Branch;
				elderBranch->x = x;
				elderBranch->y = y;
				elderBranch->length = length;
				elderBranch->paths = numDir;
				elderBranch->next = 0;
				elderBranch->prev = 0;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						elderBranch->mazeInstance[i][j] = visitedMaze[i][j];
					}
				}

				newBranch = elderBranch;
			}
			else if (newBranch->x != x || newBranch->y != y) // If this branch is not already an existsing branch, create a new branch
			{
				newBranch->next = new Branch;
				newBranch->next->prev = newBranch;
				newBranch = newBranch->next;
				newBranch->x = x;
				newBranch->y = y;
				newBranch->length = length;
				newBranch->paths = numDir;
				newBranch->next = 0;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						newBranch->mazeInstance[i][j] = visitedMaze[i][j];
					}
				}
			}

			dir = rand() % numDir; // Pseudo-randomly choosing which direction to travel in
			if (numDir == 3)
			{
				if (!canUp)
				{
					if (dir == 0)
						y = y + 1;
					else if (dir = 1)
						x = x - 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canDown)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						x = x - 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canLeft)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						y = y + 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canRight)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						y = y + 1;
					else if (dir = 2)
						x = x - 1;
				}
			}
			else // If only two directions are possible. Not random due to amount of possible variability of available directions
			{
				if (canUp)
					y = y - 1;
				else if (canDown)
					y = y + 1;
				else if (canLeft)
					x = x - 1;
				else if (canRight)
					x = x + 1;
			}

			newBranch->mazeInstance[y][x] = 2; // Special value for map that blocks off directions from brach cells, does not block the cells from other locations. Stops repeat paths
			newBranch->paths--;
		}

		if (x == endX && y == endY) // If the end has been reached
		{
			if (longestPath < length) // If the current path is longer than the longest path, change the longest path length
			{
				longestPath = length;
			}
			if (elderBranch == 0) // If no more branches exist to check for new paths with, return the longest path
			{
				return longestPath;
			}
			else if (elderBranch->y == y && elderBranch->x == x) // If the oldest is on the endpoint, return the longest path because no other path can go back to the endpoint as it is now marked 0
			{
				return longestPath;
			}
			else if (newBranch->y == y && newBranch->x == x) // If the newest branch is on the endpoint
			{
				// Setting the current values to the branch values, just in case something goes wrong
				x = newBranch->x;
				y = newBranch->y;
				length = newBranch->length - 1;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						visitedMaze[i][j] = newBranch->mazeInstance[i][j];
					}
				}

				// Deleting the branch as the endpoint cannot be reached from here again
				curBranch = newBranch;
				newBranch = newBranch->prev;
				delete curBranch;
				newBranch->next = 0;
				curBranch = 0;
			}
		}
		numDir = 0;
	}

	while (elderBranch != 0) // In case the program somehow breaks out of the while loop with a branch still in memory. Deletes the branches
	{
		delete elderBranch->prev;
		elderBranch = elderBranch->next;
	}

	return -1; // Catch all return
}

int shortestPath(int maze[22][22], int startX, int startY, int endX, int endY) // Finds the shortest possible path from the start point in the maze to the end point, mirrors longestpath for the most part
{
	if (startX < 1 || startX > 20 || startY < 0 || startY > 20)
	{
		throw "Error: Cannot start outside of the board or on the edge";
	}
	else if (endX < 1 || endX > 20 || endY < 0 || endY > 20)
	{
		throw "Error: Cannot set the end cell outside of the board or on the edge";
	}
	else if (maze[startY][startX] == 0)
	{
		throw "Error: Cannot start the maze on a wall tile";
	}
	else if (maze[endY][endX] == 0)
	{
		return -1;
	}

	srand(time(NULL));

	bool canUp = false;
	bool canDown = false;
	bool canLeft = false;
	bool canRight = false;
	int numDir = 0;
	int dir = 0;
	int length = 1;
	int shortestPath = 0; // Shortestpath variable instead of longestpath

	int x = startX;
	int y = startY;

	int visitedMaze[22][22];
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 22; j++)
		{
			visitedMaze[i][j] = maze[i][j];
		}
	}

	Branch* elderBranch = 0;
	Branch* newBranch = 0;
	Branch* curBranch = 0;

	while (true)
	{
		length++;
		canUp = false;
		canDown = false;
		canLeft = false;
		canRight = false;

		if (x == 1)
		{
			canRight = checkDirection('r', visitedMaze, x, y, newBranch);
		}
		else if (x == 20)
		{
			canLeft = checkDirection('l', visitedMaze, x, y, newBranch);
		}
		else
		{
			canRight = checkDirection('r', visitedMaze, x, y, newBranch);
			canLeft = checkDirection('l', visitedMaze, x, y, newBranch);
		}

		if (y == 1)
		{
			canDown = checkDirection('d', visitedMaze, x, y, newBranch);
		}
		else if (y == 20)
		{
			canUp = checkDirection('u', visitedMaze, x, y, newBranch);
		}
		else
		{
			canUp = checkDirection('u', visitedMaze, x, y, newBranch);
			canDown = checkDirection('d', visitedMaze, x, y, newBranch);
		}

		if (canUp)
			numDir++;
		if (canDown)
			numDir++;
		if (canLeft)
			numDir++;
		if (canRight)
			numDir++;

		if (numDir == 0)
		{
			if (elderBranch == 0)
			{
				if (shortestPath != 0)
				{
					return shortestPath; // Same as longestpath function, but returns shortest path instead of the longest
				}
				return -1;
			}

			x = newBranch->x;
			y = newBranch->y;
			length = newBranch->length - 1;

			for (int i = 0; i < 22; i++)
			{
				for (int j = 0; j < 22; j++)
				{
					visitedMaze[i][j] = newBranch->mazeInstance[i][j];
				}
			}
		}
		else if (numDir == 1)
		{
			if (newBranch != 0)
			{
				if (newBranch->x == x && newBranch->y == y)
				{
					curBranch = elderBranch;
					while (curBranch != 0)
					{
						if (curBranch->x == x && curBranch->y == y)
						{
							if (curBranch == elderBranch)
							{
								if (elderBranch->next != 0)
								{
									elderBranch = elderBranch->next;
									delete curBranch;
									elderBranch->prev = 0;
									curBranch = 0;
									break;
								}
								else
								{
									delete elderBranch;
									elderBranch = 0;
									break;
								}
							}
							else if (curBranch == newBranch)
							{
								newBranch = newBranch->prev;
								delete curBranch;
								newBranch->next = 0;
								curBranch = 0;
								break;
							}
							else
							{
								curBranch->prev->next = curBranch->next;
								curBranch->next->prev = curBranch->prev;
								delete curBranch;
								curBranch = 0;
								break;
							}
						}
						curBranch = curBranch->next;
					}
					curBranch = 0;
				}
			}

			visitedMaze[y][x] = 0;

			if (canUp)
				y = y - 1;
			else if (canDown)
				y = y + 1;
			else if (canLeft)
				x = x - 1;
			else if (canRight)
				x = x + 1;
		}
		else
		{
			visitedMaze[y][x] = 0;

			if (elderBranch == 0)
			{
				elderBranch = new Branch;
				elderBranch->x = x;
				elderBranch->y = y;
				elderBranch->length = length;
				elderBranch->paths = numDir;
				elderBranch->next = 0;
				elderBranch->prev = 0;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						elderBranch->mazeInstance[i][j] = visitedMaze[i][j];
					}
				}

				newBranch = elderBranch;
			}
			else if (newBranch->x != x || newBranch->y != y)
			{
				newBranch->next = new Branch;
				newBranch->next->prev = newBranch;
				newBranch = newBranch->next;
				newBranch->x = x;
				newBranch->y = y;
				newBranch->length = length;
				newBranch->paths = numDir;
				newBranch->next = 0;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						newBranch->mazeInstance[i][j] = visitedMaze[i][j];
					}
				}
			}

			dir = rand() % numDir;
			if (numDir == 3)
			{
				if (!canUp)
				{
					if (dir == 0)
						y = y + 1;
					else if (dir = 1)
						x = x - 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canDown)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						x = x - 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canLeft)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						y = y + 1;
					else if (dir = 2)
						x = x + 1;
				}
				else if (!canRight)
				{
					if (dir == 0)
						y = y - 1;
					else if (dir = 1)
						y = y + 1;
					else if (dir = 2)
						x = x - 1;
				}
			}
			else
			{
				if (canUp)
					y = y - 1;
				else if (canDown)
					y = y + 1;
				else if (canLeft)
					x = x - 1;
				else if (canRight)
					x = x + 1;
			}

			newBranch->mazeInstance[y][x] = 2;
			newBranch->paths--;
		}

		if (x == endX && y == endY) // Same as longestpath function, but stores shortest path instead of the longest
		{
			if (shortestPath > length || shortestPath == 0)
			{
				shortestPath = length;
			}
			if (elderBranch == 0)
			{
				return shortestPath;
			}
			else if (elderBranch->y == y && elderBranch->x == x)
			{
				return shortestPath;
			}
			else if (newBranch->y == y && newBranch->x == x)
			{
				x = newBranch->x;
				y = newBranch->y;
				length = newBranch->length - 1;

				for (int i = 0; i < 22; i++)
				{
					for (int j = 0; j < 22; j++)
					{
						visitedMaze[i][j] = newBranch->mazeInstance[i][j];
					}
				}

				curBranch = newBranch;
				newBranch = newBranch->prev;
				delete curBranch;
				newBranch->next = 0;
				curBranch = 0;
			}
		}
		numDir = 0;
	}

	while (elderBranch != 0)
	{
		delete elderBranch->prev;
		elderBranch = elderBranch->next;
	}

	return -1;
}

bool checkDirection(char dir, int maze[22][22], int x, int y, Branch* branch) // Checks if a direction for the current cell can be traveled in
{
	switch (dir)
	{
	case 'u': // Upward direction
		if (maze[y - 1][x] == 1) // If the cell above equals 1, it is traversable from everywhere
		{
			return true;
		}
		else if (maze[y - 1][x] == 2) // If the cell above is marked as a 2, it cannot be accessed from the current newest branch's location (this stops branches from repeating paths while still keeping the upward cell open for other cells)
		{
			if (branch != 0) // If the branch variable is not NULL
			{
				if (branch->x == x && branch->y == y) // If the current location equals the branch location, return the direction as untraversable. Otherwise, it can be traversed
					return false;
			}
			return true;
		}
		return false; // Catch all return for the 'u' case
	case 'd': // Downward direction
		if (maze[y + 1][x] == 1)
		{
			return true;
		}
		else if (maze[y + 1][x] == 2)
		{
			if (branch != 0)
			{
				if (branch->x == x && branch->y == y)
					return false;
			}
			return true;
		}
		return false;
	case 'l': // Leftward direction
		if (maze[y][x - 1] == 1)
		{
			return true;
		}
		else if (maze[y][x - 1] == 2)
		{
			if (branch != 0)
			{
				if (branch->x == x && branch->y == y)
					return false;
			}
			return true;
		}
		return false;
	case 'r': // Rightward direction
		if (maze[y][x + 1] == 1)
		{
			return true;
		}
		else if (maze[y][x + 1] == 2)
		{
			if (branch != 0)
			{
				if (branch->x == x && branch->y == y)
					return false;
			}
			return true;
		}
		return false;
	}
	return false; // Catch all return for all cases
}

//int main() // Main function used for testing the program
//{
//	int mazeDef[22][22]{ { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
//	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
//
//	printf("%d\n", longestPath(mazeDef, 1, 1, 6, 1));
//	printf("%d\n", longestPath(mazeDef, 1, 1, 1, 2));
//	printf("%d\n", longestPath(mazeDef, 3, 3, 4, 3));
//	printf("%d\n", longestPath(mazeDef, 3, 3, 3, 3));
//	printf("%d\n", longestPath(mazeDef, 1, 1, 1, 5));
//	printf("%d\n", shortestPath(mazeDef, 3, 3, 4, 3));
//}