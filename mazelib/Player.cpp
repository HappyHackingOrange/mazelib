#include "Player.h"

Player::Player() {
	fX = 0;
	fY = 0;
	fAngle = 0;
	fSpeed = 2.0944f;
	isComputer = false;
}

//Player::~Player()
//{
//}

void Player::turnLeft(const float fElapsedTime) {
	fAngle -= (fSpeed * 0.75f) * fElapsedTime;
	if (fAngle < -PI_F)
		fAngle += 2 * PI_F;
}

void Player::turnRight(const float fElapsedTime) {
	fAngle += (fSpeed * 0.75f) * fElapsedTime;
	if (fAngle > PI_F)
		fAngle -= 2 * PI_F;
}

void Player::moveForward(const float fElapsedTime, const std::wstring map, const int nMapWidth) {
	fX += sinf(fAngle) * fSpeed * fElapsedTime;
	fY += cosf(fAngle) * fSpeed * fElapsedTime;
	if (map.c_str()[(int)fX * nMapWidth + (int)fY] == '#') {
		fX -= sinf(fAngle) * fSpeed * fElapsedTime;
		fY -= cosf(fAngle) * fSpeed * fElapsedTime;
	}
}

void Player::moveBack(const float fElapsedTime, const std::wstring map, const int nMapWidth) {
	fX -= sinf(fAngle) * fSpeed * fElapsedTime;
	fY -= cosf(fAngle) * fSpeed * fElapsedTime;
	if (map.c_str()[(int)fX * nMapWidth + (int)fY] == '#') {
		fX += sinf(fAngle) * fSpeed * fElapsedTime;
		fY += cosf(fAngle) * fSpeed * fElapsedTime;
	}
}

void Player::strafeLeft(const float fElapsedTime, const std::wstring map, const int nMapWidth) {
	fX -= cosf(fAngle) * fSpeed * fElapsedTime;
	fY += sinf(fAngle) * fSpeed * fElapsedTime;
	if (map.c_str()[(int)fX * nMapWidth + (int)fY] == '#') {
		fX += cosf(fAngle) * fSpeed * fElapsedTime;
		fY -= sinf(fAngle) * fSpeed * fElapsedTime;
	}
}

void Player::strafeRight(const float fElapsedTime, const std::wstring map, const int nMapWidth) {
	fX += cosf(fAngle) * fSpeed * fElapsedTime;
	fY -= sinf(fAngle) * fSpeed * fElapsedTime;
	if (map.c_str()[(int)fX * nMapWidth + (int)fY] == '#') {
		fX -= cosf(fAngle) * fSpeed * fElapsedTime;
		fY += sinf(fAngle) * fSpeed * fElapsedTime;
	}
}
