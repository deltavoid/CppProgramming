#include <cstdio>
#include <cstdlib>
#include <memory>

typedef long long ll;

struct Task
{
    ll id;
    Task(ll id) : id(id) { printf("Task::Constructor\n"); }
    ~Task() { printf("Task::Destructor\n"); }
};

void output_null(bool null, std::string name)
{
    if (null)
        printf("%s is null\n", name.c_str());
    else
        printf("%s is not null\n", name.c_str());
}

std::unique_ptr<Task> inc(std::unique_ptr<Task> task)
{
    task->id++;
    printf("inc\n");
    // return std::unique_ptr<Task>(new Task(task->id));
    // return std::make_unique<Task>(task->id);
    return std::move(task);
    // return task;
}

int main()
{
    {
        printf("1\n");
        std::unique_ptr<int> ptr1;
        output_null(ptr1 == nullptr, "ptr1");
    }
    printf("\n");

    {
        printf("2\n");
        std::unique_ptr<Task> taskPtr2(new Task(2));
        output_null(taskPtr2 == nullptr, "taskPtr");

        unsigned long addr = (unsigned long)taskPtr2.get();
        printf("addr: %lx\n", addr);

        std::unique_ptr<Task> taskPtr3 = std::move(taskPtr2);
        output_null(taskPtr2 == nullptr, "taskPtr2");
        output_null(taskPtr3 == nullptr, "taskPtr3");
        printf("taskPtr3->id: %lld\n", taskPtr3->id);
    }
    printf("\n");

    {
        printf("3\n");
        std::unique_ptr<Task> taskPtr8 = std::make_unique<Task>(8);
        output_null(taskPtr8 == nullptr, "taskPtr8");

        Task* addr = taskPtr8.release();
        printf("addr: %lx\n", addr);
        delete addr;
        // delete (void*)addr; // delete as mem, no destructor
    }
    printf("\n");

    // {
    //     Task *ptr = new Task(3);
    //     delete ptr;
    //     delete ptr;
    // }

    {
        printf("4\n");
        std::unique_ptr<Task> taskPtr5(new Task(3));
        output_null(taskPtr5 == nullptr, "taskPtr5");

        Task *ptr = taskPtr5.release();
        output_null(taskPtr5 == nullptr, "taskPtr5");
        output_null(ptr == NULL, "ptr");

        std::unique_ptr<Task> taskPtr6(ptr);
        output_null(taskPtr6 == nullptr, "taskPtr6");

        // delete ptr;
        // delete ptr;

        // std::unique_ptr<Task> taskPtr7(ptr);
        // output_null(taskPtr7 == nullptr, "taskPtr7");
    }
    printf("\n");

    {
        printf("5\n");
        std::unique_ptr<Task> taskPtr(new Task(1));
        output_null(taskPtr == nullptr, "taskPtr");
        printf("taskPtr-> id: %lld\n", taskPtr->id);

        taskPtr.reset();
        output_null(taskPtr == nullptr, "taskPtr");
    }
    printf("\n");

    {
        printf("6\n");
        std::unique_ptr<Task> taskPtr6(new Task(6));
        std::unique_ptr<Task> taskPtr7 = inc(std::move(taskPtr6));
        output_null(taskPtr6 == nullptr, "taskPtr6");
        output_null(taskPtr7 == nullptr, "taskPtr7");
    }
    printf("\n");

    return 0;
}