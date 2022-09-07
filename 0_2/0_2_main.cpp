#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
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

			for (unsigned i = 0; i < str.size(); ++i)
			{
				string temp = "";
				for (unsigned j = 0; j < str[i].size(); ++j)
				{
					if (str[i][j] != ' ' && j != str[i].size() - 1)
					{
						temp += str[i][j];
						continue;
					}
					for (unsigned k = 0; k < temp.size(); ++k)
					{
						str[i][j - temp.size() + k] = temp[temp.size() - k - 1];
					}
					temp = "";
				}
			}
			break;
		case 'g':
			cin.ignore();
			char target, change_char;
			cin >> target >> change_char;

			for (unsigned i = 0; i < str.size(); ++i)
			{
				for (unsigned j = 0; j < str[i].size(); ++j)
				{
					if (str[i][j] == target)
						str[i][j] = change_char;
				}
			}
			break;

		case 'h':
			for (unsigned i = 0; i < str.size(); ++i)
			{
				string temp;
				for (unsigned j = 0; j < str[i].size()/2; ++j)
				{
					if (str[i][j] != str[i][str[i].size() - 1 - j])
						break;
	
					temp += str[i][j];
				}
				if(!temp.empty())
					cout << str[i] << ": " << temp << endl;
			}
		
			break;

		default:
			break;
		}
		cin.ignore();
		cout << endl;
		for (auto& e : str)
		{
			cout << e << endl;
		}
		cin >> command;
	}
	

	return 0;
}