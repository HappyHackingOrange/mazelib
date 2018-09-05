#include "Maze.h"
#include "Prims.h"
#include "Collision.h"
#include "CommandLineFPS.h"

int main() {
	int mazeHeight = 40, mazeWidth = 10;
	bool hasBounds = false;
	Maze maze = Maze();
	maze.generator = new Prims(mazeHeight, mazeWidth, hasBounds);
	maze.generate();
	maze.generate_entrances();
	maze.solver = new Collision();
	maze.solve();
	View * view = new CommandLineFPS(maze);
	view->player.isComputer = false;
	view->start();
	delete view;
}