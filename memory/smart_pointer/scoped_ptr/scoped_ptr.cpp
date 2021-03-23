#include <cstdio>
#include <cstdlib>
#include <boost/smart_ptr.hpp>


typedef long long ll;

struct Task
{
    ll id;
    Task(ll id) : id(id) { printf("Task::Constructor\n"); }
    ~Task() { printf("Task::Destructor\n"); }
};

int main()
{
    {
        boost::scoped_ptr<Task> ptr1(new Task(1));
    }
    return 0;
}
