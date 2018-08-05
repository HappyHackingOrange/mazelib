#pragma once
#pragma warning ( disable : 4996 )

#include "random.h"
#include "xtensor/xarray.hpp"

class MazeGenAlgo {

public:

	int h;
	int w;
	int H;
	int W;
	bool hasBounds;

	MazeGenAlgo(int h, int w, bool hasBounds) {
		if (h < 3 || w < 3)
			throw std::out_of_range("Mazes cannot be smaller than 3x3.");
		this->h = h;
		this->w = w;
		this->H = (2 * h) + hasBounds;
		this->W = (2 * w) + hasBounds;
		this->hasBounds = hasBounds;
	}

	virtual xt::xarray<int8_t> generate() = NULL;

protected:

	/*
	All of the methods below this are helper methods,
	common to many maze - generating algorithms.
	*/

	/*
	Find all the grid neighbors of the current position;
	visited, or not.
	*/
	auto find_neighbors(int r, int c, xt::xarray<int8_t> grid, int is_wall = 0) {

		std::vector<std::pair<int, int>> ns;

		if (hasBounds) {
			if (r > 1 && grid(r - 2, c) == is_wall)
				ns.push_back(std::make_pair(r - 2, c));
			if (r < H - 2 && grid(r + 2, c) == is_wall)
				ns.push_back(std::make_pair(r + 2, c));
			if (c > 1 && grid(r, c - 2) == is_wall)
				ns.push_back(std::make_pair(r, c - 2));
			if (c < W - 2 && grid(r, c + 2) == is_wall)
				ns.push_back(std::make_pair(r, c + 2));
		}
		else {
			if (grid((r - 2 + H) % H, c) == is_wall)
				ns.push_back(std::make_pair((r - 2 + H) % H, c));
			if (grid((r + 2) % H, c) == is_wall)
				ns.push_back(std::make_pair((r + 2) % H, c));
			if (grid(r, (c - 2 + W) % W) == is_wall)
				ns.push_back(std::make_pair(r, (c - 2 + W) % W));
			if (grid(r, (c + 2) % W) == is_wall)
				ns.push_back(std::make_pair(r, (c + 2) % W));
		}

		random::shuffle(ns);

		return ns;

	}


};

