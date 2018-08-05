#include "Player.h"

Player::Player() { }

void Player::turnLeft(const float elapsedTime) {
	angle -= (speed * 0.75f) * elapsedTime;
	if (angle < -PI_F)
		angle += 2 * PI_F;
}

void Player::turnRight(const float elapsedTime) {
	angle += (speed * 0.75f) * elapsedTime;
	if (angle > PI_F)
		angle -= 2 * PI_F;
}

void Player::moveForward(const float elapsedTime, const std::wstring map, 
	const int mapWidth, const int mapHeight) {
	float tempX = x;
	float tempY = y;
	x = fmod(x + cosf(angle) * speed * elapsedTime + (float)mapWidth, (float)mapWidth);
	y = fmod(y + sinf(angle) * speed * elapsedTime + (float)mapHeight, (float)mapHeight);
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		x = tempX;
		y = tempY;
	}
}

void Player::moveBack(const float elapsedTime, const std::wstring map,
	const int mapWidth, const int mapHeight) {
	float tempX = x;
	float tempY = y;
	x = fmod(x - cosf(angle) * speed * elapsedTime + (float)mapWidth, (float)mapWidth);
	y = fmod(y - sinf(angle) * speed * elapsedTime + (float)mapHeight, (float)mapHeight);
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		x = tempX;
		y = tempY;
	}
}

void Player::strafeLeft(const float elapsedTime, const std::wstring map,
	const int mapWidth, const int mapHeight) {
	float tempX = x;
	float tempY = y;
	x = fmod(x + sinf(angle) * speed * elapsedTime + (float)mapWidth, (float)mapWidth);
	y = fmod(y - cosf(angle) * speed * elapsedTime + (float)mapHeight, (float)mapHeight);
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		x = tempX;
		y = tempY;
	}
}

void Player::strafeRight(const float elapsedTime, const std::wstring map,
	const int mapWidth, const int mapHeight) {
	float tempX = x;
	float tempY = y;
	x = fmod(x - sinf(angle) * speed * elapsedTime + (float)mapWidth, (float)mapWidth);
	y = fmod(y + cosf(angle) * speed * elapsedTime + (float)mapHeight, (float)mapHeight);
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		x = tempX;
		y = tempY;
	}
}
