#include "Prims.h"

Prims::Prims(int h, int w) : MazeGenAlgo(h, w) {}

xt::xarray<int8_t> Prims::generate() {

	// create empty grid
	xt::xarray<int8_t> grid = xt::ones<int8_t>({ H, W });

	// choose a random starting position
	int current_row = random::randrange(1, H, 2);
	int current_col = random::randrange(1, W, 2);
	grid(current_row, current_col) = 0;

	// create a weighted list of all vertices connected in the graph
	auto neighbors = find_neighbors(current_row, current_col, grid, true);

	// loop over all current neigbors, until empty
	int visited = 1;
	while (visited < h * w) {

		// find neighbor with lowest weight, make it current
		int nn = random::randrange(static_cast<int>(neighbors.size()));
		std::tie(current_row, current_col) = neighbors[nn];
		visited++;
		grid(current_row, current_col) = 0;
		neighbors.erase(neighbors.begin() + nn);

		// connect that neighbor to a random neighbor with grid[posi] == 0
		const auto [nearest_n0, nearest_n1] = find_neighbors(current_row, current_col, grid)[0];
		grid((current_row + nearest_n0) / 2, (current_col + nearest_n1) / 2) = 0;

		// find all unvisited neighbors of current, add them to neighbors
		auto unvisited = find_neighbors(current_row, current_col, grid, true);
		std::set<std::pair<int, int>> s(neighbors.begin(), neighbors.end());
		std::copy(unvisited.begin(), unvisited.end(), std::inserter(s, s.end()));
		std::vector<std::pair<int, int>> v(s.begin(), s.end());
		neighbors = v;

	}

	return grid;
}
