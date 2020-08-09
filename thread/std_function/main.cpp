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
 

int main()
{
    //普通函数绑定  需要构造类对象
	Computer c;

    //3、 普通函数 需使用bind,将类对象地址 &c 绑定上
	std::function<int(int, int)> fN = std::bind(&Computer::AddN, &c, placeholders::_1, placeholders::_2);
	cout << fN(1, 1) << endl;

    printf("hello world\n");
    return 0;
}