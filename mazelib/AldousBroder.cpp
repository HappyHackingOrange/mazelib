#include "AldousBroder.h"

AldousBroder::AldousBroder(int h, int w) : MazeGenAlgo(h, w) {}

xt::xarray<int8_t> AldousBroder::generate() {

	// Create empty grid, with walls
	xt::xarray<int8_t> grid = xt::ones<int8_t>({H, W});

	int crow = random::randrange(1, H, 2);
	int ccol = random::randrange(1, W, 2);
	grid(crow, ccol) = 0;
	int num_visited = 1;

	while (num_visited < h * w) {

		// find neighbors
		auto neighbors = find_neighbors(crow, ccol, grid, true);

		// how many neighbors have already been visited?
		if (neighbors.empty()) {
			// mark random neighbor as current
			std::tie(crow, ccol) = random::choice(find_neighbors(crow, ccol, grid));
			continue;
		}

		// loop through neighbors
		for (const auto & [nrow, ncol] : neighbors)
			if (grid(nrow, ncol) > 0) {
				// open up wall to new neighbor
				grid((nrow + crow) / 2, (ncol + ccol) / 2) = 0;
				// mark neighbor as visited
				grid(nrow, ncol) = 0;
				// bump the number visited
				num_visited++;
				// current becomes new neighbor
				crow = nrow;
				ccol = ncol;
				// break loop
				break;
			}

	}

	return grid;
}