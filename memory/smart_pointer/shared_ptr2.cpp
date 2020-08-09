#include <cstdio>
#include <cstdlib>
#include <memory>


class Student : public std::enable_shared_from_this<Student>


{public:
    Student() {  printf("Student::constructor\n");}
    ~Student()  {  printf("Student::destructor\n");}

    std::shared_ptr<Student> getStudent()
    {
        return shared_from_this();
    }

    void setName(std::string name)  {  this->name = name;}
    std::string getName()  {  return name;}

    std::string name;
};


void output_count(int cnt, std::string name)
{
    printf("%s's use_count: %d\n", name.c_str(), cnt);
}
int main()
{
    {
        printf("1\n");
        std::shared_ptr<int> ptr(new int(10));
    }

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
        output_count(ptr1.use_count(), "ptr1");
        output_count(ptr2.use_count(), "ptr2");
    }

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


    return 0;
}