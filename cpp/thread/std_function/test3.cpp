// lambda func
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;



auto g_Lambda = [](int i, int j)
{
	return i + j;
}; //匿名函数 此处有分号
 
int main()
{
	std::function<int(int, int)> f = g_Lambda;
	cout<<f(2,3);
 
	getchar();
	return 0;

}