#include <iostream>
#include <random>
#include <conio.h>
using namespace std;

struct Rect
{
	int x1, y1;
	int x2, y2;
};

void print_rect(const Rect& r)
{
	cout << "{{" << r.x1 << ',' << r.y1 << "}, {" << r.x2 << ',' << r.y2 << "}}" << endl;
}

void rect_move(Rect& r, const char& cm)
{
	if (cm == 'w' || cm == 'i')
	{
		r.y1 -= 20;
		r.y2 -= 20;
		if (r.y1 < 0 || r.y2 < 0)
		{
			cout << "이동불가!"<<endl;
			r.y1 += 20;
			r.y2 += 20;
		}
	}
	else if (cm == 'a' || cm == 'j')
	{
		r.x1 -= 20;
		r.x2 -= 20;
		if (r.x1 < 0 || r.x2 < 0)
		{
			cout << "이동불가!" << endl;
			r.x1 += 20;
			r.x2 += 20;
		}
	}
	else if (cm == 's' || cm == 'k')
	{
		r.y1 += 20;
		r.y2 += 20;
		if (r.y1 > 600 || r.y2 > 600)
		{
			cout << "이동불가!" << endl;
			r.y1 -= 20;
			r.y2 -= 20;
		}
	}
	else if (cm == 'd' || cm == 'l')
	{
		r.x1 += 20;
		r.x2 += 20;
		if (r.x2 > 800 || r.x1 > 800)
		{
			cout << "이동불가!" << endl;
			r.x1 -= 20;
			r.x2 -= 20;
		}
	}
}

char keyhit()
{
	while (!_kbhit())
	{
		return _getch();
	}
}

void check_collision(const Rect& r1, const Rect& r2)
{
	if (r1.x1 < r2.x2 && r1.x2 > r2.x1 && r1.y1 < r2.y2 && r1.y2 > r2.y1)
	{
		cout << "충돌상태!" << endl;
	}
}

int main()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis_x(0, 800);
	std::uniform_int_distribution<int> dis_y(0, 600);

	int x1{ dis_x(gen) };
	int x2{ dis_x(gen) };
	int y1{ dis_y(gen) };
	int y2{ dis_y(gen) };

	Rect rect1, rect2;

	if (x1 <= x2)
	{
		rect1.x1 = x1;
		rect1.x2 = x2;
	}
	else
	{
		rect1.x1 = x2;
		rect1.x2 = x1;
	}
	if (y1 <= y2)
	{
		rect1.y1 = y1;
		rect1.y2 = y2;
	}
	else
	{
		rect1.y1 = y2;
		rect1.y2 = y1;
	}

	x1= dis_x(gen);
	x2= dis_x(gen);
	y1= dis_y(gen);
	y2= dis_y(gen);

	if (x1 <= x2)
	{
		rect2.x1 = x1;
		rect2.x2 = x2;
	}
	else
	{
		rect2.x1 = x2;
		rect2.x2 = x1;
	}
	if (y1 <= y2)
	{
		rect2.y1 = y1;
		rect2.y2 = y2;
	}
	else
	{
		rect2.y1 = y2;
		rect2.y2 = y1;
	}


	cout << "사각형 1: ";
	print_rect(rect1);
	cout << "사각형 2: ";
	print_rect(rect2);

	char command;
	command = keyhit();


	while (command != 'q')
	{
		if (command == 'a' || command == 's' || command == 'd' || command == 'w')
		{
			rect_move(rect1, command);
		}
		else if(command == 'j' || command == 'k' || command == 'l' || command == 'i')
		{
			rect_move(rect2, command);
		}

		check_collision(rect1, rect2);
		cout << endl;
		cout << "사각형 1: ";
		print_rect(rect1);
		cout << "사각형 2: ";
		print_rect(rect2);
		
		command = keyhit();
	}
	cout << "종료" << endl;

	return 0;
}