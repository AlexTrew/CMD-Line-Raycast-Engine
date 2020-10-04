#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>

int screen_width = 120;
int screen_height = 40;

int map_width = 16;
int map_height = 16;

float player_x = 8.0f;
float player_y = 8.0f;
float player_dir = 0.0f;

float depth = 16.0f;

std::wstring build_map(std::string mapName) {
	mapName += ".txt";

	std::wfstream infile(mapName);
	std::wstring line;
	std::wstring map;
	while (std::getline(infile, line)) {
		map += line;
	}
	return map;
}



int main() {
	wchar_t* screen = new wchar_t[screen_width * screen_height];
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD bytes_written = 0;

	std::wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.........#....#";
	map += L"#..............#";
	map += L"################";

	float fov = 3.14 / 4;

	while (true) {
		for (int x = 0; x < screen_width; x++) {


			float ray_angle = (player_dir - fov / 2.0f) + ((float)x / (float)screen_width) * fov;
			float ray_x = std::sinf(ray_angle);
			float ray_y = std::cosf(ray_angle);

			float distance_to_wall = 0;
			bool hit = false;

			while (!hit) {
				distance_to_wall += 0.1f;

				int try_x = (int)(player_x + ray_x * distance_to_wall);
				int try_y = (int)(player_x + ray_y * distance_to_wall);

				if (map[try_y* map_width + try_x] == '#') {
					hit = true;
				}

			}

			int ceiling_height = (float)(screen_height / 2.0) - screen_height / ((float)distance_to_wall);
			int floor_height = screen_height - ceiling_height;

			for (int y = 0; y < screen_height; y++) {
				if (y < ceiling_height) {
					screen[y * screen_width + x] = ' ';
				}
				else if(y >= ceiling_height && y < floor_height) {
					screen[y * screen_width + x] = '#';
				}
				else{
				screen[y * screen_width + x] = '.';
				}
			}
		}
		screen[screen_width * screen_height - 1] = '\0';
		WriteConsoleOutputCharacter(console, screen, screen_width * screen_height, { 0,0 }, &bytes_written);
	}



	return 0;
}