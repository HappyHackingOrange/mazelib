#include "Maze.h"

Maze::Maze() {
	generator = NULL;
	grid = xt::xarray<int8_t>();
	start = NONE;
	end = NONE;
	hasBounds = false;
	solver = NULL;
	solutions = {};
}
Maze::~Maze() {
	delete generator;
	delete solver;
}

void Maze::generate() {
	if (generator == NULL)
		throw "no maze-generation algorithm has been set.";
	else {
		grid = generator->generate();
		start = NONE;
		end = NONE;
		hasBounds = generator->hasBounds;
		solutions = {};
	}
}

void Maze::generate_entrances(bool start_outer, bool end_outer) {

	if ((start_outer || end_outer) && !hasBounds)
		throw "can't have start or end on edges in boundless maze";
	if (start_outer && end_outer)
		generate_outer_entrances();
	else if (!start_outer && !end_outer)
		generate_inner_entrances();
	else if (start_outer)
		std::tie(start, end) = generate_opposite_entrances();
	else
		std::tie(end, start) = generate_opposite_entrances();

	// the start and end shouldn't be right next to each other
	if (abs(start.first - end.first) + abs(start.second - end.second) < 2)
		generate_entrances(start_outer, end_outer);

}

void Maze::generate_outer_entrances() {

	auto H = static_cast<int>(grid.shape()[0]);
	auto W = static_cast<int>(grid.shape()[1]);

	auto start_side = random::randrange(4);

	// maze entrances will be on opposite sides of the maze.
	switch (start_side) {
	case 0: // North
		start = std::make_pair(0, random::randrange(1, W, 2));
		end = std::make_pair(H - 1, random::randrange(1, W, 2));
		break;
	case 1: // South
		start = std::make_pair(H - 1, random::randrange(1, W, 2));
		end = std::make_pair(0, random::randrange(1, W, 2));
		break;
	case 2: // West
		start = std::make_pair(random::randrange(1, H, 2), 0);
		end = std::make_pair(random::randrange(1, H, 2), W - 1);
		break;
	default: // East
		start = std::make_pair(random::randrange(1, H, 2), W - 1);
		end = std::make_pair(random::randrange(1, H, 2), 0);
	}

}

void Maze::generate_inner_entrances() {

	auto H = static_cast<int>(grid.shape()[0]);
	auto W = static_cast<int>(grid.shape()[1]);

	start = std::make_pair(random::randrange(hasBounds, H - !hasBounds, 2), 
		random::randrange(hasBounds, W - !hasBounds, 2));
	auto end = std::make_pair(random::randrange(hasBounds, H - !hasBounds, 2), 
		random::randrange(hasBounds, W - !hasBounds, 2));

	// make certain the start and end points aren't the same
	while (end == start)
		end = std::make_pair(random::randrange(hasBounds, H - !hasBounds, 2),
			random::randrange(hasBounds, W - !hasBounds, 2));

	this->end = end;

}

std::pair<std::pair<int, int>, std::pair<int, int>> Maze::generate_opposite_entrances() {

	auto H = static_cast<int>(grid.shape()[0]);
	auto W = static_cast<int>(grid.shape()[1]);

	auto start_side = random::randrange(4);

	// pick a side for the outer maze entrance
	std::pair<int, int> first;
	switch (start_side) {
	case 0: // North
		first = std::make_pair(0, random::randrange(1, W, 2));
		break;
	case 1: // South
		first = std::make_pair(H - 1, random::randrange(1, W, 2));
		break;
	case 2: // West
		first = std::make_pair(random::randrange(1, H, 2), 0);
		break;
	default: // East
		first = std::make_pair(random::randrange(1, H, 2), W - 1);
	}

	// Create an inner maze entrance
	auto second = std::make_pair(random::randrange(1, H, 2), random::randrange(1, W, 2));

	return std::make_pair(first, second);

}

void Maze::generate_monte_carlo(int repeat, int entrances, float difficulty,
	int(*reducer)(std::vector<std::pair<int, int>>)) {

	if (difficulty < 0.0f || difficulty > 1.0f)
		throw std::out_of_range("Maze difficulty must be set from 0 to 1.");

	// generate different mazes

	struct MazeStruct {
		xt::xarray<int8_t> grid;
		std::pair<int, int> start;
		std::pair<int, int> end;
		std::vector<std::vector<std::pair<int, int>>> solutions;
	};

	std::vector<MazeStruct> mazes;

	for (int i = 0; i < repeat; i++) {

		generate();
		std::vector<MazeStruct> this_maze;

		// for each maze, generate different entrances, and solve
		for (int j = 0; j < entrances; j++) {
			generate_entrances();
			solve();
			this_maze.push_back({ grid, start, end, solutions });
		}

		mazes.push_back(*std::max_element(this_maze.begin(), this_maze.end(),
			[](const MazeStruct& mazeX, const MazeStruct& mazeY) {
			return mazeX.solutions.size() < mazeY.solutions.size();
		}));

	}

	std::sort(mazes.begin(), mazes.end(),
		[reducer](const MazeStruct& mazeX, const MazeStruct& mazeY) {
		return reducer(mazeX.solutions[0]) < reducer(mazeY.solutions[0]);
	});

	// based on optional parameter, choose the maze of the correct difficulty
	int posi = static_cast<int>((mazes.size() - 1) * difficulty);

	// save final results of Monte Carlo Simulations to this object
	grid = mazes[posi].grid;
	start = mazes[posi].start;
	end = mazes[posi].end;
	solutions = mazes[posi].solutions;

}

void Maze::solve() {

	if (generator == NULL)
		throw "No maze-solving algorithm has been set";
	else if (start == NONE || end == NONE)
		throw "Start and end times must be set first";
	else
		solutions = solver->solve(grid, start, end, hasBounds);

}

std::string Maze::tostring(bool entrances, bool solutions, std::string delim) {

	if (grid.size() == 0)
		return "";

	// build the walls of the grid
	auto H = static_cast<int>(grid.shape()[0]);
	auto W = static_cast<int>(grid.shape()[1]);
	std::vector<std::string> txt;
	for (int h = 0; h < H; h++) {
		std::ostringstream oss;
		for (int w = 0; w < W; w++)
			oss << (grid(h, w) > 0 ? "#" : ".");
		txt.push_back(oss.str());
	}

	// insert the start and end points
	if (entrances && start != NONE && end != NONE) {
		auto[r, c] = start;
		txt[r][c] = 'S';
		std::tie(r, c) = end;
		txt[r][c] = 'E';
	}

	// if extant, insert the solution path
	if (solutions && !this->solutions.empty())
		for (const auto &[r, c] : this->solutions[0])
			txt[r][c] = '+';

	return boost::algorithm::join(txt, delim);

}
