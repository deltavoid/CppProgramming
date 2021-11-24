#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
#include <string>
using namespace std;

// template class
// a simple template stack class

template<typename Elem>
class Stack
{public:
    std::vector<Elem> data;

    Stack() {}
    ~Stack() {}

    int size() { return data.size();}

    void push(Elem& elem);
    Elem& top();
    void pop();
};

template<typename Elem>
void Stack<Elem>::push(Elem& elem)
{
    data.push_back(elem);
}

template<typename Elem>
Elem& Stack<Elem>::top()
{
    return data[data.size() - 1];
}

template<typename Elem>
void Stack<Elem>::pop()
{
    data.pop_back();
}


int main()
{
    Stack<string> stack;

    string hello("hello");
    stack.push(hello);

    cout << stack.top() << endl;

    stack.pop();

    cout << "size: " << stack.size() << endl;
    
    return 0;
}