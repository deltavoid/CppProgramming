// object func
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;


class Computer
{
public:
	static int Add(int i, int j)
	{
		return i + j;
	}
 
	template<class T>
	static T AddT(T i, T j)
	{
		return i + j;
	}
 
	int AddN(int i, int j)
	{
		return i + j;
	}
};
 
//存储对成员函数的调用 
 
int main()
{
	//1、 类静态函数
	std::function<int(int, int)> f = &Computer::Add;
	cout << f(1, 1) << endl;
 
	//2、 类静态模板函数
	std::function<int(int, int)> ft = &Computer::AddT<int>;
	cout << ft(1, 1) << endl;
 
 
 
	//普通函数绑定  需要构造类对象
	Computer c;
 
	//3、 普通函数 需使用bind,将类对象地址 &c 绑定上
	std::function<int(int, int)> fN = std::bind(&Computer::AddN, &c, placeholders::_1, placeholders::_2);
	cout << fN(1, 1) << endl;
 
 
	//4、普通函数， 也可以这样调用  个人觉得这个比 bind 麻烦，不建议
	std::function <int(Computer &, int, int)> fN2 = &Computer::AddN;
	cout << fN2(c,1, 1) << endl;
 
 
	getchar();
	return 0;
}
