#include "procession.h"
using namespace std;


int main()
{
	procession p1,p2;
	
	p1.print();

	cout << "det: " << p1.getDet() << endl;
	
	p2.print();

	cout << "det: " << p2.getDet() << endl;

	cout << endl;

	add(p1, p2);
	cout << endl;

	sub(p1, p2);
	cout << endl;
	mul(p1, p2);

	cout << endl;
	p1.expand();
	p1.print();
	return 0;
}