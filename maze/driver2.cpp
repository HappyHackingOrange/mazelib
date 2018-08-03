#include "Maze.h"
#include "Prims.h"
#include "Collision.h"

int main() {
	Maze m = Maze();
	m.generator = new Prims(8, 16);
	m.generate();
	std::cout << m.tostring() << std::endl;
	std::cout << std::endl;
	m.solver = new Collision();
	m.generate_entrances();
	m.solve();
	std::cout << m.tostring(true, true) << std::endl;
}