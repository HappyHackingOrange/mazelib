#include "Maze.h"
#include "Prims.h"
#include "Collision.h"
#include "CommandLineFPS.h"

int main() {
	int h = 8, w = 16;
	Maze maze = Maze();
	maze.solver = new Collision();

	//while (true){
		maze.generator = new Prims(h, w, false);
		maze.generate();
		maze.generate_entrances(false, false);
		maze.solve();
		View * view = new CommandLineFPS(maze);
		view->player.isComputer = true;
		view->start();
		delete view;
		h++, w++;
	//}
}