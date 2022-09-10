#include <iostream>
#include <iomanip>
#include <random>
#include <algorithm>
using namespace std;


const int mapSize = 10;
std::random_device rd;
std::mt19937 gen(rd());

class Me
{
private:
	int m_x;
	int m_y;
	int m_walk;
	int m_continue;
	unsigned int m_dir; //0left 1right 2up 3down
	bool m_left, m_right, m_up, m_down;

public:
	Me()
		:m_x(0),m_y(0),m_walk(1),m_continue(1),m_left(false),m_right(false),m_up(false),m_down(false)
	{
		std::uniform_int_distribution<unsigned int> dis(0, 3);

		m_dir = dis(gen);
		if (m_dir == 0)
			m_dir = 3;
		if (m_dir == 2)
			m_dir = 1;
	}

	int getValueX()
	{
		return m_x;
	}

	int getValueY()
	{
		return m_y;
	}

	int getValueWalk()
	{
		return m_walk;
	}

	bool move()
	{
		bool move_suc = false;
		unsigned int pre_dir = m_dir;
		if (m_dir == 0)
		{
			move_suc = move_left();
		}
		else if (m_dir == 1)
		{
			move_suc = move_right();
		}
		else if (m_dir == 2)
		{
			move_suc = move_up();
		}
		else if (m_dir == 3)
		{
			move_suc = move_down();
		}

		if (move_suc)
		{
			m_continue++;
			m_walk++;

			std::uniform_int_distribution<unsigned int> dis(0, 3);
			m_dir = dis(gen);

			while(m_continue == 8 && pre_dir == m_dir)
			{
				m_dir = dis(gen);
			}

			if (m_dir != pre_dir)
			{
				if (m_dir == 0)
					m_left = true;
				else if (m_dir == 1)
					m_right = true;
				else if (m_dir == 2)
					m_up = true;
				else if (m_dir == 3)
					m_down = true;
			}

			if (pre_dir != m_dir)
				m_continue = 1;

			return true;
		}
		else
		{
			std::uniform_int_distribution<unsigned int> dis(0, 3);
			m_dir = dis(gen);

			while (m_continue == 8 && pre_dir == m_dir)
			{
				m_dir = dis(gen);
			}

			return false;
		}
	}

	bool move_left()
	{
		m_x--;
		if (m_x == -1)
		{
			m_x++;
			return false;
		}

		return true;
	}

	bool move_right()
	{
		m_x++;

		if (m_x == mapSize)
		{
			m_x--;
			return false;
		}

		if (m_x == mapSize - 1 && m_y == mapSize - 1)
			if (m_down && m_left && m_right && m_up)
			{
				return true;
			}
			else
			{
				m_x--;
				return false;
			}

		return true;
	}

	bool move_up()
	{
		m_y--;
		if (m_y == -1)
		{
			m_y++;
			return false;
		}

		return true;
	}

	bool move_down()
	{
		m_y++;

		if (m_y == mapSize)
		{
			m_y--;
			return false;
		}

		if (m_y == mapSize - 1 && m_x == mapSize - 1)
			if (m_down && m_left && m_right && m_up)
			{
				return true;
			}
			else 
			{
				m_y--;
				return false;
			}

		return true;
	}

	void reset()
	{
		m_x = 0;
		m_y = 0;
		m_walk = 1;
		m_continue = 1;
		m_left = false;
		m_right = false;
		m_up = false; 
		m_down = false;

		std::uniform_int_distribution<unsigned int> dis(0, 3);

		m_dir = dis(gen);
		if (m_dir == 0)
			m_dir = 3;
		if (m_dir == 2)
			m_dir = 1;
	}
};


void printMap(const int map[][mapSize] )
{
	for (unsigned i = 0; i < mapSize; ++i)
	{
		for (auto& e : map[i])
		{
			
			cout << setw(5) << e ;
		}
		cout << endl;
		cout << endl;

	}
}

int main()
{
	Me me;

	int map[mapSize][mapSize] = {1,};

	//printMap(map);

	//cout << me.getValueWalk() << "번째: " << me.getValueX() << ", " << me.getValueY() << endl;
	while (1)
	{

		if (me.move())
		{
			//cout << me.getValueWalk() << "번째: " << me.getValueX() << ", " << me.getValueY() << endl;
			if (me.getValueWalk() % 10 == 0)
			{
				cout << endl;
				cout << endl;
				printMap(map);
				cout << endl;
				cout << endl;
			}
			map[me.getValueY()][me.getValueX()] = me.getValueWalk();
		}

		if (me.getValueY() == mapSize-1 && me.getValueX() == mapSize - 1)
			break;
	}

	printMap(map);

	char command;
	while (1)
	{
		cin >> command;
		if (command == 'q')
		{
			break;
		}
		else if (command == 'r')
		{
			memset(map, 0, sizeof(map));
			me.reset();

			//cout << me.getValueWalk() << "번째: " << me.getValueX() << ", " << me.getValueY() << endl;
			while (1)
			{

				if (me.move())
				{
					//cout << me.getValueWalk() << "번째: " << me.getValueX() << ", " << me.getValueY() << endl;
					map[me.getValueY()][me.getValueX()] = me.getValueWalk();
				}

				if (me.getValueY() == mapSize - 1 && me.getValueX() == mapSize - 1)
					break;
			}

			printMap(map);
		}
		else if (command == 'd')
		{
			for(unsigned i =0;i<mapSize;++i)
			{
				for (unsigned j = mapSize; j > 1; --j)
				{
					swap(map[i][j - 1], map[i][j - 2]);
				}
			}
			cout << "오른쪽으로 이동!" << endl;
			printMap(map);
		}
		else if (command == 'a')
		{
			for (unsigned i = 0; i < mapSize; ++i)
			{
				for (unsigned j = 0; j < mapSize - 1; ++j)
				{
					swap(map[i][j + 1], map[i][j]);
				}
			}
			cout << "왼쪽으로 이동!" << endl;
			printMap(map);
		}
		else
		{
			cout << "잘못된 명령어!" << endl;
		}
		cin.ignore();
	}

	return 0;
}