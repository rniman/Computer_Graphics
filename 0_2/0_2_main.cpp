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


	

	return 0;
}