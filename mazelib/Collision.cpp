#include "Collision.h"

static const std::pair<int, int> END(-999, 9);
static const std::pair<int, int> DEAD_END(-9, -999);

std::vector<std::vector<std::pair<int, int>>> Collision::solve() {

	// deal with the case where the start is on the edge
	auto start = this->start;
	if (hasBounds && on_edge(this->start))
		start = push_edge(this->start);

	// need a comparator to sort paths first before using set_difference
	auto comp = [](const std::vector<std::pair<int, int>>& pathX,
		const std::vector<std::pair<int, int>>& pathY) {
		for (int i = 0; i < std::min(pathX.size(), pathY.size()); i++)
			if (pathX[i] != pathY[i])
				return pathX[i] < pathY[i];
		return pathX.size() < pathY.size();
	};

	// flood the maze twice, and compare the results
	auto paths = flood_maze(start);
	std::sort(paths.begin(), paths.end(), comp);
	auto temp_paths = flood_maze(start);
	std::sort(temp_paths.begin(), temp_paths.end(), comp);
	std::vector<std::vector<std::pair<int, int>>> diff;
	std::set_difference(paths.begin(), paths.end(), temp_paths.begin(), temp_paths.end(),
		std::back_inserter(diff));

	while (!diff.empty()) {
		paths = temp_paths;
		temp_paths = flood_maze(start);
		std::sort(temp_paths.begin(), temp_paths.end(), comp);
		std::vector<std::vector<std::pair<int, int>>> temp_diff;
		std::set_difference(paths.begin(), paths.end(), temp_paths.begin(), temp_paths.end(),
			std::back_inserter(temp_diff));
		diff = temp_diff;
	}

	paths = fix_entrances(paths);

	return paths;

}
std::vector<std::vector<std::pair<int, int>>> Collision::flood_maze(std::pair<int, int> start) {

	auto paths = one_time_step({ {start} });
	auto temp_paths = paths;

	while (!temp_paths.empty()) {
		paths = temp_paths;
		temp_paths = one_time_step(paths);
	}

	return paths;

}

std::vector<std::vector<std::pair<int, int>>> Collision::one_time_step(std::vector<std::vector<std::pair<int, int>>> paths) {

	std::vector<std::vector<std::pair<int, int>>> temp_paths;
	bool step_made = false;

	for (const auto & path : paths) {

		if (path.empty() || path.back() == DEAD_END)
			continue;
		else if (path.back() == END) {
			temp_paths.push_back(path);
			continue;
		}

		auto ns = find_unblocked_neighbors(path.back());
		if (path.size() > 2) {
			auto it = std::find(ns.begin(), ns.end(), *(path.end() - 3));
			if (it != ns.end())
				ns.erase(it);
		}

		std::vector<std::pair<int, int>> temp_path;
		if (ns.empty()) {
			temp_path = path;
			temp_path.push_back(DEAD_END);
			temp_paths.push_back(temp_path);
		}
		else {
			for (const auto & neighbor : ns) {
				auto mid = midpoint(path.back(), neighbor);
				temp_path = path;
				temp_path.push_back(mid);
				temp_path.push_back(neighbor);
				if (within_one(neighbor, end))
					temp_path.push_back(END);
				temp_paths.push_back(temp_path);
			}
		}
		step_made = true;

	}

	if (!step_made)
		return {};

	// fix collisions
	temp_paths = fix_collisions(temp_paths);

	return temp_paths;

}

std::vector<std::vector<std::pair<int, int>>> Collision::fix_collisions(std::vector<std::vector<std::pair<int, int>>> paths) {

	size_t N = paths.size();

	for (int i = 0; i < N - 1; i++) {
		if (paths[i].back() == DEAD_END || paths[i].back() == END)
			continue;
		for (int j = i + 1; j < N; j++) {
			if (paths[j].back() == DEAD_END || paths[j].back() == END)
				continue;
			if (paths[i].back() == paths[j].back()) {
				const auto &[row, col] = paths[i].back();
				grid(row, col) = 1;
				paths[i].back() = NONE;
				paths[j].back() = NONE;
			}
		}
	}

	return paths;

}

std::vector<std::vector<std::pair<int, int>>> Collision::fix_entrances(std::vector<std::vector<std::pair<int, int>>> paths) {

	// Filter out paths ending in 'dead_end'
	// (also: remove 'end' from solution paths)
	std::vector<std::vector<std::pair<int, int>>> temp_paths;
	std::copy_if(paths.begin(), paths.end(), std::back_inserter(temp_paths),
		[](std::vector<std::pair<int, int>> p) { return p.back() != DEAD_END; });
	for (auto & p : temp_paths)
		p.pop_back();

	// if start not on edge, remove first position in all paths
	if (hasBounds && !on_edge(start) || !hasBounds)
		for (auto & p : temp_paths)
			p.erase(p.begin());

	// if end not on edge, remove last position in all paths
	if (hasBounds && !on_edge(end) || !hasBounds)
		for (auto & p : temp_paths)
			p.pop_back();

	return temp_paths;

}

