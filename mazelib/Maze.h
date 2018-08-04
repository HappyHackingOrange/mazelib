#pragma once

#include "MazeGenAlgo.h"
#include "MazeSolveAlgo.h"
#include <boost/algorithm/string/join.hpp>
#include <algorithm>

/*
This is a master object meant to hold a rectangular, 2D maze.
This object includes the methods used to generate and solve the maze,
as well as the start and end points.
*/
class Maze {

public:

	MazeGenAlgo * generator;
	xt::xarray<int8_t> grid;
	std::pair<int, int> start;
	std::pair<int, int> end;

	MazeSolveAlgo * solver;
	std::vector<std::vector<std::pair<int, int>>> solutions;

	Maze();
	~Maze();

	/*
	Public method to generate a new maze, and handle some clean-up
	*/
	void generate();

	/*
	Generate maze entrances.
	Entrances can be on the walls, or inside the maze.
	*/
	void generate_entrances(bool start_outer = true, bool end_outer = true);

	/*
	Use the Monte Carlo method to generate a maze of defined difficulty.
	This method assumes the generator and solver algorithms are already set.
	1. Generate a maze.
	2. For each maze, generate a series of entrances.
	3. To eliminate boring entrance choices, select only the entrances

	that yield the longest solution to a given maze.
	4. Repeat steps 1 through 3 for several mazes.
	5. Order the mazes based on a reduction function applied to their maximal
	solutions. By default, this reducer will return the solution length.
	6. Based on the 'difficulty' parameter, select one of the mazes.
	*/
	void generate_monte_carlo(int repeat, int entrances = 3, float difficulty = 1.0,
		int (*reducer)(std::vector<std::pair<int, int>>) = [](std::vector<std::pair<int, int>> solution) { return static_cast<int>(solution.size()); });

	/*
	Solves the maze.
	*/
	void solve();

	/*
	Return a string representation of the maze.
	*/
	std::string tostring(bool entrances = false, bool solutions = false, std::string delim = "\n");


protected:

	/*
	Generate maze entrances, along the outer walls.
	*/
	void generate_outer_entrances();

	/*
	Generate maze entrances, randomly within the maze.
	*/
	void generate_inner_entrances();

	/*
	Generate one inner and one outer entrance.
	*/
	std::pair<std::pair<int, int>, std::pair<int, int>> generate_opposite_entrances();

};
