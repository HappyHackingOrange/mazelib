#pragma once

#include <string>

const float  PI_F = 3.14159265358979f;

class Player {

public:

	float fX;
	float fY;
	float fAngle;
	float fSpeed;
	bool isComputer;

	Player();
	//~Player();

	void turnLeft(const float fElapsedTime);
	void turnRight(const float fElapsedTime);
	void moveForward(const float fElapsedTime, const std::wstring map, const int nMapWidth);
	void moveBack(const float fElapsedTime, const std::wstring map, const int nMapWidth);
	void strafeLeft(const float fElapsedTime, const std::wstring map, const int nMapWidth);
	void strafeRight(const float fElapsedTime, const std::wstring map, const int nMapWidth);

};

