#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <vector>
using namespace std;

// template member function

class PrintIt
{public:
    ostream& out;

    PrintIt(ostream& out) : out(out) {}
    ~PrintIt() {}

    template<typename Elem>
    void print(Elem& elem)
    {
        out << elem << endl;
    }
};


// template class 's template member function
// just show grammer, no acutal use

template<typename OutStream>
class OutPrintIt
{public:
    ostream& out;

    OutPrintIt(OutStream& out) : out(out) {}
    ~OutPrintIt() {}

    template<typename Elem>
    void print(const Elem& elem)
    {
        out << elem;
    }

    template<typename Elem>
    void println(const Elem& elem);
};

template<typename OutStream>
template<typename Elem>
void OutPrintIt<OutStream>::println(const Elem& elem)
{
    out << elem << endl;
}


int main()
{
    PrintIt print_to_cout(cout);
    print_to_cout.print("hello");
    
    int num = 1024;
    print_to_cout.print(num);

    // print_to_cout.print(string("world")); is invalid
    string world("world");
    print_to_cout.print(world);
    

    OutPrintIt<ostream> print_also_to_cout(cout);
    print_also_to_cout.print("hello\n");
    print_also_to_cout.println(num);
    print_also_to_cout.println(world);

    return 0;
}