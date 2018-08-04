#pragma once

#include "MazeGenAlgo.h"

/*
1. Choose a random cell.
2. Choose a random neighbor of the current cell and visit it. If the neighbor has not
	yet been visited, add the traveled edge to the spanning tree.
3. Repeat step 2 until all cells have been visited.
*/
class AldousBroder : public MazeGenAlgo {

public:

	AldousBroder(int h, int w, bool hasBounds = true);
	xt::xarray<int8_t> generate() override;

};

