#ifndef _TRYCATCH_H
#define _TRYCATCH_H

#include <setjmp.h>

// need to add more exception types later
enum exception_type {
    Exception
};

struct Exception {
    enum exception_type type;
    void* data;
};

// message is NOT copied so make sure it's in either static or heap memory
// if it's in heap memory you'd better ensure you free it yourself
struct Exception new_exception(char* message);

void throw(struct Exception e);

struct try_catch_node {
    jmp_buf jmp;
    struct try_catch_node* next;
};

struct try_catch_stack {
    struct try_catch_node* head;
};

// init must be called once at start of main
// nonzero return means you can't use try-catch
int try_catch_init();

// cleanup must be called before exit
// (if you want valgrind to believe you aren't leaking memory)
// it will be called automatically on an uncaught exception
void try_catch_cleanup();

// never call these functions
// they are exposed because the macro uses them
int try_catch_stack_push(jmp_buf jmp);
int try_catch_stack_pop(jmp_buf dest);

// the macro for trying and catching
#define try_catch(try, catch) { \
    jmp_buf jmp; \
    if(!setjmp(jmp)) { \
        try_catch_stack_push(jmp); \
        { try; } \
        try_catch_stack_pop(NULL); \
    } else { catch; } }

#endif
