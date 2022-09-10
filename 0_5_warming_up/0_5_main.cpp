#include <iostream>
#include <array>
#include <algorithm>
#include <cstring>
using namespace std;

struct Vertex
{
	int x, y, z;
	int index = -1;
	int len = 0;
	bool empty = true;

	unsigned cal_lenth() const;
};

unsigned Vertex::cal_lenth() const
{
	return x * x + y * y + z * z;
}

void max_lenth(const array<Vertex, 10>& arr_v, const int& v_num)
{
	if (v_num == 0)
	{
		cout << "����� ���� ����!" << endl;
		return;
	}

	Vertex max_v = { 0,0,0,-1,-1 };
	for (unsigned i = 0; i < v_num; ++i)
	{
		if (arr_v[i].len > max_v.len)
			max_v = arr_v[i];
	}

	cout << "���� �� �Ÿ��� �ִ� ��: (" << max_v.x << ", " << max_v.y << ", " << max_v.z << ")" << endl;
}

void min_lenth(const array<Vertex, 10>& arr_v, const int& v_num)
{
	if (v_num == 0)
	{
		cout << "����� ���� ����!" << endl;
		return;
	}

	Vertex min_v = { 0,0,0,-1,100000000000000000 };
	for (unsigned i = 0; i < v_num; ++i)
	{
		if (arr_v[i].len < min_v.len)
			min_v = arr_v[i];
	}

	cout << "���� ����� �Ÿ��� �ִ� ��: (" << min_v.x << ", " << min_v.y << ", " << min_v.z << ")" << endl;
}

void print_Vertex(const array<Vertex, 10>& list)
{
	for (int i = 9; i >= 0; --i)
	{
		cout << i << '|';
		if (!list[i].empty)
			cout << " ( " << list[i].x << ", " << list[i].y << ", " << list[i].z << " )" << " \t" << list[i].index;
		cout << endl;
	}
}

int main()
{
	array<Vertex, 10> vertex_list;
	int vertex_num = 0;
	bool sort_state = false;


	while (1)
	{
		print_Vertex(vertex_list);
		char command;
		cin >> command;
		if (command == 'q')
			break;
		else if (command == '+')
		{
			if (vertex_num == 10)
			{
				cin.ignore();
				cout << "�߰� �Ұ���!" << endl;
				continue;
			}
			int i_x, i_y, i_z;
			cin >> i_x >> i_y >> i_z;
		
			if (cin.fail())
			{
				cout << "�߸��� ��ǥ���Է�!" << endl;
				cin.clear();
				cin.ignore(100,'\n');
				continue;
			}

			for (int i = 9; i >= 0; --i)
			{
				if (!vertex_list[i].empty)
				{
					//���� ������ ���ִ� ���
					if (i == 9)
					{
						int j = 0;
						for (j; !vertex_list[j].empty; ++j)
						{}
						
						vertex_list[j].x = i_x;
						vertex_list[j].y = i_y;
						vertex_list[j].z = i_z;
						vertex_list[j].empty = false;
						break;
					}

					vertex_list[i+1].x = i_x;
					vertex_list[i+1].y = i_y;
					vertex_list[i+1].z = i_z;
					vertex_list[i+1].empty = false;
					break;
				}
				
				//0��° 
				if (i == 0)
				{
					vertex_list[i].x = i_x;
					vertex_list[i].y = i_y;
					vertex_list[i].z = i_z;
					vertex_list[i].empty = false;
				}
			}

			vertex_num++;
			for (unsigned i = 0; i < 10; ++i)
				if(!vertex_list[i].empty)
					vertex_list[i].index = i;

		}
		else if (command == '-')
		{
			if (vertex_num == 0)
			{
				cin.ignore();
				cout << "���� �Ұ���!" << endl;
				continue;
			}

			for (int i = 9; i >= 0; --i)
			{
				if (!vertex_list[i].empty)
				{
					vertex_list[i].x = 0;
					vertex_list[i].y = 0;
					vertex_list[i].z = 0;
					vertex_list[i].empty = true;
					break;
				}
			}
			vertex_num--;
			for (unsigned i = 0; i < 10; ++i)
				if (!vertex_list[i].empty)
					vertex_list[i].index = i;
		}
		else if (command == 'e')
		{
			if (vertex_num == 10)
			{
				cin.ignore();
				cout << "�߰� �Ұ���!" << endl;
				continue;
			}
			int i_x, i_y, i_z;
			cin >> i_x >> i_y >> i_z;

			if (cin.fail())
			{
				cout << "�߸��� ��ǥ���Է�!" << endl;
				cin.clear();
				cin.ignore(100, '\n');
				continue;
			}


			//0��°�� �������� ������ �̵�
			if (!vertex_list[0].empty)
			{
				int i = 0;
				for ( i ; !vertex_list[i].empty ; ++i)
				{}

				memmove(&vertex_list.at(1), &vertex_list.at(0), sizeof(Vertex)* (i));
			}
				
			vertex_list[0].empty = false;
			vertex_list[0].x = i_x;
			vertex_list[0].y = i_y;
			vertex_list[0].z = i_z;
			vertex_num++;

			for (unsigned i = 0; i < 10; ++i)
				if (!vertex_list[i].empty)
					vertex_list[i].index = i;
		}
		else if (command == 'd')
		{
			if (vertex_num == 0)
			{
				cin.ignore();
				cout << "���� �Ұ���!" << endl;
				continue;
			}

			vertex_num--;
			for (int i = 0; i < 10; ++i)
			{
				if (!vertex_list[i].empty)
				{
					vertex_list[i].x = 0;
					vertex_list[i].y = 0;
					vertex_list[i].z = 0;
					vertex_list[i].index = -1;
					vertex_list[i].empty = true;
					break;
				}
			}
		}
		else if (command == 'l')
		{
			cout << "����� ���� ����: " << vertex_num << endl;
		}
		else if (command == 'c')
		{
			vertex_num = 0;
			vertex_list.fill({ 0,0,0,-1,0,true });
		}
		else if (command == 'm')
		{
			for (unsigned i = 0; i < vertex_num; ++i)
			{
				vertex_list[i].len = vertex_list[i].cal_lenth();
			}
			max_lenth(vertex_list, vertex_num);
		}
		else if (command == 'n')
		{
			for (unsigned i = 0; i < vertex_num; ++i)
			{
				vertex_list[i].len = vertex_list[i].cal_lenth();
			}
			min_lenth(vertex_list, vertex_num);
		}
		else if (command == 'p')
		{
			for (unsigned i = vertex_num; i > 0; --i)
			{
				cout << "���� ��ǥ: (" << vertex_list[i - 1].x << ", " << vertex_list[i - 1].y <<
					", " << vertex_list[i - 1].z << ")" << endl;
			}
		}
		else if (command == 's')
		{
			if (vertex_num == 0)
			{
				cout << "������ ���� �����ϴ�!" << endl;
				continue;
			}

			if (!sort_state)
			{
				for (unsigned i = 0; i < 10; ++i)
				{
					if (!vertex_list[i].empty)
						vertex_list[i].len = vertex_list[i].cal_lenth();
					else
						vertex_list[i].len = -1;
				}
				sort(&vertex_list[0] ,&vertex_list[vertex_num] + 1,
					[](Vertex v1, Vertex v2) {
						return v1.len > v2.len;
					});
				sort_state = true;
			}
			else
			{
				array<Vertex, 10> temp_arr;
				for (unsigned i = 0; i < 10; ++i)
				{
					if (!vertex_list[i].empty)
						temp_arr[vertex_list[i].index] = vertex_list[i];
				}
				vertex_list = temp_arr;
				sort_state = false;
			}
		}
		else
		{
			cout << "�߸��� ��ɾ�!" << endl;
		}
		cin.ignore(100, '\n');
	}

	return 0;
}


