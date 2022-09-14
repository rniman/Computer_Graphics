#include <iostream>
#include <random>
#include <string>
#include <limits>
#include <iomanip>
#include <Windows.h>
#include <conio.h>

using namespace std;

const int board_width = 3;
const int max_choice = 20;

random_device rd;
mt19937_64 gen(rd());

class Card
{
private:
	string m_kind;
	bool m_upend;
	int m_x, m_y;

public:
	Card()
		:m_upend(false)
	{

	}

	Card(const string& i_kind)
		:m_kind(i_kind), m_upend(false)
	{

	}

	string getValue() const
	{
		return m_kind;
	}

	int getValue_int() const
	{
		return m_kind[0];
	}

	bool getUpend() const
	{
		return m_upend;
	}

	void Upend()
	{
		m_upend = m_upend ? false : true;
	}

	bool empty()
	{

	}



	friend void initCard(Card card[][4]);
	friend bool operator ==(const Card& c1, const Card& c2);
	friend bool operator !=(const Card& c1, const Card& c2);

};

void initCard(Card card[][4])
{
	bool empty[4][4];

	for (char c = 'A'; c <= 'H'; ++c)
	{
		for (int j = 0; j < 2; ++j)
		{
			uniform_int_distribution<int> dis(0, 3);
			int t_x = dis(gen);
			int t_y = dis(gen);
			if (empty[t_y][t_x])
			{
				card[t_y][t_x].m_kind = c;
				card[t_y][t_x].m_upend = false;
				empty[t_y][t_x] = false;
				//m_x,m_y필요없을듯?
			}
			else
				j--;

		}
	}
}

void printCard(const Card card[][4])
{

	cout << setw(board_width) << ' ';
	for (char c = 'a'; c <= 'd'; ++c)
	{
		cout << setw(board_width) << c;
	}
	cout << endl;
	for (int i = 0; i < 4; ++i)
	{
		cout << setw(board_width) << i + 1;
		for (int j = 0; j < 4; ++j)
		{
			if (card[i][j].getUpend())
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), card[i][j].getValue_int() - 64);
				cout << setw(board_width) << card[i][j].getValue();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else
				cout << setw(board_width) << '*';
		}
		cout << endl;
	}
}

void printCard(const Card card[][4],const int& n_x,const int& n_y)
{

	cout << setw(board_width) << ' ';
	for (char c = 'a'; c <= 'd'; ++c)
	{
		cout << setw(board_width) << c;
	}
	cout << endl;
	for (int i = 0; i < 4; ++i)
	{
		cout << setw(board_width) << i + 1;
		for (int j = 0; j < 4; ++j)
		{	

			if (card[i][j].getUpend())
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), card[i][j].getValue_int() - 64);
				cout << setw(board_width) << card[i][j].getValue();
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
			}
			else if (i == n_y && j == n_x)
			{
				cout << setw(board_width) << 'O';
			}
			else
				cout << setw(board_width) << '*';
		}
		cout << endl;
	}
}

bool operator ==(const Card& c1, const Card& c2)
{
	return c1.m_kind == c2.m_kind;
}

bool operator !=(const Card& c1, const Card& c2)
{
	return c1.m_kind != c2.m_kind;
}

bool findAll(Card card[][4])
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (!card[i][j].getUpend())
			{
				return false;
			}
		}
	}
	return true;
}

int main()
{
	Card card[4][4];

	initCard(card);

	int choiceNum = 0;
	int score = 0;



	while (1)
	{
		if (findAll(card))
		{
			system("cls");
			cout << "Game Clear" << endl;
			cout << "Score: " << score + choiceNum * 150 << endl;
			cout << "Now ChoiceNum: " << choiceNum << endl;

			char end;
			cout << "r-다시 시작 / 아무키 종료" << endl;
			cout << "명령 입력: ";
			cin >> end;

			if (end != 'r')
				break;
			system("cls");
			initCard(card);

			choiceNum = 0;
			score = 0;
			cin.ignore(100, '\n');
			continue;
		}

		if (choiceNum >= max_choice)
		{
			system("cls");
			cout << "Game Over" << endl;
			cout << "Score: " << score << endl;

			char end;
			cout << "r-다시 시작 / 아무키 종료" << endl;
			cout << "명령 입력: ";
			cin >> end;

			if (end != 'r')
				break;
			system("cls");
			initCard(card);

			choiceNum = 0;
			score = 0;
			cin.ignore(100, '\n');
			continue;
		}

		cout << "q-종료 / r-다시시작 / k-키보드이동 사용 / 아무키 입력시 위치 직접 입력" << endl;
		cout << "명령 입력:";
		char command;
		cin >> command;
		system("cls");
		
		if (command == 'q')
		{
			cout << "게임 종료" << endl;
			break;
		}
		else if (command == 'r')
		{
			initCard(card);

			choiceNum = 0;
			score = 0;
			cin.ignore(100, '\n');
			continue;
		}
		else if (command == 'k')
		{
			int enter_num = 0;
			int n_x = 0, n_y = 0;
			int fir_x = 0, fir_y = 0;
			while (1)
			{
				system("cls");
				cout << "Now Score: " << score << endl;
				cout << "Now ChoiceNum: " << choiceNum << endl;

				cout << "현재 커서: " << (char)(n_x + 97) << ", " << n_y << endl;
				printCard(card, n_x, n_y);
				char move;
				
				move = _getch();
				if (move == 72)//UP
				{
					n_y--;
					if (n_y < 0)
						n_y = 3;
				}
				else if (move == 80)//DOWN
				{
					n_y++;
					if (n_y > 3)
						n_y = 0;
				}
				else if (move == 75)//LEFT
				{
					n_x--;
					if (n_x < 0)
						n_x = 3;
				}
				else if (move == 77)//RIGHT
				{
					n_x++;
					if (n_x > 3)
						n_x = 0;
				}


				if (move == 13)
				{
					if (card[n_y][n_x].getUpend())
						continue;

					if (enter_num == 0)
					{
						fir_x = n_x;
						fir_y = n_y;
					}
					card[n_y][n_x].Upend();
					enter_num++;
				}

				if (enter_num == 2)
				{
					printCard(card, n_x, n_y);
					if (card[n_y][n_x] != card[fir_y][fir_x])
					{
						card[n_y][n_x].Upend();
						card[fir_y][fir_x].Upend();
					}
					else
					{
						score += 100;
					}
					choiceNum++;
					
					break;
				}
			}

			cin.ignore(100, '\n');
			continue;
		}
		
		
		cin.ignore(100, '\n');
		string choice1, choice2;

		cout << "Now Score: " << score << endl;
		cout << "Now ChoiceNum: " << choiceNum << endl;
		printCard(card);
		
		cin >> choice1;
		cin >> choice2;

		if (choice1[0] < 97 || choice1[0] > 105)
		{
			cin.ignore(100, '\n');
			continue;
		}
		if (choice1[1] < 49 || choice1[1] > 57)
		{
			cin.ignore(100, '\n');
			continue;
		}
		if (choice2[0] < 97 || choice2[0] > 105)
		{
			cin.ignore(100, '\n');
			continue;
		}
		if (choice2[1] < 49 || choice2[1] > 57)
		{
			cin.ignore(100, '\n');
			continue;
		}

		//뒤집히지 않은 것을 뒤집을때까지 뒤집음
		while (card[choice1[1] - 49][choice1[0] - 97].getUpend())
		{
			cin.ignore(100, '\n');
			cin >> choice1;
		}

		card[choice1[1] - 49][choice1[0] - 97].Upend();

		while (card[choice2[1] - 49][choice2[0] - 97].getUpend())
		{
			cin.ignore(100, '\n');
			cin >> choice2;
		}

		card[choice2[1] - 49][choice2[0] - 97].Upend();

		cout << endl;
		printCard(card);
		cout << endl;

		if (card[choice1[1] - 49][choice1[0] - 97] != card[choice2[1] - 49][choice2[0] - 97])
		{
			card[choice1[1] - 49][choice1[0] - 97].Upend();
			card[choice2[1] - 49][choice2[0] - 97].Upend();
		}
		else
			score += 100;

		choiceNum++;
		cin.ignore(100, '\n');



	}


	return 0;
}