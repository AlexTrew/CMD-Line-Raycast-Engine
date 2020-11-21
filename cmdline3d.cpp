#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>

int screen_width = 120;
int screen_height = 40;

int map_width = 16;
int map_height = 16;

float player_x = 8.0f;
float player_y = 8.0f;
float player_dir = 0.0f;

float depth = 16.0f;
float speed = 10;
float turn_speed = 0.1f;

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
	map += L"#......###.....#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....#....#....#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....##..##....#";
	map += L"#....##..##....#";
	map += L"#....##..##....#";
	map += L"#....##..##....#";
	map += L"#....##..##....#";
	map += L"################";

	float fov = 3.14 / 4; 

	auto time_1 = std::chrono::system_clock::now();

	while (true) {
		auto time_2 = std::chrono::system_clock::now();
		std::chrono::duration<float> time_since_last_frame = time_2 - time_1;
		float delta = time_since_last_frame.count();
		time_1 = time_2;
		

		if (GetAsyncKeyState('D') < 0) {
			player_dir += 2.0f * delta;
		}
		if (GetAsyncKeyState('A') < 0) {
			player_dir -= 2.0f * delta;
		}

		if (GetAsyncKeyState('W') < 0) {
			player_x += std::sinf(player_dir) * speed * delta;
			player_y += std::cosf(player_dir) * speed * delta;
		}
		if (GetAsyncKeyState('S') < 0) {
			player_x -= std::sinf(player_dir) * speed * delta;
			player_y -= std::cosf(player_dir) * speed * delta;
		}


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

				if (map[try_y * map_width + try_x] == '#') {
					hit = true;
				}

			}

			int ceiling_height = (float)(screen_height / 2.0) - screen_height / ((float)distance_to_wall);
			int floor_height = screen_height - ceiling_height;

			int dim = 2;

			char wall_char = ' ';
			if (distance_to_wall <= dim) wall_char = '@';
			else if (distance_to_wall <= 2 * dim) wall_char = '#';
			else if (distance_to_wall <= 3 * dim) wall_char = 'x';
			else if (distance_to_wall <= 4 * dim) wall_char = '*';
			else if (distance_to_wall <= 5 * dim) wall_char = '~';
			else if (distance_to_wall <= 6 * dim) wall_char = '-';
			else wall_char = '\'';




			for (int y = 0; y < screen_height; y++) {
				if (y < ceiling_height) {
					screen[y * screen_width + x] = ' ';
				}
				else if (y >= ceiling_height && y < floor_height) {
					screen[y * screen_width + x] = wall_char;
				}
				else {
					screen[y * screen_width + x] = ' ';
				}
			}

		}
		screen[screen_width * screen_height - 1] = '\0';
		WriteConsoleOutputCharacter(console, screen, screen_width * screen_height, { 0,0 }, &bytes_written);
		//std::cout << player_x << std::endl;


	}




	return 0;
}