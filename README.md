# mazelib C++ library

The mazelib library provides tools to generate and solving 2D mazes, and 3D engines to visualize mazes in 3D and allow an user to interact with it. Most algorithms come with opinional parameters to customize the results.

This library only supports C++17 version or higher.

![](images/mazeExample.gif){:height="50%" width="50%"}

## Quickstart

### Creating a maze

Here's a simple example to generate a maze:

	#include "maze.h"
	#include "prims.h"

	int main() {
		Maze m = Maze();
		m.generator = new Prims(8, 16);
		m.generate();
		std::cout << m.tostring() << std::endl;
	}

A generating algorithm is selected, the Prim's algorithm in this case, to generate a random maze which is 8 high and 16 wide.  The above code should print a maze which should look something like this below:

	#################################
	#.#...#.....#.#.#.....#.#.#.....#
	#.###.###.#.#.#.#####.#.#.###.#.#
	#.......#.#...........#.#...#.#.#
	###.###.###.###.###.#.#.#.#####.#
	#...#.........#...#.#.......#.#.#
	#.#.#.#.#.#.#.#.#####.#.#.###.#.#
	#.#.#.#.#.#.#.#.#...#.#.#.......#
	#.#####.#.#.###.###.#.#########.#
	#.#.....#.#.#...#.#...#.........#
	#.#.###.#####.###.#.#########.###
	#.#...#...#.#...#...#...#.#...#.#
	#.#.#.#.#.#.#.###.#####.#.#.#.#.#
	#.#.#.#.#.#...#.....#.#.....#...#
	#.#####.###.###.#####.#.#.#.#.#.#
	#...#.....#.#...#.......#.#.#.#.#
	#################################

### Solving the maze

To solve the maze using one of the solving algorithms (Collision algorithm in this case), just add in few lines:

	#include "Maze.h"
	#include "Prims.h"
	#include "Collision.h"

	int main() {
		Maze m = Maze();
		m.generator = new Prims(8, 16);
		m.generate();
		m.solver = new Collision();
		m.generate_entrances();
		m.solve();
		std::cout << m.tostring(true, true) << std::endl;
	}

This code should print the following maze, which shows the starting position "S", the ending position "E", and the solution path which is a trail of "+"s.

	#################E###############
	#.#...#.....#.#.#+++++#.#.#.....#
	#.###.###.#.#.#.#####+#.#.###.#.#
	#.......#.#..........+#.#...#.#.#
	###.###.###.###.###.#+#.#.#####.#
	#...#.........#...#.#+++++..#.#.#
	#.#.#.#.#.#.#.#.#####.#.#+###.#.#
	#.#.#.#.#.#.#.#.#...#.#.#+++++++#
	#.#####.#.#.###.###.#.#########+#
	#.#.....#.#.#...#.#...#......+++#
	#.#.###.#####.###.#.#########+###
	#.#...#...#.#...#...#...#.#+++#.#
	#.#.#.#.#.#.#.###.#####.#.#+#.#.#
	#.#.#.#.#.#...#.....#.#....+#...#
	#.#####.###.###.#####.#.#.#+#.#.#
	#...#.....#.#...#.......#.#+#.#.#
	###########################S#####

