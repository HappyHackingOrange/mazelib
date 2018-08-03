#include "maze.h"
#include "prims.h"

int main() {
	Maze m = Maze();
	m.generator = new Prims(8, 16);
	m.generate();
	std::cout << m.tostring() << std::endl;
}