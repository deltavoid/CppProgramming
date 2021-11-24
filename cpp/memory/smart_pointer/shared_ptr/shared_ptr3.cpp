#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>




class Task {
public:
    int _id;

    Task()
    {
        printf("Task::Task\n");
    }

    Task(int id)
      : _id(id)
    {
        printf("Task::Task, id: %d\n", _id);
    }

    ~Task()
    {
        printf("Task::~Task, id: %d\n", _id);
    }

    void print()
    {
        printf("Task::print, id: %d\n", _id);
    }

};


std::shared_ptr<Task> set_task(std::shared_ptr<Task> task, int id)
{
    printf("set_task: count: %d\n", task.use_count());
    task->_id = id;
    
    return task;
}


int main()
{
    printf("1\n");
    {
        
        std::shared_ptr<Task> sptr1(new Task(1));
        printf("stpr1 count: %d\n", sptr1.use_count());
        printf("stpr1 unqiue: %d\n", sptr1.unique());
        
        void* addr = sptr1.get();
        printf("addr: %lx\n", addr);
        printf("\n");


        std::shared_ptr<Task> sptr2(sptr1);
        printf("sptr1 count: %d\n", sptr1.use_count());
        printf("sptr2 count: %d\n", sptr2.use_count());
        sptr1->print();
        sptr2->print();
        printf("\n");


        sptr2.reset(new Task(2));
        printf("sptr1 count: %d\n", sptr1.use_count());
        printf("sptr2 count: %d\n", sptr2.use_count());
        sptr1->print();
        sptr2->print();
        printf("\n");

        
        sptr2.reset();
        printf("sptr1 count: %d\n", sptr1.use_count());
        printf("sptr2 count: %d\n", sptr2.use_count());
        sptr1->print();
        // sptr2->print(); // sptr2 is NULL
        printf("\n");
    }
    printf("\n");

    printf("2\n");
    {
        std::shared_ptr<Task> sptr3 = std::make_shared<Task>(3);
        printf("stpr3 count: %d\n", sptr3.use_count());
        sptr3->print();
        printf("\n");
 

        std::shared_ptr<Task> sptr4 = set_task(sptr3, 4);
        printf("stpr3 count: %d\n", sptr3.use_count());
        printf("stpr4 count: %d\n", sptr4.use_count());
        sptr3->print();
        sptr4->print();
        printf("\n");

    }

    return 0;
}
