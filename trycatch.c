#include "trycatch.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Exception new_exception(char* message) {
    struct Exception e;
    e.type = Exception;
    e.data = message;
    return e;
}

struct try_catch_stack* try_catch_catchers;
struct Exception try_catch_thrown;

// this is the default way each exception type will be reported.
// will need to be updated if/when additional exception types are added in
// trycatch.h to enum exception_type
int try_catch_default_catches(struct Exception e) {
    switch(e.type) {
        case Exception: 
            fprintf(stderr, "Exception with data %p: ", e.data);
            fprintf(stderr, "%s\n", e.data); // separate print in case data is invalid 
            return -1;
        default: 
            fprintf(stderr, "error with data %p\n", e.data); 
            return -1;
    }
}

// this handles uncaught exceptions
void try_catch_default_handler() {
    jmp_buf jmp;
    if(!setjmp(jmp)) try_catch_stack_push(jmp);
    else {
        fprintf(stderr, "Uncaught ");
        int status = try_catch_default_catches(try_catch_thrown);
        try_catch_cleanup();
        exit(status);
    }
}

int try_catch_init() {
    try_catch_catchers = malloc(sizeof (struct try_catch_stack));
    if(try_catch_catchers == NULL) return -1;
    try_catch_catchers -> head = NULL;
    return 0;
}

int try_catch_stack_empty() {
    return try_catch_catchers -> head == NULL;
}

void try_catch_cleanup() {
    while(!try_catch_stack_empty()) try_catch_stack_pop(NULL);
    free(try_catch_catchers);
}

void throw(struct Exception e) {
    try_catch_thrown = e;
    if(try_catch_stack_empty()) try_catch_default_handler();
    jmp_buf jmp;
    try_catch_stack_pop(jmp);
    longjmp(jmp, 1);
}

int try_catch_stack_peek(jmp_buf dest) {
    if(try_catch_stack_empty()) return -1;
    if(dest == NULL) return 0;
    memcpy(dest, try_catch_catchers -> head -> jmp, sizeof (jmp_buf));
    return 0;
}

int try_catch_stack_pop(jmp_buf dest) {
    if(try_catch_stack_peek(dest)) return -1;
    struct try_catch_node* new_head = try_catch_catchers -> head -> next;
    free(try_catch_catchers -> head);
    try_catch_catchers -> head = new_head;
    return 0;
}

int try_catch_stack_push(jmp_buf jmp) {
    struct try_catch_node* new_head = malloc(sizeof (struct try_catch_node));
    if(new_head == NULL) return -1;
    memcpy(new_head -> jmp, jmp, sizeof (jmp_buf));
    new_head -> next = try_catch_catchers -> head;
    try_catch_catchers -> head = new_head;
    return 0;
}
