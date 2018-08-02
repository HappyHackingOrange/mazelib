#include "random.h"

int random::randrange(int stop) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, stop - 1);
	return dis(gen);
}

int random::randrange(int start, int stop, int step) {
	if (stop <= start) {
		char buffer[100];
		sprintf_s(buffer, "Empty range for randrange() (%d, %d, %d)", start, stop, stop - start);
		throw std::range_error(buffer);
	}
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, (stop - start) / step - 1);
	return start + dis(gen) * step;
}

void random::shuffle(std::vector<std::pair<int, int>> &vect) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::shuffle(vect.begin(), vect.end(), gen);
}

std::pair<int, int> random::choice(const std::vector<std::pair<int, int>> c) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	auto start = begin(c);
	std::uniform_int_distribution<> dis(0, static_cast<int>(std::distance(start, end(c)) - 1));
	std::advance(start, dis(gen));
	return *start;
}

