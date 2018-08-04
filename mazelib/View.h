#pragma once

#include "Maze.h"
#include "Player.h"

class View {

public:
	Maze &maze;
	Player player = Player();
	View(Maze &maze) : maze(maze), player(Player()) {}
	virtual void start() = 0;

};