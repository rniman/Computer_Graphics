#include "procession.h"
using namespace std;


int main()
{
	procession p1,p2;
	char command;
	
	while (1)
	{
		cout << "행렬1" << endl;
		p1.print();
		cout << "행렬2" << endl;
		p2.print();

		cin >> command;
		cin.ignore();
		cout << "\x1B[2J\x1B[H";

		if (command == 'q')
		{
			cout << "종료" << endl;
			break;
		}

		switch (command)
		{
		case 'a':
			add(p1, p2);
			break;
		case 'd':
			sub(p1, p2);
			break;
		case 'm':
			mul(p1, p2);
			break;
		case 'r':
			cout << "행렬1 det: " << p1.getDet() << endl;
			cout << "행렬2 det: " << p2.getDet() << endl;
			break;
		case 't':
			p1.transpose();
			p2.transpose();
			p1.print();
			cout << endl;
			p2.print();
			cout << "행렬1 det: " << p1.getDet() << endl;
			cout << "행렬2 det: " << p2.getDet() << endl;
			break;
		case 'h':
			p1.expand();
			p2.expand();
			cout << "행렬1 det: " << p1.getDet() << endl;
			cout << "행렬2 det: " << p2.getDet() << endl;
			break;
		case 's':
			p1.reSet();
			p2.reSet();
			break;
		default:
			break;
		}
		cout << endl;
	}

	return 0;
}