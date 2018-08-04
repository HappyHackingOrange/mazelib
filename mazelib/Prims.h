#pragma once

#include "MazeGenAlgo.h"
#include <set>

/*
The Algorithm
1. Choose an arbitrary cell from the grid, and add it to some
(initially empty) set visited nodes (V).
2. Randomly select a wall from the grid that connects a cell in
V with another cell not in V.
3. Add that wall to the Minimal Spanning Tree (MST), and the edge's other cell to V.
4. Repeat steps 2 and 3 until V includes every cell in G.
*/
class Prims : public MazeGenAlgo {

public:
	Prims(int h, int w, bool hasBounds = true);
	xt::xarray<int8_t> generate() override;

};

