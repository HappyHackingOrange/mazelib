# mazelib C++ library

The mazelib library provides tools to generate and solving 2D mazes, and 3D engines to visualize mazes in 3D and allow an user to interact with it.

## Quickstart

### Creating a maze

Here's a simple C++ code example to generate a maze:

	#include "maze.h"
	#include "prims.h"

	int main() {
		Maze m = Maze();
		m.generator = new Prims(16, 53);
		m.generate();
		std::cout << m.tostring() << std::endl;
	}

A generating algorihm is selected, the Prim's algorithm in this case, to generate a random maze which is 16 high and 53 wide.  The maze should look something like this:

![Generated 16x53 Maze](/images/generated_maze.png)