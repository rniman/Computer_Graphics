#include "procession.h"
using namespace std;


int main()
{
	procession p1,p2;
	



	//콘솔창 클리어
	cout << "\x1B[2J\x1B[H";

	p1.expand();
	p1.print();
	p1.reSet();
	p1.print();
	cout << p1.cal_diterminant() << endl;
	return 0;
}