#pragma once

#include "View.h"
#include <chrono>
#include <codecvt>

#define NOMINMAX
#include <Windows.h>


class CommandLineFPS : public View {

public:

	int screenWidth = 120;				// Console Screen Size X (columns)
	int screenHeight = 40;				// Console Screen Size Y (rows)
	float FOV = PI_F / 4;				// Field of View
	float Depth = 16.0f;				// Maximum rendering distance
	bool displayStats = true;			// Do you want to see statistics info?
	bool showMinimap = true;			// Display minimap?
	int minimapWidth = 25;				// Minimap width
	int minimapHeight = 13;				// Minimap height

	CommandLineFPS(Maze &maze);
	void start();

};

