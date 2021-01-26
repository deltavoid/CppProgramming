#include <cstdio>
#include <cstdlib>
#include <cstdint>




enum event_type {
    EVENT_TYPE0,
    EVENT_TYPE1,
    EVENT_TYPE_NUM
};

struct event {
    uint8_t type;
    uint8_t blank;
    uint16_t blank1;
    uint32_t blank2;
};


typedef int (*event_handler_t)(struct event* ev);

event_handler_t event_handlers[EVENT_TYPE_NUM];


int register_event_handler(enum event_type type, event_handler_t handler)
{
    event_handlers[type] = handler;
}


int main(int argc, char** argv)
{
    printf("args:");
    for (int i = 0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");


    

    return 0;
}