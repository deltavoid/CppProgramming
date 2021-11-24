#include "hello.h"

#include <stdio.h>
#include <stdlib.h>


struct hello* hello_new()
{
    struct hello* obj = (struct hello*)malloc(sizeof(*obj));

    obj->id = 0;

    return obj;
}

void hello_delete(struct hello* obj)
{
    free(obj);
}


int hello_set_id(struct hello* obj, int id)
{
    obj->id = id;

    return 0;
}

int hello_get_id(struct hello* obj)
{
    return obj->id;
}

void hello_display_id(struct hello* obj)
{
    printf("obj: 0x%lx, id: %d\n", (unsigned long)obj, obj->id);
}

