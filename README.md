# mazelib C++ library

The mazelib library provides tools to generate and solving 2D mazes, and 3D engines to visualize mazes in 3D and interact with it.

## Creating a maze

Here's a simple C++ example to generate a maze:

	#include "maze.h"
	#include "prims.h"

	int main() {
		Maze m = Maze();
		m.generator = new Prims(8, 10);
		m.generate();
	}

The Maze object contains:
* Maze
* grid: maze representation in 2D array of data
* start: starting position in the maze
* end: ending position in the maze
* generator: algorithm to generate the maze
* solver: algorithm to solve the maze
* solution: list of solution paths
	
