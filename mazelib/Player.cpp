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

void Player::moveForward(const float elapsedTime, const std::wstring map, const int mapWidth) {
	y += sinf(angle) * speed * elapsedTime;
	x += cosf(angle) * speed * elapsedTime;
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		y -= sinf(angle) * speed * elapsedTime;
		x -= cosf(angle) * speed * elapsedTime;
	}
}

void Player::moveBack(const float elapsedTime, const std::wstring map, const int mapWidth) {
	y -= sinf(angle) * speed * elapsedTime;
	x -= cosf(angle) * speed * elapsedTime;
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		y += sinf(angle) * speed * elapsedTime;
		x += cosf(angle) * speed * elapsedTime;
	}
}

void Player::strafeLeft(const float elapsedTime, const std::wstring map, const int mapWidth) {
	y -= cosf(angle) * speed * elapsedTime;
	x += sinf(angle) * speed * elapsedTime;
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		y += cosf(angle) * speed * elapsedTime;
		x -= sinf(angle) * speed * elapsedTime;
	}
}

void Player::strafeRight(const float elapsedTime, const std::wstring map, const int mapWidth) {
	y += cosf(angle) * speed * elapsedTime;
	x -= sinf(angle) * speed * elapsedTime;
	if (map.c_str()[(int)y * mapWidth + (int)x] == '#') {
		y -= cosf(angle) * speed * elapsedTime;
		x += sinf(angle) * speed * elapsedTime;
	}
}
