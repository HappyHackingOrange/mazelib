# mazelib C++ library

The mazelib library provides tools to generate and solving 2D mazes, and 3D engines to visualize mazes in 3D and allow an user to interact with it.

This library only supports C++17 version or higher.

## Quickstart

### Creating a maze

Here's a simple C++ code example to generate a maze:

	#include "maze.h"
	#include "prims.h"

	int main() {
		Maze m = Maze();
		m.generator = new Prims(8, 16);
		m.generate();
		std::cout << m.tostring() << std::endl;
	}

A generating algorithm is selected, the Prim's algorithm in this case, to generate a random maze which is 8 high and 16 wide.  The maze should look something like this:

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

To solve the maze, you can add few lines like this:

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

This code prints the following maze:

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

where S is the starting position, E is the ending position, and the sequence of "+"s is one possible route from the beginning to the end.