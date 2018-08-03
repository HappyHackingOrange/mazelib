#include "maze.h"
#include "prims.h"

int main() {
	Maze m = Maze();
	m.generator = new Prims(16, 53);
	m.generate();
	std::cout << m.tostring() << std::endl;
}