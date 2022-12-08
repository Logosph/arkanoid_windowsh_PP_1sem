#define _USE_MATH_DEFINES
#include <iostream>
#include <WinUser.h>
#include <Windows.h>
#include <time.h>
#include <cmath>
#include <string>
#include <algorithm>


using namespace std;

// General preferences
const float tick = 0.01f;
int score = 0;
int x_intersection = 0;
int y_intersection = 0;
double hypotenuse = 1;
double angle = 0;
double new_angle = 0;
double k = 0;
double gradus_to_radians(double gradus);
HWND hwnd = GetConsoleWindow();
HDC hdc = GetDC(hwnd);
COLORREF background = 0x004D0000;
HBRUSH brush;
HBRUSH brush1;
HBRUSH brush2;
HBRUSH brush3;
HPEN pen;
string path;

// Environment preferences
int border_left = 130;
int border_right = 930;
int border_down = 610;
int border_up = 10;
COLORREF border_color = 0x00FFFFFF;

// Platform preferences
enum class Direction {left = -1, stop, right};
Direction platform_direction = Direction::stop;
int platform_width = 300;
int platform_height = 10;
int platform_coordinate = (border_right + border_left - platform_width) / 2;
int platform_speed = 30;
COLORREF platform_color = 0x00CCCCCC;

// Ball preferences
COLORREF ball_color = 0x0000FFCC;
int ball_radius = 8;
int ball_x = (border_right + border_left) / 2;
int ball_y = border_down - ball_radius - 10;
double ball_max_angle = 75;
double ball_min_angle = 20;
double ball_vector_x = cos(gradus_to_radians(ball_max_angle));
double ball_vector_y = sin(gradus_to_radians(ball_max_angle));
double ball_max_vector_x = cos(gradus_to_radians(ball_max_angle));
double ball_max_vector_y = sin(gradus_to_radians(ball_max_angle));
double ball_min_vector_x = cos(gradus_to_radians(ball_min_angle));
double ball_min_vector_y = sin(gradus_to_radians(ball_min_angle));
int ball_speed = 25;


// Bricks preferences
int bricks_easy[20][3];
int bricks_standart[20][3];
int bricks_middle[20][3];
int bricks_strong[20][3];
int bricks_width = 80;
int bricks_height = 25;
int enable_fourth_level = 0;
COLORREF bricks_easy_color = 0x00FFFFFF;
COLORREF bricks_standart_color = 0x0000FF00;
COLORREF bricks_middle_color_2 = 0x0000FFFF;
COLORREF bricks_middle_color_1 = 0x00009999;
COLORREF bricks_strong_color_3 = 0x000000FF;
COLORREF bricks_strong_color_2 = 0x000000AA;
COLORREF bricks_strong_color_1 = 0x00000066;

// General methods
template <typename T>
T print(int argc, T* argv, string sep, string end) {
	for (int i = 0; i < argc; i++) {
		cout << argv[i] << sep;
	}
	cout << end;
	return NULL;
}
void display() {
	

	// Clear console
	system("cls");
	/*int tmp1[2] = { platform_coordinate, platform_coordinate + platform_width };
	print<int>(2, tmp1, " ", "\n");*/

	// Enter score
	cout << "Score: " << score << endl;

	// Display borders firstly
	pen = CreatePen(PS_SOLID, 2, border_color);
	SelectObject(hdc, pen);
	MoveToEx(hdc, border_left, border_up, NULL);
	LineTo(hdc, border_left, border_down);
	MoveToEx(hdc, border_left, border_up, NULL);
	LineTo(hdc, border_right, border_up);
	MoveToEx(hdc, border_right, border_down, NULL);
	LineTo(hdc, border_right, border_up);
	DeleteObject(pen);

	// Display platform
	brush = CreateSolidBrush(platform_color);
	SelectObject(hdc, brush);
	Rectangle(
		hdc,
		platform_coordinate, // x left-up
		border_down + platform_height, // y left-up
		platform_coordinate + platform_width, // x right-down
		border_down // y right-down
	);
	DeleteObject(brush);

	brush = CreateSolidBrush(ball_color);
	SelectObject(hdc, brush);
	Ellipse(
		hdc,
		ball_x - ball_radius,
		ball_y - ball_radius,
		ball_x + ball_radius,
		ball_y + ball_radius
		);
	DeleteObject(brush);

	brush3 = CreateSolidBrush(bricks_strong_color_3);
	brush2 = CreateSolidBrush(bricks_strong_color_2);
	brush1 = CreateSolidBrush(bricks_strong_color_1);
	
	for (int i = 0; i < 20; i++) {
		switch (bricks_strong[i][2])
		{
		case 3:
			SelectObject(hdc, brush3);
			break;
		case 2:
			SelectObject(hdc, brush2);
			break;
		case 1:
			SelectObject(hdc, brush1);
			break;
		default:
			SelectObject(hdc, brush);
		}
		if (bricks_strong[i][2] > 0) {
			Rectangle(
				hdc,
				bricks_strong[i][0],
				bricks_strong[i][1],
				bricks_strong[i][0] + bricks_width,
				bricks_strong[i][1] + bricks_height
			);
		}
	}
	DeleteObject(brush);

	brush2 = CreateSolidBrush(bricks_middle_color_2);
	brush1 = CreateSolidBrush(bricks_middle_color_1);
	
	for (int i = 0; i < 20; i++) {
		switch (bricks_middle[i][2])
		{
		case 2:
			SelectObject(hdc, brush2);
			break;
		case 1:
			SelectObject(hdc, brush1);
			break;
		default:
			SelectObject(hdc, brush);
			break;
		}
		if (bricks_middle[i][2] > 0) {
			Rectangle(
				hdc,
				bricks_middle[i][0],
				bricks_middle[i][1],
				bricks_middle[i][0] + bricks_width,
				bricks_middle[i][1] + bricks_height
			);
		}
	}
	DeleteObject(brush);

	brush = CreateSolidBrush(bricks_standart_color);
	SelectObject(hdc, brush);
	for (int i = 0; i < 20; i++) {
		if (bricks_standart[i][2] > 0) {
			Rectangle(
				hdc,
				bricks_standart[i][0],
				bricks_standart[i][1],
				bricks_standart[i][0] + bricks_width,
				bricks_standart[i][1] + bricks_height
			);
		}
	}
	DeleteObject(brush);

	brush = CreateSolidBrush(bricks_easy_color);
	SelectObject(hdc, brush);
	for (int i = 0; i < 20; i++) {
		if (bricks_easy[i][2] > 0) {
			Rectangle(
				hdc,
				bricks_easy[i][0],
				bricks_easy[i][1],
				bricks_easy[i][0] + bricks_width,
				bricks_easy[i][1] + bricks_height
			);
		}
	}
	DeleteObject(brush);
}
void controller() {
	// GetKeyConsole returns -128 if VK_<KEY> is pressed and 0 if it is not
	if (GetKeyState(VK_LEFT) < 0) {
		platform_direction = Direction::left;
	}
	else if (GetKeyState(VK_RIGHT) < 0) {
		platform_direction = Direction::right;
	}
}
bool find_intersection(int x1, int y1, int x2, int y2) {
	if (
		y1 >= x2 && x1 <= x2 || // intersection type: [    (   ]     )
		x1 >= x2 && y1 <= y2 || // intersection type: (  [        ]  )
		x1 <= y2 && y1 >= y2 || // intersection type: (      [   )   ]
		x1 <= x2 && y1 >= y2    // intersection type: [  (        )  ]
		) {
		return true;
	}
	return false;
}
void normalize() {
	hypotenuse = sqrt(pow(ball_vector_x, 2) + pow(ball_vector_y, 2));
	ball_vector_x /= hypotenuse;
	ball_vector_y /= hypotenuse;
}
double gradus_to_radians(double gradus) {
	return M_PI * gradus / 180.0;  // 2Pi = 360 grads -> 1 grad = Pi / 180 -> N grad = Pi * N / 180 rad
}
short sign(double n) {
	return n < 0 ? -1 : n > 0 ? 1 : 0;
}
bool is_digit(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] > '9' || s[i] < '0') {
			return false;
		}
	}
	return true;
}


// Platform methods
void platform_move() {
	if (platform_direction == Direction::left) {
		if (platform_coordinate - platform_speed > border_left) {
			platform_coordinate -= platform_speed;
		}
		else {
			platform_coordinate = border_left;
		}
	}
	else if (platform_direction == Direction::right) {
		if (platform_coordinate + platform_speed < border_right - platform_width) {
			platform_coordinate += platform_speed;
		}
		else {
			platform_coordinate = border_right - platform_width;
		}
	}
}

// Ball methods
void ball_move() {
	ball_x += ball_speed * ball_vector_x;
	ball_y += ball_speed * ball_vector_y;
}
void ball_check_collision() {
	// Collision with right border
	if (ball_x + ball_radius >= border_right) {
		ball_vector_x *= -1;
		ball_x = border_right - ball_radius;
	}

	// Collision with left border
	if (ball_x - ball_radius <= border_left) {
		ball_vector_x *= -1;
		ball_x = border_left + ball_radius;
	}
	
	// Collision with ceil
	if (ball_y - ball_radius <= border_up) {
		ball_vector_y *= -1;
		ball_y = border_up + ball_radius;
	}

	// Collision with platform
	if (ball_y + ball_radius >= border_down) {
		if (find_intersection(
			ball_x - ball_radius,
			ball_x + ball_radius,
			platform_coordinate,
			platform_coordinate + platform_width
		)) {
			ball_vector_y *= -1;

			/* I know that this code may be more optimal, but now 2:21 AM and it works.
			   Please don't touch this code and don't try to simplify it with math.
			   I think I had some reason to do in in this style, but I forgot everything. */
			int quarter = 0;
			if (ball_x - platform_coordinate < platform_width / 4) {
				quarter = 0;
			}
			else if (ball_x - platform_coordinate < platform_width / 2) {
				quarter = 1;
			}
			else if (ball_x - platform_coordinate < 3 * platform_width / 4) {
				quarter = 2;
			}
			else {
				quarter = 3;
			}

			// It is difficult formula that I deleted because I drew it in paint
			k = 0;
			switch (quarter)
			{
			case 0:
				if (ball_x - platform_coordinate <= 0) { k = 1; }
				else if (ball_x - platform_coordinate >= platform_width / 4) { k = 0; }
				else { k = 1.0 - (double)(ball_x - platform_coordinate) / (double)(platform_width / 4); }
				break;
			case 1:
				if (ball_x - platform_coordinate - platform_width / 4 <= 0) { k = 0; }
				else if (ball_x - platform_coordinate - platform_width / 4 >= platform_width / 4) { k = 1; }
				else { k = (double)(ball_x - platform_coordinate - platform_width / 4) / (double)(platform_width / 4); }
				break;
			case 2:
				if (ball_x - platform_coordinate - platform_width / 2 <= 0) { k = 1; }
				else if (ball_x - platform_coordinate - platform_width / 2 >= platform_width / 4) { k = 0; }
				else { k = 1.0 - (double)(ball_x - platform_coordinate - platform_width / 2) / (double)(platform_width / 4); }
				break;
			case 3:
				if (ball_x - platform_coordinate - 3 * platform_width / 4 <= 0) { k = 0; }
				else if (ball_x - platform_coordinate - 3 * platform_width / 4 >= platform_width / 4) { k = 1; }
				else { k = (double)(ball_x - platform_coordinate - 3 * platform_width / 4) / (double)(platform_width / 4); }
				break;
			default:
				break;
			}

			angle = acos(abs(ball_vector_x));
			new_angle = angle * (1 - k) + k * gradus_to_radians(quarter == 1 or quarter == 2 ? ball_max_angle : ball_min_angle);
			ball_vector_x = cos(new_angle) * sign(ball_vector_x);
			ball_vector_y = sin(new_angle) * sign(ball_vector_y);
			normalize();
			ball_y = border_down - ball_radius;
		}
		else {
			system("cls");
			string flag_play_again = "";
			cout << "Game Over" << endl;
			cout << "Your score: " << score << endl;
			cout << "Play again? (y/n) ";
			cin >> flag_play_again;
			while (flag_play_again != "y" && flag_play_again != "n") {
				cout << "Enter y or n: ";
				cin >> flag_play_again;
			}
			if (flag_play_again == "y") {
				system(path.c_str());
			}
			exit(-1);
		}
	}

	// Collision with easy brick
	for (int i = 0; i < 20; i++) {
		if (
			find_intersection(
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_easy[i][0],
				bricks_easy[i][0] + bricks_width
			) && 
			find_intersection(
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_easy[i][1],
				bricks_easy[i][1] + bricks_height
			) && bricks_easy[i][2] != 0
			) {
				// Value of vertical intersection (     {    [......}    ]     )
				int ys[4] = {
					ball_y - ball_radius,
					ball_y + ball_radius,
					bricks_easy[i][1],
					bricks_easy[i][1] + bricks_height
				};
				sort(ys, ys + 3);
				y_intersection = ys[2] - ys[1];

				// Value of horisontal intersection (     {    [......}    ]     )
				int xs[4] = {
					ball_x - ball_radius,
					ball_x + ball_radius,
					bricks_easy[i][0],
					bricks_easy[i][0] + bricks_width
				};
				sort(xs, xs + 3);
				x_intersection = xs[2] - xs[1];

				// If horisontal intersection is greater than vertical one - reflect on X axis
				if (x_intersection > y_intersection) {
					ball_vector_x *= -1;
					if (ball_vector_x < 0) {
						ball_x -= x_intersection;
					}
					else {
						ball_x += x_intersection;
					}
				}
				else {
					ball_vector_y *= -1;
					if (ball_vector_y < 0) {
						ball_y -= y_intersection;
					}
					else {
						ball_y += y_intersection;
					}
				}
				bricks_easy[i][2]--;
				score += 1;
				break;
		}
	}

	// Collision with standart brick
	for (int i = 0; i < 20; i++) {
		if (
			find_intersection(
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_standart[i][0],
				bricks_standart[i][0] + bricks_width
			) &&
			find_intersection(
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_standart[i][1],
				bricks_standart[i][1] + bricks_height
			) && bricks_standart[i][2] != 0
			) {
			int ys[4] = {
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_standart[i][1],
				bricks_standart[i][1] + bricks_height
			};
			sort(ys, ys + 3);
			int y_intersection = ys[2] - ys[1];

			int xs[4] = {
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_standart[i][0],
				bricks_standart[i][0] + bricks_width
			};
			sort(xs, xs + 3);
			int x_intersection = xs[2] - xs[1];
			if (x_intersection > y_intersection) {
				ball_vector_x *= -1;
				if (ball_vector_x < 0) {
					ball_x -= x_intersection;
				}
				else {
					ball_x += x_intersection;
				}
			}
			else {
				ball_vector_y *= -1;
				if (ball_vector_y < 0) {
					ball_y -= y_intersection;
				}
				else {
					ball_y += y_intersection;
				}
			}
			bricks_standart[i][2]--;
			score += 2;
			break;
		}
	}

	// Collision with middle brick
	for (int i = 0; i < 20; i++) {
		if (
			find_intersection(
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_middle[i][0],
				bricks_middle[i][0] + bricks_width
			) &&
			find_intersection(
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_middle[i][1],
				bricks_middle[i][1] + bricks_height
			) && bricks_middle[i][2] != 0
			) {
			int ys[4] = {
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_middle[i][1],
				bricks_middle[i][1] + bricks_height
			};
			sort(ys, ys + 3);
			y_intersection = ys[2] - ys[1];

			int xs[4] = {
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_middle[i][0],
				bricks_middle[i][0] + bricks_width
			};
			sort(xs, xs + 3);
			x_intersection = xs[2] - xs[1];
			if (x_intersection > y_intersection) {
				ball_vector_x *= -1;
				if (ball_vector_x < 0) {
					ball_x -= x_intersection;
				}
				else {
					ball_x += x_intersection;
				}
			}
			else {
				ball_vector_y *= -1;
				if (ball_vector_y < 0) {
					ball_y -= y_intersection;
				}
				else {
					ball_y += y_intersection;
				}
			}
			bricks_middle[i][2]--;
			if (bricks_middle[i][2] == 0) {
				score += 3;
			}
			break;
		}
	}

	// Collision with strong brick
	for (int i = 0; i < 20; i++) {
		if (
			find_intersection(
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_strong[i][0],
				bricks_strong[i][0] + bricks_width
			) &&
			find_intersection(
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_strong[i][1],
				bricks_strong[i][1] + bricks_height
			) && bricks_strong[i][2] != 0
			) {
			int ys[4] = {
				ball_y - ball_radius,
				ball_y + ball_radius,
				bricks_strong[i][1],
				bricks_strong[i][1] + bricks_height
			};
			sort(ys, ys + 3);
			y_intersection = ys[2] - ys[1];

			int xs[4] = {
				ball_x - ball_radius,
				ball_x + ball_radius,
				bricks_strong[i][0],
				bricks_strong[i][0] + bricks_width
			};
			sort(xs, xs + 3);
			x_intersection = xs[2] - xs[1];
			if (x_intersection > y_intersection) {
				ball_vector_x *= -1;
				if (ball_vector_x < 0) {
					ball_x -= x_intersection;
				}
				else {
					ball_x += x_intersection;
				}
			}
			else {
				ball_vector_y *= -1;
				if (ball_vector_y < 0) {
					ball_y -= y_intersection;
				}
				else {
					ball_y += y_intersection;
				}
			}
			bricks_strong[i][2]--;
			if (bricks_strong[i][2] == 0) {
				score += 4;
			}
			break;
		}
	}
}

// Bricks methods
void bricks_setup() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			bricks_strong[10 * i + j][0] = border_left + j * bricks_width;
			bricks_strong[10 * i + j][1] = border_up + i * bricks_height;
			bricks_strong[10 * i + j][2] = 3;
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			bricks_middle[10 * i + j][0] = border_left + j * bricks_width;
			bricks_middle[10 * i + j][1] = border_up + (i+2) * bricks_height;
			bricks_middle[10 * i + j][2] = 2;
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			bricks_standart[10 * i + j][0] = border_left + j * bricks_width;
			bricks_standart[10 * i + j][1] = border_up + (i + 4) * bricks_height;
			bricks_standart[10 * i + j][2] = 1;
		}
	}
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 10; j++) {
			bricks_easy[10 * i + j][0] = border_left + j * bricks_width;
			bricks_easy[10 * i + j][1] = border_up + (i + 6) * bricks_height;
			bricks_easy[10 * i + j][2] = enable_fourth_level;
		}
	}
}

int main(int argc, char *argv[]) {
	system("cls");
	path = argv[0];
	cout << "Hello! It is arkanoid! Choose difficulty level:\n1. Easy\n2. Medium\n3. Hard\n4. Extreme\n";
	int diff = 0;
	while (true) {
		string difficulty;
		cin >> difficulty;
		if (is_digit(difficulty)) {
			diff = stoi(difficulty);
			if (diff <= 4 && diff >= 1) {
				break;
			}
		}
		cout << "Enter number from 1 to 4" << endl;
	}

	switch (diff)
	{
	case 1:
		platform_width = 400;
		platform_speed = 25;
		ball_speed = 18;
		border_down = 410;
		bricks_height = 25;
		break;
	case 2:
		platform_width = 200;
		platform_speed = 30;
		ball_speed = 25;
		border_down = 410;
		bricks_height = 30;
		break;
	case 3:
		platform_width = 150;
		platform_speed = 40;
		ball_speed = 30;
		border_down = 510;
		bricks_height = 35;
		break;
	case 4:
		platform_width = 90;
		platform_speed = 50;
		ball_speed = 35;
		border_down = 610;
		bricks_height = 40;
		enable_fourth_level = 1;
		break;
	default:
		break;
	}

	platform_coordinate = (border_right + border_left - platform_width) / 2;

	bricks_setup();
	while(true) {
		display();
		ball_check_collision();
		controller();
		platform_move();
		ball_move();
		Sleep(tick * 1000);
	}
	
}