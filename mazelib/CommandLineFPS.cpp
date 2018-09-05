#include "CommandLineFPS.h"

CommandLineFPS::CommandLineFPS(Maze &maze) : View(maze) {}

void CommandLineFPS::start() {

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[screenWidth*screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL,
		CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// If player is computer, use first solution in the list
	auto solution = maze.solutions[0];
	solution.push_back(maze.end);

	// Create Map of world space # = wall block, . = space
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	std::wstring map = converter.from_bytes(maze.tostring(true, false, ""));

	//Player Starting Position
	player.x = maze.start.second + 0.5f;
	player.y = maze.start.first + 0.5f;

	// Player Starting Rotation
	int position_index = static_cast<int>(player.y) * maze.generator->W + static_cast<int>(player.x);
	int row = position_index / maze.generator->W;
	int col = position_index % maze.generator->W;
	int rowStep = solution.front().first - row;
	int colStep = solution.front().second - col;
	if (abs(rowStep) > 1)
		rowStep = (rowStep < 0) - (rowStep > 0);
	if (abs(colStep) > 1)
		colStep = (colStep < 0) - (colStep > 0);
	std::pair<int, int> direction(rowStep, colStep);
	if ((direction.first == 1 || direction.first == 1 - maze.generator->H) &&
		direction.second == 0)
		player.angle = PI_F / 2;
	else if (direction.first == 0 &&
		(direction.second == -1 || direction.second == maze.generator->W - 1))
		player.angle = PI_F;
	if ((direction.first == -1 || direction.first == maze.generator->H - 1) &&
		direction.second == 0)
		player.angle = -PI_F / 2;

	auto tp1 = std::chrono::system_clock::now();
	auto tp2 = std::chrono::system_clock::now();

	// If player is computer, the computer goes to the end goal by itself.
	// Else, navigate yourself to the end
	while (player.isComputer && solution.size() != 0 || !player.isComputer &&
		map[static_cast<int>(player.y) * maze.generator->W + static_cast<int>(player.x)] != 'E') {

		// We'll need time differential per frame to calculate modification
		// to movement speeds, to ensure consistant movement, as ray-tracing
		// is non-deterministic
		tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		if (player.isComputer) {

			float xDiff = solution.front().second + 0.5f - player.x;
			float yDiff = solution.front().first + 0.5f - player.y;
			if (abs(xDiff) > 2)
				xDiff = -copysignf(1.0, xDiff);
			if (abs(yDiff) > 2)
				yDiff = -copysignf(1.0, yDiff);

			float angleDiff = (xDiff == 0 && yDiff == 0) ? 0 :
				atan2(yDiff, xDiff) - player.angle;
			if (angleDiff <= -PI_F)
				angleDiff += 2 * PI_F;
			else if (angleDiff > PI_F)
				angleDiff -= 2 * PI_F;

			// Computer needs to complete turning before going forward.
			if (angleDiff != 0) {
				(angleDiff < 0) ? player.turnLeft(fElapsedTime) :
					player.turnRight(fElapsedTime);
				float newAngleDiff = (xDiff == 0 && yDiff == 0) ? 0 :
					atan2(yDiff, xDiff) - player.angle;
				if (newAngleDiff <= -PI_F)
					newAngleDiff += 2 * PI_F;
				else if (newAngleDiff > PI_F)
					newAngleDiff -= 2 * PI_F;
				if (angleDiff * newAngleDiff < 0)
					player.angle = atan2(yDiff, xDiff);
			}

			// Go forward after computer finishes turning.
			else if (xDiff != 0 || yDiff != 0) {
				player.moveForward(fElapsedTime, map, maze.generator->W, maze.generator->H);
				float newXDiff = solution.front().second + 0.5f - player.x;
				float newYDiff = solution.front().first + 0.5f - player.y;
				if (abs(newXDiff) > 2)
					newXDiff = -copysignf(1.0, newXDiff);
				if (abs(newYDiff) > 2)
					newYDiff = -copysignf(1.0, newYDiff);
				if (xDiff * newXDiff < 0)
					player.x = solution.front().second + 0.5f;
				if (yDiff * newYDiff < 0)
					player.y = solution.front().first + 0.5f;
			}

			// Computer finished reaching to the next cell. Remove that cell from the list.
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
				player.moveForward(fElapsedTime, map, maze.generator->W, maze.generator->H);

			// Handle backwards movement & collision
			if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
				player.moveBack(fElapsedTime, map, maze.generator->W, maze.generator->H);

			// Handle strafe left movement & collision
			if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
				player.strafeLeft(fElapsedTime, map, maze.generator->W, maze.generator->H);

			// Handle strafe right movement & collision
			if (GetAsyncKeyState((unsigned short)'E') & 0x8000)
				player.strafeRight(fElapsedTime, map, maze.generator->W, maze.generator->H);

		}

		for (int x = 0; x < screenWidth; x++) {

			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (player.angle - FOV / 2.0f) + ((float)x / (float)screenWidth) * FOV;

			// Find distance to wall
			float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
			float fDistanceToWall = 0.0f; //                                      resolution

			bool bHitWall = false;		// Set when ray hits wall block
			bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

			float eyeX = cosf(fRayAngle); // Unit vector for ray in player space
			float eyeY = sinf(fRayAngle);

			// Incrementally cast ray from player, along ray angle, testing for 
			// intersection with a block
			while (!bHitWall && fDistanceToWall < Depth) {

				fDistanceToWall += fStepSize;
				int testX = static_cast<int>(floor(player.x + eyeX * fDistanceToWall));
				int testY = static_cast<int>(floor(player.y + eyeY * fDistanceToWall));

				// Test if ray is out of bounds
				if (maze.hasBounds && (testX < 0 || testX >= maze.generator->W ||
					testY < 0 || testY >= maze.generator->H)) {
					bHitWall = true;			// Just set distance to maximum depth
					fDistanceToWall = Depth;
					continue;
				}

				// Ray is inbounds so test to see if the ray cell is a wall block
				if (map.c_str()[(testY + maze.generator->H) % maze.generator->H *
					maze.generator->W + (testX + maze.generator->W) % maze.generator->W] == '#') {

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
						for (int ty = 0; ty < 2; ty++) {
							// Angle of corner to eye
							float vx = (float)testX + tx - player.x;
							float vy = (float)testY + ty - player.y;
							float d = sqrt(vx * vx + vy * vy);
							float dot = (eyeX * vx / d) + (eyeY * vy / d);
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

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(screenHeight / 2.0) - screenHeight / ((float)fDistanceToWall);
			int nFloor = screenHeight - nCeiling;

			// Shader walls based on distance
			short nShade = ' ';
			if (fDistanceToWall <= Depth / 4.0f)			nShade = 0x2588;	// Very close	
			else if (fDistanceToWall < Depth / 3.0f)		nShade = 0x2593;
			else if (fDistanceToWall < Depth / 2.0f)		nShade = 0x2592;
			else if (fDistanceToWall < Depth)				nShade = 0x2591;
			else											nShade = ' ';		// Too far away

			if (bBoundary)		nShade = ' '; // Black it out

			for (int y = 0; y < screenHeight; y++) {
				// Each Row
				if (y <= nCeiling)
					screen[y*screenWidth + x] = ' ';
				else if (y > nCeiling && y <= nFloor)
					screen[y*screenWidth + x] = nShade;
				else { // Floor
					   // Shade floor based on distance
					float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (b < 0.25)		nShade = '#';
					else if (b < 0.5)	nShade = 'x';
					else if (b < 0.75)	nShade = '.';
					else if (b < 0.9)	nShade = '-';
					else				nShade = ' ';
					screen[y*screenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		if (displayStats)
			swprintf(screen, L"%dx%d maze, X=%3.2f, Y=%3.2f, A=%3.2f\u03C0, FPS=%3.2f",
				maze.generator->w, maze.generator->h, player.x, player.y,
				player.angle / PI_F, 1.0f / fElapsedTime);

		// Display Map
		if (showMinimap) {
			for (int nx = 0; nx < minimapWidth; nx++)
				for (int ny = 0; ny < minimapHeight; ny++) {
					int nXTemp = nx - minimapWidth / 2 + static_cast<int>(player.x);
					int nYTemp = ny - minimapHeight / 2 + static_cast<int>(player.y);
					if (maze.hasBounds &&
						(nXTemp < 0 || nXTemp >= maze.generator->W ||
							nYTemp < 0 || nYTemp >= maze.generator->H))
						screen[(ny + 1) * screenWidth + nx] = ' ';
					else {
						screen[(ny + 1) * screenWidth + nx] =
							map[(nYTemp + 2 * maze.generator->H) % maze.generator->H * maze.generator->W + (nXTemp + 2 * maze.generator->W) % maze.generator->W];
					}
				}
			screen[(minimapHeight / 2 + 1) * screenWidth + minimapWidth / 2] = 'P';
		}

		// Display Frame
		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacterW(hConsole, screen, screenWidth * screenHeight, { 0, 0 },
			&dwBytesWritten);
	}

}
