// template func
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;



template <class T>
T g_Add(T i, T j)
{
	cout << i + j;
	return i + j;
}
 
 
int main()
{
	std::function<int(int,int)> f = g_Add<int>;
	f(2,3);
 
	getchar();
	return 0;
}
