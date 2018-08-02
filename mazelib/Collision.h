#pragma once

#include "MazeSolveAlgo.h"
#include <vector>
#include <utility>
#include <algorithm>

/*
1. step through the maze, flooding all directions equally
2. if two flood paths meet, create a wall where they meet
3. fill in all dead ends
4. repeat until there are no more collisions
*/
class Collision : public MazeSolveAlgo {

protected:

	std::vector<std::vector<std::pair<int, int>>> solve() override;

	/*
	from the start, flood the maze one cell at a time,
	keep track of where the water flows as paths through the maze
	*/
	std::vector<std::vector<std::pair<int, int>>> flood_maze(std::pair<int, int> start);

	/*
	Move all open paths forward one grid cell
	*/
	//auto one_time_step(std::vector<std::vector<std::pair<int, int>>> paths);
	std::vector<std::vector<std::pair<int, int>>> one_time_step(std::vector<std::vector<std::pair<int, int>>> paths);

	/*
	look through paths for collsions
	If a collision exists, build a wall in the maze at that point.
	*/
	std::vector<std::vector<std::pair<int, int>>> fix_collisions(std::vector<std::vector<std::pair<int, int>>> paths);

	/*
	Ensure the start and end are appropriately placed in the solution.
	*/
	std::vector<std::vector<std::pair<int, int>>> fix_entrances(std::vector<std::vector<std::pair<int, int>>> paths);

};

