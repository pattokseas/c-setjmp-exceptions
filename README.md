This is a terrible implementation of exceptions and try-catch blocks in C, using `setjmp.h`  

Using this is a terrible idea. Don't do it. But if you did want to use it you would need to know these functions:
- `struct Exception new_exception(char* message)` creates an exception you can throw
- `void throw(struct Exception e)` throws the exception
- `int try_catch_init()` should be called at the start of program execution.
- `void try_catch_cleanup()` should be called before exit. It's called for you if there's an uncaught exception

The `try_catch(try, catch)` macro is provided for you. It attempts to run the first code (`try`) and if it throws an exception it runs the second code (`catch`).

Example:
```
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
```
Expected output:
```
sqrt(4.000000) = 2.000000
sqrt(-9.000000) = 3.000000i
```
Explanation: The `print_sqrt(4)` call does not throw an exception so it prints as normal. The `print_sqrt(-9)` call causes `sqrt_exn` to throw an exception, which is caught by `print_sqrt`, and it calls `sqrt_exn` again without throwing an error the second time.
