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

int process_event(struct event* ev)
{
    return event_handlers[ev->type](ev);
}


int event_handler0(struct event* ev)
{
    printf("event_handler0\n");
}

int event_handler1(struct event* ev)
{
    printf("event_handler1\n");
}


const int max_events = 10;
struct event events[max_events];



int main(int argc, char** argv)
{
    printf("args:");
    for (int i = 0; i < argc; i++)
        printf("%s ", argv[i]);
    printf("\n");

    register_event_handler(EVENT_TYPE0, event_handler0);
    register_event_handler(EVENT_TYPE1, event_handler1);

    for (int i = 0; i < max_events; i++) 
        events[i].type = i % EVENT_TYPE_NUM;
    
    for (int i = 0; i < max_events; i++)
        process_event(&events[i]);
    

    printf("hello world\n");
    return 0;
}