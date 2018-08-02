#include "pch.h"
#include "..\mazelib\Prims.h"
#include "..\mazelib\Collision.h"
#include "..\mazelib\Maze.h"

class MazeTest : public testing::Test {

public:

	int h;
	int w;
	Maze *m;

	MazeTest() {
		h = 4;
		w = 5;
		m = new Maze();
		m->generator = new Prims(h, w);
	}

	~MazeTest() {
		delete m;
	}

};

/*
test helper method to deterine if a point is on the edge of a maze
*/
bool on_edge(xt::xarray<int8_t> grid, std::pair<int, int> cell) {

	const auto &[r, c] = cell;

	if (r == 0 || r == grid.shape()[0] - 1)
		return true;
	else if (c == 0 || c == grid.shape()[1] - 1)
		return true;

	return false;

}

/*
count the number of turns in a path
*/
int num_turns(std::vector<std::pair<int, int>> path) {

	if (path.size() < 3)
		return 0;

	int num = 0;

	for (int i = 1; i < path.size() - 1; i++) {
		bool same_col = path[i - 1].first == path[i].first == path[i + 1].first;
		bool same_row = path[i - 1].second == path[i].second == path[i + 1].second;
		if (!same_row && !same_col)
			num++;
	}

	return num;

}


TEST_F(MazeTest, TestGridSize) {

	int H = 2 * h + 1;
	int W = 2 * w + 1;

	m->generate();

	EXPECT_EQ(m->grid.shape()[0], H);
}

TEST_F(MazeTest, TestInnerEntrances) {

	m->generate();
	m->generate_entrances(false, false);

	EXPECT_FALSE(on_edge(m->grid, m->start));
	EXPECT_FALSE(on_edge(m->grid, m->end));

}

TEST_F(MazeTest, TestOuterEntrances) {

	m->generate();
	m->generate_entrances();

	EXPECT_TRUE(on_edge(m->grid, m->start));
	EXPECT_TRUE(on_edge(m->grid, m->end));

}

TEST_F(MazeTest, TestGeneratorWipe) {

	m->generate();
	m->generate_entrances();
	m->generate();

	EXPECT_EQ(m->start, NONE);
	EXPECT_EQ(m->end, NONE);
	EXPECT_TRUE(m->solutions.empty());

}

TEST_F(MazeTest, TestMonteCarlo) {

	int H = 2 * h + 1;
	int W = 2 * w + 1;

	m->solver = new Collision();
	m->generate_monte_carlo(3);

	// grid size
	EXPECT_EQ(m->grid.shape()[0], H);
	EXPECT_EQ(m->grid.shape()[1], W);

	// test entrances are outer
	EXPECT_TRUE(on_edge(m->grid, m->start));
	EXPECT_TRUE(on_edge(m->grid, m->end));

}

TEST_F(MazeTest, TestMonteCarloReducer) {

	int H = 2 * h + 1;
	int W = 2 * w + 1;

	m->solver = new Collision();
	m->generate_monte_carlo(3, 3, 1.0f, num_turns);

	// grid size
	EXPECT_EQ(m->grid.shape()[0], H);
	EXPECT_EQ(m->grid.shape()[1], W);

	// test entrances are outer
	EXPECT_TRUE(on_edge(m->grid, m->start));
	EXPECT_TRUE(on_edge(m->grid, m->end));

}