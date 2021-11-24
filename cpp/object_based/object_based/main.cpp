#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <thread>
#include <memory>


class Hello {
public:

    std::unique_ptr<uint32_t> _id;

    Hello()
    {
        _id = std::make_unique<uint32_t>(0);
    }

    ~Hello()
    {
    }

    Hello(const Hello&) = delete;
    Hello& operator = (const Hello&) = delete;

    Hello(Hello&& src)
    {
        this->_id = std::move(src._id);
    }

    Hello& operator = (Hello&& src)
    {
        if  (&src != this)
        {
            this->~Hello();

            new (this) Hello(std::move(src));
        }

        return *this;
    }


    void setId(uint32_t id)
    {
        *this->_id = id;
    }

    uint32_t getId()
    {
        return *this->_id;
    }

    void printId()
    {
        printf("this: 0x%lx, id: %u\n", this, *this->_id);
    }

};


int main(int argc, char** argv, char** env)
{
    
    Hello hello;

    hello.setId(1);
    hello.printId();


    return 0;
}