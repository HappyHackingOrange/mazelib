#pragma once
#pragma warning ( disable : 4996 )

#include "xtensor/xarray.hpp"
#include "random.h"

static const std::pair<int, int> NONE(-999, -999);

class MazeSolveAlgo {

public:

	xt::xarray<int8_t> grid;
	std::pair<int, int> start;
	std::pair<int, int> end;

	auto solve(xt::xarray<int8_t> grid, std::pair<int, int> start, std::pair<int, int> end) {
		solve_preprocessor(grid, start, end);
		return solve();
	}

protected:

	void solve_preprocessor(xt::xarray<int8_t> grid, std::pair<int, int> start, std::pair<int, int> end) {

		this->grid = grid;
		this->start = start;
		this->end = end;

		// validating checks
		if (grid.size() == 0)
			throw "Maze grid is not set";
		if (start == NONE || end == NONE)
			throw "Entrances are not set";
		if (start.first < 0 || start.first >= grid.shape()[0] ||
			start.second < 0 || start.second >= grid.shape()[1] ||
			end.first < 0 || end.first >= grid.shape()[0] ||
			end.second < 0 || end.second >= grid.shape()[1])
			throw std::out_of_range("Entrance if outside of the grid.");

	}

	virtual std::vector<std::vector<std::pair<int, int>>> solve() = NULL;

	/*
	All of the methods below this are helper methods,
	*/

	/*
	Find all the grid neighbors of the current position;
	wall of open passage.
	*/
	auto find_neighbors(std::pair<int, int> posi, int is_wall = 0) {

		const auto &[r, c] = posi;
		std::vector<std::pair<int, int>> ns;

		if (r > 1 && grid(r - 2, c) == is_wall)
			ns.push_back(std::make_pair(r - 2, c));
		if (r < grid.shape()[0] - 2 && grid(r + 2, c) == is_wall)
			ns.push_back(std::make_pair(r + 2, c));
		if (c > 1 && grid(r, c - 2) == is_wall)
			ns.push_back(std::make_pair(r, c - 2));
		if (c < grid.shape()[1] - 2 && grid(r, c + 2) == is_wall)
			ns.push_back(std::make_pair(r, c + 2));

		random::shuffle(ns);

		return ns;

	}

	/*
	Find all the grid neighbors of the current position; visited, or not.
	*/
	auto find_unblocked_neighbors(std::pair<int, int> posi) {

		const auto &[r, c] = posi;
		std::vector<std::pair<int, int>> ns;

		if (r > 1 && grid(r - 1, c) == 0 && grid(r - 2, c) == 0)
			ns.push_back(std::make_pair(r - 2, c));
		if (r < grid.shape()[0] - 2 && grid(r + 1, c) == 0 && grid(r + 2, c) == 0)
			ns.push_back(std::make_pair(r + 2, c));
		if (c > 1 && grid(r, c - 1) == 0 && grid(r, c - 2) == 0)
			ns.push_back(std::make_pair(r, c - 2));
		if (c < grid.shape()[1] - 2 && grid(r, c + 1) == 0 && grid(r, c + 2) == 0)
			ns.push_back(std::make_pair(r, c + 2));

		random::shuffle(ns);

		return ns;

	}

	/*
	Find the wall cell between to passage cells
	*/
	auto midpoint(std::pair<int, int> a, std::pair<int, int> b) {
		return std::make_pair((a.first + b.first) / 2, (a.second + b.second) / 2);
	}

	/*
	Convolve a position tuple with a direction tuple to
	generate a new position.
	*/
	auto move(std::pair<int, int> start, std::pair<int, int> direction) {
		return std::make_pair(start.first + direction.first, start.second + direction.second);
	}

	/*
	Does the cell lay on the edge, rather inside of the maze grid?
	*/
	auto on_edge(std::pair<int, int> cell) {
		const auto &[r, c] = cell;
		return r == 0 || r == grid.shape()[0] - 1 || c == 0 || c == grid.shape()[1] - 1;
	}

	/*
	You may need to find the cell directly inside of a start or end cell.
	*/
	auto push_edge(std::pair<int, int> cell) {
		const auto &[r, c] = cell;
		if (r == 0)
			return std::make_pair(1, c);
		else if (r == grid.shape()[0] - 1)
			return std::make_pair(r - 1, c);
		else if (c == 0)
			return std::make_pair(r, 1);
		else
			return std::make_pair(r, c - 1);
	}

	/*
	Is the current cell within one move of the desired cell?
	Note, this might be one full more, or one half move.
	*/
	auto within_one(std::pair<int, int> cell, std::pair<int, int> desire) {
		return cell.first == desire.first && abs(cell.second - desire.second) < 2 ||
			cell.second == desire.second && abs(cell.first - desire.first) < 2;
	}

	/*
	In the process of solving a maze, the algorithm might go down
	the wrong corridor then backtrack.

	Those extraneous branches need to be removed.
	*/
	auto prune_solution(std::vector<std::pair<int, int>> solution) {

		bool found = true;
		int attempt = 0;
		size_t max_attempt = solution.size();

		while (found && solution.size() > 2 && attempt < max_attempt) {

			found = false;

			std::vector<std::pair<int, int>>::iterator first_ptr, last_ptr;
			for (auto ptr = solution.begin(); ptr < solution.end(); ptr++) {
				auto it = std::find(ptr + 1, solution.end(), *ptr);
				if (it != solution.end()) {
					first_ptr = ptr;
					last_ptr = it + std::distance(solution.begin(), ptr) + 1;
					found = true;
				}
			}

			if (found)
				solution.erase(first_ptr, last_ptr);

		}

		// solution does not include entrances
		if (solution.front() == start)
			solution.erase(solution.begin());
		if (solution.back() == end)
			solution.pop_back();

		return solution;

	}

};

