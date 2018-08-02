#include "pch.h"
#include "..\mazelib\Prims.h"
#include "..\mazelib\Collision.h"
#include "..\mazelib\Maze.h"
#include "xtensor/xview.hpp"

const std::vector<bool> starts{ true, false };
const std::vector<bool> ends{ true, false };

class SolversTest : public testing::Test {

public:

	SolversTest() {
	}

	~SolversTest() {
	}

};

/*
#######
#     #
# # # #
# #   #
# #####
#     #
#######
*/
auto example_cul_de_sac_maze() {
	return xt::xarray<int8_t>{
		{ 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 0, 1, 0, 1, 0, 1 },
		{ 1, 0, 1, 0, 0, 0, 1 },
		{ 1, 0, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1 }
	};
}

/*
Is one cell exactly one move from another?
*/
bool one_away(std::pair<int, int> cell1, std::pair<int, int> cell2) {

	const auto &[r1, c1] = cell1;
	const auto &[r2, c2] = cell2;

	return (r1 == r2 && abs(c1 - c2) == 1 || c1 == c2 && abs(r1 - r2) == 1);

}

/*
No cell should appear twice in the same maze solution.
*/
bool duplicates_in_solution(std::vector<std::pair<int, int>> solution) {

	for (int i = 0; i < solution.size() - 1; i++) {
		auto it = std::find(solution.begin() + i + 1, solution.end(), solution[i]);
		if (it != solution.end())
			return true;
	}

	return false;

}

/*
Create a maze with entrances inside/outside
*/
Maze * create_maze_with_varied_entrances(bool start_outer = true, bool end_outer = true) {

	Maze * m = new Maze();
	m->generator = new Prims(4, 5);
	m->generate();
	m->generate_entrances(start_outer, end_outer);

	return m;
}

/*
Test against a maze with outer/inner entrances
*/
TEST(SolversTest, TestCollision) {

	for (const auto & s : starts)
		for (const auto & e : ends) {
			Maze * m = create_maze_with_varied_entrances(s, e);
			m->solver = new Collision();
			m->solve();
			for (const auto & sol : m->solutions) {
				EXPECT_FALSE(duplicates_in_solution(sol));
				EXPECT_TRUE(one_away(m->start, sol.front()));
				EXPECT_TRUE(one_away(m->end, sol.back()));
			}
		}

}