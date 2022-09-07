#include "procession.h"
using namespace std;


int main()
{
	procession p1,p2;
	char command;
	
	while (1)
	{
		cout << "���� ���1" << endl;
		p1.print();
		cout << "���� ���2" << endl;
		p2.print();

		cin >> command;
		cin.ignore();
		cout << "\x1B[2J\x1B[H";

		if (command == 'q')
		{
			cout << "����" << endl;
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
			cout << "���1 det: " << p1.getDet() << endl;
			cout << "���2 det: " << p2.getDet() << endl;
			break;
		case 't':
			cout << "�ٲٱ��� ���1" << endl;
			p1.transpose();
			cout << "�ٲٱ��� ���2" << endl;
			p2.transpose();
			cout << endl;
			cout << "���� ���1 det: " << p1.getDet() << endl;
			cout << "���� ���2 det: " << p2.getDet() << endl;
			break;
		case 'h':
			p1.expand();
			p2.expand();
			cout << "���1 det: " << p1.getDet() << endl;
			cout << "���2 det: " << p2.getDet() << endl;
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