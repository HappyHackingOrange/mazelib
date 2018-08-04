#include "Maze.h"
#include "Prims.h"
#include "Collision.h"
#include "CommandLineFPS.h"

int main() {
	Maze maze = Maze();
	maze.generator = new Prims(8, 16);
	maze.generator->hasBounds = false;
	maze.generate();
	maze.solver = new Collision();
	maze.generate_entrances(false, false);
	maze.solve();
	View * view = new CommandLineFPS(maze);
	view->player.isComputer = true;
	view->start();
	delete view;
}