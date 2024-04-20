#include "trycatch.h"
#include <stdio.h>
#include <math.h>

double sqrt_exn(double x) {
    if(x < 0) throw(new_exception("Square root of negative"));
    return sqrt(x);
}

void print_sqrt(double x) {
    double y;
    try_catch(
        y = sqrt_exn(x);
        printf("sqrt(%lf) = %lf\n", x, y);
    ,
        y = sqrt_exn(-x);
        printf("sqrt(%lf) = %lfi\n", x, y);
    );
}

int main(int argc, char** argv) {
    try_catch_init();
    
    print_sqrt(4);
    print_sqrt(-9);

    try_catch_cleanup();
    return 0;
}
