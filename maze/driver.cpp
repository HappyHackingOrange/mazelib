#pragma warning ( disable : 4996 )

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <codecvt>

#define NOMINMAX
#include <Windows.h>

#include "maze.h"
#include "prims.h"
#include "collision.h"
#include "Player.h"

int w = 3;							// Starting number of cells wide
int h = 3;							// Starting number of cells tall
int nScreenWidth = 120;				// Console Screen Size X (columns)
int nScreenHeight = 40;				// Console Screen Size Y (rows)
float fFOV = PI_F / 4;				// Field of View
float fDepth = 16.0f;				// Maximum rendering distance
bool displayStats = true;			// Do you want to see statistics info?
bool showMinimap = true;			// Display minimap?
int minimapWidth = 25;				// Minimap width
int minimapHeight = 13;				// Minimap height

int main() {

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Set up a player
	Player player = Player();
	player.fSpeed *= 2;
	player.isComputer = true;

	while (true) {

		// Set up a maze
		Maze m = Maze();
		m.generator = new Prims(h, w);
		m.solver = new Collision();
		m.generate_monte_carlo(1);
		std::vector<std::pair<int, int>> solution = m.solutions[0];
		solution.push_back(m.end);

		int nMapWidth = m.generator->W;
		int nMapHeight = m.generator->H;

		//Player Start Position
		player.fX = m.start.first + 0.5f;
		player.fY = m.start.second + 0.5f;

		// Player Start Rotation
		if (m.start.first == 0)
			player.fAngle = PI_F / 2;
		else if (m.start.first == nMapWidth - 1)
			player.fAngle = -PI_F / 2;
		else if (m.start.second == 0)
			player.fAngle = 0;
		else
			player.fAngle = PI_F;

		// Create Map of world space # = wall block, . = space
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring map = converter.from_bytes(m.tostring(true));

		auto tp1 = std::chrono::system_clock::now();
		auto tp2 = std::chrono::system_clock::now();

		// If player is computer, the computer goes to the end goal by itself.
		// Else, navigate yourself to the end
		while (player.isComputer && solution.size() != 0 || !player.isComputer &&
			map[((int)player.fX) * nMapWidth + (int)player.fY] != 'E') {

			// We'll need time differential per frame to calculate modification
			// to movement speeds, to ensure consistant movement, as ray-tracing
			// is non-deterministic
			tp2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime = tp2 - tp1;
			tp1 = tp2;
			float fElapsedTime = elapsedTime.count();

			if (player.isComputer) {

				float fXDiff = solution.front().first + 0.5f - player.fX;
				float fYDiff = solution.front().second + 0.5f - player.fY;

				float fAngleDiff = (fXDiff == 0 && fYDiff == 0) ? 0 :
					atan2(fXDiff, fYDiff) - player.fAngle;
				if (fAngleDiff < -PI_F)
					fAngleDiff += 2 * PI_F;
				else if (fAngleDiff > PI_F)
					fAngleDiff -= 2 * PI_F;

				if (fAngleDiff != 0) {
					(fAngleDiff < 0) ? player.turnLeft(fElapsedTime) : player.turnRight(fElapsedTime);
					float fNewAngleDiff = (fXDiff == 0 && fYDiff == 0) ? 0 :
						atan2(fXDiff, fYDiff) - player.fAngle;
					if (fNewAngleDiff < -PI_F)
						fNewAngleDiff += 2 * PI_F;
					else if (fNewAngleDiff > PI_F)
						fNewAngleDiff -= 2 * PI_F;
					if (fAngleDiff * fNewAngleDiff < 0)
						player.fAngle = atan2(fXDiff, fYDiff);
				}
				else if (fXDiff != 0 || fYDiff != 0) {
					player.moveForward(fElapsedTime, map, nMapWidth);
					float fNewXDiff = solution.front().first + 0.5f - player.fX;
					float fNewYDiff = solution.front().second + 0.5f - player.fY;
					if (fXDiff * fNewXDiff < 0)
						player.fX = solution.front().first + 0.5f;
					if (fYDiff * fNewYDiff < 0)
						player.fY = solution.front().second + 0.5f;
				}
				else
					solution.erase(solution.begin());

			}

			else {

				// Handle CCW Rotation
				if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
					player.turnLeft(fElapsedTime);

				// Handle CW Rotation
				if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
					player.turnRight(fElapsedTime);

				// Handle Forwards movement & collision
				if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
					player.moveForward(fElapsedTime, map, nMapWidth);

				// Handle backwards movement & collision
				if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
					player.moveBack(fElapsedTime, map, nMapWidth);

				// Handle strafe left movement & collision
				if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
					player.strafeLeft(fElapsedTime, map, nMapWidth);

				// Handle strafe right movement & collision
				if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
					player.strafeRight(fElapsedTime, map, nMapWidth);

			}

			for (int x = 0; x < nScreenWidth; x++) {

				// For each column, calculate the projected ray angle into world space
				float fRayAngle = (player.fAngle - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

				// Find distance to wall
				float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
				float fDistanceToWall = 0.0f; //                                      resolution

				bool bHitWall = false;		// Set when ray hits wall block
				bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

				float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
				float fEyeY = cosf(fRayAngle);

				// Incrementally cast ray from player, along ray angle, testing for 
				// intersection with a block
				while (!bHitWall && fDistanceToWall < fDepth) {

					fDistanceToWall += fStepSize;
					int nTestX = (int)(player.fX + fEyeX * fDistanceToWall);
					int nTestY = (int)(player.fY + fEyeY * fDistanceToWall);

					// Test if ray is out of bounds
					if (nTestX < 0 || nTestX >= nMapHeight || nTestY < 0 || nTestY >= nMapWidth) {
						bHitWall = true;			// Just set distance to maximum depth
						fDistanceToWall = fDepth;
					}
					else {

						// Ray is inbounds so test to see if the ray cell is a wall block
						if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
						{
							// Ray has hit wall
							bHitWall = true;

							// To highlight tile boundaries, cast a ray from each corner
							// of the tile, to the player. The more coincident this ray
							// is to the rendering ray, the closer we are to a tile 
							// boundary, which we'll shade to add detail to the walls
							std::vector<std::pair<float, float>> p;

							// Test each corner of hit tile, storing the distance from
							// the player, and the calculated dot product of the two rays
							for (int tx = 0; tx < 2; tx++)
								for (int ty = 0; ty < 2; ty++)
								{
									// Angle of corner to eye
									float vy = (float)nTestY + ty - player.fY;
									float vx = (float)nTestX + tx - player.fX;
									float d = sqrt(vx*vx + vy * vy);
									float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
									p.push_back(std::make_pair(d, dot));
								}

							// Sort Pairs from closest to farthest
							sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right) {return left.first < right.first; });

							// First two/three are closest (we will never see all four)
							//float fBound = 0.01;
							float fBound = 0.0033f;
							if (acos(p.at(0).second) < fBound) bBoundary = true;
							if (acos(p.at(1).second) < fBound) bBoundary = true;
							if (acos(p.at(2).second) < fBound) bBoundary = true;
						}
					}
				}

				// Calculate distance to ceiling and floor
				int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
				int nFloor = nScreenHeight - nCeiling;

				// Shader walls based on distance
				short nShade = ' ';
				if (fDistanceToWall <= fDepth / 4.0f)			nShade = 0x2588;	// Very close	
				else if (fDistanceToWall < fDepth / 3.0f)		nShade = 0x2593;
				else if (fDistanceToWall < fDepth / 2.0f)		nShade = 0x2592;
				else if (fDistanceToWall < fDepth)				nShade = 0x2591;
				else											nShade = ' ';		// Too far away

				if (bBoundary)		nShade = ' '; // Black it out

				for (int y = 0; y < nScreenHeight; y++) {
					// Each Row
					if (y <= nCeiling)
						screen[y*nScreenWidth + x] = ' ';
					else if (y > nCeiling && y <= nFloor)
						screen[y*nScreenWidth + x] = nShade;
					else { // Floor
						   // Shade floor based on distance
						float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
						if (b < 0.25)		nShade = '#';
						else if (b < 0.5)	nShade = 'x';
						else if (b < 0.75)	nShade = '.';
						else if (b < 0.9)	nShade = '-';
						else				nShade = ' ';
						screen[y*nScreenWidth + x] = nShade;
					}
				}
			}

			// Display Stats
			if (displayStats)
				swprintf(screen, L"%dx%d maze, X=%3.2f, Y=%3.2f, A=%3.2f, FPS=%3.2f",
					w, h, player.fX, player.fY, player.fAngle, 1.0f / fElapsedTime);

			// Display Map
			if (showMinimap) {
				for (int nx = 0; nx < minimapWidth; nx++)
					for (int ny = 0; ny < minimapHeight; ny++) {
						int nXTemp = nx - minimapWidth / 2 + (int)player.fY;
						int nYTemp = ny - minimapHeight / 2 + (int)player.fX;
						if (nXTemp < 0 || nXTemp >= nMapWidth || nYTemp < 0 || nYTemp >= nMapHeight)
							screen[(ny + 1) * nScreenWidth + nx] = ' ';
						else
							screen[(ny + 1) * nScreenWidth + nx] = map[nYTemp * nMapWidth + nXTemp];
					}
				screen[(minimapHeight / 2 + 1) * nScreenWidth + minimapWidth / 2] = 'P';
			}

			// Display Frame
			screen[nScreenWidth * nScreenHeight - 1] = '\0';
			WriteConsoleOutputCharacterW(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 },
				&dwBytesWritten);
		}

		h++;
		w++;

	}

	return 0;
}
