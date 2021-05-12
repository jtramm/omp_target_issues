#ifndef FOO_H
#define FOO_H

#pragma omp declare target
void foo(char* message);
#pragma omp end declare target

#endif
