#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	ifstream ifs;
	string file_name;
	cin >> file_name;

	ifs.open(file_name);

	while (ifs.fail())
	{
		cin.ignore();
		cin >> file_name;

		ifs.open(file_name);
	}

	vector<string> str;
	string temp;

	while (!ifs.eof())
	{
		getline(ifs, temp);
		str.push_back(temp);	
	}
	for (auto& e : str)
	{
		cout << e << endl;
	}

	ifs.seekg(0, ios::beg);
	string word;
	int word_num = 0;
	int number_num = 0;
	int capital_num = 0;

	while(ifs >> word)
	{
		for (unsigned i = 0; i < word.size(); ++i)
		{
			if (!isdigit(word[i]))
			{
				word_num++;
				break;
			}

			if (i == word.size() - 1)
				number_num++;
		}

		for (unsigned i = 0; i < word.size(); ++i)
		{
			if (word[i] >= 65 && word[i] <= 90)
			{
				capital_num++;
				break;
			}
		}
	}
	cout << endl;
	cout <<"단어의 개수: "<< word_num << endl;
	cout <<"숫자의 개수: "<< number_num << endl;
	cout << "대문자를 포함한 단어의 개수: " << capital_num << endl;

	char command;
	bool e_command = false;
	ifs.seekg(0, ios::beg);

	cin >> command;
	while (command != 'q')
	{
		switch (command)
		{
		case 'd':
			for (unsigned i = 0; i < str.size(); ++i)
			{
				string temp(str[i]);
				for (unsigned j = 0; j < str[i].size(); ++j)
				{
					temp[j] = str[i][str[i].size() - j - 1];
				}
				str[i] = temp;
			}
			break;
		case 'e':
			if (!e_command)
			{
				for (unsigned i = 0; i < str.size(); ++i)
				{
					int insert_num = 0;
					for (unsigned j = 0; j < str[i].size(); ++j)
					{
						if ((j - 2 * insert_num) % 3 == 2)
						{
							str[i].insert(j + 1, 2, '@');
							insert_num++;
							j += 2;
						}
					}
				}
			}
			else
			{
				for (unsigned i = 0; i < str.size(); ++i)
				{
					str[i].erase(remove(str[i].begin(), str[i].end(), '@'), str[i].end());
				}
			}
			e_command = e_command ? false : true;
			break;

		case 'f':
			break;

		case 'g':
			break;

		case 'h':
			break;

		default:
			break;
		}
		cin.ignore();
		cout << "\x1B[2J\x1B[H";
		for (auto& e : str)
		{
			cout << e << endl;
		}
		cin >> command;
	}
	

	return 0;
}