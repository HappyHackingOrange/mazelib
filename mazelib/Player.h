#pragma once

#include <string>

const float  PI_F = 3.14159265358979f;

class Player {

public:

	float x = 0;
	float y = 0;
	float angle = 0;
	float speed = 4.0f;
	bool isComputer = false;

	Player();

	void turnLeft(const float elapsedTime);
	void turnRight(const float elapsedTime);
	void moveForward(const float elapsedTime, const std::wstring map, const int mapWidth);
	void moveBack(const float elapsedTime, const std::wstring map, const int mapWidth);
	void strafeLeft(const float elapsedTime, const std::wstring map, const int mapWidth);
	void strafeRight(const float elapsedTime, const std::wstring map, const int mapWidth);

};

