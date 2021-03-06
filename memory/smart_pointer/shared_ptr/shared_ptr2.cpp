#include <cstdio>
#include <cstdlib>

#include <thread>
#include <memory>
#include <string>


class Student : public std::enable_shared_from_this<Student>
{public:
    Student() 
    {  
        printf("Student::constructor\n");
    }
    
    ~Student()  
    {  
        printf("Student::destructor\n");
    }

    std::shared_ptr<Student> getStudent()
    {
        return shared_from_this();
    }

    void setName(std::string name)  
    {  
        this->name = name;
    }

    std::string getName()  
    {  
        return name;
    }

    std::string name;
};


void output_count(int cnt, std::string name)
{
    printf("%s's use_count: %d\n", name.c_str(), cnt);
}


std::shared_ptr<Student> set_student_name(std::shared_ptr<Student> stu, std::string name)
{
    stu->setName(name);

    output_count(stu.use_count(), "set_student_name stu");

    return stu;
}

int main()
{
    {
        printf("1\n");
        std::shared_ptr<int> ptr(new int(10));

        unsigned long addr = (unsigned long)ptr.get();

        printf("addr: %lx\n", addr);

    }
    printf("\n");

    {
        printf("2\n");
        std::shared_ptr<int> ptr1 = std::make_shared<int>(10);
        output_count(ptr1.use_count(), "ptr1");

        std::shared_ptr<int> ptr2(ptr1);
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");

        ptr2.reset();
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");

        ptr1.reset();
        // ptr1.release(); // shared_ptr has no release
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");
    }
    printf("\n");

    {
        printf("3\n");
        std::shared_ptr<Student> ptr1(new Student());
        output_count(ptr1.use_count(), "ptr1");

        std::shared_ptr<Student> ptr2(ptr1);
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");

        ptr2.reset();
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");

        printf("ptr1 is to be reseted\n");
        ptr1.reset();
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");
    }
    printf("\n");

    {
        std::shared_ptr<Student> stu = std::make_shared<Student>();
        output_count(stu.use_count(), "stu");

        std::shared_ptr<Student> stu1 = set_student_name(stu, "world");
        output_count(stu.use_count(), "stu1");

    }
    printf("\n");


    return 0;
}