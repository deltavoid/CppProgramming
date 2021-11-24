#ifndef HELLO_H
#define HELLO_H


struct hello {
    int id;
};


struct hello* hello_new();
void hello_delete(struct hello* obj);


int hello_set_id(struct hello* obj, int id);
int hello_get_id(struct hello* obj);


void hello_display_id(struct hello* obj);





#endif // HELLO_H
