// common func
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;



void gFunc()
{
	cout << "gFunc" << endl;
}

int main()
{
	std::function<void()> f = gFunc;
	f();
 
	getchar();
	return 0;
}
