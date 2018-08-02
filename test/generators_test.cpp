#include "pch.h"
#include "..\mazelib\AldousBroder.h"
#include "..\mazelib\Prims.h"
#include "..\mazelib\Maze.h"

class GeneratorsTest : public testing::Test {

public:

	Maze * m;

	GeneratorsTest() {
		m = new Maze();
	}

	~GeneratorsTest() {
		delete m;
	}

};

/*
Algorithms should generate a maze with a solid boundary of walls.
*/
bool boundary_is_solid(xt::xarray<int8_t> grid) {

	// first row
	for (int col = 0; col < grid.shape()[1]; col++)
		if (grid(0, col) == 0)
			return false;

	// other rows
	for (int row = 1; row < grid.shape()[0]; row++)
		if (grid(row, 0) == 0 || grid(row, grid.shape()[1] - 1) == 0)
			return false;

	// last row
	for (int col = 0; col < grid.shape()[1]; col++)
		if (grid(grid.shape()[0] - 1, col) == 0)
			return false;

	return true;

}

/*
All of the (odd, odd) grid cells in a maze should be passages.
*/
bool all_passages_open(xt::xarray<int8_t> grid) {

	auto H = grid.shape()[0];
	auto W = grid.shape()[1];

	for (int r = 1; r < H; r += 2)
		for (int c = 1; c < W; c += 2)
			if (grid(r, c) == 1)
				return false;

	return true;

}

/*
All of the (even, even) grid cells in a maze should be walls.
*/
bool all_corners_complete(xt::xarray<int8_t> grid) {

	auto H = grid.shape()[0];
	auto W = grid.shape()[1];

	for (int r = 2; r < H; r += 2)
		for (int c = 2; c < W; c += 2)
			if (grid(r, c) == 0)
				return false;

	return true;

}

TEST_F(GeneratorsTest, TestAldousBroder) {

	m->generator = new AldousBroder(4, 5);
	m->generate();

	EXPECT_TRUE(boundary_is_solid(m->grid));
	EXPECT_TRUE(all_passages_open(m->grid));
	EXPECT_TRUE(all_corners_complete(m->grid));

}

TEST_F(GeneratorsTest, TestPrims) {

	m->generator = new Prims(4, 5);
	m->generate();

	EXPECT_TRUE(boundary_is_solid(m->grid));
	EXPECT_TRUE(all_passages_open(m->grid));
	EXPECT_TRUE(all_corners_complete(m->grid));

}