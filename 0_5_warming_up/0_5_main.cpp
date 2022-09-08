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

int main()
{
	array<Vertex, 10> vertex_list;
	int vertex_num = 0;
	bool sort_state = false;

	while (1)
	{
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

			vertex_list[vertex_num].x = i_x;
			vertex_list[vertex_num].y = i_y;
			vertex_list[vertex_num].z = i_z;
			vertex_num++;
			for (unsigned i = 0; i < vertex_num; ++i)
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
			vertex_num--;
			vertex_list[vertex_num].x = 0;
			vertex_list[vertex_num].y = 0;
			vertex_list[vertex_num].z = 0;
			vertex_list[vertex_num].index = -1;

			for (unsigned i = 0; i < vertex_num; ++i)
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

			memmove(&vertex_list.at(1), &vertex_list.at(0), sizeof(Vertex) * (vertex_list.size() - 1));
			vertex_list[0].x = i_x;
			vertex_list[0].y = i_y;
			vertex_list[0].z = i_z;
			vertex_num++;

			for(unsigned i=0;i<vertex_num;++i)
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

			memmove(&vertex_list.at(0), &vertex_list.at(1), sizeof(Vertex) * (vertex_list.size() - 1));
			vertex_num--;
			vertex_list[vertex_num].x = 0;
			vertex_list[vertex_num].y = 0;
			vertex_list[vertex_num].z = 0;
			vertex_list[vertex_num].index = -1;

			for (unsigned i = 0; i < vertex_num; ++i)
				vertex_list[i].index = i;
		}
		else if (command == 'l')
		{
			cout << "����� ���� ����: " << vertex_num << endl;
		}
		else if (command == 'c')
		{
			vertex_num = 0;
			vertex_list.fill({ 0,0,0,-1,0 });
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
				for (unsigned i = 0; i < vertex_num; ++i)
				{
					vertex_list[i].len = vertex_list[i].cal_lenth();
				}
				sort(&vertex_list[0] ,&vertex_list[vertex_num-1] + 1,
					[](Vertex v1, Vertex v2) {
						return v1.len < v2.len;
					});
				sort_state = true;
			}
			else
			{
				array<Vertex, 10> temp_arr;
				for (unsigned i = 0; i < vertex_num; ++i)
				{
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
		cin.ignore();
	}

	return 0;
}


